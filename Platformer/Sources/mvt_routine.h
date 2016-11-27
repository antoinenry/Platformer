#pragma once

#ifndef MVT_ROUTINE_HPP
#define MVT_ROUTINE_HPP

#include <iostream>
#include <math.h>

#include <vector>
#include "vec2.h"
#include "phys_const.h"

struct mvt_routine
{
public:
    mvt_routine();
    ~mvt_routine();

    void run();
    void add_step(const float &x, const float &y, const float &t);
    vec2 current_pos() const;
    vec2 current_dpos() const;

private:
    float timer;
    unsigned int step, n_steps;
    std::vector<vec2> pt;
    std::vector<float> duration;
    vec2 pos, prev_pos;

};

#endif
