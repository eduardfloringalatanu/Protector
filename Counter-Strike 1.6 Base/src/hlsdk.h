#pragma once

typedef int HANDLE_SPRITE;

typedef struct rect_s {
	int left;
	int right;
	int top;
	int bottom;
} wrect_t;

typedef struct client_sprite_s {
	char szName[64];
	char szSprite[64];
	int hspr;
	int iRes;
	wrect_t rc;
} client_sprite_t;

typedef struct SCREENINFO_s {
	int iSize;
	int iWidth;
	int iHeight;
	int iFlags;
	int iCharHeight;
	short charWidths[256];
} SCREENINFO;

typedef struct cvar_s {
	char* name;
	char* string;
	int flags;
	float value;
	struct cvar_s* next;
} cvar_t;

typedef int (*pfnUserMsgHook)(const char*, int, void*);

typedef unsigned long long uint64;

typedef unsigned char byte;

typedef struct hud_player_info_s {
	char* name;
	short ping;
	byte thisplayer;
	byte spectator;
	byte packetloss;
	char* model;
	short topcolor;
	short bottomcolor;
	uint64 m_nSteamID;
} hud_player_info_t;

typedef struct client_textmessage_s {
	int effect;
	byte r1;
	byte g1;
	byte b1;
	byte a1;
	byte r2;
	byte g2;
	byte b2;
	byte a2;
	float x;
	float y;
	float fadein;
	float fadeout;
	float holdtime;
	float fxtime;
	const char* pName;
	const char* pMessage;
} client_textmessage_t;

typedef struct con_nprint_s {
	int index;
	float time_to_live;
	float color[3];
} con_nprint_t;

typedef int qboolean;

typedef float vec_t;
typedef vec_t vec3_t[3];

typedef struct {
	byte r;
	byte g;
	byte b;
} color24;

typedef struct entity_state_s {
	int entityType;
	int number;
	float msg_time;
	int messagenum;
	vec3_t origin;
	vec3_t angles;
	int modelindex;
	int sequence;
	float frame;
	int colormap;
	short skin;
	short solid;
	int effects;
	float scale;
	byte eflags;
	int rendermode;
	int renderamt;
	color24 rendercolor;
	int renderfx;
	int movetype;
	float animtime;
	float framerate;
	int body;
	byte controller[4];
	byte blending[4];
	vec3_t velocity;
	vec3_t mins;
	vec3_t maxs;
	int aiment;
	int owner;
	float friction;
	float gravity;
	int team;
	int playerclass;
	int health;
	qboolean spectator;
	int weaponmodel;
	int gaitsequence;
	vec3_t basevelocity;
	int usehull;
	int oldbuttons;
	int onground;
	int iStepLeft;
	float flFallVelocity;
	float fov;
	int weaponanim;
	vec3_t startpos;
	vec3_t endpos;
	float impacttime;
	float starttime;
	int iuser1;
	int iuser2;
	int iuser3;
	int iuser4;
	float fuser1;
	float fuser2;
	float fuser3;
	float fuser4;
	vec3_t vuser1;
	vec3_t vuser2;
	vec3_t vuser3;
	vec3_t vuser4;
} entity_state_t;

typedef struct {
	float animtime;
	vec3_t origin;
	vec3_t angles;
} position_history_t;

#define HISTORY_MAX 64
#define HISTORY_MASK (HISTORY_MAX - 1)

typedef struct {
	byte mouthopen;
	byte sndcount;
	int sndavg;
} mouth_t;

typedef struct {
	float prevanimtime;
	float sequencetime;
	byte prevseqblending[2];
	vec3_t prevorigin;
	vec3_t prevangles;
	int prevsequence;
	float prevframe;
	byte prevcontroller[4];
	byte prevblending[2];
} latchedvars_t;

#define MAX_MODEL_NAME 64

typedef enum {
	mod_brush = 0,
	mod_sprite,
	mod_alias,
	mod_studio
} modtype_t;

typedef enum {
	ST_SYNC = 0,
	ST_RAND
} synctype_t;

#define MAX_MAP_HULLS 4

typedef struct {
	float mins[3];
	float maxs[3];
	float origin[3];
	int	headnode[MAX_MAP_HULLS];
	int	visleafs;
	int firstface;
	int numfaces;
} dmodel_t;

typedef struct mplane_s {
	vec3_t normal;
	float dist;
	byte type;
	byte signbits;
	byte pad[2];
} mplane_t;

#define	NUM_AMBIENTS 4

typedef struct mnode_s {
	int	contents;
	int	visframe;
	short minmaxs[6];
	struct mnode_s* parent;
	mplane_t* plane;
	struct mnode_s* children[2];
	unsigned short firstsurface;
	unsigned short numsurfaces;
} mnode_t;

typedef struct efrag_s {
	struct mleaf_s* leaf;
	struct efrag_s* leafnext;
	struct cl_entity_s* entity;
	struct efrag_s* entnext;
} efrag_t;

#define MIPLEVELS 4
#define MAXLIGHTMAPS 4

typedef struct texture_s {
	char name[16];
	unsigned int width;
	unsigned int height;
	int anim_total;
	int anim_min;
	int anim_max;
	struct texture_s* anim_next;
	struct texture_s* alternate_anims;
	unsigned int offsets[MIPLEVELS];
	unsigned int paloffset;
} texture_t;

typedef struct {
	float vecs[2][4];
	float mipadjust;
	texture_t* texture;
	int flags;
} mtexinfo_t;

typedef struct decal_s decal_t;
typedef struct msurface_s msurface_t;

struct decal_s {
	decal_t* pnext;
	msurface_t* psurface;
	short dx;
	short dy;
	short texture;
	byte scale;
	byte flags;
	short entityIndex;
};

struct msurface_s {
	int visframe;
	int dlightframe;
	int dlightbits;
	mplane_t* plane;
	int flags;
	int firstedge;
	int numedges;
	struct surfcache_s* cachespots[MIPLEVELS];
	short texturemins[2];
	short extents[2];
	mtexinfo_t* texinfo;
	byte styles[MAXLIGHTMAPS];
	color24* samples;
	decal_t* pdecals;
};

typedef struct mleaf_s {
	int contents;
	int visframe;
	float minmaxs[6];
	struct mnode_s* parent;
	byte* compressed_vis;
	struct efrag_s* efrags;
	msurface_t** firstmarksurface;
	int nummarksurfaces;
	int key;
	byte ambient_sound_level[NUM_AMBIENTS];
} mleaf_t;

typedef struct {
	vec3_t position;
} mvertex_t;

typedef struct {
	unsigned short v[2];
	unsigned int cachededgeoffset;
} medge_t;

typedef struct {
	int planenum;
	short children[2];
} dclipnode_t;

typedef struct hull_s {
	dclipnode_t* clipnodes;
	mplane_t* planes;
	int firstclipnode;
	int lastclipnode;
	vec3_t clip_mins;
	vec3_t clip_maxs;
} hull_t;

#define MAX_MAP_HULLS 4

typedef struct cache_user_s {
	void* data;
} cache_user_t;

typedef struct model_s {
	char name[MAX_MODEL_NAME];
	qboolean needload;
	modtype_t type;
	int numframes;
	synctype_t synctype;
	int flags;
	vec3_t mins;
	vec3_t maxs;
	float radius;
	int firstmodelsurface;
	int nummodelsurfaces;
	int numsubmodels;
	dmodel_t* submodels;
	int numplanes;
	mplane_t* planes;
	int numleafs;
	mleaf_t* leafs;
	int numvertexes;
	mvertex_t* vertexes;
	int numedges;
	medge_t* edges;
	int numnodes;
	mnode_t* nodes;
	int numtexinfo;
	mtexinfo_t* texinfo;
	int numsurfaces;
	msurface_t* surfaces;
	int numsurfedges;
	int* surfedges;
	int numclipnodes;
	dclipnode_t* clipnodes;
	int nummarksurfaces;
	msurface_t** marksurfaces;
	hull_t hulls[MAX_MAP_HULLS];
	int numtextures;
	texture_t** textures;
	byte* visdata;
	color24* lightdata;
	char* entities;
	cache_user_t cache;
} model_t;

typedef struct {
	unsigned int r;
	unsigned int g;
	unsigned int b;
	unsigned int a;
} colorVec;

typedef struct cl_entity_s {
	int index;
	qboolean player;
	entity_state_t baseline;
	entity_state_t prevstate;
	entity_state_t curstate;
	int current_position;
	position_history_t ph[HISTORY_MAX];
	mouth_t mouth;
	latchedvars_t latched;
	float lastmove;
	vec3_t origin;
	vec3_t angles;
	vec3_t attachment[4];
	int trivial_accept;
	struct model_s* model;
	struct efrag_s* efrag;
	struct mnode_s* topnode;
	float syncbase;
	int visframe;
	colorVec cvFloorColor;
} cl_entity_t;

typedef struct {
	vec3_t normal;
	float dist;
} pmplane_t;

typedef struct pmtrace_s {
	qboolean allsolid;
	qboolean startsolid;
	qboolean inopen;
	qboolean inwater;
	float fraction;
	vec3_t endpos;
	pmplane_t plane;
	int	ent;
	vec3_t deltavelocity;
	int hitgroup;
} pmtrace_t;

typedef struct link_s {
	struct link_s* prev;
	struct link_s* next;
} link_t;

#define	MAX_ENT_LEAFS 48

typedef unsigned int string_t;

typedef struct edict_s edict_t;

