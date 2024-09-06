#include "menu.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "messages.h"
#include "hlsdk.h"
#include "global.h"
#include <regex>

ImVec2 auto_size(0.0f, 0.0f);
ImVec4 background_color(0.0f, 0.0f, 0.0f, 1.0f);
ImVec4 text_background_color(0.05f, 0.05f, 0.05f, 1.0f);
ImVec4 border_color(0.5f, 0.5f, 0.5f, 1.0f);
ImVec4 button_color(0.0f, 0.0f, 0.0f, 1.0f);
ImVec4 button_hovered_color(0.3f, 0.3f, 0.3f, 1.0f);
ImVec4 button_active_color(0.2f, 0.2f, 0.2f, 1.0f);
ImVec4 invisible(0.0f, 0.0f, 0.0f, 0.0f);

ImVec2 button_size2(120.0f, 25.0f);

ImFont* header1_font;
ImFont* header2_font;
ImFont* body_font;

struct WhiteListedCommand {
	std::string command_name;
	bool regex;
	bool forward_to_server;
};

std::vector<WhiteListedCommand> commands_whitelist;

int is_in_commands_whitelist(const char* const command_name) {
	if (command_name == nullptr || command_name[0] == '\0')
		return 0;

	for (auto& item : commands_whitelist) {
		if ((!item.regex && stricmp(item.command_name.c_str(), command_name) == 0) ||
			(item.regex && std::regex_match(command_name, std::regex(item.command_name))))
			return item.forward_to_server ? 2 : 1;
	}

	return 0;
}

int get_whitelisted_command(const char* const command_name, WhiteListedCommand*& output) {
	if (command_name == nullptr || command_name[0] == '\0')
		return -1;

	for (auto& item : commands_whitelist) {
		if (stricmp(item.command_name.c_str(), command_name) == 0) {
			output = &item;

			return 0;
		}
	}

	return 1;
}

bool scroll_to_bottom2;

void add_command_to_whitelist(const char* const command_name, bool regex, bool forward_to_server) {
	WhiteListedCommand* whitelisted_command;
	int ret = get_whitelisted_command(command_name, whitelisted_command);

	if (ret == 1) {
		WhiteListedCommand temp;
		temp.command_name = command_name;
		temp.regex = regex;
		temp.forward_to_server = forward_to_server;

		commands_whitelist.push_back(temp);

		scroll_to_bottom2 = true;
	}

	if (ret == 0) {
		whitelisted_command->regex = regex;
		whitelisted_command->forward_to_server = forward_to_server;
	}
}

struct ListedUnknownCommand {
	std::string command_name;
	bool regex;
};

std::vector<ListedUnknownCommand> unknown_commands_list;

bool is_in_unknown_commands_list(const char* const command_name) {
	if (command_name == nullptr || command_name[0] == '\0')
		return false;

	for (auto& item : unknown_commands_list) {
		if ((!item.regex && stricmp(item.command_name.c_str(), command_name) == 0) ||
			(item.regex && std::regex_match(command_name, std::regex(item.command_name))))
			return true;
	}

	return false;
}

int get_listed_unknown_command(const char* const command_name, ListedUnknownCommand*& output) {
	if (command_name == nullptr || command_name[0] == '\0')
		return -1;

	for (auto& item : unknown_commands_list) {
		if (stricmp(item.command_name.c_str(), command_name) == 0) {
			output = &item;

			return 0;
		}
	}

	return 1;
}

bool scroll_to_bottom3;

void add_unknown_command_to_list(const char* const command_name, bool regex) {
	ListedUnknownCommand* listed_unknown_command;
	int ret = get_listed_unknown_command(command_name, listed_unknown_command);

	if (ret == 1) {
		ListedUnknownCommand temp;
		temp.command_name = command_name;
		temp.regex = regex;

		unknown_commands_list.push_back(temp);

		scroll_to_bottom3 = true;
	}

	if (ret == 0)
		listed_unknown_command->regex = regex;
}

struct ListedFakeCVar {
	std::string cvar_name;
	std::string value;
	bool regex;
};

std::vector<ListedFakeCVar> fake_cvars_list;

bool is_in_fake_cvars_list(const char* const cvar_name, std::string& output) {
	if (cvar_name == nullptr || cvar_name[0] == '\0')
		return false;

	for (auto& item : fake_cvars_list) {
		if ((!item.regex && stricmp(item.cvar_name.c_str(), cvar_name) == 0) ||
			(item.regex && std::regex_match(cvar_name, std::regex(item.cvar_name)))) {

			output = item.value;

			return true;
		}
	}

	return false;
}

