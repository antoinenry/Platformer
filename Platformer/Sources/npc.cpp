/*CLASSE "NPC": désigne, en fait, uniquement les ennemis.*/

#include "npc.h"

GLuint bug_texture = 0;

npc::npc()
{
    foot = phys_point(vec2(220,200)); foot.set_bounce(0); foot.set_friction(10); foot.set_mass(25);
}

npc::npc(const vec2 &pos0)
{
    //Physique : un simple point
    foot.set_pos(pos0); foot.set_bounce(0); foot.set_friction(10); foot.set_mass(25);

    //Hitbox pour recevoir des dégats
    hitbox.set_pos(pos0); hitbox.add_pt(-5,-5); hitbox.add_pt(5,-5); hitbox.add_pt(5,5); hitbox.add_pt(-5,5);

    //Sprite
    visual.load("bugg.png", bug_texture, 60, 30, 10, 10, 6, 3);
    visual.define_seq(0,2);
    visual.define_seq(6,8);
    visual.define_seq(3,5);
    visual.define_seq(9,11);
    visual.define_seq(12,15);
    visual.resize(10,10);

    health = 5;
    dying = false;
    timer = 0; on_ground_time = 0; stuck_time = 0;
    on_ground = false; on_ground_buffer = false; on_wall = false;
}

npc::~npc()
{
}

//Réinitialiser l'ennemi
void npc::reset_at(const vec2 &pos0)
{
    health = 3;
    foot.set_pos(pos0);
    dying = false;
    timer = 0; on_ground_time = 0;
    visual.idle(0);
}

//Affichage
void npc::display()
{
    visual.display();

//    hitbox.display();

//    if(on_wall == true)
//        foot.display();
//    else
//        foot.display(0,255,0);
}


vec2 npc::get_pos() const
{
    return foot.get_pos();
}

bool npc::is_hit(const vec2 &bullet_pos) const
{
    return hitbox.is_hit(bullet_pos);
}

bool npc::is_dead() const
{
    return (health<=0 && dying == false);
}

void npc::take_damage(const int &damage)
{
    if(health <= 0) return;
    health -= damage;
    if(health <= 0)
    {
        dying = true;
        timer = 0;
    }
}

//Mort
void npc::die()
{
    //Animation différent au sol et sur un mur
    if(on_ground)
    {
        dying = visual.animate(3, 1);
        if(dying == false) visual.idle(11);
        return;
    }

    if(on_wall)
    {
        dying = visual.animate(2, 1);
        if(dying == false) visual.idle(5);
        return;
    }

    dying = visual.animate(4, 1);
    if(dying == false) visual.idle(15);
}

//Suivre une cible
void npc::follow(const vec2 &target, const Land &l)
{
    //Si l'ennemi tombe du décor
    if(foot.get_pos().y > 1000) die();

    //Si l'ennemi est tué et n'est pas en l'air
    if(health <= 0 && (on_ground || on_wall))
    {
        if(dying) die();
        return;
    }

    //Distance de la cible par rapport cet l'ennemi
    float d = norm(foot.get_pos() - target);
    //Si l'ennemi est en contact, l'ennemi ne bouge plus
    if(d <5) return;

    //Direction de la cible
    vec2 dir = target - foot.get_pos(), tangent = vec2(0,0);
    dir = normalize(dir);

    //Mise à jour physique
    foot.apply_forces();
    foot.set_acc(0, 0);

    //Détemrnine si l'ennemi est au sol ou sur un mur (avec filtrage du vecteur sol, cf actor.cpp)
    on_ground_buffer = false;
    for(unsigned int k=0, kmax = l.seg.size(); k<kmax; k++)
    {      
        tangent = foot.seg_collision(l.seg[k]);
        if(tangent != vec2(0,0))
        {
            on_ground_buffer = true;
            ground_angle = arg(l.seg[k].N());
            break;
        }
    }

    if(on_ground_buffer == true)
    {
        on_ground_time = timer;
        on_ground = true;
    }
    else if( timer - on_ground_time > 10 )
        on_ground = false;

    on_wall = false;
    for(unsigned int k=0, kmax = l.wall.size(); k<kmax; k++)
    {
        if(foot.in_box(l.wall[k]))
        {
            on_wall = true;
            break;
        }
    }

    //Si l'ennemi n'est ni au sol, ni sur un mur, il tombe
    if(tangent == vec2(0,0) && on_wall == false)
        foot.add_weight();
    else //Sinon, il suit sa cible
    {
        float a = fabs(ground_angle-arg(dir));
        if(on_ground == true && a > M_PI/2 && a < 3*M_PI/2) //Si l'ennemi est au sol est la cible se trouve en dessous
        {
            dir = rotate(vec2(1,0), ground_angle + M_PI/2); //L'ennemi tente de contourner le sol
        }
        else
            dir = rotate(dir, M_PI/10*cos(timer/10)); //Sinon, effet d'oscillation sur la trajectoire de l'ennemi
        foot.set_speed(6*dir);
    }

    //Mise à jour de la position de la hitbox
    hitbox.set_pos(foot.get_pos());

    //Animation en fonction de sur un mur, au sol
    if(on_wall == true)
    {
        visual.set_pos(foot.get_pos(), angle(dir, vec2(0,1)));
        visual.animate(0);
    }
    if(on_ground == true)
    {
        visual.set_pos(foot.get_pos(), ground_angle - M_PI/2);
        visual.animate(1);
    }
    if(on_wall == false && on_ground == false)
    {
        visual.set_pos(foot.get_pos());
        visual.animate(1);
    }

    timer++;
}



//Distributeur d'ennemis
bug_nest::bug_nest()
{
    pos = vec2(0,0);
    timer = 0; reserve = 0;
}

bug_nest::bug_nest(const vec2 &p)
{
    pos = p;
    reserve = 5;
    spawn_range = 0;
    timer = 0; respawn_time = 50;
}

bug_nest::bug_nest(const vec2 &p, const unsigned int &range)
{
    pos = p;
    reserve = 5;
    spawn_range = range;
    timer = 0; respawn_time = 50;
}

//...Distribue des ennemis
void bug_nest::spawn_on(const vec2 &target, std::vector<npc> &b)
{
    if(norm(target-pos) > spawn_range || timer < respawn_time)
        timer++;
    else
    {
        timer = 0;
        if(--reserve <=0)
        {
            timer = -500;
            reserve = 10;
        }
        b.push_back(npc(pos));
    }
}
