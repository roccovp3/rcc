#include <stdint.h>
#include <stdio.h>
#include "solver.h"
#include "cube.h"

#include <stdlib.h>

#define CORNER(a, b, c) ( a | (b << 4) | (c << 8))
#define EDGE(a, b) (a | (b << 4))

typedef struct {
    uint16_t corner;
    char letter; // Letter corresponds to resources/letterscheme.png
    char setup[8];
    char setup_undo[8];
} corner_t;

typedef struct {
    uint8_t edge;
    char letter; // Letter corresponds to resources/letterscheme.png
    char setup[12];
    char setup_undo[12];
} edge_t;

/* all corners and rotations */
const corner_t CORNERS[24] = {
    {.corner=CORNER(WHITE, ORANGE, BLUE), .letter='A', .setup={0}, .setup_undo={0}},
    {.corner=CORNER(WHITE, BLUE, RED), .letter='B', .setup="R D'", .setup_undo="D R'"},
    {.corner=CORNER(WHITE, RED, GREEN), .letter='C', .setup="F", .setup_undo="F'"},
    {.corner=CORNER(WHITE, GREEN, ORANGE), .letter='D', .setup="F R'", .setup_undo="R F'"},
    {.corner=CORNER(YELLOW, ORANGE, GREEN), .letter='U', .setup="F'", .setup_undo="F"},
    {.corner=CORNER(YELLOW, GREEN, RED), .letter='V', .setup="D' F'", .setup_undo="F D"},
    {.corner=CORNER(YELLOW, RED, BLUE), .letter='W', .setup="D2 F'", .setup_undo="F D2"},
    {.corner=CORNER(YELLOW, BLUE, ORANGE), .letter='X', .setup="D F'", .setup_undo="F D'"},

    {.corner=CORNER(BLUE, WHITE, ORANGE), .letter='R', .setup={0}, .setup_undo={0}},
    {.corner=CORNER(RED, WHITE, BLUE), .letter='N', .setup="R2", .setup_undo="R2"},
    {.corner=CORNER(GREEN, WHITE, RED), .letter='J', .setup="F2 D", .setup_undo="D' F2"},
    {.corner=CORNER(ORANGE, WHITE, GREEN), .letter='F', .setup="F2", .setup_undo="F2"},
    {.corner=CORNER(GREEN, YELLOW, ORANGE), .letter='L', .setup="D", .setup_undo="D'"},
    {.corner=CORNER(RED, YELLOW, GREEN), .letter='P', .setup={0}, .setup_undo={0}},
    {.corner=CORNER(BLUE, YELLOW, RED), .letter='T', .setup="D'", .setup_undo="D"},
    {.corner=CORNER(ORANGE, YELLOW, BLUE), .letter='H', .setup="D2", .setup_undo="D2"},

    {.corner=CORNER(ORANGE, BLUE, WHITE), .letter='E', .setup={0}, .setup_undo={0}},
    {.corner=CORNER(BLUE, RED, WHITE), .letter='Q', .setup="R' F", .setup_undo="F' R"},
    {.corner=CORNER(RED, GREEN, WHITE), .letter='M', .setup="R'", .setup_undo="R"},
    {.corner=CORNER(GREEN, ORANGE, WHITE), .letter='I', .setup="F' D", .setup_undo="D' F"},
    {.corner=CORNER(ORANGE, GREEN, YELLOW), .letter='G', .setup="D2 R", .setup_undo="R' D2"},
    {.corner=CORNER(GREEN, RED, YELLOW), .letter='K', .setup="D R", .setup_undo="R' D'"},
    {.corner=CORNER(RED, BLUE, YELLOW), .letter='O', .setup="R", .setup_undo="R'"},
    {.corner=CORNER(BLUE, ORANGE, YELLOW), .letter='S', .setup="D' R", .setup_undo="R' D"}
};

