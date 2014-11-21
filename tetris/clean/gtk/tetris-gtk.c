/* https://developer.gnome.org/gtk2/stable/GtkDrawingArea.html */
#include <stdlib.h>
#include <stdio.h>

#include "game.h"
#include "rgb_color.h"
#include "tetromino_srs.h"
#include "gui.h"




int main(int argc, char* argv[]) 
{

  gtk_init(&argc, &argv);

  TetrisGUI* gui = tetris_gui_new();
   g_signal_connect(G_OBJECT(gui->window->base), "key_press_event", G_CALLBACK(on_key_press_event), gui);
  g_signal_connect(G_OBJECT(gui->window->matrix), "expose_event", G_CALLBACK(on_matrix_expose_event),gui);

  gtk_main();

  tetris_gui_destroy(gui);

  return EXIT_SUCCESS;
}