typedef struct entvars_s {
	string_t classname;
	string_t globalname;
	vec3_t origin;
	vec3_t oldorigin;
	vec3_t velocity;
	vec3_t basevelocity;
	vec3_t clbasevelocity;
	vec3_t movedir;
	vec3_t angles;
	vec3_t avelocity;
	vec3_t punchangle;
	vec3_t v_angle;
	vec3_t endpos;
	vec3_t startpos;
	float impacttime;
	float starttime;
	int fixangle;
	float idealpitch;
	float pitch_speed;
	float ideal_yaw;
	float yaw_speed;
	int modelindex;
	string_t model;
	int viewmodel;
	int weaponmodel;
	vec3_t absmin;
	vec3_t absmax;
	vec3_t mins;
	vec3_t maxs;
	vec3_t size;
	float ltime;
	float nextthink;
	int movetype;
	int solid;
	int skin;
	int body;
	int effects;
	float gravity;
	float friction;
	int light_level;
	int sequence;
	int gaitsequence;
	float frame;
	float animtime;
	float framerate;
	byte controller[4];
	byte blending[2];
	float scale;
	int rendermode;
	float renderamt;
	vec3_t rendercolor;
	int renderfx;
	float health;
	float frags;
	int weapons;
	float takedamage;
	int deadflag;
	vec3_t view_ofs;
	int button;
	int impulse;
	edict_t* chain;
	edict_t* dmg_inflictor;
	edict_t* enemy;
	edict_t* aiment;
	edict_t* owner;
	edict_t* groundentity;
	int spawnflags;
	int flags;
	int colormap;
	int team;
	float max_health;
	float teleport_time;
	float armortype;
	float armorvalue;
	int waterlevel;
	int watertype;
	string_t target;
	string_t targetname;
	string_t netname;
	string_t message;
	float dmg_take;
	float dmg_save;
	float dmg;
	float dmgtime;
	string_t noise;
	string_t noise1;
	string_t noise2;
	string_t noise3;
	float speed;
	float air_finished;
	float pain_finished;
	float radsuit_finished;
	edict_t* pContainingEntity;
	int playerclass;
	float maxspeed;
	float fov;
	int weaponanim;
	int	pushmsec;
	int	bInDuck;
	int	flTimeStepSound;
	int	flSwimTime;
	int	flDuckTime;
	int iStepLeft;
	float flFallVelocity;
	int gamestate;
	int oldbuttons;
	int groupinfo;
	int iuser1;
	int iuser2;
	int iuser3;
	int iuser4;
	float fuser1;
	float fuser2;
	float fuser3;
	float fuser4;
	vec3_t vuser1;
	vec3_t vuser2;
	vec3_t vuser3;
	vec3_t vuser4;
	edict_t* euser1;
	edict_t* euser2;
	edict_t* euser3;
	edict_t* euser4;
} entvars_t;

struct edict_s {
	qboolean free;
	int serialnumber;
	link_t area;
	int headnode;
	int num_leafs;
	short leafnums[MAX_ENT_LEAFS];
	float freetime;
	void* pvPrivateData;
	entvars_t v;
};

typedef int int32;

typedef struct event_args_s {
	int flags;
	int	entindex;
	float origin[3];
	float angles[3];
	float velocity[3];
	int ducking;
	float fparam1;
	float fparam2;
	int	iparam1;
	int	iparam2;
	int	bparam1;
	int bparam2;
} event_args_t;

typedef struct screenfade_s {
	float fadeSpeed;
	float fadeEnd;
	float fadeTotalEnd;
	float fadeReset;
	byte fader;
	byte fadeg;
	byte fadeb;
	byte fadealpha;
	int fadeFlags;
} screenfade_t;

typedef struct sequenceCommandLine_ sequenceCommandLine_s;

struct sequenceCommandLine_ {
	int	commandType;
	client_textmessage_t clientMessage;
	char* speakerName;
	char* listenerName;
	char* soundFileName;
	char* sentenceName;
	char* fireTargetNames;
	char* killTargetNames;
	float delay;
	int	repeatCount;
	int	textChannel;
	int modifierBitField;
	sequenceCommandLine_s* nextCommandLine;
};

typedef struct sequenceEntry_ sequenceEntry_s;

struct sequenceEntry_ {
	char* fileName;
	char* entryName;
	sequenceCommandLine_s* firstCommand;
	sequenceEntry_s* nextEntry;
	qboolean isGlobal;
};

typedef struct sentenceEntry_ sentenceEntry_s;

struct sentenceEntry_ {
	char* data;
	sentenceEntry_s* nextEntry;
	qboolean isGlobal;
	unsigned int index;
};

#define MAX_ALIAS_NAME 32

typedef struct cmdalias_s {
	struct cmdalias_s* next;
	char name[MAX_ALIAS_NAME];
	char* value;
} cmdalias_t;

typedef HANDLE_SPRITE (*pfnEngSrc_pfnSPR_Load_t)(const char*);
typedef int (*pfnEngSrc_pfnSPR_Frames_t)(HANDLE_SPRITE);
typedef int (*pfnEngSrc_pfnSPR_Height_t)(HANDLE_SPRITE, int);
typedef int (*pfnEngSrc_pfnSPR_Width_t)(HANDLE_SPRITE, int);
typedef void (*pfnEngSrc_pfnSPR_Set_t)(HANDLE_SPRITE, int, int, int);
typedef void (*pfnEngSrc_pfnSPR_Draw_t)(int, int, int, const struct rect_s*);
typedef void (*pfnEngSrc_pfnSPR_DrawHoles_t)(int, int, int, const struct rect_s*);
typedef void (*pfnEngSrc_pfnSPR_DrawAdditive_t)(int, int, int, const struct rect_s*);
typedef void (*pfnEngSrc_pfnSPR_EnableScissor_t)(int, int, int, int);
typedef void (*pfnEngSrc_pfnSPR_DisableScissor_t)(void);
typedef struct client_sprite_s* (*pfnEngSrc_pfnSPR_GetList_t)(char*, int*);
typedef void (*pfnEngSrc_pfnFillRGBA_t)(int, int, int, int, int, int, int, int);
typedef int (*pfnEngSrc_pfnGetScreenInfo_t)(struct SCREENINFO_s*);
typedef void (*pfnEngSrc_pfnSetCrosshair_t)(HANDLE_SPRITE, wrect_t, int, int, int);
typedef struct cvar_s* (*pfnEngSrc_pfnRegisterVariable_t)(char*, char*, int);
typedef float (*pfnEngSrc_pfnGetCvarFloat_t)(char*);
typedef char* (*pfnEngSrc_pfnGetCvarString_t)(char*);
typedef int (*pfnEngSrc_pfnAddCommand_t)(char*, void (*)(void));
typedef int (*pfnEngSrc_pfnHookUserMsg_t)(char*, pfnUserMsgHook);
typedef int (*pfnEngSrc_pfnServerCmd_t)(char*);
typedef int (*pfnEngSrc_pfnClientCmd_t)(char*);
typedef void (*pfnEngSrc_pfnGetPlayerInfo_t)(int, struct hud_player_info_s*);
typedef void (*pfnEngSrc_pfnPlaySoundByName_t)(char*, float);
typedef void (*pfnEngSrc_pfnPlaySoundByIndex_t)(int, float);
typedef void (*pfnEngSrc_pfnAngleVectors_t)(const float*, float*, float*, float*);
typedef struct client_textmessage_s* (*pfnEngSrc_pfnTextMessageGet_t)(const char*);
typedef int (*pfnEngSrc_pfnDrawCharacter_t)(int, int, int, int, int, int);
typedef int (*pfnEngSrc_pfnDrawConsoleString_t)(int, int, char*);
typedef void (*pfnEngSrc_pfnDrawSetTextColor_t)(float, float, float);
typedef void (*pfnEngSrc_pfnDrawConsoleStringLen_t)(const char*, int*, int*);
typedef void (*pfnEngSrc_pfnConsolePrint_t)(const char*);
typedef void (*pfnEngSrc_pfnCenterPrint_t)(const char*);
typedef int (*pfnEngSrc_GetWindowCenterX_t)(void);
typedef int (*pfnEngSrc_GetWindowCenterY_t)(void);
typedef void (*pfnEngSrc_GetViewAngles_t)(float*);
typedef void (*pfnEngSrc_SetViewAngles_t)(float*);
typedef int (*pfnEngSrc_GetMaxClients_t)(void);
typedef void (*pfnEngSrc_Cvar_SetValue_t)(char*, float);
typedef int (*pfnEngSrc_Cmd_Argc_t)(void);
typedef char* (*pfnEngSrc_Cmd_Argv_t)(int);
typedef void (*pfnEngSrc_Con_Printf_t)(char*, ...);
typedef void (*pfnEngSrc_Con_DPrintf_t)(char*, ...);
typedef void (*pfnEngSrc_Con_NPrintf_t)(int, char*, ...);
typedef void (*pfnEngSrc_Con_NXPrintf_t)(struct con_nprint_s*, char*, ...);
typedef const char* (*pfnEngSrc_PhysInfo_ValueForKey_t)(const char*);
typedef const char* (*pfnEngSrc_ServerInfo_ValueForKey_t)(const char*);
typedef float (*pfnEngSrc_GetClientMaxspeed_t)(void);
typedef int (*pfnEngSrc_CheckParm_t)(char*, char**);
typedef void (*pfnEngSrc_Key_Event_t)(int, int);
typedef void (*pfnEngSrc_GetMousePosition_t)(int*, int*);
typedef int (*pfnEngSrc_IsNoClipping_t)(void);
typedef struct cl_entity_s* (*pfnEngSrc_GetLocalPlayer_t)(void);
typedef struct cl_entity_s* (*pfnEngSrc_GetViewModel_t)(void);
typedef struct cl_entity_s* (*pfnEngSrc_GetEntityByIndex_t)(int);
typedef float (*pfnEngSrc_GetClientTime_t)(void);
typedef void (*pfnEngSrc_V_CalcShake_t)(void);
typedef void (*pfnEngSrc_V_ApplyShake_t)(float*, float*, float);
typedef int (*pfnEngSrc_PM_PointContents_t)(float*, int*);
typedef int (*pfnEngSrc_PM_WaterEntity_t)(float*);
typedef struct pmtrace_s* (*pfnEngSrc_PM_TraceLine_t)(float*, float*, int, int, int);
typedef struct model_s* (*pfnEngSrc_CL_LoadModel_t)(const char*, int*);
typedef int (*pfnEngSrc_CL_CreateVisibleEntity_t)(int, struct cl_entity_s*);
typedef const struct model_s* (*pfnEngSrc_GetSpritePointer_t)(HANDLE_SPRITE);
typedef void (*pfnEngSrc_pfnPlaySoundByNameAtLocation_t)(char*, float, float*);
typedef unsigned short (*pfnEngSrc_pfnPrecacheEvent_t)(int, const char*);
typedef void (*pfnEngSrc_pfnPlaybackEvent_t)(int, const struct edict_s*, unsigned short, float, float*, float*, float, float, int, int, int, int);
typedef void (*pfnEngSrc_pfnWeaponAnim_t)(int, int);
typedef float (*pfnEngSrc_pfnRandomFloat_t)(float, float);
typedef int32 (*pfnEngSrc_pfnRandomLong_t)(int32, int32);
typedef void (*pfnEngSrc_pfnHookEvent_t)(char*, void (*)(struct event_args_s*));
typedef int (*pfnEngSrc_Con_IsVisible_t)(void);
typedef const char* (*pfnEngSrc_pfnGetGameDirectory_t)(void);
typedef struct cvar_s* (*pfnEngSrc_pfnGetCvarPointer_t)(const char*);
typedef const char* (*pfnEngSrc_Key_LookupBinding_t)(const char*);
typedef const char* (*pfnEngSrc_pfnGetLevelName_t)(void);
typedef void (*pfnEngSrc_pfnGetScreenFade_t)(struct screenfade_s*);
typedef void (*pfnEngSrc_pfnSetScreenFade_t)(struct screenfade_s*);
typedef void* (*pfnEngSrc_VGui_GetPanel_t)(void);
typedef void (*pfnEngSrc_VGui_ViewportPaintBackground_t)(int*);
typedef byte* (*pfnEngSrc_COM_LoadFile_t)(char*, int, int*);
typedef char* (*pfnEngSrc_COM_ParseFile_t)(char*, char*);
typedef void (*pfnEngSrc_COM_FreeFile_t)(void*);
typedef int (*pfnEngSrc_IsSpectateOnly_t)(void);
typedef struct model_s* (*pfnEngSrc_LoadMapSprite_t)(const char*);
typedef void (*pfnEngSrc_COM_AddAppDirectoryToSearchPath_t)(const char*, const char*);
typedef int (*pfnEngSrc_COM_ExpandFilename_t)(const char*, char*, int);
typedef const char* (*pfnEngSrc_PlayerInfo_ValueForKey_t)(int, const char*);
typedef void (*pfnEngSrc_PlayerInfo_SetValueForKey_t)(const char*, const char*);
typedef qboolean (*pfnEngSrc_GetPlayerUniqueID_t)(int, char*);
typedef int (*pfnEngSrc_GetTrackerIDForPlayer_t)(int);
typedef int (*pfnEngSrc_GetPlayerForTrackerID_t)(int);
typedef int (*pfnEngSrc_pfnServerCmdUnreliable_t)(char*);
typedef void (*pfnEngSrc_GetMousePos_t)(struct tagPOINT*);
typedef void (*pfnEngSrc_SetMousePos_t)(int, int);
typedef void (*pfnEngSrc_SetMouseEnable_t)(qboolean);
typedef struct cvar_s* (*pfnEngSrc_GetFirstCVarPtr_t)(void);
typedef unsigned int (*pfnEngSrc_GetFirstCmdFunctionHandle_t)(void);
typedef unsigned int (*pfnEngSrc_GetNextCmdFunctionHandle_t)(unsigned int);
typedef const char* (*pfnEngSrc_GetCmdFunctionName_t)(unsigned int);
typedef float (*pfnEngSrc_GetClientOldTime_t)(void);
typedef float (*pfnEngSrc_GetServerGravityValue_t)(void);
typedef struct model_s* (*pfnEngSrc_GetModelByIndex_t)(int);
typedef void (*pfnEngSrc_pfnSetFilterMode_t)(int);
typedef void (*pfnEngSrc_pfnSetFilterColor_t)(float, float, float);
typedef void (*pfnEngSrc_pfnSetFilterBrightness_t)(float);
typedef sequenceEntry_s* (*pfnEngSrc_pfnSequenceGet_t)(const char*, const char*);
typedef void (*pfnEngSrc_pfnSPR_DrawGeneric_t)(int, int, int, const struct rect_s*, int, int, int, int);
typedef sentenceEntry_s* (*pfnEngSrc_pfnSequencePickSentence_t)(const char*, int, int*);
typedef int (*pfnEngSrc_pfnDrawString_t)(int, int, const char*, int, int, int);
typedef int (*pfnEngSrc_pfnDrawStringReverse_t)(int, int, const char*, int, int, int);
typedef const char* (*pfnEngSrc_LocalPlayerInfo_ValueForKey_t)(const char*);
typedef int (*pfnEngSrc_pfnVGUI2DrawCharacter_t)(int, int, int, unsigned int);
typedef int (*pfnEngSrc_pfnVGUI2DrawCharacterAdd_t)(int, int, int, int, int, int, unsigned int);
typedef unsigned int (*pfnEngSrc_COM_GetApproxWavePlayLength)(const char*);
typedef void* (*pfnEngSrc_pfnGetCareerUI_t)(void);
typedef void (*pfnEngSrc_Cvar_Set_t)(char*, char*);
typedef int (*pfnEngSrc_pfnIsPlayingCareerMatch_t)(void);
typedef void (*pfnEngSrc_pfnPlaySoundVoiceByName_t)(char*, float, int);
typedef void (*pfnEngSrc_pfnPrimeMusicStream_t)(char*, int);
typedef double (*pfnEngSrc_GetAbsoluteTime_t)(void);
typedef void (*pfnEngSrc_pfnProcessTutorMessageDecayBuffer_t)(int*, int);
typedef void (*pfnEngSrc_pfnConstructTutorMessageDecayBuffer_t)(int*, int);
typedef void (*pfnEngSrc_pfnResetTutorMessageDecayData_t)(void);
typedef void (*pfnEngSrc_pfnPlaySoundByNameAtPitch_t)(char*, float, int);
typedef void (*pfnEngSrc_pfnFillRGBABlend_t)(int, int, int, int, int, int, int, int);
typedef int (*pfnEngSrc_pfnGetAppID_t)(void);
typedef cmdalias_t* (*pfnEngSrc_pfnGetAliases_t)(void);
typedef void (*pfnEngSrc_pfnVguiWrap2_GetMouseDelta_t)(int*, int*);
typedef int (*pfnEngSrc_pfnFilteredClientCmd_t)(char*);