const edge_t EDGES[24] = {
    {.edge=EDGE(WHITE, ORANGE), .letter='D', .setup={0}, .setup_undo={0}},
    {.edge=EDGE(WHITE, BLUE), .letter='A', .setup="R2 U' R2", .setup_undo="R2 U R2"},
    {.edge=EDGE(WHITE, RED), .letter='B', .setup={0}, .setup_undo={0}},
    {.edge=EDGE(WHITE, GREEN), .letter='C', .setup="R2 U R2", .setup_undo="R2 U' R2"},
    {.edge=EDGE(YELLOW, ORANGE), .letter='X', .setup="L2", .setup_undo="L2"},
    {.edge=EDGE(YELLOW, GREEN), .letter='U', .setup="D' L2", .setup_undo="L2 D"},
    {.edge=EDGE(YELLOW, RED), .letter='V', .setup="D2 L2", .setup_undo="L2 D2"},
    {.edge=EDGE(YELLOW, BLUE), .letter='W', .setup="D L2", .setup_undo="L2 D'"},
    {.edge=EDGE(BLUE, RED), .letter='T', .setup="U2 R' U2", .setup_undo="U2 R U2"},
    {.edge=EDGE(BLUE, ORANGE), .letter='R', .setup="L", .setup_undo="L'"},
    {.edge=EDGE(RED, GREEN), .letter='P', .setup="U' F' U", .setup_undo="U' F U"},
    {.edge=EDGE(ORANGE, GREEN), .letter='F', .setup="U' F U", .setup_undo="U' F' U"},

    {.edge=EDGE(ORANGE, WHITE), .letter='E', .setup="L' U B' U'", .setup_undo="U B U' L"},
    {.edge=EDGE(BLUE, WHITE), .letter='Q', .setup="R' B L R", .setup_undo="R' L' B' R"},
    {.edge=EDGE(RED, WHITE), .letter='M', .setup={0}, .setup_undo={0}},
    {.edge=EDGE(GREEN, WHITE), .letter='I', .setup="R F' L' R'", .setup_undo="R L F R'"},
    {.edge=EDGE(ORANGE, YELLOW), .letter='G', .setup="D F L' F'", .setup_undo="F L F' D'"},
    {.edge=EDGE(GREEN, YELLOW), .letter='K', .setup="F L' F'", .setup_undo="F L F'"},
    {.edge=EDGE(RED, YELLOW), .letter='O', .setup="D' F L' F'", .setup_undo="F L F' D"},
    {.edge=EDGE(BLUE, YELLOW), .letter='S', .setup="B' L B", .setup_undo="B' L' B"},
    {.edge=EDGE(RED, BLUE), .letter='N', .setup="U B U'", .setup_undo="U B' U'"},
    {.edge=EDGE(ORANGE, BLUE), .letter='H', .setup="U B' U'", .setup_undo="U B U'"},
    {.edge=EDGE(GREEN, RED), .letter='J', .setup="U2 R U2", .setup_undo="U2 R' U2"},
    {.edge=EDGE(GREEN, ORANGE), .letter='L', .setup="L'", .setup_undo="L"}
};

void get_solved_corners(uint32_t* cube, uint16_t* solved_corners, uint8_t* count){
    //can't cache buffer
    //if(((cube[WHITE] & 0x0000000F) | ((cube[ORANGE] & 0x0000000F) << 4) | ((cube[BLUE] & 0x00000F00))) == CORNER(WHITE, ORANGE, BLUE)) solved_corners[(*count)++] = CORNER(WHITE, ORANGE, BLUE);
    if((((cube[WHITE] & 0x00000F00) >> 8) | ((cube[BLUE] & 0x0000000F) << 4) | ((cube[RED] & 0x00000F00))) == CORNER(WHITE, BLUE, RED)) solved_corners[(*count)++] = CORNER(WHITE, BLUE, RED);
    if((((cube[WHITE] & 0x000F0000) >> 16) | ((cube[RED] & 0x0000000F) << 4) | ((cube[GREEN] & 0x00000F00))) == CORNER(WHITE, RED, GREEN)) solved_corners[(*count)++] = CORNER(WHITE, RED, GREEN);
    if((((cube[WHITE] & 0x0F000000) >> 24) | ((cube[GREEN] & 0x0000000F) << 4) | ((cube[ORANGE] & 0x00000F00))) == CORNER(WHITE, GREEN, ORANGE)) solved_corners[(*count)++] = CORNER(WHITE, GREEN, ORANGE);

    if(((cube[YELLOW] & 0x0000000F) | ((cube[ORANGE] & 0x000F0000) >> 12) | ((cube[GREEN] & 0x0F000000) >> 16)) == CORNER(YELLOW, ORANGE, GREEN)) solved_corners[(*count)++] = CORNER(YELLOW, ORANGE, GREEN);
    if((((cube[YELLOW] & 0x00000F00) >> 8) | ((cube[GREEN] & 0x000F0000) >> 12) | ((cube[RED] & 0x0F000000) >> 16)) == CORNER(YELLOW, GREEN, RED)) solved_corners[(*count)++] = CORNER(YELLOW, GREEN, RED);
    if((((cube[YELLOW] & 0x000F0000) >> 16) | ((cube[RED] & 0x000F0000) >> 12) | ((cube[BLUE] & 0x0F000000) >> 16)) == CORNER(YELLOW, RED, BLUE)) solved_corners[(*count)++] = CORNER(YELLOW, RED, BLUE);
    if((((cube[YELLOW] & 0x0F000000) >> 24) | ((cube[BLUE] & 0x000F0000) >> 12) | ((cube[ORANGE] & 0x0F000000) >> 16)) == CORNER(YELLOW, BLUE, ORANGE)) solved_corners[(*count)++] = CORNER(YELLOW, BLUE, ORANGE);
}

