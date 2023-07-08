#include "components.hpp"

void instructionsScreen() {
  glClearColor(0.0f,0.0f,0.0f,0.0f); 
  glClear(GL_COLOR_BUFFER_BIT);

  registerKeyboardEvent(ESCAPE_KEY, []() {
    change_screen(0);
  });

  float textH = glutBitmapHeight(GLUT_BITMAP_TIMES_ROMAN_24);
  float x = (WINDOW_WIDTH) / 2;
  float y = WINDOW_HEIGHT - (4 * MARGIN + 8* textH);

  paintCenteredString(x, y / 2, "Instrucoes:");

  paintCenteredString(x, y, "Utilize as teclas W, A, S, D para mover a nave.");
  y += textH + MARGIN;
  paintCenteredString(x , y, "Utilize a tecla ESPAÇO para atirar.");
  y += textH + MARGIN;
  paintCenteredString(x, y, "Utilize a tecla F para usar a luz.");
  y += textH + MARGIN;
  paintCenteredString(x, y, "Utilize a tecla ESC para voltar ao menu.");
}