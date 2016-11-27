/*Cette classe, non utilisée, devait permettre de créer des éléments de décor dynamique (plateformes mouvantes, portes...)*/

#include "mvt_routine.h"

mvt_routine::mvt_routine()
{
    timer = 0; step = 0; n_steps = 1;
    prev_pos = vec2(0,0);
    pt.push_back(vec2(0,0));
}

mvt_routine::~mvt_routine()
{
//    std::cout<<"mvt_routine deleted / "<<std::endl;
}

void mvt_routine::add_step(const float &x, const float &y, const float &t)
{
    pt.push_back(vec2(x,y));
    duration.push_back(t);
    n_steps++;
}

void mvt_routine::run()
{
    vec2 A, B;

    timer = timer + const_dt;

    while(timer>duration[step])
    {
        timer-=duration[step];
        step = step+1;
        if(step >= n_steps) step = 0;
    }

    A = pt[step]; B = pt[step+1];
    prev_pos = pos;
    pos = A + (A-B)*(cos(M_PI*timer/duration[step])-1)/2;
}

vec2 mvt_routine::current_pos() const
{
    return pos;
}

vec2 mvt_routine::current_dpos() const
{
    return pos - prev_pos;
}
