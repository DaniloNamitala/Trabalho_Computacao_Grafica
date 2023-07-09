#include "components.hpp"
#include "credits.hpp"
#include "instructions.hpp"
#include "game.hpp"
#include <map>

static int screen = 0;
static int actual_obj = -1;

struct mouseEvent {
  float x1;
  float y1;
  float x2;
  float y2;
  void(*callback)();
};

static int events_count = 0;
static mouseEvent* mouseEvents;

static std::map<int, void(*)()> keyBoardEvents = std::map<int, void(*)()>();

void registerKeyboardEvent(int key, void(*callback)()) {
  keyBoardEvents[key] = callback;
}

void registerMouseEvent(float x1, float y1, float x2, float y2, void(*callback)()) {
  if (events_count == 0) {
    mouseEvents = (mouseEvent*) malloc(sizeof(mouseEvent));
  } else {
    mouseEvents = (mouseEvent*) realloc(mouseEvents, sizeof(mouseEvent) * (events_count + 1));
  }
  mouseEvents[events_count].x1 = x1;
  mouseEvents[events_count].y1 = y1;
  mouseEvents[events_count].x2 = x2;
  mouseEvents[events_count].y2 = y2;
  mouseEvents[events_count].callback = callback;
  events_count++;
}

void init() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void change_screen(int destination) {
  screen = destination;
  if (mouseEvents != NULL) free(mouseEvents);
  mouseEvents = NULL;
  keyBoardEvents.clear();
  events_count = 0;
  switch (screen) {
  case 0:
    init();
    break;
  case 1:
    gameInit();
    break;
  default:
    break;
  }
  glutPostRedisplay();
}

void homeScreen() {
  float buttonH = 70;
  float buttonW = 300;
  
  float x = (WINDOW_WIDTH - buttonW) / 2;
  float y = WINDOW_HEIGHT - (buttonH * 4 + 4 * MARGIN);

  paintCenteredString(WINDOW_WIDTH / 2, y / 2, "SPACE INVASORS");

  button(x, y, buttonW, buttonH, "INICIAR", []() {
    change_screen(1);
  });
  y += buttonH + MARGIN;
  button(x , y, buttonW, buttonH, "CREDITOS", []() {
    change_screen(2);
  });
  y += buttonH + MARGIN;
  button(x, y, buttonW, buttonH, "INSTRUCOES", []() {
    change_screen(3);
  });
  y += buttonH + MARGIN;
  button(x, y, buttonW, buttonH, "SAIR", []() {
    exit(0);
  });
}

void render(void) {
  glClearColor(0.0f,0.0f,0.0f,0.0f); 
  glClear(GL_COLOR_BUFFER_BIT);

  switch (screen)
  {
  case 0:
    homeScreen();
    break;
  case 1:
    gameScreen();
    break;
  case 2:
    creditsScreen();
    break;
  case 3:
    instructionsScreen();
    break;
  default:
    homeScreen();
    break;
  }
  glFlush(); 
}

void handleSpecialKeyboardEvents(int key, int x, int y) { 
  if (keyBoardEvents.find(key) != keyBoardEvents.end()) {
    void(*callback)() = keyBoardEvents[key];
    callback();
  }
}

void handleKeyboardEvents(unsigned char key, int x, int y) {
  if (keyBoardEvents.find(key) != keyBoardEvents.end()) {
    void(*callback)() = keyBoardEvents[key];
    callback();
  }
}

void handleMouseEvents(int button, int state, int x, int y){
    if (button == GLUT_LEFT_BUTTON){
      if (state == GLUT_DOWN && mouseEvents != NULL) {
        for (int i = 0; i < events_count; i++) {
          if (x >= mouseEvents[i].x1 && x <= mouseEvents[i].x2 && y >= mouseEvents[i].y1 && y <= mouseEvents[i].y2) {
            mouseEvents[i].callback();
          }
        }
      }  
    } 
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
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE); 
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
