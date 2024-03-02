#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

typedef enum {
	WHITE = 0,
	GREEN,
	RED,
	BLUE,
	ORANGE,
	YELLOW
} FACE;

uint32_t CUBE[6] = {0x00000000, 0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x55555555};

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
			cube[RED] = (cube[RED] & 0x00FFFFF0) | ((cube[WHITE] & 0x0F000000) >> 24) | ((CUBE[WHITE] & 0x00FF0000) << 8);
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

void print_cube_oneline(){
	printf("%x %x %x %x %x %x\n", CUBE[0], CUBE[1], CUBE[2], CUBE[3], CUBE[4], CUBE[5]);
}

void print_cube_text() {
	printf("   %x%x%x      \n", (CUBE[WHITE] & 0x0000000F), (CUBE[WHITE] & 0x000000F0) >> 4, (CUBE[WHITE] & 0x00000F00) >> 8);
	printf("   %x%x%x      \n", (CUBE[WHITE] & 0xF0000000) >> 28, WHITE, (CUBE[WHITE] & 0x0000F000) >> 12);
	printf("   %x%x%x      \n", (CUBE[WHITE] & 0x0F000000) >> 24, (CUBE[WHITE] & 0x00F00000) >> 20, (CUBE[WHITE] & 0x000F0000) >> 16);
	printf("%x%x%x%x%x%x%x%x%x%x%x%x\n", (CUBE[ORANGE] & 0x0000000F), (CUBE[ORANGE] & 0x000000F0) >> 4, (CUBE[ORANGE] & 0x00000F00) >> 8, (CUBE[GREEN] & 0x0000000F), (CUBE[GREEN] & 0x000000F0) >> 4, (CUBE[GREEN] & 0x00000F00) >> 8, (CUBE[RED] & 0x0000000F), (CUBE[RED] & 0x000000F0) >> 4, (CUBE[RED] & 0x00000F00) >> 8, (CUBE[BLUE] & 0x0000000F), (CUBE[BLUE] & 0x000000F0) >> 4, (CUBE[BLUE] & 0x00000F00) >> 8);
	printf("%x%x%x%x%x%x%x%x%x%x%x%x\n", (CUBE[ORANGE] & 0xF0000000) >> 28, ORANGE, (CUBE[ORANGE] & 0x0000F000) >> 12, (CUBE[GREEN] & 0xF0000000) >> 28, GREEN, (CUBE[GREEN] & 0x0000F000) >> 12, (CUBE[RED] & 0xF0000000) >> 28, RED, (CUBE[RED] & 0x0000F000) >> 12, (CUBE[BLUE] & 0xF0000000) >> 28, BLUE, (CUBE[BLUE] & 0x0000F000) >> 12);
	printf("%x%x%x%x%x%x%x%x%x%x%x%x\n", (CUBE[ORANGE] & 0x0F000000) >> 24, (CUBE[ORANGE] & 0x00F00000) >> 20, (CUBE[ORANGE] & 0x000F0000) >> 16, (CUBE[GREEN] & 0x0F000000) >> 24, (CUBE[GREEN] & 0x00F00000) >> 20, (CUBE[GREEN] & 0x000F0000) >> 16, (CUBE[RED] & 0x0F000000) >> 24, (CUBE[RED] & 0x00F00000) >> 20, (CUBE[RED] & 0x000F0000) >> 16, (CUBE[BLUE] & 0x0F000000) >> 24, (CUBE[BLUE] & 0x00F00000) >> 20, (CUBE[BLUE] & 0x000F0000) >> 16);
	printf("   %x%x%x      \n", (CUBE[YELLOW] & 0x0000000F), (CUBE[YELLOW] & 0x000000F0) >> 4, (CUBE[YELLOW] & 0x00000F00) >> 8);
	printf("   %x%x%x      \n", (CUBE[YELLOW] & 0xF0000000) >> 28, YELLOW, (CUBE[YELLOW] & 0x0000F000) >> 12);
	printf("   %x%x%x      \n", (CUBE[YELLOW] & 0x0F000000) >> 24, (CUBE[YELLOW] & 0x00F00000) >> 20, (CUBE[YELLOW] & 0x000F0000) >> 16);
}

