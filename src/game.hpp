#include "components.hpp"
#include "image.hpp"

bool game_stop = false;

int helice_angle = 0;
GLuint helice_texture_id;
struct entity {
    float x = 0.0f;
    float y = 1.0f;
    int lives = 5;
    float speed = 0.01f;
    GLint texture_id;
};

GLuint fundo_texture_id;
bool player_light = false;
bool world_light = true;

entity player;
const int maxShots = 5;
typedef std::pair<float, float> FloatPair;
std::vector<FloatPair> shots;

int maxEnemies = 4;
std::vector<entity> enemies;
GLuint default_enemy_texture_id;

// Menu
int menuID;

// desenha o fundo do jogo
void drawWallpaper() {
    paintImage(0.0, 0.0, 2, 2, fundo_texture_id);
}

void drawPlayer() {
    helice_angle += 10.0f;
    helice_angle %= 360;
    paintImage(player.x, player.y, 0.15f, 0.4f, player.texture_id);
    paintImage(player.x-0.01, player.y+0.08, 0.4f, 0.4f, helice_texture_id, -helice_angle);
}

void drawEnemies() {
    for (auto &enemy : enemies) {
        float enemyX = enemy.x;
        float enemyY = enemy.y;
        float enemyWidth = 0.2f;
        float enemyHeight = 0.2f;
        paintImage(enemyX, enemyY, enemyWidth, enemyHeight, enemy.texture_id, 180);
    }
}

void create_enemies() {
    if (enemies.size() < maxEnemies) {
        entity new_enemy;
        new_enemy.x = static_cast<float>(std::rand()) / RAND_MAX * 2.0f - 1.0f;
        new_enemy.y = 1.0f;
        new_enemy.lives = (std::rand() % 8) + 1;
        new_enemy.speed = static_cast<float>(std::rand()) / RAND_MAX * 0.01f;
        new_enemy.texture_id = default_enemy_texture_id;
        enemies.push_back(new_enemy);
    }
}

void drawShots() {
    for (auto &shot : shots) {
        float shotX = shot.first;
        float shotY = shot.second;
        float shotWidth = 0.015f;
        float shotHeight = 0.03f;

        glColor3f(1.0f, 0.0f, 0.0f); // Define a cor vermelha (R, G, B)
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.5f);
        glBegin(GL_POLYGON);
        glVertex2f(shotX - shotWidth / 2.0f, shotY - shotHeight / 2.0f);
        glVertex2f(shotX + shotWidth / 2.0f, shotY - shotHeight / 2.0f);
        glVertex2f(shotX + shotWidth / 2.0f, shotY + shotHeight / 2.0f);
        glVertex2f(shotX - shotWidth / 2.0f, shotY + shotHeight / 2.0f);
        glEnd();
        glDisable(GL_ALPHA_TEST);
    }
}

void registerkeyEvents() {
    registerKeyboardEvent('w', []() {
        player.y += player.speed;
        player.y = player.y < -1.0f ? 1.0f : player.y;
        player.y = player.y > 1.0f ? -1.0f : player.y;
    });
    registerKeyboardEvent('a', []() {
        player.x -= player.speed;
        player.x = player.x < -1.0f ? 1.0f : player.x;
        player.x = player.x > 1.0f ? -1.0f : player.x;
    });
    registerKeyboardEvent('s', []() {
        player.y -= player.speed;
        player.y = player.y < -1.0f ? 1.0f : player.y;
        player.y = player.y > 1.0f ? -1.0f : player.y;
    });
    registerKeyboardEvent('d', []() {
        player.x += player.speed;
        player.x = player.x < -1.0f ? 1.0f : player.x;
        player.x = player.x > 1.0f ? -1.0f : player.x;
    });
    registerKeyboardEvent(SPACEBAR, []() {
        if (shots.size() < maxShots) {
            shots.push_back(std::make_pair(player.x, player.y));
        }
    });
    registerKeyboardEvent(ESCAPE_KEY, []() {
        change_screen(0);
    });
    registerKeyboardEvent('f', []() {
        player_light = !player_light;
    });
    registerKeyboardEvent('l', []() {
        world_light = !world_light;
        if (world_light)
            glEnable(GL_LIGHTING);
        else
            glDisable(GL_LIGHTING);
    });
}


