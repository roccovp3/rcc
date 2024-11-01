#ifndef cube_H
#define cube_H
#include <stdint.h>

typedef enum {
	WHITE = 0,
	GREEN,
	RED,
	BLUE,
	ORANGE,
	YELLOW
} FACE;

void rotate_face(uint32_t* cube, FACE face, uint8_t ccw);

void print_cube_oneline(uint32_t* cube);

void print_cube_text(uint32_t* cube);

void print_cube_color(uint32_t* cube);

uint8_t is_solved(uint32_t* cube);

// delay in ms
void make_moves(uint32_t* cube, char* algo, uint32_t delay);

#endif
