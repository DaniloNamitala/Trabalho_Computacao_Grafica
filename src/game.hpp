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
        paintImage(enemyX, enemyY, enemyWidth, enemyHeight, enemy.texture_id);
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
}

void gameScreen() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    drawWallpaper();
    drawPlayer();
    drawEnemies();
    drawShots();
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
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    createMenu();
    glutTimerFunc(0, tick, 0);
}