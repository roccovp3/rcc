#ifndef SOLVER_H
#define SOLVER_H
#include <stdint.h>

/* This generates the order to solve corner pieces similarly
    to the human method to solve a cube blindfolded. */
void solve_corners(uint32_t* cube, uint32_t delay, char* corners_letter_pairs);

/* This generates the order to solve edge pieces similarly
    to the human method to solve a cube blindfolded. */
uint8_t solve_edges(uint32_t* cube, uint32_t delay, char* edges_letter_pairs);

uint8_t solve(uint32_t* cube, uint32_t delay, char* edges_letter_pairs, char* corners_letter_pairs);

#endif