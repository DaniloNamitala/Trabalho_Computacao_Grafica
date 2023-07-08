#include "components.hpp"
#include "stb_image.h"

// Variáveis para armazenar as imagens
int imageWidth1, imageHeight1, imageChannels1;
unsigned char *imageData1;

// IDs das texturas
GLuint textureID1;

// Posições atuais das imagens
float imageX1 = 0.0f;
float imageY1 = -0.5f;

// Tiro
const int maxShots = 5; // Define o número máximo de tiros permitidos
typedef std::pair<float, float> FloatPair;
std::vector<FloatPair> shots; // Vetor para armazenar as posições dos tiros

float circleX;
float circleY;
float circleRadius = 0.1f;
bool circleVisible = false;
int circlesHit = 0;
int shotsHit = 0;
bool circleDestroyed = false;

// Menu
int menuID;

void coordenadasImag(float imageX, float imageY, int imageWidth, int imageHeight, int windowWidth, int windowHeight) {
    float imageWidthNormalized = static_cast<float>(imageWidth) / windowWidth;
    float imageHeightNormalized = static_cast<float>(imageHeight) / windowHeight;
    float imageAspectRatio = static_cast<float>(imageWidth) / imageHeight;
    float imageScale = 0.5f;
    float imageLeft = imageX - imageWidthNormalized * imageScale / 2.0f;
    float imageRight = imageX + imageWidthNormalized * imageScale / 2.0f;
    float imageBottom = imageY - imageHeightNormalized * imageScale / 2.0f;
    float imageTop = imageY + imageHeightNormalized * imageScale / 2.0f;

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(imageLeft, imageBottom);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(imageRight, imageBottom);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(imageRight, imageTop);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(imageLeft, imageTop);
    glEnd();
}

void initializeCircle() {
    // Gerar coordenadas aleatórias para o círculo dentro da janela
    circleX = static_cast<float>(std::rand()) / RAND_MAX * 2.0f - 1.0f;
    circleY = 0.9f;

    // Reinicia a contagem de tiros atingidos apenas quando um novo círculo for gerado
    if (circleVisible) {
        shotsHit = 0;
    }
}

void circleTimer(int value) {
    // Verifica se o círculo foi destruído antes de gerar novas coordenadas aleatórias
    if (!circleDestroyed) {
        // Verifica se o círculo já está visível antes de gerar um novo círculo
        if (!circleVisible) {
            // Gera novas coordenadas aleatórias para o círculo
            initializeCircle();
            // Torna o círculo visível novamente
            circleVisible = true;
        }
    }
    // Redesenha a cena
    glutPostRedisplay();
}

void updateShotPosition() {
    for (int i = 0; i < shots.size(); i++) {
        float &shotX = shots[i].first;
        float &shotY = shots[i].second;
        shotY += 0.08f;
        // Verifica se o tiro saiu da janela, se sim, remove o tiro da lista
        if (shotY > 1.0f) {
            shots.erase(shots.begin() + i);
            i--;
            continue;
        }
        // Verifica a colisão apenas se o círculo estiver visível
        if (circleVisible) {
            float dx = shotX - circleX;
            float dy = shotY - circleY;
            float distance = std::sqrt(dx * dx + dy * dy);
            if (distance <= circleRadius) {
                // O tiro colidiu com o círculo
                shots.erase(shots.begin() + i);
                i--;
                // Incrementa o contador de tiros atingidos
                shotsHit++;
                if (shotsHit >= std::rand() % 5 + 1) {
                    // Oculta o círculo
                    circleVisible = false;

                    // Incrementa o contador de círculos atingidos
                    circlesHit = circlesHit + 50;
                }
                // Define um temporizador para fazer o círculo reaparecer após 2 segundos
                glutTimerFunc(2000, circleTimer, 0);
                return; // Encerra a função para evitar a verificação de colisão para outros tiros
            }
        }
    }
}

void updateCirclePosition() {
    circleY -= 0.01f;

    // Verifica se o círculo atingiu a parte inferior da janela
    if (circleY < -1.0f) {
        // Gera novas coordenadas aleatórias para o círculo na parte superior da janela
        initializeCircle();
    }
}

void menu(int option) {
    switch (option) {
    case 1: // Opção de jogar
        // Reinicialize os valores do jogo aqui, se necessário
        break;
    case 2: // Opção de sair
        change_screen(0);
        break;
    }
}

