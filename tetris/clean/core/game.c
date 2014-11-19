#include <stdlib.h>
#include <string.h>

#include "game.h"
#include <time.h>

static void gameNewPiece(Game* game)
{
    TetrominoType type = rand() % game->tetrominosCollection->numberOfTetrominos;
    game->piece->tetromino = game->tetrominosCollection->tetrominos[type];
    gridSetCellsWithPiece(game->grid, game->piece, game->piece->tetromino.type);
}

Game* gameNew(Grid* grid, TetrominosCollection* tetrominosCollection)
{
    srand(time(NULL));
    Game* game = (Game*) malloc(sizeof(Game));
    game->grid = grid;
    game->tetrominosCollection = tetrominosCollection;

    // Initialize piece
    game->piece = (Piece*) malloc(sizeof(Piece));
    game->piece->topLeftCorner.rowIndex = 0;
    game->piece->topLeftCorner.columnIndex = (grid->numberOfColumns - 
                                              TETROMINO_GRID_SIZE)/2;
    game->piece->angle = ANGLE_0;
    gameNewPiece(game);

    return game;
}

void gameDestroy(Game* game)
{
    free(game);
}

void gameTryToMoveRight(Game* game)
{
    gameTryToMove(game,pieceMoveToRight,pieceMoveToLeft);
}

void gameTryToMoveLeft(Game* game)
{
    gameTryToMove(game,pieceMoveToLeft,pieceMoveToRight);
}

void gameTryToMoveBottom(Game* game)
{
    gameTryToMove(game,pieceMoveToBottom,pieceMoveToTop);
}

void gameTryToRotateClockwise(Game* game)
{
    gameTryToMove(game,pieceRotateClockwise,pieceRotateCounterClockwise);
}

void gameTryToMove(Game* game,void (*move)(Piece*),void (*unmove)(Piece*))
{
    gridSetCellsWithPiece(game->grid, 
                          game->piece,
                          TETROMINO_VOID);

    move(game->piece);

    if (! gridCanSetCellsWithPiece(game->grid, game->piece)) {
        unmove(game->piece);
    }

    gridSetCellsWithPiece(game->grid, 
                          game->piece,
                          game->piece->tetromino.type);

}