typedef enum {
	TRI_FRONT = 0,
	TRI_NONE
} TRICULLSTYLE;

typedef struct triangleapi_s {
	int version;
	void (*RenderMode)(int);
	void (*Begin)(int);
	void (*End)(void);
	void (*Color4f)(float, float, float, float);
	void (*Color4ub)(unsigned char, unsigned char, unsigned char, unsigned char);
	void (*TexCoord2f)(float, float);
	void (*Vertex3fv)(float*);
	void (*Vertex3f)(float, float, float);
	void (*Brightness)(float);
	void (*CullFace)(TRICULLSTYLE);
	int (*SpriteTexture)(struct model_s*, int);
	int (*WorldToScreen)(float*, float*);
	void (*Fog)(float*, float, float, int);
	void (*ScreenToWorld)(float*, float*);
	void (*GetMatrix)(const int, float*);
	int (*BoxInPVS)(float*, float*);
	void (*LightAtPoint)(float*, float*);
	void (*Color4fRendermode)(float, float, float, float, int);
	void (*FogParams)(float, int);
} triangleapi_t;

typedef enum {
	pt_static = 0,
	pt_grav,
	pt_slowgrav,
	pt_fire,
	pt_explode,
	pt_explode2,
	pt_blob,
	pt_blob2,
	pt_vox_slowgrav,
	pt_vox_grav,
	pt_clientcustom
} ptype_t;

typedef struct particle_s {
	vec3_t org;
	short color;
	short packedColor;
	struct particle_s* next;
	vec3_t vel;
	float ramp;
	float die;
	ptype_t	type;
	void (*deathfunc)(struct particle_s*);
	void (*callback)(struct particle_s*, float);
	unsigned char context;
} particle_t;

typedef struct tempent_s {
	int flags;
	float die;
	float frameMax;
	float x;
	float y;
	float z;
	float fadeSpeed;
	float bounceFactor;
	int hitSound;
	void (*hitcallback)(struct tempent_s*, struct pmtrace_s*);
	void (*callback)(struct tempent_s*, float, float);
	struct tempent_s* next;
	int priority;
	short clientIndex;
	vec3_t tentOffset;
	cl_entity_t	entity;
} TEMPENTITY;

typedef struct beam_s BEAM;

struct beam_s {
	BEAM* next;
	int	type;
	int	flags;
	vec3_t source;
	vec3_t target;
	vec3_t delta;
	float t;
	float freq;
	float die;
	float width;
	float amplitude;
	float r;
	float g;
	float b;
	float brightness;
	float speed;
	float frameRate;
	float frame;
	int segments;
	int startEntity;
	int endEntity;
	int modelIndex;
	int frameCount;
	struct model_s* pFollowModel;
	struct particle_s* particles;
};

typedef struct dlight_s {
	vec3_t origin;
	float radius;
	color24 color;
	float die;
	float decay;
	float minlight;
	int key;
	qboolean dark;
} dlight_t;

typedef struct efx_api_s {
	particle_t* (*R_AllocParticle)(void (*)(struct particle_s*, float));
	void (*R_BlobExplosion)(float*);
	void (*R_Blood)(float*, float*, int, int);
	void (*R_BloodSprite)(float*, int, int, int, float);
	void (*R_BloodStream)(float*, float*, int, int);
	void (*R_BreakModel)(float*, float*, float*, float, float, int, int, char);
	void (*R_Bubbles)(float*, float*, float, int, int, float);
	void (*R_BubbleTrail)(float*, float*, float, int, int, float);
	void (*R_BulletImpactParticles)(float*);
	void (*R_EntityParticles)(struct cl_entity_s*);
	void (*R_Explosion)(float*, int, float, float, int);
	void (*R_FizzEffect)(struct cl_entity_s*, int, int);
	void (*R_FireField)(float*, int, int, int, int, float);
	void (*R_FlickerParticles)(float*);
	void (*R_FunnelSprite)(float*, int, int);
	void (*R_Implosion)(float*, float, int, float);
	void (*R_LargeFunnel)(float*, int);
	void (*R_LavaSplash)(float*);
	void (*R_MultiGunshot)(float*, float*, float*, int, int, int*);
	void (*R_MuzzleFlash)(float*, int);
	void (*R_ParticleBox)(float*, float*, unsigned char, unsigned char, unsigned char, float);
	void (*R_ParticleBurst)(float*, int, int, float);
	void (*R_ParticleExplosion)(float*);
	void (*R_ParticleExplosion2)(float*, int, int);
	void (*R_ParticleLine)(float*, float*, unsigned char, unsigned char, unsigned char, float);
	void (*R_PlayerSprites)(int, int, int, int);
	void (*R_Projectile)(float*, float*, int, int, int, void (*)(struct tempent_s*, struct pmtrace_s*));
	void (*R_RicochetSound)(float*);
	void (*R_RicochetSprite)(float*, struct model_s*, float, float);
	void (*R_RocketFlare)(float*);
	void (*R_RocketTrail)(float*, float*, int);
	void (*R_RunParticleEffect)(float*, float*, int, int);
	void (*R_ShowLine)(float*, float*);
	void (*R_SparkEffect)(float*, int, int, int);
	void (*R_SparkShower)(float*);
	void (*R_SparkStreaks)(float*, int, int, int);
	void (*R_Spray)(float*, float*, int, int, int, int, int);
	void (*R_Sprite_Explode)(TEMPENTITY*, float, int);
	void (*R_Sprite_Smoke)(TEMPENTITY*, float);
	void (*R_Sprite_Spray)(float*, float*, int, int, int, int);
	void (*R_Sprite_Trail)(int, float*, float*, int, int, float, float, float, int, float);
	void (*R_Sprite_WallPuff)(TEMPENTITY*, float);
	void (*R_StreakSplash)(float*, float*, int, int, float, int, int);
	void (*R_TracerEffect)(float*, float*);
	void (*R_UserTracerParticle)(float*, float*, float, int, float, unsigned char, void (*)(struct particle_s*));
	particle_t* (*R_TracerParticles)(float*, float*, float);
	void (*R_TeleportSplash)(float*);
	void (*R_TempSphereModel)(float*, float, float, int, int);
	TEMPENTITY* (*R_TempModel)(float*, float*, float*, float, int, int);
	TEMPENTITY* (*R_DefaultSprite)(float*, int, float);
	TEMPENTITY* (*R_TempSprite)(float*, float*, float, int, int, int, float, float, int);
	int (*Draw_DecalIndex)(int);
	int (*Draw_DecalIndexFromName)(char*);
	void (*R_DecalShoot)(int, int, int, float*, int);
	void (*R_AttachTentToPlayer)(int, int, float, float);
	void (*R_KillAttachedTents)(int);
	BEAM* (*R_BeamCirclePoints)(int, float*, float*, int, float, float, float, float, float, int, float, float, float, float);
	BEAM* (*R_BeamEntPoint)(int, float*, int, float, float, float, float, float, int, float, float, float, float);
	BEAM* (*R_BeamEnts)(int, int, int, float, float, float, float, float, int, float, float, float, float);
	BEAM* (*R_BeamFollow)(int, int, float, float, float, float, float, float);
	void (*R_BeamKill)(int);
	BEAM* (*R_BeamLightning)(float*, float*, int, float, float, float, float, float);
	BEAM* (*R_BeamPoints)(float*, float*, int, float, float, float, float, float, int, float, float, float, float);
	BEAM* (*R_BeamRing)(int, int, int, float, float, float, float, float, int, float, float, float, float);
	dlight_t* (*CL_AllocDlight)(int);
	dlight_t* (*CL_AllocElight)(int);
	TEMPENTITY* (*CL_TempEntAlloc)(float*, struct model_s*);
	TEMPENTITY* (*CL_TempEntAllocNoModel)(float*);
	TEMPENTITY* (*CL_TempEntAllocHigh)(float*, struct model_s*);
	TEMPENTITY* (*CL_TentEntAllocCustom)(float*, struct model_s*, int, void (*)(struct tempent_s*, float, float));
	void (*R_GetPackedColor)(short*, short);
	short (*R_LookupColor)(unsigned char, unsigned char, unsigned char);
	void (*R_DecalRemoveAll)(int);
	void (*R_FireCustomDecal)(int, int, int, float*, int, float);
} efx_api_t;

