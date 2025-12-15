//
#include "config.h"
#include "sand.h"
#include "ui.h"

struct mouse_control {
	int pos[2];
	int press;
	int release;
	int held;
};
struct mouse_control mouse = { 0 };

struct key_control {
	int selected_element;
};
struct key_control key_control = { 0 };

//funcs for buttons
void temp() {};

void b_clearGrid() {
	grid_Clear();
}
void b_toggleSim() {
	grid_ToggleSim();
}
void b_toggleRulers() {
	render_toggleGrid();
}

//menus
struct menu option_menu = { 0 };

#define M_defualtPOS 100
#define M_defaultWIDTH 120
#define M_defualtHEIGHT_EXTRA 100
#define M_PADDING 10
#define B_height 20
int menu_Init(struct menu* target, int preset) {

	//setup buttons
	switch (preset) {
	case 1: //option
		target->buttons_num = 3;
		target->buttons_ptr = malloc(sizeof(struct button) * target->buttons_num);
		if (target->buttons_ptr == NULL)
			return 0;
		for (int i = 0; i < target->buttons_num; i++) {
			target->buttons_ptr[i].size[0] = M_defaultWIDTH - M_PADDING * 2;
			target->buttons_ptr[i].size[1] = B_height;
			target->buttons_ptr[i].offset[0] = M_PADDING;
			target->buttons_ptr[i].offset[1] = M_PADDING + (B_height + M_PADDING * i * 3);
			switch (i) {
			case 0:
				target->buttons_ptr[i].txt = "pause";
				target->buttons_ptr[i].func = b_toggleSim;
				break;
			case 1:
				target->buttons_ptr[i].txt = "show rulers";
				target->buttons_ptr[i].func = b_toggleRulers;
				break;
			case 2:
				target->buttons_ptr[i].txt = "clear grid";
				target->buttons_ptr[i].func = b_clearGrid;
				break;
			}
		}
		break;
	default: 
		return 0;
	}

	//setup menu
	target->pos[0] = M_defualtPOS;
	target->pos[1] = M_defualtPOS;
	target->size[0] = M_defaultWIDTH;
	target->size[1] = (target->buttons_num * B_height + M_PADDING) + (M_PADDING * 2) + M_defualtHEIGHT_EXTRA;
	switch (preset) {
	case 1:
		target->title = "options";
		break;
	default:
		return 0;
	}

	//we set up the buttons, then menu, so that the menu is sized according to the buttons
	return 1;
}
void menu_CleanUp(struct menu* target) {
	free(target->buttons_ptr);
	target->buttons_ptr = NULL;
}

//
struct menu* ui_returnOptionPTR() {
	return &option_menu;
}

int ui_Init() {
	if (!menu_Init(&option_menu, 1))
		return 0;
	key_control.selected_element = GRID_SAND;
}

void ui_CleanUp(){
	menu_CleanUp(&option_menu);
}

void ui_EventHandle(SDL_Event* event) {
	if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		mouse.press = 1;
		mouse.held = 1;
	}
	if (event->type == SDL_EVENT_MOUSE_BUTTON_UP) {
		mouse.release = 1;
		mouse.held = 0;
	}
	if (event->type == SDL_EVENT_MOUSE_MOTION) {
		float mX, mY;
		SDL_GetMouseState(&mX, &mY);
		mouse.pos[0] = floorf(mX);
		mouse.pos[1] = floorf(mY);
	}
}

void ui_Loop() {
	static int window_selected = 0;
	static int window_offset[2] = { 0 };

	if (mouse.press) {
		if (mouse.pos[0] > option_menu.pos[0] &&
			mouse.pos[0] < option_menu.pos[0] + option_menu.size[0] &&
			mouse.pos[1] > option_menu.pos[1] &&
			mouse.pos[1] < option_menu.pos[1] + option_menu.size[1]) {
			window_selected++;
			int success = 0;
			for (int i = 0; i < option_menu.buttons_num; i++) {
				struct menu* m = &option_menu;
				struct button* b = &option_menu.buttons_ptr[i];
				if (mouse.pos[0] > m->pos[0] + b->offset[0] &&
					mouse.pos[0] < m->pos[0] + b->offset[0] + b->size[0] &&
					mouse.pos[1] > m->pos[1] + b->offset[1] &&
					mouse.pos[1] < m->pos[1] + b->offset[1] + b->size[1]) {
					window_selected++;
					if (b->func == NULL)
						SDL_Log("function ptr to butten is null.");
					else {
						void (*func_ptr)(void) = b->func;
						func_ptr();
					}
					success = 1;
				}
			}

			if (!success) {
				window_selected = 1;
				window_offset[0] = mouse.pos[0] - option_menu.pos[0];
				window_offset[1] = mouse.pos[1] - option_menu.pos[1];
			}
		}
	}
	
	if (mouse.held) {
		if (window_selected == 1) {
			option_menu.pos[0] = mouse.pos[0] - window_offset[0];
			option_menu.pos[1] = mouse.pos[1] - window_offset[1];

			if (option_menu.pos[0] < 0)
				option_menu.pos[0] = 0;
			if (option_menu.pos[0] + option_menu.size[0] > WIN_W)
				option_menu.pos[0] = WIN_W - option_menu.size[0];
			if (option_menu.pos[1] < 0)
				option_menu.pos[1] = 0;
			if (option_menu.pos[1] + option_menu.size[1] > WIN_H)
				option_menu.pos[1] = WIN_H - option_menu.size[1];
		}
		if (window_selected == 0)
			grid_CreateElement(mouse.pos[0] / GRID_SIZE, mouse.pos[1] / GRID_SIZE, key_control.selected_element);
	}

	if (mouse.release) {
		if (window_selected)
			window_selected = 0;
	}
	mouse.press = 0;
	mouse.release = 0;
}