void solve_corners(uint32_t* cube, uint32_t delay, char* corners_letter_pairs){
    uint8_t corners_letter_pairs_i = 0;
    corners_letter_pairs[0] = 0;
    char swap_algo[40] = "R U' R' U' R U R' F' R U R' U' R' F R";
    uint16_t solved_corners[8] = {0};
    uint8_t solved_corners_count = 0;
    get_solved_corners(cube, solved_corners, &solved_corners_count);
    while(solved_corners_count < 7){
        uint8_t buffer_top = cube[WHITE]&0xF;
        uint8_t buffer_left = cube[ORANGE]&0xF;
        uint8_t buffer_back = (cube[BLUE]&0x00000F00) >> 8;
        uint16_t buffer = CORNER(buffer_top, buffer_left, buffer_back);
        for(uint8_t i = 0; i < 24; i++){
            // != are for the buffer corner.
            if((buffer == CORNERS[i].corner) && (buffer != 0x340) && (buffer != 0x403) && (buffer != 0x034)){
                corners_letter_pairs[corners_letter_pairs_i++] = CORNERS[i].letter;
                corners_letter_pairs[corners_letter_pairs_i] = 0;
                if(is_solved(cube)) return;
                solved_corners[solved_corners_count++] = CORNERS[i%8].corner;
                make_moves(cube, (char*)CORNERS[i].setup, delay);
                make_moves(cube, swap_algo, delay);
                make_moves(cube, (char*)CORNERS[i].setup_undo, delay);
                break;
            } else if ((buffer == 0x340) || (buffer == 0x403) || (buffer == 0x034)){
                corner_t swap;
                for(uint8_t j = 0; j < 8; j++){
                    uint8_t already_solved = 0;
                    for(uint8_t k = 0; k < 8; k++){
                        if((CORNERS[j].corner == solved_corners[k]) || 
                            (CORNERS[j].corner == 0x340) || 
                            (CORNERS[j].corner == 0x403) || 
                            (CORNERS[j].corner == 0x034)) {
                                already_solved = 1;
                                break;
                            }
                    }
                    if(!already_solved){
                        swap = CORNERS[j];
                        break;
                    }
                }
                corners_letter_pairs[corners_letter_pairs_i++] = swap.letter;
                corners_letter_pairs[corners_letter_pairs_i] = 0;
                if(is_solved(cube)) return;
                make_moves(cube, swap.setup, delay);
                make_moves(cube, swap_algo, delay);
                make_moves(cube, swap.setup_undo, delay);
                break;
            }
        }
    }
}

void get_solved_edges(uint32_t* cube, uint8_t* solved_edges, uint8_t* count){
    //can't cache buffer
    //if((((cube[WHITE] & 0x0000F000) >> 12) | (cube[RED] & 0x000000F0)) == EDGE(WHITE, RED)) solved_edges[(*count)++] = EDGE(WHITE, RED);
    if((((cube[WHITE] & 0xF0000000) >> 28) | (cube[ORANGE] & 0x000000F0)) == EDGE(WHITE, ORANGE)) solved_edges[(*count)++] = EDGE(WHITE, ORANGE);
    if((((cube[WHITE] & 0x00F00000) >> 20) | (cube[GREEN] & 0x000000F0)) == EDGE(WHITE, GREEN)) solved_edges[(*count)++] = EDGE(WHITE, GREEN);
    if((((cube[WHITE] & 0x000000F0) >> 4) | (cube[BLUE] & 0x000000F0)) == EDGE(WHITE, BLUE)) solved_edges[(*count)++] = EDGE(WHITE, BLUE);

    if((((cube[YELLOW] & 0xF0000000) >> 28) | ((cube[ORANGE] & 0x00F00000) >> 16)) == EDGE(YELLOW, ORANGE)) solved_edges[(*count)++] = EDGE(YELLOW, ORANGE);
    if((((cube[YELLOW] & 0x00F00000) >> 20) | ((cube[GREEN] & 0x00F00000) >> 16)) == EDGE(YELLOW, GREEN)) solved_edges[(*count)++] = EDGE(YELLOW, GREEN);
    if((((cube[YELLOW] & 0x0000F000) >> 12) | ((cube[RED] & 0x00F00000) >> 16)) == EDGE(YELLOW, RED)) solved_edges[(*count)++] = EDGE(YELLOW, RED);
    if((((cube[YELLOW] & 0x000000F0) >> 4) | ((cube[BLUE] & 0x00F00000) >> 16)) == EDGE(YELLOW, BLUE)) solved_edges[(*count)++] = EDGE(YELLOW, BLUE);

    if((((cube[BLUE] & 0xF0000000) >> 28) | ((cube[RED] & 0x0000F000) >> 8)) == EDGE(BLUE, RED)) solved_edges[(*count)++] = EDGE(BLUE, RED);
    if((((cube[BLUE] & 0x0000F000) >> 12) | ((cube[ORANGE] & 0xF0000000) >> 24)) == EDGE(BLUE, ORANGE)) solved_edges[(*count)++] = EDGE(BLUE, ORANGE);
    if((((cube[RED] & 0xF0000000) >> 28) | ((cube[GREEN] & 0x0000F000) >> 8)) == EDGE(RED, GREEN)) solved_edges[(*count)++] = EDGE(RED, GREEN);
    if((((cube[ORANGE] & 0x0000F000) >> 12) | ((cube[GREEN] & 0xF0000000) >> 24)) == EDGE(ORANGE, GREEN)) solved_edges[(*count)++] = EDGE(ORANGE, GREEN);
}

