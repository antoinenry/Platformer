#ifndef SPRITE_H
#define SPRITE_H

#include <stdio.h>
#include <stdlib.h>

//Librairies graphiques
#include <GL/glew.h>
#include <GL/glut.h>
#include "glutils.hpp"
#include "lodepng.h"
#include "vec2.h"

extern int CALLBACK_TIME;

class sprite
{
private:
    vec2 pos, size;
    float angle;
    GLuint texture_id;
    unsigned int frame_nx, frame_ny;
    float frame_uvw, frame_uvh;
    std::vector<unsigned int> sequence;
    unsigned int current_frame, current_seq;
    unsigned int period, timer;
    bool h_mirrored;

public:
    sprite();
    sprite(const vec2& v_pos, const vec2& v_size);
    ~sprite();

    void load(const char* file, GLuint &txt_id,
              const unsigned int &file_w, const unsigned int &file_h,
              const unsigned int &framew, const unsigned int &frameh,
              const unsigned int &nw, const unsigned int &nh);

    void define_seq(const unsigned int &start, const unsigned int &end);
    void resize(const float &w_val, const float &h_val);
    void set_pos(const vec2 &v_pos);
    void set_pos(const vec2 &v_pos, const float &v_angle);
    void h_mirror(const bool &dir);

    bool animate(unsigned int seq, const bool &once = 0);
    void reset_timer();
    void idle(unsigned int frame);
    void display(const float &flicker_period = 0);

};

class textured_area
{
private:
    std::vector<vec2> pt;
    GLuint texture_id;
    vec2 txsize, txoffset;

public:
    textured_area();
    ~textured_area();

    void add_pt(const float &x, const float &y);
    GLuint load(const char* file, const unsigned int &file_w, const unsigned int &file_h);
    void load(const GLuint &id, const vec2 &size);

    void rescale(const float &w, const float &h);
    void translate(const float &w, const float &h);

    GLuint get_txid() const;
    vec2 get_txsize() const;

    void display() const;
};

class decoy
{
private:
    textured_area surface;
    float z;
    GLuint texture_id;
    vec2 txsize, txoffset, txscale;

public:
    decoy();
    ~decoy();

    void load(const char* file, const unsigned int &file_w, const unsigned int &file_h);
    void rescale(const float &w, const float &h);
    void translate(const float &w, const float &h);
    void display(const vec2 &pos, const float &zoom, const float &angle);
};

GLuint png_texture_load(const char *filename, const unsigned int &filewidth, const unsigned int &fileheight);

#endif
