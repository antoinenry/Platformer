#ifndef WEAPON_H
#define WEAPON_H

#include <stdlib.h>
#include <vector>

#include "phys_point.h"
#include "phys_segment.h"
#include "phys_box.h"
#include "land.h"
#include "npc.h"
#include "sprite.h"

enum weapon_name { HANDS, PISTOL, SHOTGUN, SMG, BLASTER };

class weapon
{
public:
    weapon();
    ~weapon();

    void set_pos(const vec2 &pos, const float &a);
    void flip_H(bool dir);
    void timing();
    void reload();
    int get_magazine() const;
    int get_magazine_size() const;

protected:
    vec2 position;
    sprite visual;
    std::vector<sprite> explosion_visual;
    std::vector<phys_point> bullet;
    std::vector<vec2> bullet_idir;
    std::vector<int> target;
    float timer, fire_time, fire_delay, reload_time, reload_delay;

    float angle;
    float weight;
    unsigned int magazine, magazine_size, bullets_per_shot, damage;
    bool firing;
};

class area_weapon : public weapon
{
public:
    area_weapon();
    void display(const int &flicker);
    vec2 fire(std::vector<npc> &creature);
    void apply_collisions(const Land &land, const std::vector<npc> &creature);

    float range, recoil;
};

class shotgun : public area_weapon
{
public:
    shotgun();
};

class smg : public area_weapon
{
public:
    smg();
};

class pistol : public area_weapon
{
public:
    pistol();
};

class projectile_weapon : public weapon
{
public:
    projectile_weapon();
    void display(const int &flicker = 0);
    void fire();
    void apply_collisions(const Land &land, const std::vector<npc> &creature);
    void apply_forces();
    std::vector<phys_point> bullet;
    std::vector<bool> bullet_active;
    std::vector<unsigned int> bullet_bounce;
    unsigned int magazine, bounce_limit;
    float bullet_speed;
};

class blaster : public projectile_weapon
{
public:
    blaster();
};

class rpg : public projectile_weapon
{
public:
    rpg();
};

struct arsenal
{
    shotgun SGun;
    smg SMGun;
    pistol PGun;
    blaster BLASTGun;
    weapon_name current;

    void display(const int &flicker = 0);
    vec2 fire(std::vector<npc> &creature);
    void reload();
    void apply_collisions(const Land &land, const std::vector<npc> &creature);
    void apply_forces();
    void set(const vec2 &pos, const float &angle);
    void getto_magazine(int &mag, int &magsize) const;
};

#endif // WEAPON
