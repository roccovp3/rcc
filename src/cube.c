#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "cube.h"
#include "console.h"

static struct timespec ts = {
	.tv_sec = 0, 
	.tv_nsec = 0
};

void set_delay(struct timespec *ts, uint32_t delay_in_ms) {
	ts->tv_sec = delay_in_ms / 1000;
	ts->tv_nsec = (delay_in_ms % 1000) * 1000 * 1000;
}

void rotate_face(uint32_t* cube, FACE face, uint8_t ccw) {

	if (ccw) {
		rotate_face(cube, face, 0);
		rotate_face(cube, face, 0);
		rotate_face(cube, face, 0);
		return;
	}

	uint32_t temp;
	cube[face] = (cube[face] << 8) | (cube[face] >> (32 - 8));
	switch (face) {
		case WHITE:
			temp = cube[GREEN];
			cube[GREEN] = (cube[GREEN] & 0xFFFFF000) | (cube[RED] & 0x00000FFF);
			cube[RED] = (cube[RED] & 0xFFFFF000) | (cube[BLUE] & 0x00000FFF);
			cube[BLUE] = (cube[BLUE] & 0xFFFFF000) | (cube[ORANGE] & 0x00000FFF);
			cube[ORANGE] = (cube[ORANGE] & 0xFFFFF000) | (temp & 0x00000FFF);
			break;
		case YELLOW:
			temp = cube[GREEN];
			cube[GREEN] = (cube[GREEN] & 0xF000FFFF) | (cube[ORANGE] & 0x0FFF0000);
			cube[ORANGE] = (cube[ORANGE] & 0xF000FFFF) | (cube[BLUE] & 0x0FFF0000);
			cube[BLUE] = (cube[BLUE] & 0xF000FFFF) | (cube[RED] & 0x0FFF0000);
			cube[RED] = (cube[RED] & 0xF000FFFF) | (temp & 0x0FFF0000);
			break;
		case GREEN:
			temp = cube[YELLOW];
			cube[YELLOW] = (cube[YELLOW] & 0xFFFFF000) | ((cube[RED] & 0xFF000000) >> 24) | ((cube[RED] & 0x0000000F) << 8);
			cube[RED] = (cube[RED] & 0x00FFFFF0) | ((cube[WHITE] & 0x0F000000) >> 24) | ((cube[WHITE] & 0x00FF0000) << 8);
			cube[WHITE] = (cube[WHITE] & 0xF000FFFF) | ((cube[ORANGE] & 0x000FFF00) << 8);
			cube[ORANGE] = (cube[ORANGE] & 0xFFF000FF) | ((temp & 0x00000FFF) << 8);
			break;
		case BLUE:
			temp = cube[YELLOW];
			cube[YELLOW] = (cube[YELLOW] & 0xF000FFFF) | ((cube[ORANGE] & 0xFF000000) >> 8) | ((cube[ORANGE] & 0x0000000F) << 24);
			cube[ORANGE] = (cube[ORANGE] & 0x00FFFFF0) | ((cube[WHITE] & 0x000000FF) << 24) | ((cube[WHITE] & 0x00000F00) >> 8);
			cube[WHITE] = (cube[WHITE] & 0xFFFFF000) | ((cube[RED] & 0x000FFF00) >> 8);
			cube[RED] = (cube[RED] & 0xFFF000FF) | ((temp & 0x0FFF0000) >> 8);
			break;
		case RED:
			temp = cube[YELLOW];
			cube[YELLOW] = (cube[YELLOW] & 0xFFF000FF) | ((cube[BLUE] & 0xFF000000) >> 16) | ((cube[BLUE] & 0x0000000F) << 16);
			cube[BLUE] = (cube[BLUE] & 0x00FFFFF0) | ((cube[WHITE] & 0x0000FF00) << 16) | ((cube[WHITE] & 0x000F0000) >> 16);
			cube[WHITE] = (cube[WHITE] & 0xFFF000FF) | (cube[GREEN] & 0x000FFF00);
			cube[GREEN] = (cube[GREEN] & 0xFFF000FF) | (temp & 0x000FFF00);
			break;
		case ORANGE:
			temp = cube[YELLOW];
			cube[YELLOW] = (cube[YELLOW] & 0x00FFFFF0) | (cube[GREEN] & 0xFF00000F);
			cube[GREEN] = (cube[GREEN] & 0x00FFFFF0) | (cube[WHITE] & 0xFF00000F);
			cube[WHITE] = (cube[WHITE] & 0x00FFFFF0) | ((cube[BLUE] & 0x0000FF00) << 16) | ((cube[BLUE] & 0x000F0000) >> 16);
			cube[BLUE] = (cube[BLUE] & 0xFFF000FF) | ((temp & 0xFF000000) >> 16) | ((temp & 0x0000000F) << 16);
			break;
	}
}