void print_cube_color() {
	const char* unicode_colors[6] = {"\U00002B1C", "\U0001F7E9", "\U0001F7E5", "\U0001F7E6", "\U0001F7E7", "\U0001F7E8"};
	printf("      %s%s%s\n", unicode_colors[(CUBE[WHITE] & 0x0000000F)], unicode_colors[(CUBE[WHITE] & 0x000000F0) >> 4], unicode_colors[(CUBE[WHITE] & 0x00000F00) >> 8]);
	printf("      %s%s%s\n", unicode_colors[(CUBE[WHITE] & 0xF0000000) >> 28], unicode_colors[WHITE], unicode_colors[(CUBE[WHITE] & 0x0000F000) >> 12]);
	printf("      %s%s%s\n", unicode_colors[(CUBE[WHITE] & 0x0F000000) >> 24], unicode_colors[(CUBE[WHITE] & 0x00F00000) >> 20], unicode_colors[(CUBE[WHITE] & 0x000F0000) >> 16]);
	printf("%s%s%s%s%s%s%s%s%s%s%s%s\n", 
	unicode_colors[(CUBE[ORANGE] & 0x0000000F)], unicode_colors[(CUBE[ORANGE] & 0x000000F0) >> 4], unicode_colors[(CUBE[ORANGE] & 0x00000F00) >> 8], 
	unicode_colors[(CUBE[GREEN] & 0x0000000F)], unicode_colors[(CUBE[GREEN] & 0x000000F0) >> 4], unicode_colors[(CUBE[GREEN] & 0x00000F00) >> 8], 
	unicode_colors[(CUBE[RED] & 0x0000000F)], unicode_colors[(CUBE[RED] & 0x000000F0) >> 4], unicode_colors[(CUBE[RED] & 0x00000F00) >> 8], 
	unicode_colors[(CUBE[BLUE] & 0x0000000F)], unicode_colors[(CUBE[BLUE] & 0x000000F0) >> 4], unicode_colors[(CUBE[BLUE] & 0x00000F00) >> 8]);
	printf("%s%s%s%s%s%s%s%s%s%s%s%s\n", 
	unicode_colors[(CUBE[ORANGE] & 0xF0000000) >> 28], unicode_colors[ORANGE], unicode_colors[(CUBE[ORANGE] & 0x0000F000) >> 12], 
	unicode_colors[(CUBE[GREEN] & 0xF0000000) >> 28], unicode_colors[GREEN], unicode_colors[(CUBE[GREEN] & 0x0000F000) >> 12], 
	unicode_colors[(CUBE[RED] & 0xF0000000) >> 28], unicode_colors[RED], unicode_colors[(CUBE[RED] & 0x0000F000) >> 12], 
	unicode_colors[(CUBE[BLUE] & 0xF0000000) >> 28], unicode_colors[BLUE], unicode_colors[(CUBE[BLUE] & 0x0000F000) >> 12]);
	printf("%s%s%s%s%s%s%s%s%s%s%s%s\n", 
	unicode_colors[(CUBE[ORANGE] & 0x0F000000) >> 24], unicode_colors[(CUBE[ORANGE] & 0x00F00000) >> 20], unicode_colors[(CUBE[ORANGE] & 0x000F0000) >> 16], 
	unicode_colors[(CUBE[GREEN] & 0x0F000000) >> 24], unicode_colors[(CUBE[GREEN] & 0x00F00000) >> 20], unicode_colors[(CUBE[GREEN] & 0x000F0000) >> 16], 
	unicode_colors[(CUBE[RED] & 0x0F000000) >> 24], unicode_colors[(CUBE[RED] & 0x00F00000) >> 20], unicode_colors[(CUBE[RED] & 0x000F0000) >> 16], 
	unicode_colors[(CUBE[BLUE] & 0x0F000000) >> 24], unicode_colors[(CUBE[BLUE] & 0x00F00000) >> 20], unicode_colors[(CUBE[BLUE] & 0x000F0000) >> 16]);
	printf("      %s%s%s\n", unicode_colors[(CUBE[YELLOW] & 0x0000000F)], unicode_colors[(CUBE[YELLOW] & 0x000000F0) >> 4], unicode_colors[(CUBE[YELLOW] & 0x00000F00) >> 8]);
	printf("      %s%s%s\n", unicode_colors[(CUBE[YELLOW] & 0xF0000000) >> 28], unicode_colors[YELLOW], unicode_colors[(CUBE[YELLOW] & 0x0000F000) >> 12]);
	printf("      %s%s%s\n", unicode_colors[(CUBE[YELLOW] & 0x0F000000) >> 24], unicode_colors[(CUBE[YELLOW] & 0x00F00000) >> 20], unicode_colors[(CUBE[YELLOW] & 0x000F0000) >> 16]);
}

