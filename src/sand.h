#pragma once
#include <stdint.h>

uint8_t* grid_returnPtr();

void grid_Clear();

void grid_CreateElement(int index_x, int index_y, int element);
void grid_DestroyElement(int index_x, int index_y);

void grid_ToggleSim();

void grid_Init();
void grid_Loop();