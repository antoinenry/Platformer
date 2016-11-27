#ifndef PHYS_BOX_H
#define PHYS_BOX_H

//Librairies graphiques
#include <GL/glew.h>
#include <GL/glut.h>
#include "glutils.hpp"
#include <iostream>
#include "vec2.h"

#include "math.h"
#include <vector>

#include "phys_segment.h"

class phys_box
{
public:
    phys_box();
    phys_box(const vec2 &c);
    phys_box(const float &cx, const float &cy);
    ~phys_box();

    void add_pt(const vec2 &npt);
    void add_pt(const float &x, const float &y);

    vec2 get_pos() const;
    void set_pos(const vec2 &pos);
    void set_pos(const vec2 &pos, const float &angle);
    void set_pos(const vec2 &pos, const float &angl, const float &v_scale, const float &h_scale);

    void set_abs_pt(unsigned int i, const vec2 &pos);

    void flip_H(bool dir);

    bool is_hit(const vec2 &pos) const;

    void display();
    void display(int R, int G, int B);

public:
    std::vector<vec2> abs_pt;
    unsigned int nbpt;

private:
    vec2 center;
    std::vector<vec2> rel_pt;
    bool H_flipped;
};

/*-----------------------------------------------------------------------*/

class phys_bloc
{
public:
    phys_bloc();
    ~phys_bloc();
    void display();

    phys_box get_box() const;
    phys_segment get_edge(unsigned int i) const;
    int get_nb_edges() const;

private:
    phys_box hitbox;
    std::vector<phys_segment> edge;
};

#endif