int main(int argc, char *argv[]) {
	char algo[1000];
	strcpy(algo, "R U R' U' R' F R2 U' R' U' R U R' F' F R U' R' U' R U R' F' R U R' U' R' F R F'");
	if(argv[1]){
		strncpy(algo, argv[1], 999);
	}
	algo[1000] = 0;
	char *token;
	token = strtok(algo, " ");

	while (token != NULL) {
		printf("%s ", token);

		if (!strcmp(token, "U2")) {
			rotate_face(CUBE, WHITE, 0);
			rotate_face(CUBE, WHITE, 0);
		} else if (!strcmp(token, "U'")) {
			rotate_face(CUBE, WHITE, 1);
		} else if (!strcmp(token, "U")) {
			rotate_face(CUBE, WHITE, 0);
		} else if (!strcmp(token, "F2")) {
			rotate_face(CUBE, GREEN, 0);
			rotate_face(CUBE, GREEN, 0);
		} else if (!strcmp(token, "F'")) {
			rotate_face(CUBE, GREEN, 1);
		} else if (!strcmp(token, "F")) {
			rotate_face(CUBE, GREEN, 0);
		} else if (!strcmp(token, "R2")) {
			rotate_face(CUBE, RED, 0);
			rotate_face(CUBE, RED, 0);
		} else if (!strcmp(token, "R'")) {
			rotate_face(CUBE, RED, 1);
		} else if (!strcmp(token, "R")) {
			rotate_face(CUBE, RED, 0);
		} else if (!strcmp(token, "B2")) {
			rotate_face(CUBE, BLUE, 0);
			rotate_face(CUBE, BLUE, 0);
		} else if (!strcmp(token, "B'")) {
			rotate_face(CUBE, BLUE, 1);
		} else if (!strcmp(token, "B")) {
			rotate_face(CUBE, BLUE, 0);
		} else if (!strcmp(token, "L2")) {
			rotate_face(CUBE, ORANGE, 0);
			rotate_face(CUBE, ORANGE, 0);
		} else if (!strcmp(token, "L'")) {
			rotate_face(CUBE, ORANGE, 1);
		} else if (!strcmp(token, "L")) {
			rotate_face(CUBE, ORANGE, 0);
		} else if (!strcmp(token, "D2")) {
			rotate_face(CUBE, YELLOW, 0);
			rotate_face(CUBE, YELLOW, 0);
		} else if (!strcmp(token, "D'")) {
			rotate_face(CUBE, YELLOW, 1);
		} else if (!strcmp(token, "D")) {
			rotate_face(CUBE, YELLOW, 0);
		}
		token = strtok(NULL, " ");
	}
	printf("\n");
	print_cube_color();
	return 0;
}