typedef struct physent_s {
	char name[32];
	int player;
	vec3_t origin;
	struct model_s* model;
	struct model_s* studiomodel;
	vec3_t mins;
	vec3_t maxs;
	int info;
	vec3_t angles;
	int solid;
	int skin;
	int rendermode;
	float frame;
	int sequence;
	byte controller[4];
	byte blending[2];
	int movetype;
	int takedamage;
	int blooddecal;
	int team;
	int classnumber;
	int iuser1;
	int iuser2;
	int iuser3;
	int iuser4;
	float fuser1;
	float fuser2;
	float fuser3;
	float fuser4;
	vec3_t vuser1;
	vec3_t vuser2;
	vec3_t vuser3;
	vec3_t vuser4;
} physent_t;

typedef struct event_api_s {
	int version;
	void (*EV_PlaySound)(int, float*, int, const char*, float, float, int, int);
	void (*EV_StopSound)(int, int, const char*);
	int (*EV_FindModelIndex)(const char*);
	int (*EV_IsLocal)(int);
	int (*EV_LocalPlayerDucking)(void);
	void (*EV_LocalPlayerViewheight)(float*);
	void (*EV_LocalPlayerBounds)(int, float*, float*);
	int (*EV_IndexFromTrace)(struct pmtrace_s*);
	struct physent_s* (*EV_GetPhysent)(int);
	void (*EV_SetUpPlayerPrediction)(int, int);
	void (*EV_PushPMStates)(void);
	void (*EV_PopPMStates)(void);
	void (*EV_SetSolidPlayers)(int);
	void (*EV_SetTraceHull)(int);
	void (*EV_PlayerTrace)(float*, float*, int, int, struct pmtrace_s*);
	void (*EV_WeaponAnimation)(int, int);
	unsigned short (*EV_PrecacheEvent)(int, const char*);
	void (*EV_PlaybackEvent)(int, const struct edict_s*, unsigned short, float, float*, float*, float, float, int, int, int, int);
	const char* (*EV_TraceTexture)(int, float*, float*);
	void (*EV_StopAllSounds)(int, int);
	void (*EV_KillEvents)(int, const char*);
} event_api_t;

typedef struct demo_api_s {
	int (*IsRecording)(void);
	int (*IsPlayingback)(void);
	int (*IsTimeDemo)(void);
	void (*WriteBuffer)(int, unsigned char*);
} demo_api_t;

typedef enum {
	NA_UNUSED = 0,
	NA_LOOPBACK,
	NA_BROADCAST,
	NA_IP,
	NA_IPX,
	NA_BROADCAST_IPX
} netadrtype_t;

typedef struct netadr_s {
	netadrtype_t type;
	unsigned char ip[4];
	unsigned char ipx[10];
	unsigned short port;
} netadr_t;

struct net_status_s {
	int connected;
	netadr_t local_address;
	netadr_t remote_address;
	int packet_loss;
	double latency;
	double connection_time;
	double rate;
};

typedef void (*net_api_response_func_t)(struct net_response_s*);

typedef struct net_api_s {
	void (*InitNetworking)(void);
	void (*Status)(struct net_status_s*);
	void (*SendRequest)(int, int, int, double, struct netadr_s*, net_api_response_func_t);
	void (*CancelRequest)(int);
	void (*CancelAllRequests)(void);
	char* (*AdrToString)(struct netadr_s*);
	int (*CompareAdr)(struct netadr_s*, struct netadr_s*);
	int (*StringToAdr)(char*, struct netadr_s*);
	const char* (*ValueForKey)(const char*, const char*);
	void (*RemoveKey)(char*, const char*);
	void (*SetValueForKey)(char*, const char*, const char*, int);
} net_api_t;

typedef enum {
	MicrophoneVolume = 0,
	OtherSpeakerScale
} VoiceTweakControl;

typedef struct IVoiceTweak_s {
	int (*StartVoiceTweakMode)();
	void (*EndVoiceTweakMode)();
	void (*SetControlFloat)(VoiceTweakControl, float);
	float (*GetControlFloat)(VoiceTweakControl);
	int (*GetSpeakingVolume)();
} IVoiceTweak;

