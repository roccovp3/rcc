#ifndef SOLVER_H
#define SOLVER_H
#include <stdint.h>

/* This generates the order to solve corner pieces similarly
    to the human method to solve a cube blindfolded. */
void solve_corners(uint32_t* cube);

/* This generates the order to solve edge pieces similarly
    to the human method to solve a cube blindfolded. */
int solve_edges(uint32_t* cube);

void solve(uint32_t* cube);

#endif