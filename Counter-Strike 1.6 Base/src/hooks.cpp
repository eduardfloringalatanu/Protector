#include "global.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_opengl2.h"
#include "menu.h"
#include "stdio.h"
#include "json.hpp"
#include <fstream>
#include "parsemsg.h"
#include "utils.h"
#include "emulators/revemu.h"
#include <stdexcept>
#include "messages.h"
#include <queue>
#include <thread>
#include <mutex>
#define CURL_STATICLIB
#include <curl/curl.h>

HWND window_handle;
WNDPROC original_WndProc;

void clean_imgui() {
	show_menu_window = false;

	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	SetWindowLong(window_handle, GWL_WNDPROC, (LONG)original_WndProc);
}

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc_hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_KEYDOWN:
		keys[wParam] = true;

		if (wParam == VK_INSERT) {
			show_menu_window = !show_menu_window;

			if (show_menu_window)
				cl_funcs->pIN_DeactivateMouse();
			else
				cl_funcs->pIN_ActivateMouse();

			ImGui::GetIO().MouseDrawCursor = show_menu_window;
		}

		break;
	case WM_KEYUP:
		keys[wParam] = false;

		break;
	}

	if (show_menu_window) {
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

		if ((uMsg == WM_KEYDOWN || uMsg == WM_MOUSEMOVE || uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN || uMsg == WM_MBUTTONDOWN || uMsg == WM_MOUSEWHEEL || uMsg == WM_XBUTTONDOWN ))
			return 0;
	}

	return CallWindowProc(original_WndProc, hWnd, uMsg, wParam, lParam);
}

HINSTANCE module_instance_handle;

template<typename T>
void read_value(nlohmann::ordered_json& j, std::string key_name, T& output) {
	if (key_name.empty())
		return;

	if (j.dump().find(key_name) == std::string::npos)
		return;

	j.at(key_name).get_to(output);
}

void load_config() {
	char path[MAX_PATH];
	GetModuleFileName(module_instance_handle, path, MAX_PATH);
	
	char* extension = strrchr(path, '.');

	if (extension)
		*extension = '\0';

	char config_filename[MAX_PATH];
	snprintf(config_filename, MAX_PATH, "%s.json", path);

	std::fstream config_file_stream(config_filename);
	nlohmann::ordered_json j = nlohmann::ordered_json::parse(config_file_stream, nullptr, false);

	std::map<std::string, nlohmann::ordered_json> list1;
	read_value(j, "commands_whitelist", list1);

	bool regex;
	bool forward_to_server;

	for (auto& item : list1) {
		read_value(item.second, "regex", regex);
		read_value(item.second, "forward_to_server", forward_to_server);

		add_command_to_whitelist(item.first.c_str(), regex, forward_to_server);
	}

	std::map<std::string, nlohmann::ordered_json> list2;
	read_value(j, "unknown_commands_list", list2);

	for (auto& item : list2) {
		read_value(item.second, "regex", regex);

		add_unknown_command_to_list(item.first.c_str(), regex);
	}

	std::map<std::string, nlohmann::ordered_json> list3;
	read_value(j, "fake_cvars_list", list3);

	std::string value;

	for (auto& item : list3) {
		read_value(item.second, "value", value);
		read_value(item.second, "regex", regex);

		add_fake_cvar_to_list(item.first.c_str(), value.c_str(), regex);
	}

	std::vector<std::string> list4;
	read_value(j, "extensions_whitelist", list4);

	for (auto& item : list4)
		add_extension_to_whitelist(item.c_str());

	std::map<std::string, nlohmann::ordered_json> list5;
	read_value(j, "consistency_list", list5);

	std::string md5;

	for (auto& item : list5) {
		read_value(item.second, "md5", md5);

		add_consistency_to_list(item.first.c_str(), md5.c_str());
	}

	read_value(j, "follow_redirects", follow_redirects);
	read_value(j, "block_motd", block_motd);

	read_value(j, "random_cdkey", random_cdkey);

	std::string custom_cdkey2;
	read_value(j, "custom_cdkey", custom_cdkey2);
	strncpy(custom_cdkey, custom_cdkey2.c_str(), ARRAYSIZE(custom_cdkey) - 1);

	read_value(j, "random_steamid", random_steamid);
	read_value(j, "custom_steamid", custom_steamid);
	read_value(j, "auto_retry", auto_retry);
	read_value(j, "remove_hud_messages", remove_hud_messages);
	read_value(j, "remove_effects", remove_effects);
	read_value(j, "remove_non_player_entities", remove_non_player_entities);
	read_value(j, "catch_Host_Frame_exceptions", catch_Host_Frame_exceptions);
	read_value(j, "chat_color", chat_color);
}

