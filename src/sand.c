//
#include <string.h>
#include "config.h"
#include "sand.h"

//grid var
uint8_t GRID[GRID_W * GRID_H] = { 0 };
int sim = 1;

//helper functions

//ok im stupid, this checks if the coordinates are INSIDE the grid
int insideBounds(int x, int y) {
	return (x >= 0 && x < GRID_W && y >= 0 && y < GRID_H);
}

uint8_t* grid_returnPtr() {
	return &GRID;
}

void grid_Clear() {
	uint8_t temp[GRID_W * GRID_W] = { 0 };
	memcpy(&GRID, &temp, sizeof(temp));
}

void grid_CreateElement(int index_x, int index_y, int element) {
	if (!insideBounds(index_x, index_y))
		return 0;
	if (GRID[GRID_INDEX(index_x, index_y)] != GRID_AIR)
		return 0;
	GRID[GRID_INDEX(index_x, index_y)] = element;
	return 1;
}

void grid_DestroyElement(int index_x, int index_y) {
	grid_CreateElement(index_x, index_y, GRID_AIR);
}

void grid_ToggleSim() {
    sim = !sim;
}

void grid_Init() {
	grid_Clear();
    grid_CreateElement(GRID_W / 2, GRID_H / 2, GRID_SAND);
}
void grid_Loop() {
    if (!sim)
        return;
    uint8_t GRID_SNAPSHOT[GRID_W * GRID_H] = { 0 };
    memcpy(&GRID_SNAPSHOT, &GRID, sizeof(GRID));
    for (int x = 0; x < GRID_W; x++)
        for (int y = 0; y < GRID_H; y++) {
            switch (GRID_SNAPSHOT[GRID_INDEX(x, y)]) {
            case GRID_AIR:
                continue;
            case GRID_SAND:
                if (insideBounds(x, y + 1)) {
                    if (GRID_SNAPSHOT[GRID_INDEX(x, y + 1)] == GRID_AIR) {
                        GRID[GRID_INDEX(x, y)] = GRID_AIR;
                        GRID[GRID_INDEX(x, y + 1)] = GRID_SAND;
                        break;
                    }
                }
                if (insideBounds(x - 1, y + 1)) {
                    if (GRID_SNAPSHOT[GRID_INDEX(x - 1, y + 1)] == GRID_AIR) {
                        GRID[GRID_INDEX(x, y)] = GRID_AIR;
                        GRID[GRID_INDEX(x - 1, y + 1)] = GRID_SAND;
                        break;
                    }
                }
                if (insideBounds(x + 1, y + 1)) {
                    if (GRID_SNAPSHOT[GRID_INDEX(x + 1, y + 1)] == GRID_AIR) {
                        GRID[GRID_INDEX(x, y)] = GRID_AIR;
                        GRID[GRID_INDEX(x + 1, y + 1)] = GRID_SAND;
                        break;
                    }
                }
                break;
            default:
                break;
            }
        }
}