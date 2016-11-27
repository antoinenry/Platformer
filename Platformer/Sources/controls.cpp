#include "controls.h"

#define ESCAPE_KEY 27

std::map<char, bool> control_buffer;
std::map<char, bool> release_buffer;

char control_key [N_CONTROLS] = { 'L', 'R', 'U' , 'D', '1', '2', 'S', 'r', '+', '-', 'G'};
unsigned int control_key_size = N_CONTROLS;
float mouse_dx = 0, mouse_dy = 0;

int prev_mouse_x, prev_mouse_y;


void control_init()
{
    //Clavier
    glutKeyboardFunc(keydown_callback);
    glutKeyboardUpFunc(keyup_callback);
    glutSpecialFunc(special_callback);
    //Souris
    glutMouseFunc(mouse_callback);
    glutPassiveMotionFunc(passive_mouse_callback);
    glutMotionFunc(mouse_motion_callback);
    //Manette
    //glutJoystickFunc();

    //Initialisation buffers
     release_buffer['1'] = true;
}

void keydown_callback(unsigned char key, int, int)
{
    switch (key)
    {
    case 'q': control_buffer['L'] = true;
        break;
    case 'd': control_buffer['R'] = true;
        break;
    case 'z': control_buffer['U'] = true;
        break;
    case 's': control_buffer['D'] = true;
        break;
    case 'a': press_control_nohold('1');
        break;
    case 'e': press_control_nohold('2');
        break;
    case '+': control_buffer['+'] = true;
        break;
    case '-': control_buffer['-'] = true;
        break;

    case 'r': control_buffer['G'] = true;
        break;
    case ESCAPE_KEY: glutDestroyWindow(1);
        exit(0);
        break;
    }
}

void keyup_callback(unsigned char key, int, int)
{
    switch (key)
    {
    case 'q': control_buffer['L'] = false;
        break;
    case 'd': control_buffer['R'] = false;
        break;
    case 'z': control_buffer['U'] = false;
        break;
    case 's': control_buffer['D'] = false;
        break;
    case 'a': control_buffer['1'] = false; release_buffer['1'] = true;
        break;
    case 'e': control_buffer['2'] = false; release_buffer['2'] = true;
        break;
    case '+': control_buffer['+'] = false;
        break;
    case '-': control_buffer['-'] = false;
        break;
    case 'r': control_buffer['G'] = false;
        break;

    }
}

void special_callback(int key, int, int)
{

}

void mouse_callback(int button, int state, int x, int y)
{
    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        if(state==GLUT_DOWN)
        {
            control_buffer['S'] = true;
        }
        else if(state==GLUT_UP)
        {
            control_buffer['S'] = false;
        }
        break;
    case GLUT_RIGHT_BUTTON:
        if(state==GLUT_DOWN)
        {
            control_buffer['r'] = true;
        }
        else if(state==GLUT_UP)
        {
            control_buffer['r'] = false;
        }
        break;
    }
}

void passive_mouse_callback(int x, int y)
{    
    mouse_dx = float(x - prev_mouse_x)/1000;
    mouse_dy = float(y - prev_mouse_y)/800;
    prev_mouse_x = x; prev_mouse_y = y;

    if(x<SCREEN_W/3 || x>2*SCREEN_W/3 || y<SCREEN_H/3 || y>2*SCREEN_H/3)
        glutWarpPointer(SCREEN_W/2, SCREEN_H/2);
}

void mouse_motion_callback(int x, int y)
{
    mouse_dx = float(x - prev_mouse_x)/1000;
    mouse_dy = float(y - prev_mouse_y)/800;
    prev_mouse_x = x; prev_mouse_y = y;

    if(x<SCREEN_W/3 || x>2*SCREEN_W/3 || y<SCREEN_H/3 || y>2*SCREEN_H/3) glutWarpPointer(SCREEN_W/2, SCREEN_H/2);
}

void press_control_nohold(const char &key)
{
    if(release_buffer[key] == true)
    {
        control_buffer[key] = true;
        release_buffer[key] = false;
    }
    else
        control_buffer[key] = false;
}
