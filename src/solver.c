#include <stdint.h>
#include <stdio.h>
#include "solver.h"
#include "cube.h"

#define CORNER(a, b, c) ( a | (b << 4) | (c << 8))
#define EDGE(a, b) (a | (b << 4))

typedef struct {
    uint16_t corner;
    char setup[8];
    char setup_undo[8];
} corner_t;

typedef struct {
    uint8_t edge;
    char setup[12];
    char setup_undo[12];
} edge_t;

/* all corners and rotations */
const corner_t CORNERS[24] = {
    {.corner=CORNER(WHITE, ORANGE, BLUE), .setup={0}, .setup_undo={0}},
    {.corner=CORNER(WHITE, BLUE, RED), .setup="R D'", .setup_undo="D R'"},
    {.corner=CORNER(WHITE, RED, GREEN), .setup="F", .setup_undo="F'"},
    {.corner=CORNER(WHITE, GREEN, ORANGE), .setup="F R'", .setup_undo="R F'"},
    {.corner=CORNER(YELLOW, ORANGE, GREEN), .setup="F'", .setup_undo="F"},
    {.corner=CORNER(YELLOW, GREEN, RED), .setup="D' F'", .setup_undo="F D"},
    {.corner=CORNER(YELLOW, RED, BLUE), .setup="D2 F'", .setup_undo="F D2"},
    {.corner=CORNER(YELLOW, BLUE, ORANGE),.setup="D F'", .setup_undo="F D'"},

    {.corner=CORNER(BLUE, WHITE, ORANGE), .setup={0}, .setup_undo={0}},
    {.corner=CORNER(RED, WHITE, BLUE), .setup="R2", .setup_undo="R2"},
    {.corner=CORNER(GREEN, WHITE, RED), .setup="F2 D", .setup_undo="D' F2"},
    {.corner=CORNER(ORANGE, WHITE, GREEN), .setup="F2", .setup_undo="F2"},
    {.corner=CORNER(GREEN, YELLOW, ORANGE), .setup="D", .setup_undo="D'"},
    {.corner=CORNER(RED, YELLOW, GREEN), .setup={0}, .setup_undo={0}},
    {.corner=CORNER(BLUE, YELLOW, RED), .setup="D'", .setup_undo="D"},
    {.corner=CORNER(ORANGE, YELLOW, BLUE), .setup="D2", .setup_undo="D2"},

    {.corner=CORNER(ORANGE, BLUE, WHITE), .setup={0}, .setup_undo={0}},
    {.corner=CORNER(BLUE, RED, WHITE), .setup="R' F", .setup_undo="F' R"},
    {.corner=CORNER(RED, GREEN, WHITE), .setup="R'", .setup_undo="R"},
    {.corner=CORNER(GREEN, ORANGE, WHITE), .setup="F' D", .setup_undo="D' F"},
    {.corner=CORNER(ORANGE, GREEN, YELLOW), .setup="D2 R", .setup_undo="R' D2"},
    {.corner=CORNER(GREEN, RED, YELLOW), .setup="D R", .setup_undo="R' D'"},
    {.corner=CORNER(RED, BLUE, YELLOW), .setup="R", .setup_undo="R'"},
    {.corner=CORNER(BLUE, ORANGE, YELLOW), .setup="D' R", .setup_undo="R' D"}
};

