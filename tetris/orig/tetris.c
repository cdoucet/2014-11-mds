#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#define width 16
#define height 16

const unsigned int block_width = 16;
const unsigned int block_height = 16;

unsigned int grid[height][width];

#define shape_count 5

unsigned int next_shape;

struct shape {
  unsigned int index;
  unsigned int x;
  unsigned int y;
  unsigned int o;
} current_shape;

struct shapes {
  int coords[4][2];
  int center[2];
  float colors[3];
} shapes[shape_count] = {
  { { { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 } }, { 3, 0 }, { 1, 0, 0 } },
  { { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 2, 0 } }, { 1, 1 }, { 0, 1, 0 } },
  { { { 0, 1 }, { 0, 0 }, { 1, 1 }, { 2, 1 } }, { 1, 1 }, { 0, 0, 1 } },
  { { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } }, { 1, 1 }, { 1, 1, 0 } },
  { { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 0 } }, { 2, 1 }, { 1, 0, 1 } } };

GtkWidget *window;

gboolean realize(GtkWidget *widget, gpointer data) {
  gtk_widget_queue_draw(widget); 
  return TRUE;
}

void fill_rectangle(cairo_t *cr, int c, int i, int j) {
  const int p = 2;
  cairo_rectangle(cr, j * block_width + p, i * block_height + p, block_width - p, block_height - p);
  float r = shapes[c].colors[0];
  float g = shapes[c].colors[1];
  float b = shapes[c].colors[2];
  cairo_set_source_rgb(cr, r, g, b);
  cairo_fill_preserve(cr);      
  cairo_set_line_width(cr, p);
  cairo_set_source_rgb(cr, r * 0.5, g * 0.5, b * 0.5);
  cairo_stroke(cr);
}

gboolean drawing_area_expose_event(GtkWidget *widget, gpointer data) {
  int i, j;
  
  cairo_t* cr = gdk_cairo_create (widget->window);
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);

  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      int c = grid[i][j];
      if (c != 0) {
        fill_rectangle(cr, c - 1, i, j);
      }
    }
  }
  cairo_destroy(cr);
  return TRUE;
}

gboolean next_piece_expose_event(GtkWidget *widget, gpointer data) {
  int k;
  
  cairo_t* cr = gdk_cairo_create (widget->window);
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);

  for (k = 0; k < 4; k++) {
    int j = 2 + shapes[next_shape].coords[k][1];
    int i = shapes[next_shape].coords[k][0];
    const int p = 2;
    fill_rectangle(cr, next_shape, i, j);
  }
  cairo_destroy(cr);
  return TRUE;
}

int get_shape_x(unsigned int i) {
  int oy = shapes[current_shape.index].coords[i][0];
  int ox = shapes[current_shape.index].coords[i][1];
  int cy = shapes[current_shape.index].center[0];
  int cx = shapes[current_shape.index].center[1];
  int x;
  if (current_shape.o == 0) {
    x = ox;
  }
  if (current_shape.o == 1) {
    x = oy;
  }
  if (current_shape.o == 2) {
    x = cx - ox;
  }
  if (current_shape.o == 3) {
    x = cy - oy;
  }
  x += current_shape.x;
}

int get_shape_y(unsigned int i) {
  int oy = shapes[current_shape.index].coords[i][0];
  int ox = shapes[current_shape.index].coords[i][1];
  int cy = shapes[current_shape.index].center[0];
  int cx = shapes[current_shape.index].center[1];
  int y;
  if (current_shape.o == 0) {
    y = oy;
  }
  if (current_shape.o == 1) {
    y = cx - ox;
  }
  if (current_shape.o == 2) {
    y = cy - oy;
  }
  if (current_shape.o == 3) {
    y = ox;
  }
  y += current_shape.y;
}

void fill_current_shape(unsigned int color) {
  unsigned int i;
  for (i = 0; i < 4; i++) {
    int y = get_shape_y(i);
    int x = get_shape_x(i);
    grid[y][x] = color;
  }
}

int valid_position() {
  unsigned int i;
  for (i = 0; i < 4; i++) {
    int y = get_shape_y(i);
    int x = get_shape_x(i);
    if (!(x >= 0 && y >= 0 && x < width && y < height && grid[y][x] == 0)) {
      return 0;
    }
  }
  return 1;
}

int move_shape(int x, int y, int o) {
  struct shape old_shape = current_shape;
  fill_current_shape(0);
  current_shape.x += x;
  current_shape.y += y;
  current_shape.o += o;
  if (current_shape.o > 3) {
    current_shape.o -= 4;
  }
  if (current_shape.o < 0) {
    current_shape.o += 4;
  }
  int v = valid_position();
  if (!v) {
    current_shape = old_shape;
  }
  fill_current_shape(current_shape.index + 1);
  gtk_widget_queue_draw(window);
  return v;
}