void print_cube_oneline(uint32_t* cube){
	printf("%x %x %x %x %x %x\n", cube[0], cube[1], cube[2], cube[3], cube[4], cube[5]);
}

void print_cube_text(uint32_t* cube) {
	printf("   %x%x%x      \n", (cube[WHITE] & 0x0000000F), (cube[WHITE] & 0x000000F0) >> 4, (cube[WHITE] & 0x00000F00) >> 8);
	printf("   %x%x%x      \n", (cube[WHITE] & 0xF0000000) >> 28, WHITE, (cube[WHITE] & 0x0000F000) >> 12);
	printf("   %x%x%x      \n", (cube[WHITE] & 0x0F000000) >> 24, (cube[WHITE] & 0x00F00000) >> 20, (cube[WHITE] & 0x000F0000) >> 16);
	printf("%x%x%x%x%x%x%x%x%x%x%x%x\n", (cube[ORANGE] & 0x0000000F), (cube[ORANGE] & 0x000000F0) >> 4, (cube[ORANGE] & 0x00000F00) >> 8, (cube[GREEN] & 0x0000000F), (cube[GREEN] & 0x000000F0) >> 4, (cube[GREEN] & 0x00000F00) >> 8, (cube[RED] & 0x0000000F), (cube[RED] & 0x000000F0) >> 4, (cube[RED] & 0x00000F00) >> 8, (cube[BLUE] & 0x0000000F), (cube[BLUE] & 0x000000F0) >> 4, (cube[BLUE] & 0x00000F00) >> 8);
	printf("%x%x%x%x%x%x%x%x%x%x%x%x\n", (cube[ORANGE] & 0xF0000000) >> 28, ORANGE, (cube[ORANGE] & 0x0000F000) >> 12, (cube[GREEN] & 0xF0000000) >> 28, GREEN, (cube[GREEN] & 0x0000F000) >> 12, (cube[RED] & 0xF0000000) >> 28, RED, (cube[RED] & 0x0000F000) >> 12, (cube[BLUE] & 0xF0000000) >> 28, BLUE, (cube[BLUE] & 0x0000F000) >> 12);
	printf("%x%x%x%x%x%x%x%x%x%x%x%x\n", (cube[ORANGE] & 0x0F000000) >> 24, (cube[ORANGE] & 0x00F00000) >> 20, (cube[ORANGE] & 0x000F0000) >> 16, (cube[GREEN] & 0x0F000000) >> 24, (cube[GREEN] & 0x00F00000) >> 20, (cube[GREEN] & 0x000F0000) >> 16, (cube[RED] & 0x0F000000) >> 24, (cube[RED] & 0x00F00000) >> 20, (cube[RED] & 0x000F0000) >> 16, (cube[BLUE] & 0x0F000000) >> 24, (cube[BLUE] & 0x00F00000) >> 20, (cube[BLUE] & 0x000F0000) >> 16);
	printf("   %x%x%x      \n", (cube[YELLOW] & 0x0000000F), (cube[YELLOW] & 0x000000F0) >> 4, (cube[YELLOW] & 0x00000F00) >> 8);
	printf("   %x%x%x      \n", (cube[YELLOW] & 0xF0000000) >> 28, YELLOW, (cube[YELLOW] & 0x0000F000) >> 12);
	printf("   %x%x%x      \n", (cube[YELLOW] & 0x0F000000) >> 24, (cube[YELLOW] & 0x00F00000) >> 20, (cube[YELLOW] & 0x000F0000) >> 16);
}