BOOL WINAPI SwapBuffers_hook(HDC hdc) {
	static bool imgui_init = false;

	if (!imgui_init) {
		window_handle = WindowFromDC(hdc);
		original_WndProc = (WNDPROC)SetWindowLong(window_handle, GWL_WNDPROC, (LONG)WndProc_hook);

		ImGui::CreateContext();
		ImGui_ImplWin32_Init(window_handle);
		ImGui_ImplOpenGL2_Init();

		ImGui::GetIO().IniFilename = nullptr;
		ImGui::GetStyle().WindowRounding = 0.0f;
		ImGui::GetStyle().ScrollbarRounding = 0.0f;
		ImGui::GetStyle().AntiAliasedLines = false;
		ImGui::GetStyle().AntiAliasedFill = false;
		ImGui::GetStyle().Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

		header1_font = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\verdana.ttf", 20.0f);
		header2_font = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\verdana.ttf", 18.0f);
		body_font = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\verdana.ttf", 16.0f);

		load_config();

		imgui_init = true;
	}

	ImGui_ImplWin32_NewFrame();
	ImGui_ImplOpenGL2_NewFrame();

	ImGui::NewFrame();

	{
		DrawMenu();
		//
	}

	ImGui::EndFrame();

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	return original_SwapBuffers(hdc);
}

cmd_function_s* get_cmd_function(const char* command_name) {
	cmd_function_s* p = (cmd_function_s *)gEngfuncs->GetFirstCmdFunctionHandle();

	while (p != nullptr) {
		if (stricmp(p->name, command_name) == 0)
			return p;

		p = p->next;
	}

	return nullptr;
}

const char* Cvar_IsMultipleTokens(const char* varname) {
	static char firstToken[516];
	char* name;

	firstToken[0] = '\0';
	name = (char*)varname;

	name = COM_Parse(name);

	if (com_token[0] == '\0')
		return nullptr;

	if (name == nullptr)
		return nullptr;

	strncpy(firstToken, com_token, ARRAYSIZE(firstToken) - 1);
	firstToken[ARRAYSIZE(firstToken) - 1] = '\0';

	name = COM_Parse(name);

	if (com_token[0] == '\0')
		return nullptr;

	return firstToken;
}

cvar_s* get_cvar(const char* cvar_name) {
	cvar_s* p = gEngfuncs->GetFirstCvarPtr();

	while (p != nullptr) {
		if (stricmp(p->name, cvar_name) == 0)
			return p;

		p = p->next;
	}

	return nullptr;
}

/*
* return -1 => unk command(forward_to_server)
* return 0 => blocked
* return 1 => not blocked
* return 2 => forward_to_server
*/

int filter_command(const char* command_name) {
	cmd_function_s* cmd_function = get_cmd_function(command_name);

	if (cmd_function != nullptr) {
		int ret = is_in_commands_whitelist(command_name);

		if (ret == 1) {
			if (cls->demorecording == 1 && (cmd_function->flags & FCMD_HUD_COMMAND) != 0 && cls->spectator == 0)
				CL_RecordHUDCommand(cmd_function->name);

			cmd_function->function();

			return 1;
		}
		
		if (ret == 2) {
			if (cls->state >= ca_connected)
				Cmd_ForwardToServer();
			else
				return 0;

			return 2;
		}

		return 0;
	}

	return -1;
}

