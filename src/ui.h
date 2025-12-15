#pragma once
#include <SDL3/SDL.h>

struct button {
	int offset[2];
	int size[2];
	char* txt;
	void* func;
};

struct menu {
	int pos[2];
	int size[2];
	char* title;
	//
	struct button* buttons_ptr;
	int buttons_num;
};

struct menu* ui_returnOptionPTR();

int ui_Init();

void ui_CleanUp();

void ui_EventHandle(SDL_Event* event);

void ui_Loop();