#include <stdint.h>
#include "solver.h"
#include "cube.h"

#include <stdio.h>

#define CORNER(a, b, c) ( a | (b << 4) | (c << 8))

typedef struct {
    uint16_t corner;
    char setup[8];
    char setup_undo[8];
} corner_t;
/* all corners and rotations */
corner_t CORNERS[24] = {
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

void solve_corners(uint32_t* cube){
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
                make_moves(cube, CORNERS[i].setup);
                make_moves(cube, swap_algo);
                make_moves(cube, CORNERS[i].setup_undo);
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
                make_moves(cube, swap.setup);
                make_moves(cube, swap_algo);
                make_moves(cube, swap.setup_undo);
                break;
            }
        }
    }
}