#include <string.h>
#include <stdio.h>
#include "cube.h"

uint32_t cube[6] = {0x00000000, 0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x55555555};

int main(int argc, char *argv[]) {
	char algo[1000];
	strcpy(algo, "R U R' U' R' F R2 U' R' U' R U R' F' F R U' R' U' R U R' F' R U R' U' R' F R F'");
	if(argv[1] && argc > 1){
		strncpy(algo, argv[1], 999);
	}
	algo[1000] = 0;
	char *token;
	token = strtok(algo, " ");

	while (token != 0) {
		printf("%s ", token);

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
		token = strtok(0, " ");
	}
	printf("\n");
	print_cube_color(cube);
	return 0;
}