/*
* return -1 => unk cvar(forward_to_server)
* return 0 => blocked
* return 1 => not blocked
* return 2 => forward_to_server
*/
int filter_cvar(const char* cvar_name) {
	const char* first_token = Cvar_IsMultipleTokens(cvar_name);
	cvar_s* cvar;

	int ret;

	if (first_token != nullptr) {
		cvar = get_cvar(first_token);

		if (cvar != nullptr) {
			ret = is_in_commands_whitelist(first_token);

			if (ret == 1) {
				gEngfuncs->Con_Printf((char*)"\"%s\" is \"%s\"\n", cvar->name, cvar->string);

				return 1;
			}

			if (ret == 2) {
				if (cls->state >= ca_connected)
					Cmd_ForwardToServer();
				else
					return 0;

				return 2;
			}

			return 0;
		}
	} else {
		cvar = get_cvar(cvar_name);

		if (cvar != nullptr) {
			ret = is_in_commands_whitelist(cvar_name);

			if (gEngfuncs->Cmd_Argc() == 1) {
				if (ret == 1) {
					gEngfuncs->Con_Printf((char*)"\"%s\" is \"%s\"\n", cvar->name, cvar->string);

					return 1;
				}

				if (ret == 2) {
					if (cls->state >= ca_connected)
						Cmd_ForwardToServer();
					else
						return 0;

					return 2;
				}

				return 0;
			}

			if ((cvar->flags & FCVAR_SPONLY) != 0 && cls->state >= ca_connecting && nMax->maxclients > 1) {
				if (ret == 1) {
					gEngfuncs->Con_Printf((char*)"Can't set %s in multiplayer\n", cvar->name);

					return 1;
				}

				if (ret == 2) {
					if (cls->state >= ca_connected)
						Cmd_ForwardToServer();
					else
						return 0;

					return 2;
				}

				return 0;
			}

			if (ret == 1) {
				char* arg1 = gEngfuncs->Cmd_Argv(1);

				Cvar_DirectSet(cvar, arg1);

				return 1;
			}

			if (ret == 2) {
				if (cls->state >= ca_connected)
					Cmd_ForwardToServer();
				else
					return 0;

				return 2;
			}

			return 0;
		}
	}

	return -1;
}

bool filter_unknown_command(const char* command_name) {
	if (!is_in_unknown_commands_list(command_name)) {
		if (cls->state >= ca_connected)
			Cmd_ForwardToServer();
		else
			return false;

		return true;
	}

	return false;
}

void filter_command_string(const char* const command_string, const char* const function_name) {
	unsigned long i = 0;
	unsigned long beginning;
	unsigned long quotes;
	unsigned long num;
	char line[MAX_CMD_LINE];

	while (command_string[i] != '\0') {
		beginning = i;
		quotes = 0;

		for (; command_string[i] != '\0'; ++i) {
			if (command_string[i] == '"')
				++quotes;

			if (command_string[i] == ';' && (quotes & 1) == 0)
				break;

			if (command_string[i] == '\n')
				break;
		}

		num = i - beginning;

		if (num > MAX_CMD_LINE - 1)
			num = MAX_CMD_LINE - 1;

		memcpy(line, command_string + beginning, num);
		line[num] = '\0';

		Cmd_TokenizeString(line);

		if (gEngfuncs->Cmd_Argc() != 0) {
			char* arg0 = gEngfuncs->Cmd_Argv(0);
			int ret;

			if ((ret = filter_command(arg0)) != -1 || (ret = filter_cvar(arg0)) != -1) {
				switch (ret) {
				case 0:
					Log("[%s] command line: %s [Blocked]\n", function_name, line);

					break;
				case 1:
					Log("[%s] command line: %s [Not blocked]\n", function_name, line);

					break;
				case 2:
					Log("[%s] command line: %s [Not blocked (forwarded to server)]\n", function_name, line);

					break;
				}
			} else if (!filter_unknown_command(arg0))
				Log("[%s] command line: %s [Blocked]\n", function_name, line);
			else
				Log("[%s] command line: %s [Not blocked (forwarded to server)]\n", function_name, line);
		}

		if (command_string[i] == '\0')
			break;
		else
			++i;
	}
}

void CL_Parse_StuffText_hook(void) {
	char* command_string = MSG_ReadString();

	filter_command_string(command_string, "SVC_STUFFTEXT");
}

void CL_Parse_Director_hook(void) {
	int backup = *msg_readcount;

	MSG_ReadByte();
	
	if (MSG_ReadByte() == DRC_CMD_STUFFTEXT) {
		char* command_string = MSG_ReadString();

		return filter_command_string(command_string, "DRC_CMD_STUFFTEXT");
	}

	*msg_readcount = backup;

	original_CL_Parse_Director();
}

qboolean CL_ReadDemoMessage_OLD_ValidStuffText_hook(const char* cmd) {
	return 1;
}

void CL_ReadDemoMessage_OLD_Cbuf_AddFilteredText_hook(char* text) {
	filter_command_string(text, "DEMO_CMD_STRINGCMD");
}

