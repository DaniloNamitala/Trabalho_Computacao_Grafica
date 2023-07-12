#include "components.hpp"

void creditsInit() {
    registerKeyboardEvent(ESCAPE_KEY, []() {
    change_screen(0);
  });
}

void creditsScreen() {
  glClearColor(0.0f,0.0f,0.0f,0.0f); 
  glClear(GL_COLOR_BUFFER_BIT);

  float textH = glutBitmapHeight(GLUT_BITMAP_TIMES_ROMAN_24);
  float x = (WINDOW_WIDTH) / 2;
  float y = WINDOW_HEIGHT - (4 * MARGIN + 8* textH);

  paintCenteredString(x, y / 2, "DESENVOLVIDO POR:");

  paintCenteredString(x, y, "Danilo Aparecido Namitala");
  y += textH + MARGIN;
  paintCenteredString(x , y, "Tulio Oliveira Cruz");
  y += textH + MARGIN;
  paintCenteredString(x, y, "Lislaila Tarsila Pereira");
  y += textH + MARGIN;
  paintCenteredString(x, y, "Tiago Viegas Pires Lage");
  y += textH*4 + MARGIN;
  paintCenteredString(x, y, "Pressione ESC para voltar ao menu.");
}