void print_cube_color(uint32_t* cube) {
    const char* unicode_colors[6] = {
        "\033[47m",           // White
        "\033[42m",           // Green
        "\033[41m",           // Red
        "\033[44m",           // Blue
        "\033[48;5;214m",     // Orange
        "\033[48;2;255;255;0m" // Yellow
    };

    // Print the white face
    printf("      %s  \033[0m%s  \033[0m%s  \033[0m\n",
           unicode_colors[(cube[WHITE] & 0x0000000F)],
           unicode_colors[(cube[WHITE] & 0x000000F0) >> 4],
           unicode_colors[(cube[WHITE] & 0x00000F00) >> 8]);
    printf("      %s  \033[0m%s  \033[0m%s  \033[0m\n",
           unicode_colors[(cube[WHITE] & 0xF0000000) >> 28],
           unicode_colors[WHITE],
           unicode_colors[(cube[WHITE] & 0x0000F000) >> 12]);
    printf("      %s  \033[0m%s  \033[0m%s  \033[0m\n",
           unicode_colors[(cube[WHITE] & 0x0F000000) >> 24],
           unicode_colors[(cube[WHITE] & 0x00F00000) >> 20],
           unicode_colors[(cube[WHITE] & 0x000F0000) >> 16]);

    // Print the middle four faces
    printf("%s  \033[0m%s  \033[0m%s  \033[0m%s  \033[0m%s  \033[0m%s  \033[0m%s  \033[0m%s  \033[0m%s  \033[0m%s  \033[0m%s  \033[0m%s  \033[0m\n",
           unicode_colors[(cube[ORANGE] & 0x0000000F)],
           unicode_colors[(cube[ORANGE] & 0x000000F0) >> 4],
           unicode_colors[(cube[ORANGE] & 0x00000F00) >> 8],
           unicode_colors[(cube[GREEN] & 0x0000000F)],
           unicode_colors[(cube[GREEN] & 0x000000F0) >> 4],
           unicode_colors[(cube[GREEN] & 0x00000F00) >> 8],
           unicode_colors[(cube[RED] & 0x0000000F)],
           unicode_colors[(cube[RED] & 0x000000F0) >> 4],
           unicode_colors[(cube[RED] & 0x00000F00) >> 8],
           unicode_colors[(cube[BLUE] & 0x0000000F)],
           unicode_colors[(cube[BLUE] & 0x000000F0) >> 4],
           unicode_colors[(cube[BLUE] & 0x00000F00) >> 8]);

    printf("%s  \033[0m%s  \033[0m%s  \033[0m%s  \033[0m%s  \033[0m%s  \033[0m%s  \033[0m%s  \033[0m%s  \033[0m%s  \033[0m%s  \033[0m%s  \033[0m\n",
           unicode_colors[(cube[ORANGE] & 0xF0000000) >> 28],
           unicode_colors[ORANGE],
           unicode_colors[(cube[ORANGE] & 0x0000F000) >> 12],
           unicode_colors[(cube[GREEN] & 0xF0000000) >> 28],
           unicode_colors[GREEN],
           unicode_colors[(cube[GREEN] & 0x0000F000) >> 12],
           unicode_colors[(cube[RED] & 0xF0000000) >> 28],
           unicode_colors[RED],
           unicode_colors[(cube[RED] & 0x0000F000) >> 12],
           unicode_colors[(cube[BLUE] & 0xF0000000) >> 28],
           unicode_colors[BLUE],
           unicode_colors[(cube[BLUE] & 0x0000F000) >> 12]);

    printf("%s  \033[0m%s  \033[0m%s  \033[0m%s  \033[0m%s  \033[0m%s  \033[0m%s  \033[0m%s  \033[0m%s  \033[0m%s  \033[0m%s  \033[0m%s  \033[0m\n",
           unicode_colors[(cube[ORANGE] & 0x0F000000) >> 24],
           unicode_colors[(cube[ORANGE] & 0x00F00000) >> 20],
           unicode_colors[(cube[ORANGE] & 0x000F0000) >> 16],
           unicode_colors[(cube[GREEN] & 0x0F000000) >> 24],
           unicode_colors[(cube[GREEN] & 0x00F00000) >> 20],
           unicode_colors[(cube[GREEN] & 0x000F0000) >> 16],
           unicode_colors[(cube[RED] & 0x0F000000) >> 24],
           unicode_colors[(cube[RED] & 0x00F00000) >> 20],
           unicode_colors[(cube[RED] & 0x000F0000) >> 16],
           unicode_colors[(cube[BLUE] & 0x0F000000) >> 24],
           unicode_colors[(cube[BLUE] & 0x00F00000) >> 20],
           unicode_colors[(cube[BLUE] & 0x000F0000) >> 16]);

    // Print the bottom face
    printf("      %s  \033[0m%s  \033[0m%s  \033[0m\n",
           unicode_colors[(cube[YELLOW] & 0x0000000F)],
           unicode_colors[(cube[YELLOW] & 0x000000F0) >> 4],
           unicode_colors[(cube[YELLOW] & 0x00000F00) >> 8]);
    printf("      %s  \033[0m%s  \033[0m%s  \033[0m\n",
           unicode_colors[(cube[YELLOW] & 0xF0000000) >> 28],
           unicode_colors[YELLOW],
           unicode_colors[(cube[YELLOW] & 0x0000F000) >> 12]);
    printf("      %s  \033[0m%s  \033[0m%s  \033[0m\n",
           unicode_colors[(cube[YELLOW] & 0x0F000000) >> 24],
           unicode_colors[(cube[YELLOW] & 0x00F00000) >> 20],
           unicode_colors[(cube[YELLOW] & 0x000F0000) >> 16]);
}

