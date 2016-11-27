#ifndef CONTROLS_H
#define CONTROLS_H

#include <GL/glew.h>
#include <GL/glut.h>
#include "glutils.hpp"
#include <iostream>

#include "control_buffer.h"
#include "display.h"

void control_init();
void keydown_callback(unsigned char key, int, int);
void keyup_callback(unsigned char key, int, int);
void special_callback(int key, int, int);
void mouse_callback(int button, int state, int x, int y);
void passive_mouse_callback(int x, int y);
void mouse_motion_callback(int x, int y);
void press_control_nohold(const char &key);

#endif // CONTROLS_H