void CL_ReadPackets_CL_ConnectionlessPacket_hook(void) {
	MSG_BeginReading();

	MSG_ReadLong();
	char* text = MSG_ReadStringLine();
	Cmd_TokenizeString(text);

	switch (gEngfuncs->Cmd_Argv(0)[0]) {
	case 'L':
		char* p1 = strchr(text, ';');
		char* p2 = strchr(text, '\n');

		char* p = nullptr;

		if (p1 != nullptr && p2 != nullptr)
			p = (p1 < p2) ? p1 : p2;
		else if (p1 != nullptr && p2 == nullptr)
			p = p1;
		else if (p1 == nullptr && p2 != nullptr)
			p = p2;

		char server_address[64];
		unsigned long server_address_len;

		if (p)
			server_address_len = p - text - 1;
		else
			server_address_len = strlen(text + 1);

		strncpy(server_address, text + 1, server_address_len);
		server_address[server_address_len] = '\0';

		if (server_address_len != 0) {
			if (!follow_redirects) {
				Log("[S2C_REDIRECT] server address: %s [Blocked]\n", server_address);

				return CL_Disconnect();
			}

			char command[MAX_CMD_LINE];
			snprintf(command, MAX_CMD_LINE, "connect %s\n", server_address);

			Cbuf_AddText(command);

			Log("[S2C_REDIRECT] server address: %s [Not blocked]\n", server_address);

			return;
		}

		return CL_Disconnect();
	}

	original_CL_ConnectionlessPacket();
}

qboolean IsSafeFileToDownload(const char* filename) {
	if (filename == nullptr)
		return 0;

	char lwrfilename[MAX_PATH];
	strncpy(lwrfilename, filename, ARRAYSIZE(lwrfilename));
	lwrfilename[ARRAYSIZE(lwrfilename) - 1] = '\0';
	strlwr(lwrfilename);

	char* first = strchr(lwrfilename, '.');

	if (first == nullptr)
		return 0;

	char* last = strrchr(lwrfilename, '.');

	if (first != last)
		return 0;

	if (lwrfilename[0] == '/' ||
		strstr(lwrfilename, "\\") ||
		strstr(lwrfilename, ":") ||
		strstr(lwrfilename, "~") ||
		strlen(first) != 4)
		return 0;

	return is_in_extensions_whitelist(first) == 0;
}

qboolean CL_BatchResourceRequest_IsSafeFileToDownload_hook(const char* filename) {
	if (!IsSafeFileToDownload(filename)) {
		Log("[CL_BatchResourceRequest] filename: %s [Blocked]\n", filename);

		return 0;
	}

	Log("[CL_BatchResourceRequest] filename: %s [Not blocked]\n", filename);

	return original_IsSafeFileToDownload(filename);
}

qboolean Netchan_CopyFileFragments_IsSafeFileToDownload_hook(const char* filename) {
	if (!IsSafeFileToDownload(filename)) {
		Log("[Netchan_CopyFileFragments] filename: %s [Blocked]\n", filename);

		return 0;
	}

	Log("[Netchan_CopyFileFragments] filename: %s [Not blocked]\n", filename);

	return original_IsSafeFileToDownload(filename);
}

void CL_Parse_VoiceInit_hook(void) {
	int backup = *msg_readcount;

	char* codec_name = MSG_ReadString();
	int quality = MSG_ReadByte();

	if (codec_name[0] != '\0' && stricmp(codec_name, "voice_miles") && stricmp(codec_name, "voice_speex")) {
		Log("[SVC_VOICEINIT] codec name: %s | quality: %d [Blocked]\n", codec_name, quality);

		return;
	}

	Log("[SVC_VOICEINIT] codec name: %s | quality: %d [Not blocked]\n", codec_name, quality);

	*msg_readcount = backup;

	return original_CL_Parse_VoiceInit();
}

void CL_Send_CvarValue_hook(void) {
	char* name = MSG_ReadString();

	cvar_s* cvar = get_cvar(name);

	std::string value;
	const char* send = nullptr;

	if (strlen(name) > 254 || cvar == nullptr)
		send = "Bad CVAR request";
	else if ((cvar->flags & FCVAR_SERVER) != 0)
		send = "CVAR is server-only";
	else if ((cvar->flags & FCVAR_PROTECTED) != 0)
		send = "CVAR is protected";
	else if ((cvar->flags & FCVAR_PRIVILEGED) != 0)
		send = "CVAR is privileged";
	else if (is_in_fake_cvars_list(name, value))
		send = value.c_str();

	if (send != nullptr) {
		MSG_WriteByte(&cls->netchan.message, 10);
		MSG_WriteString(&cls->netchan.message, send);

		Log("[SVC_SENDCVARVALUE] cvar name: %s [Not blocked (sent value: %s)]\n", name, send);

		return;
	}
	
	Log("[SVC_SENDCVARVALUE] cvar name: %s [Blocked]\n", name);
}

