#ifndef ACTOR_H
#define ACTOR_H

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
#include "weapon.h"
#include "sprite.h"

class actor
{
public:
        actor();
        ~actor();

        void display();
        vec2 get_pos() const;
        vec2 get_dir() const;
        vec2 get_aim() const;
        int get_health() const;
        void refresh();

        void apply_forces();
        bool apply_collisions(const std::vector<phys_segment> &phys_seg);
        void reset_forces();

        void walk(bool dir);
        void run(bool on);
        void duck(bool on);
        void allow_duck(bool on);
        void jump();
        void allow_jump(bool on);
        void stand_ground();
        void compute_delayed_gf();
        void set_stance();
        void set_hitbox();
        bool is_hit(const vec2 &bullet_pos) const;
        bool take_damage(const int &damage, const vec2 &force = vec2(0,0));
        bool is_taking_damage() const;

        void draw_weapon(const weapon_name &wn);
        void set_weapon(const Land &l, const std::vector<npc> &creature);
        void move_aim(float dx, float dy);
        void fire_weapon(std::vector<npc> &creature);
        void reload_weapon();
        void getto_magazine(int &mag, int &magsize) const;

private:
        phys_point foot, front_foot, head, hand;
        vec2 stance;
        float height;
        vec2 ground_friction, gfriction_buffer, ground_mvt, ground_normal;
        phys_box hitbox;
        int health;
        sprite visual_body, visual_hands;

        unsigned int ground_timer, ground_delay;
        unsigned int jump_timer, jump_delay;
        unsigned int stand_timer, stand_delay;
        int timer, damage_time;
        bool taking_damage;

        float stand_friction, slide_friction;
        float max_stand_angle, min_stand_friction;
        float walk_speed, run_speed, duck_speed, airwalk_speed, jump_force, airjump_force, slide_speed;
        bool facing_dir, running, ducking, sliding, hanging;
        bool unduck_allowed, duck_allowed, jump_allowed, drop_allowed;
        bool wallpush;

        float aim_angle;
        bool aiming_dir;
        arsenal equipment;
};


#endif
