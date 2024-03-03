#include <string.h>
#include <stdio.h>
#include "cube.h"
#include "solver.h"

uint32_t cube[6] = {0x00000000, 0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x55555555};

int main(int argc, char *argv[]) {
	char algo[1000];
	strcpy(algo, "R U R' U' R' F R2 U' R' U' R U R' F' F R U' R' U' R U R' F' R U R' U' R' F R F'");
	if(argv[1] && argc > 1){
		strncpy(algo, argv[1], 999);
	}
	make_moves(cube, algo);
	solve(cube);
	return 0;
}