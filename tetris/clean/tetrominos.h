#ifndef TETRIS_TETROMINOS_H
#define TETRIS_TETROMINOS_H

#include "rgb_color.h"

#define DIMENSION 2
#define NUMBER_OF_SQUARES 4
#define NUMBER_OF_TETROMINO_TYPES 5

/* Official names of tetrominos
 * Do NOT modify these names */
#define POLYOMINO_I { { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 } }
#define POLYOMINO_J { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 2, 0 } }
#define POLYOMINO_L { { 0, 1 }, { 0, 0 }, { 1, 1 }, { 2, 1 } }
#define POLYOMINO_O { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } }
#define POLYOMINO_T { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 0 } }

enum RotationAngle { ANGLE_0=0, ANGLE_90=1, ANGLE_180=2, ANGLE_270=3 };

struct shape {
  unsigned int index;
  unsigned int column_index;
  unsigned int row_index;
  unsigned int rotation_angle;
} current_shape;

unsigned int next_shape;

static struct tetromino {
  int coords[NUMBER_OF_SQUARES][DIMENSION];
  int center[DIMENSION];
  rgb_color color;
} tetrominos[NUMBER_OF_TETROMINO_TYPES] = {
  { POLYOMINO_I , { 3, 0 }, RED },
  { POLYOMINO_J , { 1, 1 }, GREEN },
  { POLYOMINO_L , { 1, 1 }, BLUE },
  { POLYOMINO_O , { 1, 1 }, YELLOW },
  { POLYOMINO_T , { 2, 1 }, PURPLE } };

#endif