const edge_t EDGES[24] = {
    {.edge=EDGE(WHITE, ORANGE), .setup={0}, .setup_undo={0}},
    {.edge=EDGE(WHITE, BLUE), .setup="R2 U' R2", .setup_undo="R2 U R2"},
    {.edge=EDGE(WHITE, RED), .setup={0}, .setup_undo={0}},
    {.edge=EDGE(WHITE, GREEN), .setup="R2 U R2", .setup_undo="R2 U' R2"},
    {.edge=EDGE(YELLOW, ORANGE), .setup="L2", .setup_undo="L2"},
    {.edge=EDGE(YELLOW, GREEN), .setup="D' L2", .setup_undo="L2 D"},
    {.edge=EDGE(YELLOW, RED), .setup="D2 L2", .setup_undo="L2 D2"},
    {.edge=EDGE(YELLOW, BLUE),.setup="D L2", .setup_undo="L2 D'"},
    {.edge=EDGE(BLUE, RED), .setup="U2 R' U2", .setup_undo="U2 R U2"},
    {.edge=EDGE(BLUE, ORANGE), .setup="L", .setup_undo="L'"},
    {.edge=EDGE(RED, GREEN), .setup="U' F' U", .setup_undo="U' F U"},
    {.edge=EDGE(ORANGE, GREEN), .setup="U' F U", .setup_undo="U' F' U"},

    {.edge=EDGE(ORANGE, WHITE), .setup="L' U B' U'", .setup_undo="U B U' L"},
    {.edge=EDGE(BLUE, WHITE), .setup="R' B L R", .setup_undo="R' L' B' R"},
    {.edge=EDGE(RED, WHITE), .setup={0}, .setup_undo={0}},
    {.edge=EDGE(GREEN, WHITE), .setup="R F' L' R'", .setup_undo="R L F R'"},
    {.edge=EDGE(ORANGE, YELLOW), .setup="D F L' F'", .setup_undo="F L F' D'"},
    {.edge=EDGE(GREEN, YELLOW), .setup="F L' F'", .setup_undo="F L F'"},
    {.edge=EDGE(RED, YELLOW), .setup="D' F L' F'", .setup_undo="F L F' D"},
    {.edge=EDGE(BLUE, YELLOW), .setup="B' L B", .setup_undo="B' L' B"},
    {.edge=EDGE(RED, BLUE), .setup="U B U'", .setup_undo="U B' U"},
    {.edge=EDGE(ORANGE, BLUE), .setup="U B' U'", .setup_undo="U B U'"},
    {.edge=EDGE(GREEN, RED), .setup="U2 R U2", .setup_undo="U2 R' U2"},
    {.edge=EDGE(GREEN, ORANGE), .setup="L'", .setup_undo="L"}
};

void solve_corners(uint32_t* cube, uint32_t delay){
    char swap_algo[40] = "R U' R' U' R U R' F' R U R' U' R' F R";
    uint16_t solved_corners[8] = {0};
    uint8_t solved_corners_count = 0;
    while(solved_corners_count < 7){
        uint8_t buffer_top = cube[WHITE]&0xF;
        uint8_t buffer_left = cube[ORANGE]&0xF;
        uint8_t buffer_back = (cube[BLUE]&0x00000F00) >> 8;
        uint16_t buffer = CORNER(buffer_top, buffer_left, buffer_back);
        for(uint8_t i = 0; i < 24; i++){
            // != are for the buffer corner.
            if((buffer == CORNERS[i].corner) && (buffer != 0x340) && (buffer != 0x403) && (buffer != 0x034)){
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
                make_moves(cube, swap.setup, delay);
                make_moves(cube, swap_algo, delay);
                make_moves(cube, swap.setup_undo, delay);
                break;
            }
        }
    }
}

int solve_edges(uint32_t* cube, uint32_t delay){
    char swap_algo[40] = "R U R' U' R' F R2 U' R' U' R U R' F'";
    uint8_t parity;
    uint16_t solved_edges[12] = {0};
    uint8_t solved_edges_count = 0;
    while(solved_edges_count < 11){
        uint8_t buffer_top = (cube[WHITE] & 0x0000F000) >> 12;
        uint8_t buffer_right = (cube[RED] & 0x000000F0) >> 4;
        uint8_t buffer = EDGE(buffer_top, buffer_right);
        for(uint8_t i = 0; i < 24; i++){
            // != are for the buffer edge.
            if((buffer == EDGES[i].edge) && (buffer != EDGE(WHITE, RED)) && (buffer != EDGE(RED, WHITE))){
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

void solve(uint32_t* cube, uint32_t delay){
    if(!is_solved(cube)){
        if(solve_edges(cube, delay)){
            make_moves(cube, "B U2 B' U2 B L' B' U' B U B L B2 U", delay);
        }
        solve_corners(cube, delay);
    }
}
