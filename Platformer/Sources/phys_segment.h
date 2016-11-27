#ifndef PHYS_SEGMENT_H
#define PHYS_SEGMENT_H

//Librairies graphiques
#include <GL/glew.h>
#include <GL/glut.h>
#include "glutils.hpp"
#include <iostream>
#include "vec2.h"
#include "math.h"

#include <vector>
#include "vec2.h"

#include "mvt_routine.h"

class phys_segment
{
public:
        phys_segment();
        phys_segment(vec2 ptA, vec2 ptB, float f);
        phys_segment(vec2 ptA, vec2 ptB, float f, bool normal_dir);
        phys_segment(float xA, float yA, float xB, float yB, float f);
        phys_segment(float xA, float yA, float xB, float yB, float f, bool normal_dir);
//        ~phys_segment();

        vec2 A() const;
        vec2 B() const;
        vec2 T() const;
        vec2 N() const;
        float F() const;
        float damp() const;
        bool is_dynamic();

        void set_mvt(mvt_routine *mvt);
        vec2 get_dpos() const;

        void display();

        vec2 speed_A, speed_B;
private:
        vec2 extr_A, extr_B;
        vec2 normal, tangent;
        float friction;
        mvt_routine *traj;
};

#endif
