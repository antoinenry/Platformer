/* CLASSE "SCENE" ----------------------------------------------------------------------------------------*/
/*Classe permettant de gérer le jeu*/

#include "scene.h"

#define RIGHT true
#define LEFT false

Scene::Scene(bool intro)
{
    font1 = 0;

    //Ecran titre
    if(intro)
    {
        game_over = true;

        score_meter = indic_meter(vec2(10,10), vec2(0.15,0.05), 3);
        score_meter.load_font("font.png", 1300, 500, font1);
        game_over_text = indic_text(vec2(-0.5,0.4), vec2(1.0,0.2), "PLEASE SHOOT THE BUGS");
        game_over_text.load_font("font.png", 1300, 500, font1);
        score_text = indic_text(vec2(-0.1,0.2), vec2(0.2,0.05), "");
        score_text.load_font("font.png", 1300, 500, font1);
        restart_text = indic_text(vec2(-0.5,-0.5), vec2(1.0,0.08), "Press R to play or ESC to quit");
        restart_text.load_font("font.png", 1300, 500, font1);
        return;
    }

    //Jeux
    //Ennemis
    nest.push_back(bug_nest(vec2(0,100), 200));
    nest.push_back(bug_nest(vec2(0,-150), 200));
    nest.push_back(bug_nest(vec2(-150,250), 200));

    //UI
    hero_heathbar = indic_bar(vec2(-0.95,0.9), vec2(0.8,0.05), 20);
    magazine_bar = indic_bar(vec2(-0.95,0.85), vec2(0.4,0.03), 5);
    score_meter = indic_meter(vec2(-0.075,0.90), vec2(0.15,0.05), 3);
    score_meter.load_font("font.png", 1300, 500, font1);
    score_meter.set_val(0);
    game_over_text = indic_text(vec2(-0.25,0.4), vec2(0.5,0.1), "GAME OVER");
    game_over_text.load_font("font.png", 1300, 500, font1);
    score_text = indic_text(vec2(-0.1,0.2), vec2(0.2,0.05), "Your score");
    score_text.load_font("font.png", 1300, 500, font1);
    restart_text = indic_text(vec2(-0.5,-0.5), vec2(1.0,0.08), "Press R to play again or ESC to quit");
    restart_text.load_font("font.png", 1300, 500, font1);

    score = 0;
    game_over = false;
}

Scene::~Scene()
{
//    std::cout<<"scene deleted / "<<std::endl;
}

//Mise à jour de la scene
bool Scene::refresh()
{
    if(game_over)
    {
        return gameOver();
    }

    unsigned int cpt=0, compute_limit = 100;
    bool collide = true;
    int mag, magsize;

    hero.reset_forces();

    //Apparition des ennemis (20 max)
    if(bug.size() < 20)
        for(unsigned int i=0, imax = nest.size(); i<imax; i++)
            nest[i].spawn_on(hero.get_pos(), bug);

    //Destruction des ennemis
    for(unsigned int i=0, imax = bug.size(); i<imax; i++)
        if(bug[i].is_dead())
        {
            score ++;
            bug.erase(bug.begin() + i);
        }

    //Comportement des ennemis
    for(unsigned int i=0, imax = bug.size(); i<imax; i++)
    {
        bug[i].follow(hero.get_pos(), land);
        if(hero.is_hit(bug[i].get_pos()) == true)
        {
            vec2 hit_force = hero.get_pos() - bug[i].get_pos();
            hit_force = 2000*normalize(hit_force);
            hit_force.y = -fabs(hit_force.y);
            hero.take_damage(1, hit_force);
        }
    }

    //UI
    hero_heathbar.set_val(hero.get_health());
    hero.getto_magazine(mag, magsize);
    magazine_bar.set_val(mag);
    magazine_bar.set_max(magsize);
    score_meter.set_val(score);   

    //Controles du personnage
    apply_controls();
    hero.apply_forces();
    hero.set_weapon(land, bug);

    //Collisions
    cpt=0; compute_limit = 100;
    do{
        collide = hero.apply_collisions(land.seg);
        if(++cpt>compute_limit) break;
    }while (collide == true);

    land.animate(); //Non utilisé

    //Orientation du personnage
    hero.set_stance();
    hero.set_hitbox();

    //Game Over
    if(hero.get_health() <= 0)
    {
        score_meter.set_pos(vec2(-0.075, 0));
        game_over = true;
    }

    return false;
}