void CL_Send_CvarValue2_hook(void) {
	int request_id = MSG_ReadLong();
	char* name = MSG_ReadString();

	cvar_s* cvar = get_cvar(name);

	std::string value;
	const char* send = nullptr;

	if (strlen(name) > 254 || cvar == nullptr)
		send = "Bad CVAR request";
	else if ((cvar->flags & FCVAR_SERVER) != 0)
		send = "CVAR is server-only";
	else if ((cvar->flags & FCVAR_PROTECTED) != 0)
		send = "CVAR is protected";
	else if ((cvar->flags & FCVAR_PRIVILEGED) != 0)
		send = "CVAR is privileged";
	else if (is_in_fake_cvars_list(name, value))
		send = value.c_str();

	if (send != nullptr) {
		MSG_WriteByte(&cls->netchan.message, 11);
		MSG_WriteLong(&cls->netchan.message, request_id);
		MSG_WriteString(&cls->netchan.message, name);
		MSG_WriteString(&cls->netchan.message, send);

		Log("[SVC_SENDCVARVALUE2] request id: %d | cvar name: %s [Not blocked (sent value: %s)]\n", request_id, name, send);

		return;
	}
	
	Log("[SVC_SENDCVARVALUE2] request id: %d | cvar name: %s [Blocked]\n", request_id, name);
}

BOOL CL_SendConsistencyInfo_MD5_Hash_File_hook(unsigned char* digest, char* pszFileName, BOOL bUsefopen, BOOL bSeed, unsigned int* seed) {
	unsigned char digest2[16];

	if (is_in_consistency_list(pszFileName, digest2)) {
		static unsigned char empty[16] = { };

		if (memcmp(digest2, empty, ARRAYSIZE(digest2)) == 0) {
			Log("[CL_SendConsistencyInfo] filename: %s [Blocked]\n", pszFileName);

			return FALSE;
		}

		memcpy(digest, digest2, ARRAYSIZE(digest2));

		Log("[CL_SendConsistencyInfo] filename: %s [Not blocked (sent md5: %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X)]\n", pszFileName,
			digest2[0], digest2[1], digest2[2], digest2[3], digest2[4], digest2[5], digest2[6], digest2[7],
			digest2[8], digest2[9], digest2[10], digest2[11], digest2[12], digest2[13], digest2[14], digest2[15]);

		return TRUE;
	}
	
	Log("[CL_SendConsistencyInfo] filename: %s [Not blocked]\n", pszFileName);

	return original_MD5_Hash_File(digest, pszFileName, bUsefopen, bSeed, seed);
}

int __MsgFunc_MOTD_hook(const char* pszName, int iSize, void* pbuf) {
	BEGIN_READ(pbuf, iSize);

	int m_iGotAllMOTD = READ_BYTE();
	static char m_szMOTD[1536] = { };

	size_t count = ARRAYSIZE(m_szMOTD) - strlen(m_szMOTD) - 1;

	strncat(m_szMOTD, READ_STRING(), (count < 0) ? 0 : count);

	if (m_iGotAllMOTD == 1) {
		if (block_motd)
			Log("[__MsgFunc_MOTD] motd: {\n%s\n} [Blocked]\n", m_szMOTD);
		else
			Log("[__MsgFunc_MOTD] motd: {\n%s\n} [Not blocked]\n", m_szMOTD);

		m_szMOTD[0] = '\0';
	}

	return block_motd ? 0 : original___MsgFunc_MOTD(pszName, iSize, pbuf);
}

char* CL_GetCDKeyHash_hook(void) {
	if (random_cdkey) {
		static const char hex_alphabet[] = "0123456789abcdef";

		static char cdkey[33];
		generate_random_string(hex_alphabet, cdkey, ARRAYSIZE(cdkey) - 1);

		return cdkey;
	}

	if (strlen(custom_cdkey) == 32)
		return custom_cdkey;

	return original_CL_GetCDKeyHash();
}

