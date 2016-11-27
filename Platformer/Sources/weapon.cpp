/* CLASSE "WEAPON" ----------------------------------------------------------------------------------------*/
/*Classe permettant de gérer les armes du joueur*/
/*"Area_weapon permet de créer des armes à effet immédiat (les points visés prennent des dégât immédiatement lorsque l joueur tire)
 *"Projectile_weapon permet de créer des armes qui tirent des projectiles physiques, pouvant rebondir, être affecter par la gravité, avoir une certaine vitesse, etc.*/

#include "weapon.h"

#define RIGHT true
#define LEFT false

GLuint shotgun_texture = 0, shotgun_explosion_texture = 0;

weapon::weapon(){}

weapon::~weapon()
{
//    std::cout<<"weapon deleted / "<<std::endl;
}

void weapon::set_pos(const vec2 &pos, const float &a)
{
    position = pos;

    if(timer - reload_time < reload_delay)
    {
        if(angle<M_PI/2 || angle > 3*M_PI/2)
            angle = 0;
        else
            angle = M_PI;
    }
    else
        angle = a;
}

int weapon::get_magazine() const
{
    return magazine;
}

int weapon::get_magazine_size() const
{
    return magazine_size;
}

//Affichage
void area_weapon::display(const int &flicker)
{
//    for(unsigned int i=0; i<bullets_per_shot; i++)
//    {
////        if(target[i] != -1)
//            bullet[i].display();
//    }

    std::vector<unsigned int> deleted_explosions;

    visual.set_pos(position, angle);
    if(angle < M_PI/2 || angle > 3*M_PI/2)
    {
        if(firing == true)
            firing = visual.animate(1,1);
        else
        {
            if(timer - reload_time < reload_delay)
                visual.animate(3);
            else
                visual.idle(8);
        }
    }
    else
    {
        if(firing == true)
            firing = visual.animate(0,1);
        else
        {
            if(timer - reload_time < reload_delay)
                visual.animate(2);
            else
                visual.idle(0);
        }
    }
    visual.display();


    for(unsigned int i=1, imax = explosion_visual.size(); i<imax; i++)
    {
        if(explosion_visual[i].animate(0,1) == true)
            explosion_visual[i].display();
        else
            deleted_explosions.push_back(i);
    }
    for(unsigned int i=0, imax = deleted_explosions.size(); i<imax; i++)
        explosion_visual.erase(explosion_visual.begin() + deleted_explosions[i]);
}

void weapon::flip_H(bool dir)
{
    visual.h_mirror(dir);
}

area_weapon::area_weapon(){}

void area_weapon::apply_collisions(const Land &land, const std::vector<npc> &creature)
{
    bool collision;
    float d, step = 10;

    timer++;
    if(timer - reload_time == reload_delay) magazine = magazine_size;

    for(unsigned int i=0; i<bullets_per_shot; i++)
    {
        bullet[i].set_pos(position);
        collision = false;
        d = 0;
        while(collision == false && d<range)
        {
            bullet[i].set_pos(bullet[i].get_pos() + rotate(step*bullet_idir[i], angle));
            bullet[i].set_speed(rotate(step*bullet_idir[i], angle));
            d += step;
            for(unsigned int k=0, kmax = land.seg.size(); k<kmax; k++)
            {
                if(bullet[i].seg_collision(land.seg[k]) != vec2(0,0))
                {
                    target[i] = -1;
                    collision = true;
                    break;
                }
            }
            if(collision == true) break;

            for(unsigned int k=0, kmax = creature.size(); k<kmax; k++)
            {
                if(creature[k].is_hit(bullet[i].get_pos()))
                {
                    target[i] = k;
                    collision = true;
                    break;
                }
                else
                    target[i] = -1;
            }
        }
    }
}

vec2 area_weapon::fire(std::vector<npc> &creature)
{
    if(timer - fire_time < fire_delay) return vec2(0,0);

    if(magazine <= 0 || timer - reload_time < reload_delay)
    {
        if(timer - reload_time > reload_delay)
            reload();

        return vec2(0,0);
    }

    int k;
    sprite expl = explosion_visual[0];

    fire_time = timer;
    magazine --;
    visual.reset_timer();
    firing = true;

    for(unsigned int i=0; i<bullets_per_shot; i++)
    {
        k = target[i];
        if(k!=-1)
            creature[k].take_damage(damage);
    }

    expl.set_pos(position + rotate(vec2(15,0), angle));
    explosion_visual.push_back(expl);

    return -recoil*vec2(cos(angle), sin(angle));
}

