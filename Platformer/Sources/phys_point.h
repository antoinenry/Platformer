#ifndef PHYS_POINT_H
#define PHYS_POINT_H

//Librairies graphiques
#include <GL/glew.h>
#include <GL/glut.h>
#include "glutils.hpp"

//Librairies standards
#include <iostream>
#include "math.h"
#include <vector>

//Autres
#include "vec2.h"
#include "phys_const.h"
#include "phys_segment.h"
#include "phys_box.h"


class phys_point
{
public:
        phys_point();
        phys_point(const vec2 &p);
        ~phys_point();

        void set_pos(const vec2 &pn, bool set_prev = true);
        vec2 get_pos() const;
        void translate(const vec2 &v);
        void set_speed(const vec2 &s, bool set_prev = true);
        vec2 get_speed() const;
        void set_acc(const float &x, const float &y);
        void set_bounce(const float &b);
        void set_friction(const float &f);
        void set_mass(const float &m);
        float damp() const;

        void add_force(const vec2 &f);
        void add_weight();
        void add_dampling();
        void apply_forces();
        void cancel_move();
        void cancel_move(const vec2 &dpos);

        vec2 seg_collision(const phys_segment &s);
        bool in_box(const phys_box &box);
        vec2 bloc_collision(const phys_bloc &b);

        void display(unsigned int R=255, unsigned int G=0, unsigned int B=0);

private:
        vec2 pos, speed, acc;
        vec2 prev_pos, prev_speed, prev_acc;
        float radius, mass, bounce, friction;
};

#endif