typedef struct cl_enginefuncs_s {
	pfnEngSrc_pfnSPR_Load_t pfnSPR_Load;
	pfnEngSrc_pfnSPR_Frames_t pfnSPR_Frames;
	pfnEngSrc_pfnSPR_Height_t pfnSPR_Height;
	pfnEngSrc_pfnSPR_Width_t pfnSPR_Width;
	pfnEngSrc_pfnSPR_Set_t pfnSPR_Set;
	pfnEngSrc_pfnSPR_Draw_t pfnSPR_Draw;
	pfnEngSrc_pfnSPR_DrawHoles_t pfnSPR_DrawHoles;
	pfnEngSrc_pfnSPR_DrawAdditive_t pfnSPR_DrawAdditive;
	pfnEngSrc_pfnSPR_EnableScissor_t pfnSPR_EnableScissor;
	pfnEngSrc_pfnSPR_DisableScissor_t pfnSPR_DisableScissor;
	pfnEngSrc_pfnSPR_GetList_t pfnSPR_GetList;
	pfnEngSrc_pfnFillRGBA_t pfnFillRGBA;
	pfnEngSrc_pfnGetScreenInfo_t pfnGetScreenInfo;
	pfnEngSrc_pfnSetCrosshair_t pfnSetCrosshair;
	pfnEngSrc_pfnRegisterVariable_t pfnRegisterVariable;
	pfnEngSrc_pfnGetCvarFloat_t pfnGetCvarFloat;
	pfnEngSrc_pfnGetCvarString_t pfnGetCvarString;
	pfnEngSrc_pfnAddCommand_t pfnAddCommand;
	pfnEngSrc_pfnHookUserMsg_t pfnHookUserMsg;
	pfnEngSrc_pfnServerCmd_t pfnServerCmd;
	pfnEngSrc_pfnClientCmd_t pfnClientCmd;
	pfnEngSrc_pfnGetPlayerInfo_t pfnGetPlayerInfo;
	pfnEngSrc_pfnPlaySoundByName_t pfnPlaySoundByName;
	pfnEngSrc_pfnPlaySoundByIndex_t pfnPlaySoundByIndex;
	pfnEngSrc_pfnAngleVectors_t pfnAngleVectors;
	pfnEngSrc_pfnTextMessageGet_t pfnTextMessageGet;
	pfnEngSrc_pfnDrawCharacter_t pfnDrawCharacter;
	pfnEngSrc_pfnDrawConsoleString_t pfnDrawConsoleString;
	pfnEngSrc_pfnDrawSetTextColor_t pfnDrawSetTextColor;
	pfnEngSrc_pfnDrawConsoleStringLen_t pfnDrawConsoleStringLen;
	pfnEngSrc_pfnConsolePrint_t pfnConsolePrint;
	pfnEngSrc_pfnCenterPrint_t pfnCenterPrint;
	pfnEngSrc_GetWindowCenterX_t GetWindowCenterX;
	pfnEngSrc_GetWindowCenterY_t GetWindowCenterY;
	pfnEngSrc_GetViewAngles_t GetViewAngles;
	pfnEngSrc_SetViewAngles_t SetViewAngles;
	pfnEngSrc_GetMaxClients_t GetMaxClients;
	pfnEngSrc_Cvar_SetValue_t Cvar_SetValue;
	pfnEngSrc_Cmd_Argc_t Cmd_Argc;
	pfnEngSrc_Cmd_Argv_t Cmd_Argv;
	pfnEngSrc_Con_Printf_t Con_Printf;
	pfnEngSrc_Con_DPrintf_t Con_DPrintf;
	pfnEngSrc_Con_NPrintf_t Con_NPrintf;
	pfnEngSrc_Con_NXPrintf_t Con_NXPrintf;
	pfnEngSrc_PhysInfo_ValueForKey_t PhysInfo_ValueForKey;
	pfnEngSrc_ServerInfo_ValueForKey_t ServerInfo_ValueForKey;
	pfnEngSrc_GetClientMaxspeed_t GetClientMaxspeed;
	pfnEngSrc_CheckParm_t CheckParm;
	pfnEngSrc_Key_Event_t Key_Event;
	pfnEngSrc_GetMousePosition_t GetMousePosition;
	pfnEngSrc_IsNoClipping_t IsNoClipping;
	pfnEngSrc_GetLocalPlayer_t GetLocalPlayer;
	pfnEngSrc_GetViewModel_t GetViewModel;
	pfnEngSrc_GetEntityByIndex_t GetEntityByIndex;
	pfnEngSrc_GetClientTime_t GetClientTime;
	pfnEngSrc_V_CalcShake_t V_CalcShake;
	pfnEngSrc_V_ApplyShake_t V_ApplyShake;
	pfnEngSrc_PM_PointContents_t PM_PointContents;
	pfnEngSrc_PM_WaterEntity_t PM_WaterEntity;
	pfnEngSrc_PM_TraceLine_t PM_TraceLine;
	pfnEngSrc_CL_LoadModel_t CL_LoadModel;
	pfnEngSrc_CL_CreateVisibleEntity_t CL_CreateVisibleEntity;
	pfnEngSrc_GetSpritePointer_t GetSpritePointer;
	pfnEngSrc_pfnPlaySoundByNameAtLocation_t pfnPlaySoundByNameAtLocation;
	pfnEngSrc_pfnPrecacheEvent_t pfnPrecacheEvent;
	pfnEngSrc_pfnPlaybackEvent_t pfnPlaybackEvent;
	pfnEngSrc_pfnWeaponAnim_t pfnWeaponAnim;
	pfnEngSrc_pfnRandomFloat_t pfnRandomFloat;
	pfnEngSrc_pfnRandomLong_t pfnRandomLong;
	pfnEngSrc_pfnHookEvent_t pfnHookEvent;
	pfnEngSrc_Con_IsVisible_t Con_IsVisible;
	pfnEngSrc_pfnGetGameDirectory_t pfnGetGameDirectory;
	pfnEngSrc_pfnGetCvarPointer_t pfnGetCvarPointer;
	pfnEngSrc_Key_LookupBinding_t Key_LookupBinding;
	pfnEngSrc_pfnGetLevelName_t pfnGetLevelName;
	pfnEngSrc_pfnGetScreenFade_t pfnGetScreenFade;
	pfnEngSrc_pfnSetScreenFade_t pfnSetScreenFade;
	pfnEngSrc_VGui_GetPanel_t VGui_GetPanel;
	pfnEngSrc_VGui_ViewportPaintBackground_t VGui_ViewportPaintBackground;
	pfnEngSrc_COM_LoadFile_t COM_LoadFile;
	pfnEngSrc_COM_ParseFile_t COM_ParseFile;
	pfnEngSrc_COM_FreeFile_t COM_FreeFile;
	struct triangleapi_s* pTriAPI;
	struct efx_api_s* pEfxAPI;
	struct event_api_s* pEventAPI;
	struct demo_api_s* pDemoAPI;
	struct net_api_s* pNetAPI;
	struct IVoiceTweak_s* pVoiceTweak;
	pfnEngSrc_IsSpectateOnly_t IsSpectateOnly;
	pfnEngSrc_LoadMapSprite_t LoadMapSprite;
	pfnEngSrc_COM_AddAppDirectoryToSearchPath_t COM_AddAppDirectoryToSearchPath;
	pfnEngSrc_COM_ExpandFilename_t COM_ExpandFilename;
	pfnEngSrc_PlayerInfo_ValueForKey_t PlayerInfo_ValueForKey;
	pfnEngSrc_PlayerInfo_SetValueForKey_t PlayerInfo_SetValueForKey;
	pfnEngSrc_GetPlayerUniqueID_t GetPlayerUniqueID;
	pfnEngSrc_GetTrackerIDForPlayer_t GetTrackerIDForPlayer;
	pfnEngSrc_GetPlayerForTrackerID_t GetPlayerForTrackerID;
	pfnEngSrc_pfnServerCmdUnreliable_t pfnServerCmdUnreliable;
	pfnEngSrc_GetMousePos_t pfnGetMousePos;
	pfnEngSrc_SetMousePos_t pfnSetMousePos;
	pfnEngSrc_SetMouseEnable_t pfnSetMouseEnable;
	pfnEngSrc_GetFirstCVarPtr_t GetFirstCvarPtr;
	pfnEngSrc_GetFirstCmdFunctionHandle_t GetFirstCmdFunctionHandle;
	pfnEngSrc_GetNextCmdFunctionHandle_t GetNextCmdFunctionHandle;
	pfnEngSrc_GetCmdFunctionName_t GetCmdFunctionName;
	pfnEngSrc_GetClientOldTime_t hudGetClientOldTime;
	pfnEngSrc_GetServerGravityValue_t hudGetServerGravityValue;
	pfnEngSrc_GetModelByIndex_t hudGetModelByIndex;
	pfnEngSrc_pfnSetFilterMode_t pfnSetFilterMode;
	pfnEngSrc_pfnSetFilterColor_t pfnSetFilterColor;
	pfnEngSrc_pfnSetFilterBrightness_t pfnSetFilterBrightness;
	pfnEngSrc_pfnSequenceGet_t pfnSequenceGet;
	pfnEngSrc_pfnSPR_DrawGeneric_t pfnSPR_DrawGeneric;
	pfnEngSrc_pfnSequencePickSentence_t pfnSequencePickSentence;
	pfnEngSrc_pfnDrawString_t pfnDrawString;
	pfnEngSrc_pfnDrawStringReverse_t pfnDrawStringReverse;
	pfnEngSrc_LocalPlayerInfo_ValueForKey_t LocalPlayerInfo_ValueForKey;
	pfnEngSrc_pfnVGUI2DrawCharacter_t pfnVGUI2DrawCharacter;
	pfnEngSrc_pfnVGUI2DrawCharacterAdd_t pfnVGUI2DrawCharacterAdd;
	pfnEngSrc_COM_GetApproxWavePlayLength COM_GetApproxWavePlayLength;
	pfnEngSrc_pfnGetCareerUI_t pfnGetCareerUI;
	pfnEngSrc_Cvar_Set_t Cvar_Set;
	pfnEngSrc_pfnIsPlayingCareerMatch_t pfnIsCareerMatch;
	pfnEngSrc_pfnPlaySoundVoiceByName_t pfnPlaySoundVoiceByName;
	pfnEngSrc_pfnPrimeMusicStream_t pfnPrimeMusicStream;
	pfnEngSrc_GetAbsoluteTime_t GetAbsoluteTime;
	pfnEngSrc_pfnProcessTutorMessageDecayBuffer_t pfnProcessTutorMessageDecayBuffer;
	pfnEngSrc_pfnConstructTutorMessageDecayBuffer_t pfnConstructTutorMessageDecayBuffer;
	pfnEngSrc_pfnResetTutorMessageDecayData_t pfnResetTutorMessageDecayData;
	pfnEngSrc_pfnPlaySoundByNameAtPitch_t pfnPlaySoundByNameAtPitch;
	pfnEngSrc_pfnFillRGBABlend_t pfnFillRGBABlend;
	pfnEngSrc_pfnGetAppID_t pfnGetAppID;
	pfnEngSrc_pfnGetAliases_t pfnGetAliasList;
	pfnEngSrc_pfnVguiWrap2_GetMouseDelta_t pfnVguiWrap2_GetMouseDelta;
	pfnEngSrc_pfnFilteredClientCmd_t pfnFilteredClientCmd;
} cl_enginefunc_t;

#define	MAX_INFO_STRING 256
#define MAX_SCOREBOARDNAME 32
#define MAX_QPATH 64

typedef enum {
	t_sound = 0,
	t_skin,
	t_model,
	t_decal,
	t_generic,
	t_eventscript,
	t_world
} resourcetype_t;

typedef struct resource_s {
	char szFileName[MAX_QPATH];
	resourcetype_t type;
	int nIndex;
	int nDownloadSize;
	unsigned char ucFlags;
	unsigned char rgucMD5_hash[16];
	unsigned char playernum;
	unsigned char rguc_reserved[32];
	struct resource_s* pNext;
	struct resource_s* pPrev;
} resource_t;

typedef struct customization_s {
	qboolean bInUse;
	resource_t resource;
	qboolean bTranslated;
	int nUserData1;
	int nUserData2;
	void* pInfo;
	void* pBuffer;
	struct customization_s* pNext;
} customization_t;

typedef struct player_info_s {
	int userid;
	char userinfo[MAX_INFO_STRING];
	char name[MAX_SCOREBOARDNAME];
	int spectator;
	int ping;
	int packet_loss;
	char model[MAX_QPATH];
	int topcolor;
	int bottomcolor;
	int renderframe;
	int gaitsequence;
	float gaitframe;
	float gaityaw;
	vec3_t prevgaitorigin;
	customization_t customdata;
	char hashedcdkey[16];
	uint64 m_nSteamID;
} player_info_t;

typedef struct alight_s {
	int ambientlight;
	int shadelight;
	vec3_t color;
	float* plightvec;
} alight_t;

typedef struct engine_studio_api_s {
	void* (*Mem_Calloc)(int, size_t);
	void* (*Cache_Check)(struct cache_user_s*);
	void (*LoadCacheFile)(char*, struct cache_user_s*);
	struct model_s* (*Mod_ForName)(const char*, int);
	void* (*Mod_Extradata)(struct model_s*);
	struct model_s* (*GetModelByIndex)(int);
	struct cl_entity_s* (*GetCurrentEntity)(void);
	struct player_info_s* (*PlayerInfo)(int);
	struct entity_state_s* (*GetPlayerState)(int);
	struct cl_entity_s* (*GetViewEntity)(void);
	void (*GetTimes)(int*, double*, double*);
	struct cvar_s* (*GetCvar)(const char*);
	void (*GetViewInfo)(float*, float*, float*, float*);
	struct model_s* (*GetChromeSprite)(void);
	void (*GetModelCounters)(int**, int**);
	void (*GetAliasScale)(float*, float*);
	float**** (*StudioGetBoneTransform)(void);
	float**** (*StudioGetLightTransform)(void);
	float*** (*StudioGetAliasTransform)(void);
	float*** (*StudioGetRotationMatrix)(void);
	void (*StudioSetupModel)(int, void**, void**);
	int (*StudioCheckBBox)(void);
	void (*StudioDynamicLight)(struct cl_entity_s*, struct alight_s*);
	void (*StudioEntityLight)(struct alight_s*);
	void (*StudioSetupLighting)(struct alight_s*);
	void (*StudioDrawPoints)(void);
	void (*StudioDrawHulls)(void);
	void (*StudioDrawAbsBBox)(void);
	void (*StudioDrawBones)(void);
	void (*StudioSetupSkin)(void*, int);
	void (*StudioSetRemapColors)(int, int);
	struct model_s* (*SetupPlayerModel)(int);
	void (*StudioClientEvents)(void);
	int (*GetForceFaceFlags)(void);
	void (*SetForceFaceFlags)(int);
	void (*StudioSetHeader)(void*);
	void (*SetRenderModel)(struct model_s*);
	void (*SetupRenderer)(int);
	void (*RestoreRenderer)(void);
	void (*SetChromeOrigin)(void);
	int (*IsHardware)(void);
	void (*GL_StudioDrawShadow)(void);
	void (*GL_SetRenderMode)(int);
	void (*StudioSetRenderamt)(int);
	void (*StudioSetCullState)(int);
	void (*StudioRenderShadow)(int, float*, float*, float*, float*);
} engine_studio_api_t;