void registerkeyEvents() {
    registerKeyboardEvent('w', []() {
        imageY1 += 0.1f;
        if (imageY1 > 1.0f)
            imageY1 = -1.0f;
        else if (imageY1 < -1.0f)
            imageY1 = 1.0f;
    });
    registerKeyboardEvent('a', []() {
        imageX1 -= 0.1f;
        if (imageX1 > 1.0f)
            imageX1 = -1.0f;
        else if (imageX1 < -1.0f)
            imageX1 = 1.0f;
    });
    registerKeyboardEvent('s', []() {
        imageY1 -= 0.1f;
        if (imageY1 > 1.0f)
            imageY1 = -1.0f;
        else if (imageY1 < -1.0f)
            imageY1 = 1.0f;
    });
    registerKeyboardEvent('d', []() {
        imageX1 += 0.1f;
        if (imageX1 > 1.0f)
            imageX1 = -1.0f;
        else if (imageX1 < -1.0f)
            imageX1 = 1.0f;
    });
    registerKeyboardEvent(SPACEBAR, []() {
        if (shots.size() < maxShots) {
            shots.push_back(std::make_pair(imageX1, imageY1));
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

    // Renderização da primeira imagem
    glBindTexture(GL_TEXTURE_2D, textureID1);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // Define o valor de alpha para tornar a imagem opaca
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5f);
    coordenadasImag(imageX1, imageY1, imageWidth1, imageHeight1, WINDOW_WIDTH, WINDOW_WIDTH);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_TEXTURE_2D);

    // Renderização do círculo
    if (circleVisible) {
        glColor3f(1.0f, 0.0f, 0.0f); // Define a cor vermelha (R, G, B)
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.5f);

        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(circleX, circleY);
        for (int i = 0; i <= 360; i++) {
            float angle = static_cast<float>(i) * 3.14159f / 180.0f;
            float x = circleX + circleRadius * std::cos(angle);
            float y = circleY + circleRadius * std::sin(angle);
            glVertex2f(x, y);
        }
        glEnd();
        glDisable(GL_ALPHA_TEST);
    }

    // Exibe o contador de círculos atingidos na tela
    if (circleDestroyed) {
        glColor3f(1.0f, 0.0f, 0.0f);
        glRasterPos2f(0.4f, 0.8f);
        std::string destructionText = "Circle Destroyed!";
        for (char c : destructionText)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }
    } else {
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(0.6f, 0.9f);
        std::string counterText = "Pontos: " + std::to_string(circlesHit);
        for (char c : counterText)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }
    }

    if (!shots.empty()) {
        glColor3f(1.0f, 0.0f, 0.0f);
        glPointSize(5.0f);

        glBegin(GL_POINTS);
        for (const auto &shot : shots)
        {
            glVertex2f(shot.first, shot.second);
        }
        glEnd();
    }

    updateShotPosition();
    updateCirclePosition();
    glutSwapBuffers();
}


void timerFunc(int value) {
    glutPostRedisplay();
    glutTimerFunc(50, timerFunc, 0);
}

void gameInit() {
    registerkeyEvents();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Carregando a primeira imagem usando a biblioteca stb_image
    imageData1 = stbi_load("res/helicoptero.png", &imageWidth1, &imageHeight1, &imageChannels1, 0);

    if (imageData1 == nullptr)
    {
        std::cout << "Erro ao carregar a primeira imagem." << std::endl;
        return;
    }

    // Gerando as texturas
    glGenTextures(1, &textureID1);
    glBindTexture(GL_TEXTURE_2D, textureID1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth1, imageHeight1, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData1);

    stbi_image_free(imageData1);

    glEnable(GL_TEXTURE_2D);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Inicializa a semente para números aleatórios
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Inicializa as coordenadas do círculo
    initializeCircle();

    // Define um temporizador para fazer o círculo aparecer após 2 segundos
    glutTimerFunc(2000, circleTimer, 0);

    // Criação do menu
    menuID = glutCreateMenu(menu);
    glutAddMenuEntry("Jogar", 1);
    glutAddMenuEntry("Sair", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutTimerFunc(3000, circleTimer, 0);
    glutTimerFunc(0, timerFunc, 0);
}