//Affichage
void Scene::display()
{
    if(game_over)
    {
        score_meter.display();
        game_over_text.display();
        score_text.display();
        restart_text.display();
    }
    else
    {
        compute_camera();

    //    background.display(view.get_pos(), view.get_zoom(), view.get_angle());
        land.display();
        for(unsigned int i=0, imax = bug.size(); i<imax; i++)
            bug[i].display();
        hero.display();

        hero_heathbar.display();
        magazine_bar.display();
        score_meter.display();
    }
}

//Gestion du clavier
void Scene::apply_controls()
{
    char key;

    if ((control_buffer['U'] || control_buffer['L'] || control_buffer['R'] || hero.is_taking_damage() ) == false)
        hero.stand_ground();
    if(control_buffer['U'] == false)
        hero.allow_jump(true);
    if(control_buffer['D'] == false)
    {   hero.duck(false); hero.allow_duck(true);    }

    for(unsigned int i=0; i<control_key_size; i++)
    {
        key = control_key[i];

        if(control_buffer[key] == true)
        {
            switch(key)
            {
            case 'R': if(control_buffer['L'] == false) hero.walk(RIGHT);
                    else hero.stand_ground();
                break;
            case 'L': if(control_buffer['R'] == false) hero.walk(LEFT);
                else hero.stand_ground();
                break;
            case 'U': hero.jump();
                break;
            case 'D': hero.duck(true);
                break;
            case 'S': hero.fire_weapon(bug);
                break;
            case 'r': hero.reload_weapon();
                break;
            case '1': hero.draw_weapon(SHOTGUN); control_buffer['1'] = false;
                break;
            case '2': hero.draw_weapon(SMG); control_buffer['2'] = false;
                break;
            case '+': view.zoom_in();
                break;
            case '-': view.zoom_out();
                break;
            }
        }
    }

    hero.move_aim(mouse_dx, mouse_dy);
    mouse_dx = 0; mouse_dy = 0;
}

//Vue
void Scene::compute_camera()
{
    view.follow(hero.get_pos() + 30*hero.get_aim() + vec2(0,-15));
    view.compute();
}


/* CLASSE "CAMERA" ----------------------------------------------------------------------------------------*/
/*Classe permettant de gérer la vue. Caméra montée sur ressort suivant la viséee du héros*/
camera::camera()
{
    pos = vec2(0,0); tspeed = vec2(0,0); tacc = vec2(0,0);
    angle = 0.0;
    zoom = 4.0; zmin = 1.0; zmax = 8.0; ztarget = 7.0; ztargetspeed = 0.2;
    dampling = 0.8;
    tspring = 0.5; zspring = 0.4;
}

camera::~camera(){}

vec2 camera::get_pos() const
{
    return pos;
}

float camera::get_angle() const
{
    return angle;
}

float camera::get_zoom() const
{
    return zoom;
}

void camera::zoom_in()
{
    ztarget += ztargetspeed * const_dt;
    if(ztarget>zmax) ztarget = zmax;
}

void camera::zoom_out()
{
    ztarget -= ztargetspeed * const_dt;
    if(ztarget<zmin) ztarget = zmin;
}

void camera::compute()
{
    tacc = tspring*(ttarget - pos);
    tspeed += tacc * const_dt;
    tspeed *= dampling;
    pos += tspeed * const_dt;

    zacc = zspring*(ztarget - zoom);
    zspeed += zacc * const_dt;
    zspeed *= dampling;
    zoom += zspeed * const_dt;
}

void camera::follow(const vec2 &pt)
{
    ttarget = pt;
}

bool Scene::gameOver()
{
    if(control_buffer['G'] == true)
        return true;
    else
        return false;
}