typedef struct client_data_s {
	vec3_t origin;
	vec3_t viewangles;
	int iWeaponBits;
	float fov;
} client_data_t;

#define MAX_PHYSENTS 600
#define MAX_MOVEENTS 64

typedef struct usercmd_s {
	short lerp_msec;
	byte msec;
	vec3_t viewangles;
	float forwardmove;
	float sidemove;
	float upmove;
	byte lightlevel;
	unsigned short buttons;
	byte impulse;
	byte weaponselect;
	int impact_index;
	vec3_t impact_position;
} usercmd_t;

#define MAX_PHYSINFO_STRING 256

typedef struct movevars_s {
	float gravity;
	float stopspeed;
	float maxspeed;
	float spectatormaxspeed;
	float accelerate;
	float airaccelerate;
	float wateraccelerate;
	float friction;
	float edgefriction;
	float waterfriction;
	float entgravity;
	float bounce;
	float stepsize;
	float maxvelocity;
	float zmax;
	float waveHeight;
	qboolean footsteps;
	char skyName[32];
	float rollangle;
	float rollspeed;
	float skycolor_r;
	float skycolor_g;
	float skycolor_b;
	float skyvec_x;
	float skyvec_y;
	float skyvec_z;
} movevars_t;

#define MAX_MAP_HULLS 4

typedef struct {
	vec3_t normal;
	float dist;
} plane_t;

typedef struct {
	qboolean allsolid;
	qboolean startsolid;
	qboolean inopen;
	qboolean inwater;
	float fraction;
	vec3_t endpos;
	plane_t plane;
	edict_t* ent;
	int hitgroup;
} trace_t;

typedef struct playermove_s {
	int player_index;
	qboolean server;
	qboolean multiplayer;
	float time;
	float frametime;
	vec3_t forward;
	vec3_t right;
	vec3_t up;
	vec3_t origin;
	vec3_t angles;
	vec3_t oldangles;
	vec3_t velocity;
	vec3_t movedir;
	vec3_t basevelocity;
	vec3_t view_ofs;
	float flDuckTime;
	qboolean bInDuck;
	int flTimeStepSound;
	int iStepLeft;
	float flFallVelocity;
	vec3_t punchangle;
	float flSwimTime;
	float flNextPrimaryAttack;
	int effects;
	int flags;
	int usehull;
	float gravity;
	float friction;
	int oldbuttons;
	float waterjumptime;
	qboolean dead;
	int deadflag;
	int spectator;
	int movetype;
	int onground;
	int waterlevel;
	int watertype;
	int oldwaterlevel;
	char sztexturename[256];
	char chtexturetype;
	float maxspeed;
	float clientmaxspeed;
	int iuser1;
	int iuser2;
	int iuser3;
	int iuser4;
	float fuser1;
	float fuser2;
	float fuser3;
	float fuser4;
	vec3_t vuser1;
	vec3_t vuser2;
	vec3_t vuser3;
	vec3_t vuser4;
	int numphysent;
	physent_t physents[MAX_PHYSENTS];
	int nummoveent;
	physent_t moveents[MAX_MOVEENTS];
	int numvisent;
	physent_t visents[MAX_PHYSENTS];
	usercmd_t cmd;
	int numtouch;
	pmtrace_t touchindex[MAX_PHYSENTS];
	char physinfo[MAX_PHYSINFO_STRING];
	struct movevars_s* movevars;
	vec3_t player_mins[MAX_MAP_HULLS];
	vec3_t player_maxs[MAX_MAP_HULLS];
	const char* (*PM_Info_ValueForKey)(const char*, const char*);
	void (*PM_Particle)(float*, int, float, int, int);
	int (*PM_TestPlayerPosition)(float*, pmtrace_t*);
	void (*Con_NPrintf)(int, char*, ...);
	void (*Con_DPrintf)(char*, ...);
	void (*Con_Printf)(char*, ...);
	double (*Sys_FloatTime)(void);
	void (*PM_StuckTouch)(int, pmtrace_t*);
	int (*PM_PointContents)(float*, int*);
	int (*PM_TruePointContents)(float*);
	int (*PM_HullPointContents)(struct hull_s*, int, float*);
	pmtrace_t (*PM_PlayerTrace)(float*, float*, int, int);
	struct pmtrace_s* (*PM_TraceLine)(float*, float*, int, int, int);
	int32 (*RandomLong)(int32, int32);
	float (*RandomFloat)(float, float);
	int (*PM_GetModelType)(struct model_s*);
	void (*PM_GetModelBounds)(struct model_s*, float*, float*);
	void* (*PM_HullForBsp)(physent_t*, float*);
	float (*PM_TraceModel)(physent_t*, float*, float*, trace_t*);
	int (*COM_FileSize)(char*);
	byte* (*COM_LoadFile)(char*, int, int*);
	void (*COM_FreeFile)(void*);
	char* (*memfgets)(byte*, int, int*, char*, int);
	qboolean runfuncs;
	void (*PM_PlaySound)(int, const char*, float, float, int, int);
	const char* (*PM_TraceTexture)(int, float*, float*);
	void (*PM_PlaybackEventFull)(int, int, unsigned short, float, float*, float*, float, float, int, int, int, int);
	pmtrace_t (*PM_PlayerTraceEx)(float*, float*, int, int (*)(physent_t*));
	int (*PM_TestPlayerPositionEx)(float*, pmtrace_t*, int (*)(physent_t*));
	struct pmtrace_s* (*PM_TraceLineEx)(float*, float*, int, int, int (*)(physent_t*));
} playermove_t;

typedef struct kbutton_s {
	int down[2];
	int state;
} kbutton_t;

typedef struct ref_params_s {
	float vieworg[3];
	float viewangles[3];
	float forward[3];
	float right[3];
	float up[3];
	float frametime;
	float time;
	int intermission;
	int paused;
	int spectator;
	int onground;
	int waterlevel;
	float simvel[3];
	float simorg[3];
	float viewheight[3];
	float idealpitch;
	float cl_viewangles[3];
	int health;
	float crosshairangle[3];
	float viewsize;
	float punchangle[3];
	int	maxclients;
	int	viewentity;
	int	playernum;
	int	max_entities;
	int	demoplayback;
	int	hardware;
	int	smoothing;
	struct usercmd_s* cmd;
	struct movevars_s* movevars;
	int	viewport[4];
	int	nextView;
	int	onlyClientDraw;
} ref_params_t;

typedef struct mstudioevent_s {
	int frame;
	int event;
	int type;
	char options[64];
} mstudioevent_t;

typedef struct clientdata_s {
	vec3_t origin;
	vec3_t velocity;
	int viewmodel;
	vec3_t punchangle;
	int	flags;
	int waterlevel;
	int watertype;
	vec3_t view_ofs;
	float health;
	int	bInDuck;
	int	weapons;
	int	flTimeStepSound;
	int	flDuckTime;
	int	flSwimTime;
	int waterjumptime;
	float maxspeed;
	float fov;
	int	weaponanim;
	int	m_iId;
	int	ammo_shells;
	int	ammo_nails;
	int	ammo_cells;
	int ammo_rockets;
	float m_flNextAttack;
	int	tfstate;
	int	pushmsec;
	int deadflag;
	char physinfo[MAX_PHYSINFO_STRING];
	int	iuser1;
	int	iuser2;
	int	iuser3;
	int iuser4;
	float fuser1;
	float fuser2;
	float fuser3;
	float fuser4;
	vec3_t vuser1;
	vec3_t vuser2;
	vec3_t vuser3;
	vec3_t vuser4;
} clientdata_t;

typedef struct weapon_data_s {
	int	m_iId;
	int	m_iClip;
	float m_flNextPrimaryAttack;
	float m_flNextSecondaryAttack;
	float m_flTimeWeaponIdle;
	int	m_fInReload;
	int	m_fInSpecialReload;
	float m_flNextReload;
	float m_flPumpTime;
	float m_fReloadTime;
	float m_fAimedDamage;
	float m_fNextAimBonus;
	int	m_fInZoom;
	int	m_iWeaponState;
	int	iuser1;
	int	iuser2;
	int	iuser3;
	int	iuser4;
	float fuser1;
	float fuser2;
	float fuser3;
	float fuser4;
} weapon_data_t;

typedef struct local_state_s {
	entity_state_t playerstate;
	clientdata_t client;
	weapon_data_t weapondata[64];
} local_state_t;

typedef struct r_studio_interface_s {
	int	version;
	int (*StudioDrawModel)(int);
	int (*StudioDrawPlayer)(int, struct entity_state_s*);
} r_studio_interface_t;