int get_listed_fake_cvar(const char* const cvar_name, ListedFakeCVar*& output) {
	if (cvar_name == nullptr || cvar_name[0] == '\0')
		return -1;

	for (auto& item : fake_cvars_list) {
		if (stricmp(item.cvar_name.c_str(), cvar_name) == 0) {
			output = &item;

			return 0;
		}
	}

	return 1;
}

bool scroll_to_bottom4;

void add_fake_cvar_to_list(const char* const cvar_name, const char* const value, bool regex) {
	ListedFakeCVar* listed_fake_cvar;
	int ret = get_listed_fake_cvar(cvar_name, listed_fake_cvar);

	if (ret == 1) {
		ListedFakeCVar temp;
		temp.cvar_name = cvar_name;
		temp.value = value;
		temp.regex = regex;

		fake_cvars_list.push_back(temp);

		scroll_to_bottom4 = true;
	}

	if (ret == 0) {
		listed_fake_cvar->value = value;
		listed_fake_cvar->regex = regex;
	}
}

std::vector<std::string> extensions_whitelist;

int is_in_extensions_whitelist(const char* const extension) {
	if (extension == nullptr || extension[0] == '\0')
		return -1;

	for (auto& item : extensions_whitelist) {
		if (stricmp(item.c_str(), extension) == 0)
			return 0;
	}

	return 1;
}

bool scroll_to_bottom5;

void add_extension_to_whitelist(const char* const extension) {
	if (is_in_extensions_whitelist(extension) == 1) {
		extensions_whitelist.push_back(extension);

		scroll_to_bottom5 = true;
	}
}

struct ListedConsistency {
	std::string filename;
	unsigned char digest[16];
};

std::vector<ListedConsistency> consistency_list;

bool is_in_consistency_list(const char* const filename, unsigned char* output) {
	if (filename == nullptr || filename[0] == '\0')
		return false;

	for (auto& item : consistency_list) {
		if (stricmp(item.filename.c_str(), filename) == 0) {

			memcpy(output, item.digest, ARRAYSIZE(item.digest));

			return true;
		}
	}

	return false;
}

int get_listed_consistency(const char* const filename, ListedConsistency*& output) {
	if (filename == nullptr || filename[0] == '\0')
		return -1;

	for (auto& item : consistency_list) {
		if (stricmp(item.filename.c_str(), filename) == 0) {
			output = &item;

			return 0;
		}
	}

	return 1;
}

bool scroll_to_bottom6;

void add_consistency_to_list(const char* const filename, const char* const md5) {
	ListedConsistency* listed_consistency;
	int ret = get_listed_consistency(filename, listed_consistency);

	if (ret == -1)
		return;

	auto hex_to_byte = [](const char* const hex) -> unsigned char {
		unsigned char ret = '\0';

		if (hex[0] >= '0' && hex[0] <= '9')
			ret = hex[0] - '0';
		else if (hex[0] >= 'A' && hex[0] <= 'Z')
			ret = hex[0] - 'A' + 10;
		else if (hex[0] >= 'a' && hex[0] <= 'z')
			ret = hex[0] - 'a' + 10;

		ret <<= 4;

		if (hex[1] >= '0' && hex[1] <= '9')
			ret |= hex[1] - '0';
		else if (hex[1] >= 'A' && hex[1] <= 'Z')
			ret |= hex[1] - 'A' + 10;
		else if (hex[1] >= 'a' && hex[1] <= 'z')
			ret |= hex[1] - 'a' + 10;

		return ret;
	};

	unsigned char digest[16] = { };

	if (md5[0] != '\0') {
		for (int i = 0; i < 16; ++i)
			digest[i] = hex_to_byte(md5 + i * 2);
	}

	if (ret == 1) {
		ListedConsistency temp;
		temp.filename = filename;
		memcpy(temp.digest, digest, ARRAYSIZE(temp.digest));

		consistency_list.push_back(temp);

		scroll_to_bottom6 = true;

		return;
	}

	if (ret == 0)
		memcpy(listed_consistency->digest, digest, ARRAYSIZE(listed_consistency->digest));
}

float get_item_width(int items) {
	return (ImGui::GetContentRegionAvail().x - (items - 1) * ImGui::GetStyle().ItemSpacing.x) / items;
}

bool follow_redirects;
bool block_motd;
bool random_cdkey;
bool random_steamid;
char custom_cdkey[33];
long custom_steamid;
bool auto_retry;
bool remove_hud_messages;
bool remove_effects;
bool remove_non_player_entities;
bool catch_Host_Frame_exceptions;
int chat_color;