void weapon::reload()
{
    if(magazine == magazine_size) return;
    reload_time = timer;

}

shotgun::shotgun()
{
    sprite expl;

    damage = 50;
    weight = 0;
    fire_delay = 40;
    reload_delay = 50;
    bullets_per_shot = 9;
    recoil = 2000;
    range = 200;
    magazine_size = 10;
    magazine = magazine_size;

    firing = false;
    timer = reload_delay; fire_time = 0; reload_time = 0;

    expl.load("explosion1.png", shotgun_explosion_texture, 320, 40, 40, 40, 8, 1);
    expl.define_seq(0,7);
    expl.resize(10, 10);
    expl.reset_timer();
    explosion_visual.push_back(expl);

    visual.load("shotgun.png", shotgun_texture, 1600, 240, 200, 60, 8, 4);
    visual.define_seq(0,7);
    visual.define_seq(8,15);
    visual.define_seq(16,23);
    visual.define_seq(24,31);
    visual.resize(40, 10);

    phys_point b(vec2(0,0));
    b.set_friction(20);
    bullet.assign(bullets_per_shot,b);bullet_idir.assign(bullets_per_shot,vec2(1,0));
    bullet_idir[1] = rotate(vec2(1,0), 0.08); bullet_idir[2] = rotate(vec2(1,0), 0.06); bullet_idir[3] = rotate(vec2(1,0), 0.04); bullet_idir[4] = rotate(vec2(1,0), 0.02);
    bullet_idir[5] = rotate(vec2(1,0), -0.08); bullet_idir[6] = rotate(vec2(1,0), -0.06); bullet_idir[7] = rotate(vec2(1,0), -0.04); bullet_idir[8] = rotate(vec2(1,0), -0.02);

    target.assign(bullets_per_shot, -1);
}

smg::smg()
{
    sprite expl;

    damage = 1;
    weight = 0;
    fire_delay = 5;
    reload_delay = 40;
    bullets_per_shot = 3;
    recoil = 1200;
    range = 400;
    magazine_size = 32;
    magazine = magazine_size;

    firing = false;
    timer = reload_delay; fire_time = 0; reload_time = 0;

    expl.load("explosion1.png", shotgun_explosion_texture, 320, 40, 40, 40, 8, 1);
    expl.define_seq(0,5);
    expl.resize(5, 5);
    expl.reset_timer();
    explosion_visual.push_back(expl);

    visual.load("smallmachinegun.png", shotgun_texture, 1600, 240, 200, 60, 8, 4);
    visual.define_seq(0,7);
    visual.define_seq(8,15);
    visual.define_seq(16,23);
    visual.define_seq(24,31);
    visual.resize(40, 10);

    phys_point b(vec2(0,0));
    b.set_friction(20);
    bullet.assign(bullets_per_shot,b);bullet_idir.assign(bullets_per_shot,vec2(1,0));

    target.assign(bullets_per_shot, -1);
}

pistol::pistol()
{
    damage = 20;
    weight = 0;
    fire_delay = 0;
    reload_delay = 1;
    bullets_per_shot = 2;
    recoil = 0;
    range = 500;



    phys_point b(vec2(0,0));
    b.set_friction(20); b.set_bounce(0); b.set_mass(0);
    bullet.assign(bullets_per_shot,b); bullet_idir.assign(bullets_per_shot,vec2(0,0));
    bullet_idir[1] = vec2(1,0);

}

projectile_weapon::projectile_weapon(){}

void projectile_weapon::display(const int &flicker)
{

    for(unsigned int i=0; i<magazine_size; i++)
    {
        if(bullet_active[i] == true) bullet[i].display();
    }
}

void projectile_weapon::fire()
{
    if(timer - fire_time > fire_delay)
    {
        fire_time = timer;
        magazine--;
        bullet[magazine].set_pos(position);
        for(unsigned int i=0; i<bullets_per_shot; i++)
        {
            bullet[magazine].add_force(bullet_speed*rotate(bullet_idir[i],angle));
            bullet_active[magazine] = true;
        }
    }
}

void projectile_weapon::apply_collisions(const Land &land, const std::vector<npc> &creature)
{
    for(unsigned int i=0; i<magazine_size; i++)
    {
        for(unsigned int k=0, kmax = land.seg.size(); k<kmax; k++)
        {
            if(bullet[i].seg_collision(land.seg[k]) != vec2(0,0) && bounce_limit>0)
            {
                bullet_bounce[i]++;
                if(bullet_bounce[i]>=bounce_limit)
                    bullet_active[i] = false;
                break;
            }
        }
    }
}

