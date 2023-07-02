#include <GL/glut.h>
#include <GL/freeglut.h>
#include "math.h"
#include <cstring>
#include <stdio.h>

void paintString(float x, float y, const char* string) {  
  glColor3f(1, 1, 1); 
  glRasterPos2f(x, y);
  glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)string);
}

void paintCenteredString(float x, float y, const char* string) {  
  glColor3f(1, 1, 1); 
  float fontH = glutBitmapHeight(GLUT_BITMAP_TIMES_ROMAN_24);
  float wordW = 0; 
  for (char* c = (char*) string; *c != '\0'; c++) {
    wordW += glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, *c);
  }
  int amount = strlen(string);
  glRasterPos2f(x - wordW / 2, y + fontH / 2);
  glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)string);
}

void paintRetangle(float x, float y, float width, float height) {
   glBegin( GL_LINE_LOOP ); 
    glVertex2f(x, y);
    glVertex2f(x, (y + height));
    glVertex2f((x + width), (y + height));
    glVertex2f((x + width), y);
  glEnd();
}

void button(float x, float y, float buttonW, float buttonH, const char* string) {
  paintRetangle(x , y, buttonW, buttonH);
  paintCenteredString(x + buttonW / 2 , y + buttonH / 2, string);
}