uint8_t solve_edges(uint32_t* cube, uint32_t delay, char* edges_letter_pairs){
    uint8_t edges_letter_pairs_i = 0;
    edges_letter_pairs[0] = 0;
    char swap_algo[40] = "R U R' U' R' F R2 U' R' U' R U R' F'";
    uint8_t solved_edges[12] = {0};
    uint8_t solved_edges_count = 0;
    get_solved_edges(cube, solved_edges, &solved_edges_count);
    uint8_t parity = solved_edges_count&0x1;
    while(solved_edges_count < 11){
        uint8_t buffer_top = (cube[WHITE] & 0x0000F000) >> 12;
        uint8_t buffer_right = (cube[RED] & 0x000000F0) >> 4;
        uint8_t buffer = EDGE(buffer_top, buffer_right);
        for(uint8_t i = 0; i < 24; i++){
            // != are for the buffer edge.
            if((buffer == EDGES[i].edge) && (buffer != EDGE(WHITE, RED)) && (buffer != EDGE(RED, WHITE))){
                edges_letter_pairs[edges_letter_pairs_i++] = EDGES[i].letter;
                edges_letter_pairs[edges_letter_pairs_i] = 0;
                if(is_solved(cube)) return 0;
                solved_edges[solved_edges_count++] = EDGES[i%12].edge;
                make_moves(cube, (char*)EDGES[i].setup, delay);
                make_moves(cube, swap_algo, delay);
                make_moves(cube, (char*)EDGES[i].setup_undo, delay);
                parity = !parity;
                break;
            } else if ((buffer == EDGE(WHITE, RED)) || (buffer == EDGE(RED, WHITE))){
                edge_t swap;
                for(uint8_t j = 0; j < 12; j++){
                    uint8_t already_solved = 0;
                    for(uint8_t k = 0; k < 12; k++){
                        if((EDGES[j].edge == solved_edges[k]) || 
                            (EDGES[j].edge == EDGE(RED, WHITE)) || 
                            (EDGES[j].edge == EDGE(WHITE, RED))) {
                                already_solved = 1;
                                break;
                            }
                    }
                    if(!already_solved){
                        swap = EDGES[j];
                        break;
                    }
                }
                edges_letter_pairs[edges_letter_pairs_i++] = swap.letter;
                edges_letter_pairs[edges_letter_pairs_i] = 0;
                if(is_solved(cube)) return 0;
                make_moves(cube, swap.setup, delay);
                make_moves(cube, swap_algo, delay);
                make_moves(cube, swap.setup_undo, delay);
                parity = !parity;
                break;
            }
        }
    }
    return parity;
}

uint8_t solve(uint32_t* cube, uint32_t delay, char* edges_letter_pairs, char* corners_letter_pairs){
    uint8_t parity = 0;
    if(!is_solved(cube)){
        parity = solve_edges(cube, delay, edges_letter_pairs);
        if(parity){
            make_moves(cube, "B U2 B' U2 B L' B' U' B U B L B2 U", delay);
        }
        solve_corners(cube, delay, corners_letter_pairs);
    }
    return parity;
}
