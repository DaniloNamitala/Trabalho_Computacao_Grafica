#ifndef IMAGE_HPP
#define IMAGE_HPP

#ifdef __APPLE__
  #include <GLUT/glut.h>
  #include <GLUT/freeglut.h>
#else
  #include <GL/glut.h>
  #include <GL/freeglut.h>
#endif
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include "math.h"
#include <cstring>
#include <stdio.h>
#include "constants.hpp"
#include "stb_image.h"

GLuint loadTexture(const char* filename) {
    GLuint texture_id;
    int image_width, image_height;
    int image_channel;

    auto image = stbi_load(filename, &image_width, &image_height, &image_channel, 0);
    if (image == nullptr) {
        std::cout << "Erro ao carregar " << filename << std::endl;
        return 0;
    }

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    stbi_image_free(image);
    return texture_id;
}

void paintImage(float center_x, float center_y, float width, float height, GLuint texture_id, int rotation = 0) {
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f); 
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5f);

    float left = - width / 2.0f;
    float right = width / 2.0f;
    float bottom = - height / 2.0f;
    float top = height / 2.0f;

    glPushMatrix();
    glTranslatef(center_x, center_y, 0.0f);
    glRotatef(rotation, 0.0f, 0.0f, 1.0f);
  
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(left, bottom);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(right, bottom);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(right, top);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(left, top);
    glEnd();
    
    glPopMatrix();

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_TEXTURE_2D);
}

#endif