bool InputIntWithLimits(const char* label, long* v, long min = INT_MIN, long max = INT_MAX) {
	unsigned long step = 1;
	unsigned long step_fast = 100;
	bool ret = ImGui::InputScalar(label, ImGuiDataType_S32, (void*)v, &step, &step_fast, "%d");

	if (*v < min)
		*v = min;
	else if (*v > max)
		*v = max;

	return ret;
};

bool InputUIntWithLimits(const char* label, unsigned long* v, unsigned long min = 0, unsigned long max = UINT_MAX) {
	unsigned long step = 1;
	unsigned long step_fast = 100;
	bool ret = ImGui::InputScalar(label, ImGuiDataType_U32, (void*)v, &step, &step_fast, "%d");

	if (*v < min)
		*v = min;
	else if (*v > max)
		*v = max;

	return ret;
};

bool show_player_info_window = false;
int player_info_index = 0;

void DrawPlayerInfoWindow() {
	if (!show_menu_window || !show_player_info_window)
		return;

	ImGui::PushStyleColor(ImGuiCol_WindowBg, background_color);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 10.0f));
	ImGui::PushFont(body_font);
	ImGui::PushStyleColor(ImGuiCol_TitleBg, background_color);
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, background_color);

	ImGui::Begin("Player info", &show_player_info_window, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::PushStyleColor(ImGuiCol_Button, button_color);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_hovered_color);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_active_color);
	ImGui::PushStyleColor(ImGuiCol_Border, border_color);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);

	bool copy_button = ImGui::Button("Copy##Button4", button_size2);

	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, text_background_color);
	ImGui::PushStyleColor(ImGuiCol_Border, border_color);

	ImGui::BeginChild("##Child13", auto_size, ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0.0f, 0.0f));

	if (copy_button)
		ImGui::LogToClipboard();

	ImGui::TextUnformatted("Userinfo:");

	char* s = nMax->players[player_info_index].userinfo;

	char key[MAX_KV_LEN];
	char value[MAX_KV_LEN];
	char* c;
	int nCount;

	while (*s != '\0') {
		if (*s == '\\')
			s++;

		nCount = 0;
		c = key;

		while (*s != '\\') {
			if (*s == '\0')
				break;

			if (nCount >= MAX_KV_LEN) {
				s++;

				continue;
			}

			*c++ = *s++;
			nCount++;
		}

		*c = '\0';

		if (*s != '\0')
			s++;

		nCount = 0;
		c = value;

		while (*s != '\\') {
			if (*s == '\0')
				break;

			if (nCount >= MAX_KV_LEN) {
				s++;

				continue;
			}

			*c++ = *s++;
			nCount++;
		}

		*c = '\0';

		ImGui::Text("\t%s \"%s\"", key, value);
	}

	ImGui::PopStyleVar();

	ImGui::EndChild();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopFont();
	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
}