int CL_SendConnectPacket_Steam_GSInitiateGameConnection_hook(void* pData, int cbMaxData, uint64 steamID, uint32 unIPServer, uint16 usPortServer, qboolean bSecure) {
	if (random_steamid)
		return generate_revemu(pData, 0);
	
	int ret;

	if (custom_steamid != 0 && (ret = generate_revemu(pData, custom_steamid)) != 0)
		return ret;

	return original_Steam_GSInitiateGameConnection(pData, cbMaxData, steamID, unIPServer, usPortServer, bSecure);
}

void R_ForceCVars_hook(qboolean mp) {
	;
}

void CL_Set_ServerExtraInfo_hook(void) {
	MSG_ReadString();
	MSG_ReadByte();
}

void CL_Parse_Disconnect_hook(void) {
	int backup = *msg_readcount;

	char* reason = MSG_ReadString();

	if (auto_retry) {
		CL_Disconnect();
		Cbuf_AddText((char*)"retry\n");

		Log("[SVC_DISCONNECT] reason: %s [Blocked (sent \"retry\")]\n", reason);

		return;
	}

	Log("[SVC_DISCONNECT] reason: %s [Not blocked]\n", reason);

	*msg_readcount = backup;

	original_CL_Parse_Disconnect();
}

int pfnVGUI2DrawCharacterAdd_hook(int x, int y, int ch, int r, int g, int b, unsigned int font) {
	if (remove_hud_messages)
		return 0;

	return original_pfnVGUI2DrawCharacterAdd(x, y, ch, r, g, b, font);
}

int pAddEntity_hook(int type, struct cl_entity_s* ent, const char* modelname) {
	if (ent->player == 0 && remove_non_player_entities)
		return 0;

	return original_pAddEntity(type, ent, modelname);
}