void set_main_light() {
    GLfloat light0_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light0_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light0_position[] = { 0.0, 0.0, 1.0, 1.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 2.0);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 1.0);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.5);

    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
}

void set_light(float x, float y, float z, float xt, float yt, float zt, float angle) {
   glDisable(GL_LIGHT1);
   if (!player_light) return;
   
   GLfloat light_position[] = { x, y, z, 1.0 };
   GLfloat light_direction[] = { xt - x, yt - y, zt - z, 0.0 };
   GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
   glLightfv(GL_LIGHT1 , GL_AMBIENT, light_ambient);
   glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
   glLightfv(GL_LIGHT1, GL_POSITION, light_position);
   glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_direction);
   glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, angle);

   
   glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1);
   glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.0);
   glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0);
   glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);

   glEnable(GL_LIGHT0 + 1);
   glEnable(GL_DEPTH_TEST);
}

void gameScreen() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    drawWallpaper();
    drawPlayer();
    drawEnemies();
    drawShots();
    set_light(player.x, player.y, 0.2, player.x, 0.8, 0.0, 20);
}

void stop_game() {
    glutDetachMenu(GLUT_RIGHT_BUTTON);
}

static unsigned long ticks = 0;
void tick(int value) {
    ticks++;

    if (ticks % 50 == 0) { // cria um inimigo a cada 50 ticks
        create_enemies();
    }

    for (auto &enemy : enemies) { // atualiza a posicao dos inimigos
        enemy.y -= enemy.speed;
        if (enemy.y < -1.0f) {
            enemy.y = 1.0f;
            enemy.x = static_cast<float>(std::rand()) / RAND_MAX * 2.0f - 1.0f;
        }
    }

    for(int i = 0; i < shots.size(); i++) { // atualiza a posicao dos tiros
        shots[i].second += 0.1f;
        if (shots[i].second > 1.0f) {
            shots.erase(shots.begin() + i);
        }
        for (auto &enemy : enemies) {
            float dx = shots[i].first - enemy.x;
            float dy = shots[i].second - enemy.y;
            float distance = std::sqrt(dx * dx + dy * dy);
            if (distance < 0.1f) {
                shots.erase(shots.begin() + i);
                enemy.lives--;
                break;
            }
        }
    }

    for (int i = 0; i < enemies.size(); i++) {
        if (enemies.at(i).lives <= 0) {
            enemies.erase(enemies.begin() + i);
        }
    }

    glutPostRedisplay();
    if (!game_stop)
        glutTimerFunc(16, tick, 0);
    else
        stop_game();
}

void initialize_data() {
    ticks = 0;
    game_stop = false;
    player.x = 0.0f;
    player.y = -0.5f;
    player.speed = 0.1f;
    player.lives = 3;
    player.texture_id = loadTexture("res/helicoptero.png");
    helice_texture_id = loadTexture("res/helice.png");
    default_enemy_texture_id = loadTexture("res/inimigo1.png");
    fundo_texture_id = loadTexture("res/fundo.png");
    enemies.clear();
    shots.clear();
}

void menu(int option) {
    switch (option) {
    case 1: 
        initialize_data();
        break;
    case 2: // Opção de sair
        game_stop = true;
        change_screen(0);
        break;
    }
}

void createMenu() {
    menuID = glutCreateMenu(menu);
    glutAddMenuEntry("Jogar", 1);
    glutAddMenuEntry("Voltar", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void gameInit() {
    initialize_data();
    registerkeyEvents();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    
    glLoadIdentity();
    set_main_light();
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    createMenu();
    glutTimerFunc(0, tick, 0);
}