#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cube.h"
#include "solver.h"

#define COMMAND_BUFFER_SIZE 1000

char corners_letter_pairs[200] = {0};
char edges_letter_pairs[200] = {0};

void draw_console(uint32_t* cube, uint8_t print_letter_pairs){
    //clear screen
    printf("\033[2J");
    //set cursor to top of terminal
	printf("\033[1;1H");

	printf("Rubik's Cube Console. Rocco Perciavalle 03/03/2024\n\n");
	print_cube_color(cube);
    if(print_letter_pairs){
        printf("Edges: %s\n", edges_letter_pairs);
        if(strlen(edges_letter_pairs) & 0x1) printf("(Parity)\n");
        printf("Corners: %s\n", corners_letter_pairs);
    }
    printf(">");
}

static void console_loop(uint32_t* cube){
    char command[COMMAND_BUFFER_SIZE];
	while(command[0] != 0x03){
        uint8_t print_letter_pairs = 0;
		fgets(command, sizeof(command), stdin);
		command[strlen(command)-1] = '\0';
		if(!strcmp(command, "solve")){
			solve(cube, 0, edges_letter_pairs, corners_letter_pairs);
            print_letter_pairs = 1;
		} else if(!strcmp(command, "solve fast")){
            solve(cube, 20*1000, edges_letter_pairs, corners_letter_pairs);
        } else if(!strcmp(command, "solve slow")){
            solve(cube, 100*1000, edges_letter_pairs, corners_letter_pairs);
        } else if(!strcmp(command, "exit")){
            exit(0);
        } else {
			make_moves(cube, command, 100*1000); //100 ms delay
		}
        draw_console(cube, print_letter_pairs);
	}
}

void start_console(uint32_t* cube){
    draw_console(cube, 0);
    console_loop(cube);
}