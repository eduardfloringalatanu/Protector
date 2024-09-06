#pragma once

#include "imgui.h"
#include "hlsdk.h"
#include <vector>
#include <string>

extern ImFont* header1_font;
extern ImFont* header2_font;
extern ImFont* body_font;

int is_in_commands_whitelist(const char* const command_name);
void add_command_to_whitelist(const char* const command_name, bool regex, bool forward_to_server);

bool is_in_unknown_commands_list(const char* const command_name);
void add_unknown_command_to_list(const char* const command_name, bool regex);

bool is_in_fake_cvars_list(const char* const cvar_name, std::string& output);
void add_fake_cvar_to_list(const char* const cvar_name, const char* const value, bool regex);

int is_in_extensions_whitelist(const char* const extension);
void add_extension_to_whitelist(const char* const extension);

bool is_in_consistency_list(const char* const filename, unsigned char* output);
void add_consistency_to_list(const char* const filename, const char* const md5);

extern bool follow_redirects;
extern bool block_motd;
extern bool random_cdkey;
extern bool random_steamid;
extern char custom_cdkey[33];
extern long custom_steamid;
extern bool auto_retry;
extern bool remove_hud_messages;
extern bool remove_effects;
extern bool remove_non_player_entities;
extern bool catch_Host_Frame_exceptions;
extern int chat_color;

void Log(const char* const text, ...);

extern bool show_menu_window;

void DrawMenu();