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

void print_cube_oneline();

void print_cube_text();

void print_cube_color();

#endif