void DrawTab1() {
	static float listbox_height = 200.0f;

	float listbox_width;
	float width1;
	float width2;

	float child_width = get_item_width(3);

	{
		ImGui::PushStyleColor(ImGuiCol_ChildBg, background_color);

		ImGui::BeginChild("##Child4", ImVec2(child_width, 0.0f), ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);

		{
			ImGui::PushFont(header2_font);

			ImGui::TextUnformatted("Commands whitelist:");

			ImGui::PopFont();

			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
			ImGui::PushStyleColor(ImGuiCol_Border, border_color);
			ImGui::PushStyleColor(ImGuiCol_FrameBg, text_background_color);

			ImGui::PushItemWidth(-FLT_MIN);

			ImGui::BeginChild("##ListBox1", ImVec2(0.0f, listbox_height), ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar);

			listbox_width = ImGui::GetWindowWidth() - 2 * ImGui::GetStyle().FramePadding.x - ImGui::GetStyle().ScrollbarSize;
			static float max_selectable_width = 0.0f;
			width2 = 0.0f;

			ImGui::PushStyleColor(ImGuiCol_Header, button_color);
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, button_hovered_color);
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, button_active_color);

			std::string label;

			for (auto it = commands_whitelist.begin(); it != commands_whitelist.end(); ++it) {
				label = (*it).command_name;

				if ((*it).regex) {
					label += " | RE";

					if ((*it).forward_to_server)
						label += " | F2S";
				} else if ((*it).forward_to_server)
					label += " | F2S";

				width1 = ImGui::CalcTextSize(label.c_str()).x;

				if (width1 > width2)
					width2 = width1;

				if (ImGui::Selectable(label.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(max_selectable_width, 0.0f)) && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
					commands_whitelist.erase(it);
					--it;
				}
			}

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			if (width2 > listbox_width) {
				if (max_selectable_width == 0.0f)
					max_selectable_width = width2;
				else if (width2 < max_selectable_width)
					max_selectable_width = width2;
			}
			else
				max_selectable_width = 0.0f;

			if (scroll_to_bottom2 && ImGui::GetScrollY() == ImGui::GetScrollMaxY())
				ImGui::SetScrollHereY(1.0f);

			scroll_to_bottom2 = false;

			ImGui::EndChild();

			ImGui::PushStyleColor(ImGuiCol_FrameBg, button_color);
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, button_hovered_color);
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, button_active_color);
			ImGui::PushStyleColor(ImGuiCol_CheckMark, border_color);

			static bool regex = false;

			ImGui::Checkbox("RE", &regex);

			static bool forward_to_server = false;

			ImGui::SameLine();
			ImGui::Checkbox("F2S", &forward_to_server);

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			static char command_name[256] = { };

			ImGui::SameLine();
			if (ImGui::InputTextWithHint("##InputText1", "Command name", command_name, IM_ARRAYSIZE(command_name), ImGuiInputTextFlags_EnterReturnsTrue)) {
				add_command_to_whitelist(command_name, regex, forward_to_server);
				command_name[0] = '\0';

				ImGui::SetKeyboardFocusHere(-1);
			}

			ImGui::PopItemWidth();

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();
		}

		ImGui::EndChild();

		ImGui::PopStyleColor();
	}

	{
		ImGui::PushStyleColor(ImGuiCol_ChildBg, background_color);

		ImGui::SameLine();
		ImGui::BeginChild("##Child5", ImVec2(child_width, 0.0f), ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);

		{
			ImGui::PushFont(header2_font);

			ImGui::TextUnformatted("Unknown Commands list:");

			ImGui::PopFont();

			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
			ImGui::PushStyleColor(ImGuiCol_Border, border_color);
			ImGui::PushStyleColor(ImGuiCol_FrameBg, text_background_color);

			ImGui::PushItemWidth(-FLT_MIN);

			ImGui::BeginChild("##ListBox2", ImVec2(0.0f, listbox_height), ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar);

			listbox_width = ImGui::GetWindowWidth() - 2 * ImGui::GetStyle().FramePadding.x - ImGui::GetStyle().ScrollbarSize;
			static float max_selectable_width = 0.0f;
			width2 = 0.0f;

			ImGui::PushStyleColor(ImGuiCol_Header, button_color);
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, button_hovered_color);
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, button_active_color);

			std::string label;

			for (auto it = unknown_commands_list.begin(); it != unknown_commands_list.end(); ++it) {
				label = (*it).command_name;

				if ((*it).regex)
					label += " | RE";

				width1 = ImGui::CalcTextSize(label.c_str()).x;

				if (width1 > width2)
					width2 = width1;

				if (ImGui::Selectable(label.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(max_selectable_width, 0.0f)) && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
					unknown_commands_list.erase(it);
					--it;
				}
			}

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			if (width2 > listbox_width) {
				if (max_selectable_width == 0.0f)
					max_selectable_width = width2;
				else if (width2 < max_selectable_width)
					max_selectable_width = width2;
			} else
				max_selectable_width = 0.0f;

			if (scroll_to_bottom3 && ImGui::GetScrollY() == ImGui::GetScrollMaxY())
				ImGui::SetScrollHereY(1.0f);

			scroll_to_bottom3 = false;

			ImGui::EndChild();

			ImGui::PushStyleColor(ImGuiCol_FrameBg, button_color);
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, button_hovered_color);
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, button_active_color);
			ImGui::PushStyleColor(ImGuiCol_CheckMark, border_color);

			static bool regex = false;

			ImGui::Checkbox("RE", &regex);

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			static char command_name[256] = { };

			ImGui::SameLine();
			if (ImGui::InputTextWithHint("##InputText2", "Unknown Command name", command_name, IM_ARRAYSIZE(command_name), ImGuiInputTextFlags_EnterReturnsTrue)) {
				add_unknown_command_to_list(command_name, regex);
				command_name[0] = '\0';

				ImGui::SetKeyboardFocusHere(-1);
			}

			ImGui::PopItemWidth();

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();
		}

		ImGui::EndChild();

		ImGui::PopStyleColor();
	}

	static float input_text_width;

	{
		ImGui::PushStyleColor(ImGuiCol_ChildBg, background_color);

		ImGui::SameLine();
		ImGui::BeginChild("##Child6", ImVec2(child_width, 0.0f), ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);

		{
			ImGui::PushFont(header2_font);

			ImGui::TextUnformatted("Fake CVars list:");

			ImGui::PopFont();

			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
			ImGui::PushStyleColor(ImGuiCol_Border, border_color);
			ImGui::PushStyleColor(ImGuiCol_FrameBg, text_background_color);

			ImGui::PushItemWidth(-FLT_MIN);

			ImGui::BeginChild("##ListBox3", ImVec2(0.0f, listbox_height), ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar);

			listbox_width = ImGui::GetWindowWidth() - 2 * ImGui::GetStyle().FramePadding.x - ImGui::GetStyle().ScrollbarSize;
			static float max_selectable_width = 0.0f;
			width2 = 0.0f;

			ImGui::PushStyleColor(ImGuiCol_Header, button_color);
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, button_hovered_color);
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, button_active_color);

			std::string label;

			for (auto it = fake_cvars_list.begin(); it != fake_cvars_list.end(); ++it) {
				label = (*it).cvar_name + " \"" + (*it).value + "\"";

				if ((*it).regex)
					label += " | RE";

				width1 = ImGui::CalcTextSize(label.c_str()).x;

				if (width1 > width2)
					width2 = width1;

				if (ImGui::Selectable(label.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(max_selectable_width, 0.0f)) && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
					fake_cvars_list.erase(it);
					--it;
				}
			}

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			if (width2 > listbox_width) {
				if (max_selectable_width == 0.0f)
					max_selectable_width = width2;
				else if (width2 < max_selectable_width)
					max_selectable_width = width2;
			} else
				max_selectable_width = 0.0f;

			if (scroll_to_bottom4 && ImGui::GetScrollY() == ImGui::GetScrollMaxY())
				ImGui::SetScrollHereY(1.0f);

			scroll_to_bottom4 = false;

			ImGui::EndChild();

			ImGui::PushStyleColor(ImGuiCol_FrameBg, button_color);
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, button_hovered_color);
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, button_active_color);
			ImGui::PushStyleColor(ImGuiCol_CheckMark, border_color);

			static bool regex = false;

			ImGui::Checkbox("RE", &regex);

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			static char cvar_name[256] = { };
			static char value[256] = { };

			ImGui::SameLine();
			ImGui::BeginChild("##Child7", ImVec2(0.0f, 0.0f));

			input_text_width = get_item_width(2);
			ImGui::PushItemWidth(input_text_width);

			bool input_text3 = ImGui::InputTextWithHint("##InputText3", "CVar name", cvar_name, IM_ARRAYSIZE(cvar_name), ImGuiInputTextFlags_EnterReturnsTrue);

			static bool input_text4 = false;

			if (input_text3 || input_text4)
				ImGui::SetKeyboardFocusHere(-1);

			ImGui::SameLine();
			input_text4 = ImGui::InputTextWithHint("##InputText4", "Value", value, IM_ARRAYSIZE(value), ImGuiInputTextFlags_EnterReturnsTrue);

			ImGui::PopItemWidth();

			if (input_text3 || input_text4) {
				add_fake_cvar_to_list(cvar_name, value, regex);

				cvar_name[0] = '\0';
				value[0] = '\0';
			}

			ImGui::EndChild();

			ImGui::PopItemWidth();

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();
		}

		ImGui::EndChild();

		ImGui::PopStyleColor();
	}

	{
		ImGui::PushStyleColor(ImGuiCol_ChildBg, background_color);

		ImGui::BeginChild("##Child8", ImVec2(child_width, 0.0f), ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);

		{
			ImGui::PushFont(header2_font);

			ImGui::TextUnformatted("Extensions whitelist:");

			ImGui::PopFont();

			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
			ImGui::PushStyleColor(ImGuiCol_Border, border_color);
			ImGui::PushStyleColor(ImGuiCol_FrameBg, text_background_color);

			ImGui::PushItemWidth(-FLT_MIN);

			ImGui::BeginChild("##ListBox4", ImVec2(0.0f, listbox_height), ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar);

			listbox_width = ImGui::GetWindowWidth() - 2 * ImGui::GetStyle().FramePadding.x - ImGui::GetStyle().ScrollbarSize;
			static float max_selectable_width = 0.0f;
			width2 = 0.0f;

			ImGui::PushStyleColor(ImGuiCol_Header, button_color);
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, button_hovered_color);
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, button_active_color);

			for (auto it = extensions_whitelist.begin(); it != extensions_whitelist.end(); ++it) {
				width1 = ImGui::CalcTextSize((*it).c_str()).x;

				if (width1 > width2)
					width2 = width1;

				if (ImGui::Selectable((*it).c_str(), false, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(max_selectable_width, 0.0f)) && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
					extensions_whitelist.erase(it);
					--it;
				}
			}

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			if (width2 > listbox_width) {
				if (max_selectable_width == 0.0f)
					max_selectable_width = width2;
				else if (width2 < max_selectable_width)
					max_selectable_width = width2;
			} else
				max_selectable_width = 0.0f;

			if (scroll_to_bottom5 && ImGui::GetScrollY() == ImGui::GetScrollMaxY())
				ImGui::SetScrollHereY(1.0f);

			scroll_to_bottom5 = false;

			ImGui::EndChild();

			static char extension[5] = { };

			if (ImGui::InputTextWithHint("##InputText5", "Extension", extension, IM_ARRAYSIZE(extension), ImGuiInputTextFlags_EnterReturnsTrue)) {
				add_extension_to_whitelist(extension);
				extension[0] = '\0';

				ImGui::SetKeyboardFocusHere(-1);
			}

			ImGui::PopItemWidth();

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();
		}

		ImGui::EndChild();

		ImGui::PopStyleColor();
	}

	{
		ImGui::PushStyleColor(ImGuiCol_ChildBg, background_color);

		ImGui::SameLine();
		ImGui::BeginChild("##Child9", ImVec2(0.0f, 0.0f), ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);

		{
			ImGui::PushFont(header2_font);

			ImGui::TextUnformatted("Consistency list:");

			ImGui::PopFont();

			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
			ImGui::PushStyleColor(ImGuiCol_Border, border_color);
			ImGui::PushStyleColor(ImGuiCol_FrameBg, text_background_color);

			ImGui::PushItemWidth(-FLT_MIN);

			ImGui::BeginChild("##ListBox5", ImVec2(0.0f, listbox_height), ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar);

			listbox_width = ImGui::GetWindowWidth() - 2 * ImGui::GetStyle().FramePadding.x - ImGui::GetStyle().ScrollbarSize;
			static float max_selectable_width = 0.0f;
			width2 = 0.0f;

			ImGui::PushStyleColor(ImGuiCol_Header, button_color);
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, button_hovered_color);
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, button_active_color);

			std::string label;

			for (auto it = consistency_list.begin(); it != consistency_list.end(); ++it) {
				label = (*it).filename + " \"";

				static unsigned char empty[16] = { };
				char temp[33] = { };

				if (memcmp((*it).digest, empty, ARRAYSIZE((*it).digest)) != 0) {
					snprintf(temp, ARRAYSIZE(temp), "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
						(*it).digest[0], (*it).digest[1], (*it).digest[2], (*it).digest[3], (*it).digest[4], (*it).digest[5], (*it).digest[6], (*it).digest[7],
						(*it).digest[8], (*it).digest[9], (*it).digest[10], (*it).digest[11], (*it).digest[12], (*it).digest[13], (*it).digest[14], (*it).digest[15]);
				}

				label += temp;
				label += "\"";

				width1 = ImGui::CalcTextSize(label.c_str()).x;

				if (width1 > width2)
					width2 = width1;

				if (ImGui::Selectable(label.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(max_selectable_width, 0.0f)) && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
					consistency_list.erase(it);
					--it;
				}
			}

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			if (width2 > listbox_width) {
				if (max_selectable_width == 0.0f)
					max_selectable_width = width2;
				else if (width2 < max_selectable_width)
					max_selectable_width = width2;
			} else
				max_selectable_width = 0.0f;

			if (scroll_to_bottom6 && ImGui::GetScrollY() == ImGui::GetScrollMaxY())
				ImGui::SetScrollHereY(1.0f);

			scroll_to_bottom6 = false;

			ImGui::EndChild();

			ImGui::PopItemWidth();

			static char filename[MAX_PATH] = { };
			static char md5[33] = { };

			input_text_width = get_item_width(2);

			ImGui::PushItemWidth(input_text_width);

			bool input_text5 = ImGui::InputTextWithHint("##InputText6", "Filename", filename, IM_ARRAYSIZE(filename), ImGuiInputTextFlags_EnterReturnsTrue);

			static bool input_text6 = false;

			if (input_text5 || input_text6)
				ImGui::SetKeyboardFocusHere(-1);

			ImGui::SameLine();
			input_text6 = ImGui::InputTextWithHint("##InputText7", "MD5 checksum", md5, IM_ARRAYSIZE(md5), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsHexadecimal);

			ImGui::PopItemWidth();

			if (input_text5 || input_text6) {
				add_consistency_to_list(filename, md5);

				filename[0] = '\0';
				memset(md5, 0, IM_ARRAYSIZE(md5));
			}

			ImGui::PushFont(header2_font);

			ImGui::TextUnformatted("Calculate MD5 checksum:");

			ImGui::PopFont();

			static char filename2[MAX_PATH] = { };
			static char md52[33] = { };

			ImGui::PushItemWidth(input_text_width);

			if (ImGui::InputTextWithHint("##InputText8", "Filename", filename2, IM_ARRAYSIZE(filename2))) {
				unsigned char digest[16];

				if (original_MD5_Hash_File(digest, filename2, FALSE, FALSE, nullptr))
					snprintf(md52, ARRAYSIZE(md52), "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
						digest[0], digest[1], digest[2], digest[3], digest[4], digest[5], digest[6], digest[7],
						digest[8], digest[9], digest[10], digest[11], digest[12], digest[13], digest[14], digest[15]);
				else
					strcpy(md52, "File not found.");
			}

			if (filename2[0] == '\0')
				md52[0] = '\0';

			ImGui::SameLine();
			bool input_text8 = ImGui::InputTextWithHint("##InputText9", "MD5 checksum", md52, IM_ARRAYSIZE(md52), ImGuiInputTextFlags_ReadOnly);

			ImGui::PopItemWidth();

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();
		}

		ImGui::EndChild();

		ImGui::PopStyleColor();
	}

	child_width = get_item_width(2);

	{
		ImGui::PushStyleColor(ImGuiCol_ChildBg, background_color);

		ImGui::BeginChild("##Child10", ImVec2(child_width, 0.0f), ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);

		{
			ImGui::PushFont(header2_font);

			ImGui::TextUnformatted("Settings:");

			ImGui::PopFont();

			ImGui::PushStyleColor(ImGuiCol_Border, border_color);

			ImGui::PushItemWidth(-FLT_MIN);

			ImGui::BeginChild("##Child11", ImVec2(0.0f, listbox_height), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);

			ImGui::PushStyleColor(ImGuiCol_FrameBg, button_color);
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, button_hovered_color);
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, button_active_color);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
			ImGui::PushStyleColor(ImGuiCol_CheckMark, border_color);

			ImGui::Checkbox("Follow redirects", &follow_redirects);
			ImGui::Checkbox("Block MOTD", &block_motd);

			ImGui::Checkbox("Random CDKey", &random_cdkey);

			ImGui::BeginDisabled(random_cdkey);

			ImGui::PushItemWidth(ImGui::CalcTextSize("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW").x + 2 * ImGui::GetStyle().FramePadding.x);

			ImGui::SameLine();
			ImGui::InputTextWithHint("##InputText10", "CDKey", custom_cdkey, IM_ARRAYSIZE(custom_cdkey));

			ImGui::PopItemWidth();

			ImGui::EndDisabled();

			ImGui::Checkbox("Random SteamID", &random_steamid);

			ImGui::BeginDisabled(random_steamid);

			ImGui::PushStyleColor(ImGuiCol_Button, button_color);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_hovered_color);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_active_color);

			ImGui::SameLine();
			InputIntWithLimits("##InputInt1", &custom_steamid, 0);

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			ImGui::EndDisabled();

			ImGui::Checkbox("Auto retry", &auto_retry);
			ImGui::Checkbox("Remove HUD messages", &remove_hud_messages);
			ImGui::Checkbox("Remove effects", &remove_effects);
			ImGui::Checkbox("Remove non-player entities", &remove_non_player_entities);
			ImGui::Checkbox("Catch Host_Frame exceptions", &catch_Host_Frame_exceptions);

			const char* items[] = { "Normal color", "Team color", "Green", "Mixed" };

			ImGui::PushStyleColor(ImGuiCol_Button, button_color);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_hovered_color);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_active_color);
			
			ImGui::PushItemWidth(ImGui::CalcTextSize("Normal color").x + ImGui::GetFrameHeight() + 2 * ImGui::GetStyle().FramePadding.x);

			if (ImGui::BeginCombo("Chat color", items[chat_color])) {
				ImGui::PushStyleColor(ImGuiCol_Header, button_color);
				ImGui::PushStyleColor(ImGuiCol_HeaderHovered, button_hovered_color);
				ImGui::PushStyleColor(ImGuiCol_HeaderActive, button_active_color);

				for (int i = 0; i < IM_ARRAYSIZE(items); ++i) {
					bool selected = (i == chat_color);

					if (ImGui::Selectable(items[i], selected)) {
						chat_color = i;

						if (selected)
							ImGui::SetItemDefaultFocus();
					}
				}

				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();

				ImGui::EndCombo();
			}

			ImGui::PopItemWidth();

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			ImGui::PopStyleColor();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			ImGui::EndChild();

			ImGui::PopItemWidth();

			ImGui::PopStyleColor();
		}

		ImGui::EndChild();

		ImGui::PopStyleColor();
	}

	{
		ImGui::PushStyleColor(ImGuiCol_ChildBg, background_color);

		ImGui::SameLine();
		ImGui::BeginChild("##Child12", ImVec2(child_width, 0.0f), ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);

		{
			ImGui::PushFont(header2_font);

			ImGui::TextUnformatted("Players:");

			ImGui::PopFont();

			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
			ImGui::PushStyleColor(ImGuiCol_Border, border_color);
			ImGui::PushStyleColor(ImGuiCol_FrameBg, text_background_color);

			ImGui::PushItemWidth(-FLT_MIN);

			ImGui::BeginChild("##ListBox6", ImVec2(0.0f, listbox_height), ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar);

			listbox_width = ImGui::GetWindowWidth() - 2 * ImGui::GetStyle().FramePadding.x - ImGui::GetStyle().ScrollbarSize;
			static float max_selectable_width = 0.0f;
			width2 = 0.0f;

			ImGui::PushStyleColor(ImGuiCol_Header, button_color);
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, button_hovered_color);
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, button_active_color);

			for (int i = 0; i < nMax->maxclients; ++i) {
				if (nMax->players[i].name[0] == '\0')
					continue;

				width1 = ImGui::CalcTextSize(nMax->players[i].name).x;

				if (width1 > width2)
					width2 = width1;
				
				if (ImGui::Selectable(nMax->players[i].name, false, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(max_selectable_width, 0.0f)) && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
					show_player_info_window = true;
					player_info_index = i;
				}
			}

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			if (width2 > listbox_width) {
				if (max_selectable_width == 0.0f)
					max_selectable_width = width2;
				else if (width2 < max_selectable_width)
					max_selectable_width = width2;
			} else
				max_selectable_width = 0.0f;

			ImGui::EndChild();

			ImGui::PopItemWidth();

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();
		}

		ImGui::EndChild();

		ImGui::PopStyleColor();
	}
}

