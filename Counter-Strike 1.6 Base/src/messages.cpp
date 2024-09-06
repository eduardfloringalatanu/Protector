#include "messages.h"
#include "Windows.h"
#include <stdio.h>
#include <stdarg.h>
#include <string>

void Error(const char* const message, ...) {
	va_list args;
	char buffer[1024];
	
	va_start(args, message);
	vsprintf(buffer, message, args);
	va_end(args);

	std::string text = buffer;
	text += "\n\nThe game will now exit.";

	if (MessageBox(NULL, text.c_str(), "Error", MB_OK | MB_ICONERROR) == IDOK)
		ExitProcess(EXIT_SUCCESS);
}

void Warning(const char* const message, ...) {
	va_list args;
	char buffer[1024];

	va_start(args, message);
	vsprintf(buffer, message, args);
	va_end(args);

	std::string text = buffer;
	text += "\n\nDo you want to exit the game?";

	if (MessageBox(NULL, text.c_str(), "Warning", MB_YESNO | MB_ICONWARNING) == IDYES)
		ExitProcess(EXIT_SUCCESS);
}