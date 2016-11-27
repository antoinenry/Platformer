#ifndef NPC_H
#define NPC_H

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
#include "phys_point.h"
#include "land.h"
#include "phys_box.h"
#include "sprite.h"

class npc
{
private:
    phys_point foot;
    bool on_wall, on_ground, on_ground_buffer;
    unsigned int timer, on_ground_time, stuck_time;
    float ground_angle;
    sprite visual;
    phys_box hitbox;
    int health;
    bool dying;

public:
    npc();
    ~npc();
    npc(const vec2 &pos0);
    void reset_at(const vec2 &pos0);

    vec2 get_pos() const;
    bool is_hit(const vec2 &bullet_pos) const;
    bool is_dead() const;
    void take_damage(const int &damage);
    void die();

    void display();
    void follow(const vec2 &target, const Land &l);

};

struct bug_nest
{
    vec2 pos;
    unsigned int reserve;
    int timer, respawn_time;
    unsigned int spawn_range;

    bug_nest();
    bug_nest(const vec2 &p);
    bug_nest(const vec2 &p, const unsigned int &range);
    void spawn_on(const vec2 &target, std::vector<npc> &b);
};

#endif