void peek_next_shape() {
  next_shape = rand() % shape_count;
}

int new_shape() {
  int v;
  current_shape.index = next_shape;
  peek_next_shape();
  current_shape.x = width / 2;
  current_shape.y = 0;
  current_shape.o = 0;
  v = valid_position();
  if (v) {
    fill_current_shape(current_shape.index + 1);
    gtk_widget_queue_draw(window);
  }
  return v;
}

int complete_line(i) {
  unsigned int j;
  for (j = 0; j < width; j++) {
    if (grid[i][j] == 0) {
      return 0;
    }
  }
  return 1;
}

void remove_line(i) {
  unsigned int k, j;
  for (k = i; k > 0; k--) {
    for (j = 0; j < width; j++) {
      grid[k][j] = grid[k - 1][j];
    }
  }
  for (j = 0; j < width; j++) {
    grid[0][j] = 0;
  }
}

GtkWidget *score_label;

unsigned int score = 0;

void update_score() {
  char score_text[255];
  sprintf(score_text, "Score: %u", score);
  gtk_label_set_text(GTK_LABEL(score_label), score_text);
}

void detect_lines() {
  unsigned int i;
  unsigned int count = 0;
  for (i = 0; i < height; i++) {
    if (complete_line(i)) {
      remove_line(i);
      count++;
    }
  }
  if (count > 0) {
    score += 1 << (count - 1);
    update_score();
  }
}

gint timeout(gpointer data) {
  if (!(move_shape(0, 1, 0))) {
    detect_lines();
    if (!new_shape()) {
      return 0;
    }
  }
  g_timeout_add(500, timeout, NULL);
  return 0;
}

gboolean key_press_event(GtkWidget *widget, GdkEventKey *event, gpointer data) {
  switch (event->keyval) {
  case GDK_Left:
    move_shape(-1, 0, 0);
    break;
  case GDK_Right:
    move_shape(1, 0, 0);
    break;
  case GDK_Up:
    move_shape(0, 0, 1);
    break;
  case GDK_Down:
    move_shape(0, 1, 0);
    break;
  case GDK_KEY_space:
    while (move_shape(0, 1, 0));
    break;
  }
  return TRUE;
}

void new_game() {
  unsigned int i, j;
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      grid[i][j] = 0;
    }
  }
  srand(time(NULL));
  peek_next_shape();
  new_shape();
  fill_current_shape(current_shape.index + 1);
  update_score();
  gtk_widget_queue_draw(window);
}

gboolean button_newgame_clicked(GtkWidget *widget, gpointer data) {
  new_game();
  return TRUE;
}

int main(int argc, char *argv[]) {
  GtkWidget *button_newgame, *hbox, *vbox, *drawing_area, *next_piece;
  gtk_init(&argc, &argv);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  hbox = gtk_hbox_new(TRUE, 10);
  gtk_container_add(GTK_CONTAINER(window), hbox);
  drawing_area = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(hbox), drawing_area);
  gtk_widget_set_size_request(drawing_area, width * block_width, height * block_height);
  g_signal_connect(G_OBJECT(drawing_area), "realize", G_CALLBACK(realize), NULL);
  g_signal_connect(G_OBJECT(drawing_area), "expose_event", G_CALLBACK(drawing_area_expose_event), NULL);
  vbox = gtk_vbox_new(TRUE, 10);
  gtk_container_add(GTK_CONTAINER(hbox), vbox);
  button_newgame = gtk_button_new_with_label ("New game");
  g_signal_connect(button_newgame, "clicked", G_CALLBACK(button_newgame_clicked), NULL);
  gtk_container_add(GTK_CONTAINER(vbox), button_newgame);
  score_label = gtk_label_new("");
  gtk_container_add(GTK_CONTAINER (vbox), score_label);
  next_piece = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(vbox), next_piece);
  gtk_widget_set_size_request(next_piece, 4 * block_width, 4 * block_height);
  g_signal_connect(G_OBJECT(next_piece), "realize", G_CALLBACK(realize), NULL);
  g_signal_connect(G_OBJECT(next_piece), "expose_event", G_CALLBACK(next_piece_expose_event), NULL);
  g_signal_connect(G_OBJECT(window), "key_press_event", G_CALLBACK(key_press_event), NULL);
  new_game();
  gtk_widget_show(drawing_area);
  gtk_widget_show(next_piece);
  gtk_widget_show(button_newgame);
  gtk_widget_show(score_label);
  gtk_widget_show(hbox);
  gtk_widget_show(vbox);
  gtk_widget_show(window);
  g_timeout_add(500, timeout, NULL);
  gtk_main();
  return EXIT_SUCCESS;
}
