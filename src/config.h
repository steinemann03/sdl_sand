#pragma once

//program window
#define WIN_W 1600.f
#define WIN_H 800.f

//grid simulation
#define GRID_SIZE 16 //pixels
#define GRID_W (int)WIN_W / GRID_SIZE
#define GRID_H (int)WIN_H / GRID_SIZE

#define GRID_INDEX(x, y) ((y) * GRID_W + (x))

#define GRID_AIR 0
#define GRID_SAND 1

//ui

//rendering
