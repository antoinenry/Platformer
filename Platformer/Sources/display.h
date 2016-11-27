#ifndef DISPLAY_H
#define DISPLAY_H

#define GLEW_STATIC 1

#include <GL/glew.h>
#include <GL/glut.h>
#include "glutils.hpp"

#include "math.h"
#include <string>

#include "scene.h"
#include "phys_box.h"

//Constantes
#define GLEW_STATIC 1
extern int SCREEN_W, SCREEN_H;

extern Scene scene1;

void display_init(int argc, char** argv);
void shaders_init(std::string vertex_file, std::string fragment_file);
void display_callback();

#endif // DISPLAY_H
