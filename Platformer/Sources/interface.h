#ifndef INTERFACE_H
#define INTERFACE_H

//Librairies graphiques
#include <GL/glew.h>
#include <GL/glut.h>
#include "glutils.hpp"

//Librairies standards
#include <iostream>
#include "math.h"
#include <vector>
#include <string>

//Autres
#include "vec2.h"
#include "sprite.h"

class indic_bar
{
public:
    indic_bar();
    indic_bar(const vec2 &vpos, const vec2 &vsize, const int &vmax);
    ~indic_bar();

    void display();
    void set_val(const int &a);
    void set_max(const int &a);

private:
    vec2 pos, size;
    int val, max;
};

class indic_meter
{
public:
    indic_meter();
    indic_meter(const vec2 &vpos, const vec2 &vsize, const int &n);
    ~indic_meter();

    void load_font(const char* file,
                   const unsigned int &file_w, const unsigned int &file_h, GLuint &txt_id);
    void display();
    void set_val(const int &a);
    void set_pos(const vec2 &p);

private:
    vec2 pos, size;
    int val, ndigits;
    GLuint font;
};


class indic_text
{
public:
    indic_text();
    indic_text(const vec2 &vpos, const vec2 &vsize, const std::string &text);
    ~indic_text();

    void load_font(const char* file,
                   const unsigned int &file_w, const unsigned int &file_h, GLuint &txt_id);
    void display();
    void set_pos(const vec2 &p);

private:
    vec2 pos, size;
    std::string txt;
    GLuint font;
};

#endif
