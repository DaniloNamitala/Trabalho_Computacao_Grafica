#include <GL/glut.h>
#include <GL/freeglut.h>
#include "math.h"
#include <cstring>
#include <stdio.h>
#include "components.hpp"

#define MARGIN 20
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500 

#define X_MENU (MARGIN + WINDOW_HEIGHT)

#define SPACEBAR 32
#define RETURN 13

int screen = 0;

int object_count = 0;
int actual_obj = -1;
struct object* objects_list = NULL;


void renderHomeScreen() {
  float buttonH = 90;
  float buttonW = 300;
  
  float x = (WINDOW_WIDTH - buttonW) / 2;
  float y = WINDOW_HEIGHT - (buttonH * 3 + 3 * MARGIN);

  paintCenteredString(WINDOW_WIDTH / 2 + MARGIN, y / 2, "SPACE INVASORS");

  button(x, y, buttonW, buttonH, "START");
  y += buttonH + MARGIN;
  button(x , y, buttonW, buttonH, "CREDITS");
  y += buttonH + MARGIN;
  button(x, y, buttonW, buttonH, "QUIT");
}

void renderRoom() { 
  glClearColor(0.0f,0.0f,0.0f,0.0f); 
  glClear(GL_COLOR_BUFFER_BIT);
} 

void render(void) {
  glClearColor(0.0f,0.0f,0.0f,0.0f); 
  glClear(GL_COLOR_BUFFER_BIT);

  if (screen == 0)
    renderHomeScreen();
  else
    renderRoom();
  glFlush(); 
}

void handleSpecialKeyboardEvents(int key, int x, int y) { 

}

void handleKeyboardEvents(unsigned char key, int x, int y) {
  
}


void handleStartClick(int x, int y) {
  
}

void handleMouseEvents(int button, int state, int x, int y){
    if (button == GLUT_LEFT_BUTTON){
      if (state == GLUT_UP) {
        if (screen == 0)
          handleStartClick(x, y);
      }  
    }  
    glutPostRedisplay();
}
  
void ChangeSize(GLsizei width,GLsizei height) { 
  glViewport(0, 0, width, height); 
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
} 
  
int main(int argc, char* argv[]) { 
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
  glutInitWindowSize (WINDOW_WIDTH, WINDOW_HEIGHT); 
  glutInitWindowPosition (100, 100); 
  glutCreateWindow("OpenGL - Linhas"); 
  glutDisplayFunc(render);
  glutReshapeFunc(ChangeSize); 
  glutMouseFunc(handleMouseEvents);
  glutSpecialFunc(handleSpecialKeyboardEvents); 
  glutKeyboardFunc(handleKeyboardEvents); 
  glutMainLoop(); 
  return 0;
} 
