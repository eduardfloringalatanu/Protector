#pragma once

#include "hlsdk.h"
#include <Windows.h>

extern bool keys[256];

extern cldll_func_t* cl_funcs;
extern cl_enginefunc_t* gEngfuncs;
extern svc_func_t* cl_parsefuncs;
extern int* msg_readcount;
extern client_static_t* cls;
extern char* com_token;
extern client_state_t* nMax;
extern UserMsg** gClientUserMsgs;
extern char** cmd_args;

extern BOOL (WINAPI* original_SwapBuffers)(HDC);
extern void (*original_CL_Parse_StuffText)(void);
extern void (*original_CL_Parse_Director)(void);
extern char* (*MSG_ReadString)(void);
extern int (*MSG_ReadByte)(void);
extern void (*Cmd_TokenizeString)(char*);
extern void (*CL_RecordHUDCommand)(char*);
extern char* (*COM_Parse)(char*);
extern void (*Cvar_DirectSet)(cvar_s*, char*);
extern void (*Cmd_ForwardToServer)(void);
extern qboolean (*original_ValidStuffText)(const char*);
extern void (*original_Cbuf_AddFilteredText)(char*);
extern void (*MSG_BeginReading)(void);
extern int (*MSG_ReadLong)(void);
extern char* (*MSG_ReadStringLine)(void);
extern void (*Cbuf_AddText)(char*);
extern void (*CL_Disconnect)(void);
extern void (*original_CL_ConnectionlessPacket)(void);
extern qboolean (*original_IsSafeFileToDownload)(const char*);
extern void (*original_CL_Parse_VoiceInit)(void);
extern void (*original_CL_Send_CvarValue)(void);
extern void (*original_CL_Send_CvarValue2)(void);
extern void (*MSG_WriteByte)(sizebuf_t*, int);
extern void (*MSG_WriteLong)(sizebuf_t*, int);
extern void (*MSG_WriteString)(sizebuf_t*, const char*);
extern BOOL (*original_MD5_Hash_File)(unsigned char*, char*, BOOL, BOOL, unsigned int*);
extern int (*original___MsgFunc_MOTD)(const char*, int, void*);
extern char* (*original_CL_GetCDKeyHash)(void);
extern int (*original_Steam_GSInitiateGameConnection)(void*, int, uint64, uint32, uint16, qboolean);
extern void (*original_R_ForceCVars)(qboolean);
extern void (*original_CL_Set_ServerExtraInfo)(void);
extern void (*original_CL_Parse_Disconnect)(void);
extern pfnEngSrc_pfnVGUI2DrawCharacterAdd_t original_pfnVGUI2DrawCharacterAdd;
extern HUD_ADDENTITY_FUNC original_pAddEntity;
extern void (*original_CL_Parse_TempEntity)(void);
extern int (*MSG_ReadShort)(void);
extern float (*MSG_ReadCoord)(void);
extern float (*MSG_ReadAngle)(void);
extern void (*original_Host_Motd_Write_f)(void);
extern void (*original_Host_WriteCustomConfig)(void);
extern void (*original_SV_WriteId_f)(void);
extern void (*original__Host_Frame)(float);
extern void (*original_Host_Say_f)(void);
extern void (*original_Host_Say_Team_f)(void);