void CL_Parse_TempEntity_hook(void) {
	int backup = *msg_readcount;
	
	if (remove_effects) {
		switch (MSG_ReadByte()) {
		case TE_BEAMPOINTS:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadShort();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();

			return;
		case TE_BEAMENTPOINT:
			MSG_ReadShort();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadShort();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();

			return;
		case TE_GUNSHOT:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();

			return;
		case TE_EXPLOSION:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadShort();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();

			return;
		case TE_TAREXPLOSION:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();

			return;
		case TE_SMOKE:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadShort();
			MSG_ReadByte();
			MSG_ReadByte();

			return;
		case TE_TRACER:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();

			return;
		case TE_LIGHTNING:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadShort();

			return;
		case TE_BEAMENTS:
			MSG_ReadShort();
			MSG_ReadShort();
			MSG_ReadShort();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();

			return;
		case TE_SPARKS:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();

			return;
		case TE_LAVASPLASH:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();

			return;
		case TE_TELEPORT:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();

			return;
		case TE_EXPLOSION2:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadByte();
			MSG_ReadByte();

			return;
		case TE_BSPDECAL:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadShort();

			if (MSG_ReadShort() != 0)
				MSG_ReadShort();

			return;
		case TE_IMPLOSION:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();

			return;
		case TE_SPRITETRAIL:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadShort();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();

			return;
		case TE_BEAM:
			return;
		case TE_SPRITE:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadShort();
			MSG_ReadByte();
			MSG_ReadByte();

			return;
		case TE_BEAMSPRITE:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadShort();
			MSG_ReadShort();

			return;
		case TE_BEAMTORUS:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadShort();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();

			return;
		case TE_BEAMDISK:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadShort();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();

			return;
		case TE_BEAMCYLINDER:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadShort();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();

			return;
		case TE_BEAMFOLLOW:
			MSG_ReadShort();
			MSG_ReadShort();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();

			return;
		case TE_GLOWSPRITE:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadShort();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();

			return;
		case TE_BEAMRING:
			MSG_ReadShort();
			MSG_ReadShort();
			MSG_ReadShort();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();

			return;
		case TE_STREAK_SPLASH:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadByte();
			MSG_ReadShort();
			MSG_ReadShort();
			MSG_ReadShort();

			return;
		case TE_BEAMHOSE:
			return;
		case TE_DLIGHT:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();

			return;
		case TE_ELIGHT:
			MSG_ReadShort();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadCoord();

			return;
		/*case TE_TEXTMESSAGE:
			return;*/
		case TE_LINE:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadShort();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();

			return;
		case TE_BOX:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadShort();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();

			return;
		case TE_KILLBEAM:
			MSG_ReadShort();

			return;
		case TE_LARGEFUNNEL:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadShort();
			MSG_ReadShort();

			return;
		case TE_BLOODSTREAM:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadByte();
			MSG_ReadByte();

			return;
		case TE_SHOWLINE:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();

			return;
		case TE_BLOOD:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadByte();
			MSG_ReadByte();

			return;
		case TE_DECAL:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadByte();
			MSG_ReadShort();

			return;
		case TE_FIZZ:
			MSG_ReadShort();
			MSG_ReadShort();
			MSG_ReadByte();

			return;
		case TE_MODEL:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadAngle();
			MSG_ReadShort();
			MSG_ReadByte();
			MSG_ReadByte();

			return;
		case TE_EXPLODEMODEL:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadShort();
			MSG_ReadShort();
			MSG_ReadByte();

			return;
		case TE_BREAKMODEL:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadByte();
			MSG_ReadShort();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();

			return;
		case TE_GUNSHOTDECAL:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadShort();
			MSG_ReadByte();

			return;
		case TE_SPRITE_SPRAY:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadShort();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();

			return;
		case TE_ARMOR_RICOCHET:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadByte();

			return;
		case TE_PLAYERDECAL:
			MSG_ReadByte();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadShort();
			MSG_ReadByte();

			return;
		case TE_BUBBLES:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadShort();
			MSG_ReadByte();
			MSG_ReadCoord();

			return;
		case TE_BUBBLETRAIL:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadShort();
			MSG_ReadByte();
			MSG_ReadCoord();

			return;
		case TE_BLOODSPRITE:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadShort();
			MSG_ReadShort();
			MSG_ReadByte();
			MSG_ReadByte();

			return;
		case TE_WORLDDECAL:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadByte();

			return;
		case TE_WORLDDECALHIGH:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadByte();

			return;
		case TE_DECALHIGH:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadByte();
			MSG_ReadShort();

			return;
		case TE_PROJECTILE:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadShort();
			MSG_ReadByte();
			MSG_ReadByte();

			return;
		case TE_SPRAY:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadShort();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();

			return;
		case TE_PLAYERSPRITES:
			MSG_ReadByte();
			MSG_ReadShort();
			MSG_ReadByte();
			MSG_ReadByte();

			return;
		case TE_PARTICLEBURST:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadShort();
			MSG_ReadByte();
			MSG_ReadByte();

			return;
		case TE_FIREFIELD:
			MSG_ReadCoord();
			MSG_ReadShort();
			MSG_ReadShort();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();

			return;
		case TE_PLAYERATTACHMENT:
			MSG_ReadByte();
			MSG_ReadCoord();
			MSG_ReadShort();
			MSG_ReadShort();

			return;
		case TE_KILLPLAYERATTACHMENTS:
			MSG_ReadByte();

			return;
		case TE_MULTIGUNSHOT:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadByte();
			MSG_ReadByte();

			return;
		case TE_USERTRACER:
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadCoord();
			MSG_ReadByte();
			MSG_ReadByte();
			MSG_ReadByte();

			return;
		}
	}

	*msg_readcount = backup;

	original_CL_Parse_TempEntity();
}

void null_function(void) {
	/*char* argv0 = gEngfuncs->Cmd_Argv(0);

	Log("[???] command: %s [Blocked]\n", argv0);*/
}

int handle_exception(unsigned long code, struct _EXCEPTION_POINTERS* information) {
	void* exception_address = information->ExceptionRecord->ExceptionAddress;

	void* inaccessible_data_address = nullptr;
	char* operation = nullptr;
	unsigned long ntstatus_code = 0;

	if (code == EXCEPTION_ACCESS_VIOLATION || code == EXCEPTION_IN_PAGE_ERROR) {
		operation = (information->ExceptionRecord->ExceptionInformation[0] == 0) ? (char*)"read" : (char*)"written";
		inaccessible_data_address = (void*)information->ExceptionRecord->ExceptionInformation[1];
	}

	if (code == EXCEPTION_IN_PAGE_ERROR)
		ntstatus_code = (unsigned long)information->ExceptionRecord->ExceptionInformation[2];

	DWORD_PTR arguments1[] = { (DWORD_PTR)exception_address, (DWORD_PTR)inaccessible_data_address, (DWORD_PTR)operation };
	DWORD_PTR arguments2[] = { (DWORD_PTR)exception_address, (DWORD_PTR)inaccessible_data_address, (DWORD_PTR)ntstatus_code };

	DWORD_PTR* arguments = nullptr;

	if (code == EXCEPTION_ACCESS_VIOLATION)
		arguments = arguments1;
	else if (code == EXCEPTION_IN_PAGE_ERROR)
		arguments = arguments2;

	char message1[1024];

	FormatMessage(
		FORMAT_MESSAGE_FROM_HMODULE |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		GetModuleHandle("ntdll.dll"),
		code,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		message1,
		ARRAYSIZE(message1), nullptr);

	va_list args;
	char message2[1024];

	va_start(args, message1);
	vsprintf(message2, message1, (va_list)arguments);
	va_end(args);

	Warning(message2);

	return EXCEPTION_EXECUTE_HANDLER;
}