ImGuiTextBuffer logs;
bool scroll_to_bottom1;

void Log(const char* const text, ...) {
	va_list args;
	va_start(args, text);
	logs.appendfv(text, args);
	va_end(args);

	scroll_to_bottom1 = true;
}

void DrawTab2() {
	ImGui::PushStyleColor(ImGuiCol_Button, button_color);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_hovered_color);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_active_color);
	ImGui::PushStyleColor(ImGuiCol_Border, border_color);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);

	bool copy_button = ImGui::Button("Copy##Button3", button_size2);

	ImGui::SameLine();
	if (ImGui::Button("Clear", button_size2))
		logs.clear();

	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, text_background_color);
	ImGui::PushStyleColor(ImGuiCol_Border, border_color);

	ImGui::BeginChild("##Child3", auto_size, ImGuiChildFlags_Border, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0.0f, 0.0f));

	if (copy_button)
		ImGui::LogToClipboard();

	ImGui::TextUnformatted(logs.begin());

	ImGui::PopStyleVar();

	if (scroll_to_bottom1 && ImGui::GetScrollY() == ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);

	scroll_to_bottom1 = false;

	ImGui::EndChild();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

bool show_menu_window = false;

void DrawMenu() {
	if (!show_menu_window)
		return;

	{
		static ImVec2 window1_size(1280.0f, 760.0f);
		ImGui::SetNextWindowSize(window1_size);

		ImGui::PushStyleColor(ImGuiCol_WindowBg, background_color);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 10.0f));

		ImGui::Begin("##Window1", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus);

		{
			ImGui::PushStyleColor(ImGuiCol_Border, border_color);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

			ImGui::BeginChild("##Child1", auto_size, ImGuiChildFlags_Border);

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

			static int current_tab = 0;

			ImGui::PushStyleColor(ImGuiCol_Button, button_color);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_hovered_color);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_active_color);
			ImGui::PushStyleColor(ImGuiCol_Border, border_color);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
			ImGui::PushFont(header1_font);

			static float button_width1 = get_item_width(2);

			static ImVec2 button_size1(button_width1, 40.0f);

			if (ImGui::Button("Protector", button_size1))
				current_tab = 0;

			ImGui::SameLine(0.0f, 0.0f);
			if (ImGui::Button("Logs", button_size1))
				current_tab = 1;

			ImGui::PopFont();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			ImGui::PopStyleVar();

			{
				ImGui::PushStyleColor(ImGuiCol_Border, invisible);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 10.0f));
				ImGui::PushFont(body_font);

				ImGui::BeginChild("##Child2", auto_size, ImGuiChildFlags_Border);

				switch (current_tab) {
				case 0:
					DrawTab1();
					break;
				case 1:
					DrawTab2();
					break;
				}

				ImGui::EndChild();

				ImGui::PopFont();
				ImGui::PopStyleVar();
				ImGui::PopStyleColor();
			}

			ImGui::EndChild();

			ImGui::PopStyleVar();
			ImGui::PopStyleColor();
		}

		DrawPlayerInfoWindow();

		ImGui::End();

		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
	}
}