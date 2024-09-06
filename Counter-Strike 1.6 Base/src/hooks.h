#pragma once

#include <Windows.h>
#include "hlsdk.h"

void clean_imgui();

extern HINSTANCE module_instance_handle;

BOOL WINAPI SwapBuffers_hook(HDC hdc);
void CL_Parse_StuffText_hook(void);
void CL_Parse_Director_hook(void);
qboolean CL_ReadDemoMessage_OLD_ValidStuffText_hook(const char* cmd);
void CL_ReadDemoMessage_OLD_Cbuf_AddFilteredText_hook(char* text);
void CL_ReadPackets_CL_ConnectionlessPacket_hook(void);
qboolean CL_BatchResourceRequest_IsSafeFileToDownload_hook(const char* filename);
qboolean Netchan_CopyFileFragments_IsSafeFileToDownload_hook(const char* filename);
void CL_Parse_VoiceInit_hook(void);
void CL_Send_CvarValue_hook(void);
void CL_Send_CvarValue2_hook(void);
BOOL CL_SendConsistencyInfo_MD5_Hash_File_hook(unsigned char* digest, char* pszFileName, BOOL bUsefopen, BOOL bSeed, unsigned int* seed);
int __MsgFunc_MOTD_hook(const char* pszName, int iSize, void* pbuf);
char* CL_GetCDKeyHash_hook(void);
int CL_SendConnectPacket_Steam_GSInitiateGameConnection_hook(void* pData, int cbMaxData, uint64 steamID, uint32 unIPServer, uint16 usPortServer, qboolean bSecure);
void R_ForceCVars_hook(qboolean mp);
void CL_Set_ServerExtraInfo_hook(void);
void CL_Parse_Disconnect_hook(void);
int pfnVGUI2DrawCharacterAdd_hook(int x, int y, int ch, int r, int g, int b, unsigned int font);
int pAddEntity_hook(int type, struct cl_entity_s* ent, const char* modelname);
void CL_Parse_TempEntity_hook(void);
void null_function(void);
void Host_Frame__Host_Frame_hook(float time);
void Host_Say_f_hook(void);
void Host_Say_Team_f_hook(void);