void Host_Frame__Host_Frame_hook(float time) {
	if (!catch_Host_Frame_exceptions)
		return original__Host_Frame(time);

	__try {
		original__Host_Frame(time);
	} __except (handle_exception(GetExceptionCode(), GetExceptionInformation())) {
		;
	}
}

void Host_Say_f_hook(void) {
	if (chat_color == 0)
		return original_Host_Say_f();

	bool quote_beginning = (*cmd_args)[0] == '"';

	size_t cmd_args_len = strlen(*cmd_args);
	bool quote_end = (*cmd_args)[cmd_args_len - 1] == '"';

	std::string new_cmd_args;

	if (quote_beginning && quote_end)
		new_cmd_args += '"';

	const char colors[] = { '\x01', '\x03', '\x04' };

	if (chat_color == 1) {
		new_cmd_args += colors[1];
		new_cmd_args += (quote_beginning && quote_end) ? (*cmd_args + 1) : *cmd_args;
	}
	else if (chat_color == 2) {
		new_cmd_args += colors[2];
		new_cmd_args += (quote_beginning && quote_end) ? (*cmd_args + 1) : *cmd_args;
	}
	else if (chat_color == 3) {
		int j = generate_random_number(0, 2);

		for (size_t i = ((quote_beginning && quote_end) ? 1 : 0); i < cmd_args_len; ++i) {
			if ((quote_beginning && quote_end)) {
				if (i != cmd_args_len - 1)
					new_cmd_args += colors[j++ % ARRAYSIZE(colors)];
			} else
				new_cmd_args += colors[j++ % ARRAYSIZE(colors)];
				
			new_cmd_args += (*cmd_args)[i];
		}
	}

	if (!quote_end)
		new_cmd_args = '"' + new_cmd_args + '"';

	*cmd_args = (char*)new_cmd_args.c_str();

	original_Host_Say_f();
}

void Host_Say_Team_f_hook(void) {
	if (chat_color == 0)
		return original_Host_Say_Team_f();

	bool quote_beginning = (*cmd_args)[0] == '"';

	size_t cmd_args_len = strlen(*cmd_args);
	bool quote_end = (*cmd_args)[cmd_args_len - 1] == '"';

	std::string new_cmd_args;

	if (quote_beginning && quote_end)
		new_cmd_args += '"';

	const char colors[] = { '\x01', '\x03', '\x04' };

	if (chat_color == 1) {
		new_cmd_args += colors[1];
		new_cmd_args += (quote_beginning && quote_end) ? (*cmd_args + 1) : *cmd_args;
	} else if (chat_color == 2) {
		new_cmd_args += colors[2];
		new_cmd_args += (quote_beginning && quote_end) ? (*cmd_args + 1) : *cmd_args;
	} else if (chat_color == 3) {
		int j = generate_random_number(0, 2);

		for (size_t i = ((quote_beginning && quote_end) ? 1 : 0); i < cmd_args_len; ++i) {
			if ((quote_beginning && quote_end)) {
				if (i != cmd_args_len - 1)
					new_cmd_args += colors[j++ % ARRAYSIZE(colors)];
			} else
				new_cmd_args += colors[j++ % ARRAYSIZE(colors)];

			new_cmd_args += (*cmd_args)[i];
		}
	}

	if (!quote_end)
		new_cmd_args = '"' + new_cmd_args + '"';

	*cmd_args = (char*)new_cmd_args.c_str();

	original_Host_Say_Team_f();
}