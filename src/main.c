#include <string.h>
#include <stdio.h>
#include "cube.h"
#include "console.h"

#define ALGO_SIZE 1000

uint32_t cube[6] = {0x00000000, 0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x55555555};

int main(int argc, char *argv[]) {
	char algo[ALGO_SIZE];
	if(argv[1] && argc > 1){
		strncpy(algo, argv[1], sizeof(algo)-1);
	}
	algo[sizeof(algo)-1] = 0;
	make_moves(cube, algo, 0);

	start_console(cube);

	return 0;
}