void projectile_weapon::apply_forces()
{
    for(unsigned int i=magazine; i<magazine_size; i++)
    {
//        bullet[i].add_weight();
        bullet[i].apply_forces();
        bullet[i].set_acc(0,0);
    }
}

blaster::blaster()
{
    magazine_size = 5;
    magazine = magazine_size;
    damage = 200;
    weight = 10;
    fire_delay = 20;
    reload_delay = 2;
    bullets_per_shot = 1;
    bullet_speed = 150;
    bounce_limit = 5;


    phys_point b(vec2(0,0));
    b.set_friction(1); b.set_bounce(1.0); b.set_mass(1);
    bullet.assign(magazine_size,b); bullet_idir.assign(bullets_per_shot,vec2(0,0));
    bullet_idir[0] = vec2(1,0);

    bullet_bounce.assign(magazine_size, 0);
    bullet_active.assign(magazine_size, false);
}

rpg::rpg()
{
    magazine_size = 5;
    magazine = magazine_size;
    damage = 1000;
    weight = 20;
    fire_delay = 500;
    reload_delay = 2;
    bullets_per_shot = 1;
    bullet_speed = 100;
    bounce_limit = 1;


    phys_point b(vec2(0,0));
    b.set_friction(1); b.set_bounce(1.0); b.set_mass(1);
    bullet.assign(magazine_size,b); bullet_idir.assign(bullets_per_shot,vec2(0,0));
    bullet_idir[0] = vec2(1,0);

    bullet_bounce.assign(magazine_size, 0);
    bullet_active.assign(magazine_size, false);
}

void arsenal::display(const int &flicker)
{
    switch(current)
    {
    case HANDS:
        break;
    case PISTOL:    PGun.display(flicker);
        break;
    case SHOTGUN:   SGun.display(flicker);
        break;
    case SMG:       SMGun.display(flicker);
        break;
    case BLASTER:   BLASTGun.display(flicker);
        break;
    }
}

vec2 arsenal::fire(std::vector<npc> &creature)
{
    switch(current)
    {
    case HANDS:
        break;
    case PISTOL:    return PGun.fire(creature);
        break;
    case SHOTGUN:   return SGun.fire(creature);
        break;
    case SMG:       return SMGun.fire(creature);
        break;
    case BLASTER:   BLASTGun.fire(); return vec2(0,0);
        break;
    }

    return vec2(0,0);
}

void arsenal::reload()
{
    switch(current)
    {
    case HANDS:
        break;
    case PISTOL:    PGun.reload();
        break;
    case SHOTGUN:   SGun.reload();
        break;
    case SMG:       SMGun.reload();
        break;
    case BLASTER:   BLASTGun.reload();
        break;
    }
}

void arsenal::apply_collisions(const Land &land, const std::vector<npc> &creature)
{
    switch(current)
    {
    case HANDS:
        break;
    case PISTOL:    PGun.apply_collisions(land, creature);
        break;
    case SHOTGUN:   SGun.apply_collisions(land, creature);
        break;
    case SMG:       SMGun.apply_collisions(land, creature);
        break;
    case BLASTER:   BLASTGun.apply_collisions(land, creature);
        break;
    }
}

void arsenal::set(const vec2 &pos, const float &angle)
{
    switch(current)
    {
    case HANDS:
        break;
    case PISTOL:
        PGun.set_pos(pos, angle);
        break;
    case SHOTGUN:
        SGun.set_pos(pos, angle);
        break;
    case SMG:
        SMGun.set_pos(pos,angle);
        break;
    case BLASTER:
        BLASTGun.set_pos(pos, angle);
        break;
    }
}

void arsenal::apply_forces()
{
    switch(current)
    {
    case BLASTER:   BLASTGun.apply_forces();
        break;
    default:        break;
    }
}

void arsenal::getto_magazine(int &mag, int &magsize) const
{
    switch(current)
    {
    case HANDS: mag = 0; magsize = 0;
        break;
    case PISTOL: mag = 0; magsize = 0;
        break;
    case SHOTGUN:  mag = SGun.get_magazine(); magsize = SGun.get_magazine_size();
        break;
    case SMG: mag = SMGun.get_magazine(); magsize = SMGun.get_magazine_size();
        break;
    case BLASTER:
        break;
    }
}