typedef int (*INITIALIZE_FUNC)(struct cl_enginefuncs_s*, int);
typedef void (*HUD_INIT_FUNC)(void);
typedef int (*HUD_VIDINIT_FUNC)(void);
typedef int (*HUD_REDRAW_FUNC)(float, int);
typedef int (*HUD_UPDATECLIENTDATA_FUNC)(struct client_data_s*, float);
typedef void (*HUD_RESET_FUNC)(void);
typedef void (*HUD_CLIENTMOVE_FUNC)(struct playermove_s*, qboolean);
typedef void (*HUD_CLIENTMOVEINIT_FUNC)(struct playermove_s*);
typedef char (*HUD_TEXTURETYPE_FUNC)(char*);
typedef void (*HUD_IN_ACTIVATEMOUSE_FUNC)(void);
typedef void (*HUD_IN_DEACTIVATEMOUSE_FUNC)(void);
typedef void (*HUD_IN_MOUSEEVENT_FUNC)(int);
typedef void (*HUD_IN_CLEARSTATES_FUNC)(void);
typedef void (*HUD_IN_ACCUMULATE_FUNC)(void);
typedef void (*HUD_CL_CREATEMOVE_FUNC)(float, struct usercmd_s*, int);
typedef int (*HUD_CL_ISTHIRDPERSON_FUNC)(void);
typedef void (*HUD_CL_GETCAMERAOFFSETS_FUNC)(float*);
typedef struct kbutton_s* (*HUD_KB_FIND_FUNC)(const char*);
typedef void (*HUD_CAMTHINK_FUNC)(void);
typedef void (*HUD_CALCREF_FUNC)(struct ref_params_s*);
typedef int (*HUD_ADDENTITY_FUNC)(int, struct cl_entity_s*, const char*);
typedef void (*HUD_CREATEENTITIES_FUNC)(void);
typedef void (*HUD_DRAWNORMALTRIS_FUNC)(void);
typedef void (*HUD_DRAWTRANSTRIS_FUNC)(void);
typedef void (*HUD_STUDIOEVENT_FUNC)(const struct mstudioevent_s*, const struct cl_entity_s*);
typedef void (*HUD_POSTRUNCMD_FUNC)(struct local_state_s*, struct local_state_s*, struct usercmd_s*, int, double, unsigned int);
typedef void (*HUD_SHUTDOWN_FUNC)(void);
typedef void (*HUD_TXFERLOCALOVERRIDES_FUNC)(struct entity_state_s*, const struct clientdata_s*);
typedef void (*HUD_PROCESSPLAYERSTATE_FUNC)(struct entity_state_s*, const struct entity_state_s*);
typedef void (*HUD_TXFERPREDICTIONDATA_FUNC)(struct entity_state_s*, const struct entity_state_s*, struct clientdata_s*, const struct clientdata_s*, struct weapon_data_s*, const struct weapon_data_s*);
typedef void (*HUD_DEMOREAD_FUNC)(int, unsigned char*);
typedef int (*HUD_CONNECTIONLESS_FUNC)(const struct netadr_s*, const char*, char*, int*);
typedef int (*HUD_GETHULLBOUNDS_FUNC)(int, float*, float*);
typedef void (*HUD_FRAME_FUNC)(double);
typedef int (*HUD_KEY_EVENT_FUNC)(int, int, const char*);
typedef void (*HUD_TEMPENTUPDATE_FUNC)(double, double, double, struct tempent_s**, struct tempent_s**, int (*)(struct cl_entity_s*), void (*)(struct tempent_s*, float));
typedef struct cl_entity_s* (*HUD_GETUSERENTITY_FUNC)(int);
typedef void (*HUD_VOICESTATUS_FUNC)(int, qboolean);
typedef void (*HUD_DIRECTORMESSAGE_FUNC)(int, void*);
typedef int (*HUD_STUDIO_INTERFACE_FUNC)(int, struct r_studio_interface_s**, struct engine_studio_api_s*);
typedef void (*HUD_CHATINPUTPOSITION_FUNC)(int*, int*);
typedef int (*HUD_GETPLAYERTEAM)(int);
typedef void* (*CLIENTFACTORY)(...);

typedef struct {
	INITIALIZE_FUNC pInitFunc;
	HUD_INIT_FUNC pHudInitFunc;
	HUD_VIDINIT_FUNC pHudVidInitFunc;
	HUD_REDRAW_FUNC pHudRedrawFunc;
	HUD_UPDATECLIENTDATA_FUNC pHudUpdateClientDataFunc;
	HUD_RESET_FUNC pHudResetFunc;
	HUD_CLIENTMOVE_FUNC pClientMove;
	HUD_CLIENTMOVEINIT_FUNC pClientMoveInit;
	HUD_TEXTURETYPE_FUNC pClientTextureType;
	HUD_IN_ACTIVATEMOUSE_FUNC pIN_ActivateMouse;
	HUD_IN_DEACTIVATEMOUSE_FUNC pIN_DeactivateMouse;
	HUD_IN_MOUSEEVENT_FUNC pIN_MouseEvent;
	HUD_IN_CLEARSTATES_FUNC pIN_ClearStates;
	HUD_IN_ACCUMULATE_FUNC pIN_Accumulate;
	HUD_CL_CREATEMOVE_FUNC pCL_CreateMove;
	HUD_CL_ISTHIRDPERSON_FUNC pCL_IsThirdPerson;
	HUD_CL_GETCAMERAOFFSETS_FUNC pCL_GetCameraOffsets;
	HUD_KB_FIND_FUNC pFindKey;
	HUD_CAMTHINK_FUNC pCamThink;
	HUD_CALCREF_FUNC pCalcRefdef;
	HUD_ADDENTITY_FUNC pAddEntity;
	HUD_CREATEENTITIES_FUNC pCreateEntities;
	HUD_DRAWNORMALTRIS_FUNC pDrawNormalTriangles;
	HUD_DRAWTRANSTRIS_FUNC pDrawTransparentTriangles;
	HUD_STUDIOEVENT_FUNC pStudioEvent;
	HUD_POSTRUNCMD_FUNC pPostRunCmd;
	HUD_SHUTDOWN_FUNC pShutdown;
	HUD_TXFERLOCALOVERRIDES_FUNC pTxferLocalOverrides;
	HUD_PROCESSPLAYERSTATE_FUNC pProcessPlayerState;
	HUD_TXFERPREDICTIONDATA_FUNC pTxferPredictionData;
	HUD_DEMOREAD_FUNC pReadDemoBuffer;
	HUD_CONNECTIONLESS_FUNC pConnectionlessPacket;
	HUD_GETHULLBOUNDS_FUNC pGetHullBounds;
	HUD_FRAME_FUNC pHudFrame;
	HUD_KEY_EVENT_FUNC pKeyEvent;
	HUD_TEMPENTUPDATE_FUNC pTempEntUpdate;
	HUD_GETUSERENTITY_FUNC pGetUserEntity;
	HUD_VOICESTATUS_FUNC pVoiceStatus;
	HUD_DIRECTORMESSAGE_FUNC pDirectorMessage;
	HUD_STUDIO_INTERFACE_FUNC pStudioInterface;
	HUD_CHATINPUTPOSITION_FUNC pChatInputPosition;
	HUD_GETPLAYERTEAM pGetPlayerTeam;
	CLIENTFACTORY pClientFactory;
} cldll_func_t;

typedef struct svc_func_s {
	unsigned char opcode;
	char* pszname;
	void* pfnParse;
} svc_func_t;

#define DRC_CMD_NONE 0
#define DRC_CMD_START 1
#define DRC_CMD_EVENT 2
#define DRC_CMD_MODE 3
#define DRC_CMD_CAMERA 4
#define DRC_CMD_TIMESCALE 5
#define DRC_CMD_MESSAGE 6
#define DRC_CMD_SOUND 7
#define DRC_CMD_STATUS 8
#define DRC_CMD_BANNER 9
#define DRC_CMD_STUFFTEXT 10
#define DRC_CMD_CHASE 11
#define DRC_CMD_INEYE 12
#define DRC_CMD_MAP 13
#define DRC_CMD_CAMPATH 14
#define DRC_CMD_WAYPOINTS 15
#define DRC_CMD_LAST 15

#define MAX_CMD_LINE 1024

typedef enum {
	ca_dedicated = 0,
	ca_disconnected,
	ca_connecting,
	ca_connected,
	ca_uninitialized,
	ca_active
} cactive_t;

typedef enum netsrc_s {
	NS_CLIENT = 0,
	NS_SERVER,
	NS_MULTICAST
} netsrc_t;

typedef struct sizebuf_s {
	const char* buffername;
	unsigned short flags;
	byte* data;
	int maxsize;
	int cursize;
} sizebuf_t;

#define FRAGMENT_MAX_SIZE 1400

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

typedef struct fragbuf_s {
	struct fragbuf_s* next;
	int bufferid;
	sizebuf_t frag_message;
	byte frag_message_buf[FRAGMENT_MAX_SIZE];
	qboolean isfile;
	qboolean isbuffer;
	qboolean iscompressed;
	char filename[MAX_PATH];
	int foffset;
	int size;
} fragbuf_t;

typedef struct fragbufwaiting_s {
	struct fragbufwaiting_s* next;
	int fragbufcount;
	fragbuf_t* fragbufs;
} fragbufwaiting_t;

#define MAX_MSGLEN 5990

enum {
	FRAG_NORMAL_STREAM = 0,
	FRAG_FILE_STREAM,
	MAX_STREAMS
};

typedef struct {
	int size;
	double time;
} flowstats_t;

#define MAX_LATENT 32

typedef struct {
	flowstats_t stats[MAX_LATENT];
	int current;
	double nextcompute;
	float kbytespersec;
	float avgkbytespersec;
} flow_t;

enum {
	FLOW_OUTGOING = 0,
	FLOW_INCOMING,
	MAX_FLOWS
};

typedef struct netchan_s {
	netsrc_t sock;
	netadr_t remote_address;
	int player_slot;
	float last_received;
	float connect_time;
	double rate;
	double cleartime;
	int incoming_sequence;
	int incoming_acknowledged;
	int incoming_reliable_acknowledged;
	int incoming_reliable_sequence;
	int outgoing_sequence;
	int reliable_sequence;
	int last_reliable_sequence;
	void* connection_status;
	int (*pfnNetchan_Blocksize)(void*);
	sizebuf_t message;
	byte message_buf[MAX_MSGLEN];
	int reliable_length;
	byte reliable_buf[MAX_MSGLEN];
	fragbufwaiting_t* waitlist[MAX_STREAMS];
	int reliable_fragment[MAX_STREAMS];
	unsigned int reliable_fragid[MAX_STREAMS];
	fragbuf_t* fragbufs[MAX_STREAMS];
	int fragbufcount[MAX_STREAMS];
	short frag_startpos[MAX_STREAMS];
	short frag_length[MAX_STREAMS];
	fragbuf_t* incomingbufs[MAX_STREAMS];
	qboolean incomingready[MAX_STREAMS];
	char incomingfilename[MAX_PATH];
	void* tempbuffer;
	int tempbuffersize;
	flow_t flow[MAX_FLOWS];
} netchan_t;

#define MAX_DATAGRAM 6000
#define MAX_DEMOS 32

typedef void* FileHandle_t;

typedef struct {
	qboolean bUsed;
	float fTime;
	int nBytesRemaining;
} downloadtime_t;

typedef struct {
	qboolean doneregistering;
	int percent;
	qboolean downloadrequested;
	downloadtime_t rgStats[8];
	int nCurStat;
	int nTotalSize;
	int nTotalToTransfer;
	int nRemainingToTransfer;
	float fLastStatusUpdate;
	qboolean custom;
} incomingtransfer_t;

typedef struct {
	int nStartPercent;
	int nClientSoundFadePercent;
	double soundFadeStartTime;
	int soundFadeOutTime;
	int soundFadeHoldTime;
	int soundFadeInTime;
} soundfade_t;

