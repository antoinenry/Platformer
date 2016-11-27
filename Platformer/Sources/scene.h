#ifndef SCENE_H
#define SCENE_H

#include <stdlib.h>
#include <vector>

#include "land.h"
#include "actor.h"
#include "npc.h"
#include "control_buffer.h"
#include "interface.h"


class camera
{
private:
    vec2 pos, tspeed, tacc, ttarget;
    float zoom, zspeed, zacc, ztarget, ztargetspeed, zmin, zmax;
    float angle;
    float dampling, tspring, zspring;

public:
    camera();
    ~camera();

    vec2 get_pos() const;
    float get_zoom() const;
    float get_angle() const;

    void zoom_in();
    void zoom_out();

    void compute();
    void follow(const vec2 &pt);
};

struct Scene
{
    camera view;
    Land land;
    actor hero;
    std::vector<npc> bug;
    std::vector<bug_nest> nest;
    decoy background;

    indic_bar hero_heathbar;
    indic_meter score_meter;
    indic_bar magazine_bar;
    indic_text game_over_text;
    indic_text score_text;
    indic_text restart_text;
    int score;
    bool game_over;
    GLuint font1;

    Scene(bool intro = false);
    ~Scene();
    bool refresh();
    void display();
    void apply_controls();
    void compute_camera();
    bool gameOver();
};



#endif // ROOM_H
