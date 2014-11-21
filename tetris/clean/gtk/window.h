#ifndef TETRIS_WINDOW_H
#define TETRIS_WINDOW_H

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#define NPIXELS 24

typedef struct {
  GtkWidget* base;

  GtkWidget* newGameButton;
  GtkWidget* pauseButton;
  GtkWidget* scoreLabel;
  GtkWidget* verticalBox;
  GtkWidget* horizontalBox;
  GtkWidget* matrix;
  GtkWidget* preview;
} TetrisWindow;

TetrisWindow* tetris_window_new(size_t numberOfRows,size_t numberOfColumns);
void tetris_window_destroy(TetrisWindow* window);
void tetris_window_show(TetrisWindow* window);
void tetris_window_refresh(TetrisWindow* window);

#endif