typedef struct client_static_s {
	cactive_t state;
	netchan_t netchan;
	sizebuf_t datagram;
	byte datagram_buf[MAX_DATAGRAM];
	double connect_time;
	int connect_retry;
	int challenge;
	byte authprotocol;
	int userid;
	char trueaddress[32];
	float slist_time;
	int signon;
	char servername[MAX_PATH];
	char mapstring[64];
	char spawnparms[2048];
	char userinfo[256];
	float nextcmdtime;
	int lastoutgoingcommand;
	int demonum;
	char demos[MAX_DEMOS][16];
	qboolean demorecording;
	qboolean demoplayback;
	qboolean timedemo;
	float demostarttime;
	int demostartframe;
	int forcetrack;
	FileHandle_t demofile;
	FileHandle_t demoheader;
	qboolean demowaiting;
	qboolean demoappending;
	char demofilename[MAX_PATH];
	int demoframecount;
	int td_lastframe;
	int td_startframe;
	float td_starttime;
	incomingtransfer_t dl;
	float packet_loss;
	double packet_loss_recalc_time;
	int playerbits;
	soundfade_t soundfade;
	char physinfo[MAX_PHYSINFO_STRING];
	unsigned char md5_clientdll[16];
	netadr_t game_stream;
	netadr_t connect_stream;
	qboolean passive;
	qboolean spectator;
	qboolean director;
	qboolean fSecureClient;
	qboolean isVAC2Secure;
	unsigned long long GameServerSteamID;
	int build_num;
} client_static_t;

typedef void (*xcommand_t)(void);

typedef struct cmd_function_s {
	struct cmd_function_s* next;
	char* name;
	xcommand_t function;
	int flags;
} cmd_function_t;

#define FCMD_HUD_COMMAND (1 << 0)
#define FCMD_GAME_COMMAND (1 << 1)
#define FCMD_WRAPPER_COMMAND (1 << 2)

#define MAX_RESOURCE_LIST 1280

typedef struct {
	int num_entities;
	unsigned char flags[128];
	entity_state_t* entities;
} packet_entities_t;

typedef struct {
	double receivedtime;
	double latency;
	qboolean invalid;
	qboolean choked;
	entity_state_t playerstate[32];
	double time;
	clientdata_t clientdata;
	weapon_data_t weapondata[64];
	packet_entities_t packet_entities;
	unsigned short clientbytes;
	unsigned short playerinfobytes;
	unsigned short packetentitybytes;
	unsigned short tentitybytes;
	unsigned short soundbytes;
	unsigned short eventbytes;
	unsigned short usrbytes;
	unsigned short voicebytes;
	unsigned short msgbytes;
} frame_t;

typedef struct {
	usercmd_t cmd;
	float senttime;
	float receivedtime;
	float frame_lerp;
	qboolean processedfuncs;
	qboolean heldback;
	int sendsize;
} cmd_t;

struct event_s {
	unsigned short index;
	const char* filename;
	int filesize;
	const char* pszScript;
};

#define MAX_EVENTS 256
#define MAX_MODELS 512

typedef struct sfx_s {
	char name[64];
	cache_user_t cache;
	int servercount;
} sfx_t;

#define MAX_SOUNDS 512

typedef struct consistency_s {
	char* filename;
	int issound;
	int orig_index;
	int value;
	int check_type;
	float mins[3];
	float maxs[3];
} consistency_t;

#define MAX_CONSISTENCY_LIST 512

typedef unsigned int uint32;
typedef uint32 CRC32_t;

#define MAX_EVENT_QUEUE 64

typedef struct event_info_s {
	unsigned short index;
	short packet_index;
	short entity_index;
	float fire_time;
	event_args_t args;
	int flags;
} event_info_t;

typedef struct event_state_s {
	struct event_info_s ei[MAX_EVENT_QUEUE];
} event_state_t;


typedef struct {
	int max_edicts;
	resource_t resourcesonhand;
	resource_t resourcesneeded;
	resource_t resourcelist[MAX_RESOURCE_LIST];
	int num_resources;
	qboolean need_force_consistency_response;
	char serverinfo[512];
	int servercount;
	int validsequence;
	int parsecount;
	int parsecountmod;
	int stats[32];
	int weapons;
	usercmd_t cmd;
	vec3_t viewangles;
	vec3_t punchangle;
	vec3_t crosshairangle;
	vec3_t simorg;
	vec3_t simvel;
	vec3_t simangles;
	vec3_t predicted_origins[64];
	vec3_t prediction_error;
	float idealpitch;
	vec3_t viewheight;
	screenfade_t sf;
	qboolean paused;
	int onground;
	int moving;
	int waterlevel;
	int usehull;
	float maxspeed;
	int pushmsec;
	int light_level;
	int intermission;
	double mtime[2];
	double time;
	double oldtime;
	frame_t frames[64];
	cmd_t commands[64];
	local_state_t predicted_frames[64];
	int delta_sequence;
	int playernum;
	struct event_s event_precache[MAX_EVENTS];
	struct model_s* model_precache[MAX_MODELS];
	int model_precache_count;
	struct sfx_s* sound_precache[MAX_SOUNDS];
	consistency_t consistency_list[MAX_CONSISTENCY_LIST];
	int num_consistency;
	int highentity;
	char levelname[40];
	int maxclients;
	int gametype;
	int viewentity;
	struct model_s* worldmodel;
	struct efrag_s* free_efrags;
	int num_entities;
	int num_statics;
	cl_entity_t viewent;
	int cdtrack;
	int looptrack;
	CRC32_t serverCRC;
	unsigned char clientdllmd5[16];
	float weaponstarttime;
	int weaponsequence;
	int fPrecaching;
	dlight_t* pLight;
	player_info_t players[32];
	entity_state_t instanced_baseline[64];
	int instanced_baseline_number;
	CRC32_t mapCRC;
	event_state_t events;
	char downloadUrl[128];
} client_state_t;

#define	FCVAR_ARCHIVE (1 << 0)
#define	FCVAR_USERINFO (1 << 1)
#define	FCVAR_SERVER (1 << 2)
#define FCVAR_EXTDLL (1 << 3)
#define FCVAR_CLIENTDLL (1 << 4)
#define FCVAR_PROTECTED (1 << 5)
#define FCVAR_SPONLY (1 << 6)
#define FCVAR_PRINTABLEONLY (1 << 7)
#define FCVAR_UNLOGGED (1 << 8)
#define FCVAR_NOEXTRAWHITEPACE (1 << 9)
#define FCVAR_PRIVILEGED (1 << 10)

typedef struct _UserMsg {
	int iMsg;
	int iSize;
	char szName[16];
	struct _UserMsg* next;
	pfnUserMsgHook pfn;
} UserMsg;

typedef unsigned short uint16;

#define	TE_BEAMPOINTS 0
#define	TE_BEAMENTPOINT 1
#define	TE_GUNSHOT 2
#define	TE_EXPLOSION 3
#define	TE_TAREXPLOSION 4
#define	TE_SMOKE 5
#define	TE_TRACER 6
#define	TE_LIGHTNING 7
#define	TE_BEAMENTS 8
#define	TE_SPARKS 9
#define	TE_LAVASPLASH 10
#define	TE_TELEPORT 11
#define TE_EXPLOSION2 12
#define TE_BSPDECAL 13
#define TE_IMPLOSION 14
#define TE_SPRITETRAIL 15
#define TE_BEAM 16
#define TE_SPRITE 17
#define TE_BEAMSPRITE 18
#define TE_BEAMTORUS 19
#define TE_BEAMDISK 20
#define TE_BEAMCYLINDER 21
#define TE_BEAMFOLLOW 22
#define TE_GLOWSPRITE 23
#define TE_BEAMRING 24
#define TE_STREAK_SPLASH 25
#define TE_BEAMHOSE 26
#define TE_DLIGHT 27
#define TE_ELIGHT 28
#define TE_TEXTMESSAGE 29
#define TE_LINE 30
#define TE_BOX 31
#define TE_KILLBEAM	99
#define TE_LARGEFUNNEL 100
#define	TE_BLOODSTREAM 101
#define	TE_SHOWLINE 102
#define TE_BLOOD 103
#define TE_DECAL 104
#define TE_FIZZ 105
#define TE_MODEL 106
#define TE_EXPLODEMODEL 107
#define TE_BREAKMODEL 108
#define TE_GUNSHOTDECAL 109
#define TE_SPRITE_SPRAY 110
#define TE_ARMOR_RICOCHET 111
#define TE_PLAYERDECAL 112
#define TE_BUBBLES 113
#define TE_BUBBLETRAIL 114
#define TE_BLOODSPRITE 115
#define TE_WORLDDECAL 116
#define TE_WORLDDECALHIGH 117
#define TE_DECALHIGH 118
#define TE_PROJECTILE 119
#define TE_SPRAY 120
#define TE_PLAYERSPRITES 121
#define TE_PARTICLEBURST 122
#define TE_FIREFIELD 123
#define TE_PLAYERATTACHMENT 124
#define TE_KILLPLAYERATTACHMENTS 125
#define TE_MULTIGUNSHOT 126
#define TE_USERTRACER 127

enum CursorCode {
	dc_user,
	dc_none,
	dc_arrow,
	dc_ibeam,
	dc_hourglass,
	dc_waitarrow,
	dc_crosshair,
	dc_up,
	dc_sizenwse,
	dc_sizenesw,
	dc_sizewe,
	dc_sizens,
	dc_sizeall,
	dc_no,
	dc_hand,
	dc_blank,
	dc_last
};

class IBaseInterface {
public:
	virtual ~IBaseInterface() {}
};

typedef void* (*CreateInterfaceFn)(const char*, int*);

class IGameUI : public IBaseInterface {
public:
	virtual void Initialize(CreateInterfaceFn* factories, int count) = 0;
	virtual void Start(cl_enginefuncs_s* engineFuncs, int interfaceVersion, /*IBaseSystem**/void* system) = 0;
	virtual void Shutdown(void) = 0;
	virtual int ActivateGameUI(void) = 0;
	virtual int ActivateDemoUI(void) = 0;
	virtual int HasExclusiveInput(void) = 0;
	virtual void RunFrame(void) = 0;
	virtual void ConnectToServer(const char* game, int IP, int port) = 0;
	virtual void DisconnectFromServer(void) = 0;
	virtual void HideGameUI(void) = 0;
	virtual bool IsGameUIActive(void) = 0;
	virtual void LoadingStarted(const char* resourceType, const char* resourceName) = 0;
	virtual void LoadingFinished(const char* resourceType, const char* resourceName) = 0;
	virtual void StartProgressBar(const char* progressType, int progressSteps) = 0;
	virtual int ContinueProgressBar(int progressPoint, float progressFraction) = 0;
	virtual void StopProgressBar(bool bError, const char* failureReason, const char* extendedReason) = 0;
	virtual int SetProgressBarStatusText(const char* statusText) = 0;
	virtual void SetSecondaryProgressBar(float progress) = 0;
	virtual void SetSecondaryProgressBarText(const char* statusText) = 0;
	virtual void ValidateCDKey(bool force, bool inConnect) = 0;
	virtual void OnDisconnectFromServer(int eSteamLoginFailure, const char* username) = 0;
	virtual void SetIsRunningOnSteamDeck(bool bIsRunningOnDeck) = 0;
	virtual bool IsRunningOnSteamDeck(void) const = 0;
};

#define MAX_KV_LEN 128