uint8_t is_solved(uint32_t* cube){
	return (cube[0] == 0x00000000) && 
		(cube[1] == 0x11111111) &&
		(cube[2] == 0x22222222) &&
		(cube[3] == 0x33333333) &&
		(cube[4] == 0x44444444) &&
		(cube[5] == 0x55555555);
}

void make_moves(uint32_t* cube, char* algo, uint32_t delay){
	if(!(algo[0])) return;
	char algo_cpy[1000];
	strncpy(algo_cpy, algo, 999);
	algo_cpy[999] = 0;
	char *token;
	token = strtok(algo_cpy, " ");

	set_delay(&ts, delay);

	while (token != 0) {
		nanosleep(&ts, &ts);
		if (!strcmp(token, "U2")) {
			rotate_face(cube, WHITE, 0);
			rotate_face(cube, WHITE, 0);
		} else if (!strcmp(token, "U'")) {
			rotate_face(cube, WHITE, 1);
		} else if (!strcmp(token, "U")) {
			rotate_face(cube, WHITE, 0);
		} else if (!strcmp(token, "F2")) {
			rotate_face(cube, GREEN, 0);
			rotate_face(cube, GREEN, 0);
		} else if (!strcmp(token, "F'")) {
			rotate_face(cube, GREEN, 1);
		} else if (!strcmp(token, "F")) {
			rotate_face(cube, GREEN, 0);
		} else if (!strcmp(token, "R2")) {
			rotate_face(cube, RED, 0);
			rotate_face(cube, RED, 0);
		} else if (!strcmp(token, "R'")) {
			rotate_face(cube, RED, 1);
		} else if (!strcmp(token, "R")) {
			rotate_face(cube, RED, 0);
		} else if (!strcmp(token, "B2")) {
			rotate_face(cube, BLUE, 0);
			rotate_face(cube, BLUE, 0);
		} else if (!strcmp(token, "B'")) {
			rotate_face(cube, BLUE, 1);
		} else if (!strcmp(token, "B")) {
			rotate_face(cube, BLUE, 0);
		} else if (!strcmp(token, "L2")) {
			rotate_face(cube, ORANGE, 0);
			rotate_face(cube, ORANGE, 0);
		} else if (!strcmp(token, "L'")) {
			rotate_face(cube, ORANGE, 1);
		} else if (!strcmp(token, "L")) {
			rotate_face(cube, ORANGE, 0);
		} else if (!strcmp(token, "D2")) {
			rotate_face(cube, YELLOW, 0);
			rotate_face(cube, YELLOW, 0);
		} else if (!strcmp(token, "D'")) {
			rotate_face(cube, YELLOW, 1);
		} else if (!strcmp(token, "D")) {
			rotate_face(cube, YELLOW, 0);
		}
		if(delay != 0){
			draw_console(cube, 0);
		}
		token = strtok(0, " ");
	}
}
