/* CLASSE "ACTOR" ----------------------------------------------------------------------------------------*/
/*Classe dédiée aux personnage jouable. Gère les mouvements, les collisions, la santé, l'équipement, etc..*/

#include "actor.h"

//Identifiants textures
GLuint  body_texture = 0,
        hands_texture = 0;

//Constructeur standard----------------------------------------------------------------------------------------|
actor::actor()
{
    //Points pour collision physique avec le décor:
    // - pour marcher:
    foot = phys_point(vec2(0,0)); foot.set_bounce(0); foot.set_friction(10); foot.set_mass(25);
    // - pour la collision basse aux murs:
    front_foot = phys_point(vec2(0,0)); front_foot.set_bounce(0); front_foot.set_friction(8); front_foot.set_mass(0);
    // - pour la collision haute aux murs:
    head = phys_point(vec2(0,0)); head.set_bounce(0); head.set_friction(20); head.set_mass(0);
    // - Pour s'accrocher aux rebords :
    hand = phys_point(vec2(0,0)); hand.set_bounce(0); hand.set_friction(8); hand.set_mass(0);
    //Hauteur "physique" du personnage:
    height = 20;
    // Vecteur pour l'orientation du personnage (gestion des pentes)
    stance = height*vec2(0,-1);

    //Gestion des dégats: santé et hitbox, délai d'invicibilité lorsque le héros est touché
    health = 20;
    hitbox.set_pos(foot.get_pos() + vec2(0,-height/2));
        hitbox.add_pt(5, height/2); hitbox.add_pt(5, -height/2); hitbox.add_pt(-5, -height/2); hitbox.add_pt(-5, height/2);
    timer = 0; damage_time = 0;

    //Vecteur sol, pour l'inclinaison du personnage, et les conditions et la direction du saut
    ground_friction = vec2(0,0);
    //Buffer permettant de "lisser" l'évolution du vecteur sol dans le temps (éviter les effets de vibration)
    gfriction_buffer = ground_friction;
    ground_delay = 5; ground_timer = 0;

    //Directions
    facing_dir = true; aiming_dir = true;
    //Pour empêcher le joueur de se relever lorsque le plafond est trop bas
    unduck_allowed = false;
    //Pour bloquer la position du personnage lorsqu'il ne marche pas (pas de glissade sur les pentes)
    stand_delay = 5; stand_timer = stand_delay + 1;
    max_stand_angle = 0.9;
    min_stand_friction = 2;
    stand_friction = 8;
    slide_friction = 2;

    //Caractéristiques des mouvements
    walk_speed = 18;
    run_speed = 50;
    duck_speed = 5;
    airwalk_speed = 20;
    jump_force = 3200;
    airjump_force = 100;
    slide_speed = 25;

    //Initialisation des armes
    equipment.current = SHOTGUN;

    //Course (non utilisé)
    running = false;

    //Sprite du corps
    visual_body.load("herobody.png", body_texture, 992, 792, 108, 132, 9, 6);
    // - Animations
    visual_body.define_seq(0,7);
    visual_body.define_seq(9,16);
    visual_body.define_seq(18,25);
    visual_body.define_seq(27,34);
    visual_body.define_seq(36,43);
    visual_body.resize(20, 1.3*height);

    //Sprite des mains
    visual_hands.load("herohands.png", hands_texture, 992, 264, 108, 132, 9, 2);
    // - Animations
    visual_hands.define_seq(0,7);
    visual_hands.resize(20, 1.3*height);
}

actor::~actor()
{
//    std::cout<<"actor deleted / "<<std::endl;
}

//Affichage----------------------------------------------------------------------------------------|
void actor::display()
{
    //Les yeux du personnage suivent la visée
    int aim_sector;
    if(facing_dir == 1) aim_sector = 6 - 5*aim_angle/(2*M_PI);
    else aim_sector = 3 + 5*aim_angle/(2*M_PI);
    aim_sector = aim_sector%5;

    visual_body.set_pos(get_pos(), arg(stance) - M_PI/2);
    visual_body.h_mirror(!facing_dir);
    visual_hands.set_pos(get_pos(), arg(stance) - M_PI/2);
    visual_hands.h_mirror(!facing_dir);

    if(taking_damage == true)
    {
        visual_body.idle(51); visual_hands.idle(1);
        visual_body.display(0.2); // Permets de faire clignoter le sprite
    }
    else
    {
        if(ground_friction == vec2(0,0)) // Dans les airs
        {
            if(hanging) {visual_body.idle(50); visual_hands.idle(9);} //Accroché à un rebord
            else        {visual_body.idle(48); visual_hands.idle(2);} //En saut
        }
        else //Au sol
        {
            if(norm(foot.get_speed()) > 2 ) { visual_body.animate(aim_sector); visual_hands.animate(0);} // marche
            else                            { visual_body.idle(9*aim_sector + 8); visual_hands.idle(0);} // immobile
        }
        visual_body.display();
    }

    if(equipment.current == HANDS) visual_hands.display(); // Si aucune arme sélectionné, un sprite d'animation des mains est utilisé
    else equipment.display(); // Sinon, l'arme est affichée (pas d'animation)
}

//Accesseurs
vec2 actor::get_pos() const
{
    return hitbox.get_pos();
}

vec2 actor::get_dir() const
{
    vec2 dir = ground_friction;
    if(dir == vec2(0,0))
        dir = vec2(-1,0);
    if(facing_dir == 1)
        return -1*normalize(dir);
    else
        return normalize(dir);
}

vec2 actor::get_aim() const
{
    return rotate(vec2(1,0), aim_angle);
}

int actor::get_health() const
{
    return health;
}

//Mise à jour physique
void actor::apply_forces()
{
    //Délai lorsque le personnage est touché (clignotement, immunité temporaire)
    timer++;
    if(timer - damage_time < 20) taking_damage = true;
    else taking_damage = false;

    //Physique
    foot.add_weight();
    foot.apply_forces();
    foot.add_dampling();
//    ground_friction = vec2(0,0);

    //Calcul de la portée des armes
    equipment.apply_forces();

    //Si le personnage tombe du décor
    if(get_pos().y>1000)
    {
        foot.set_pos(vec2(0,0)); foot.set_pos(vec2(0,0));
        head.set_pos(vec2(0,0)); head.set_pos(vec2(0,0));
        hand.set_pos(vec2(0,0)); hand.set_pos(vec2(0,0));
    }
}

//Entre chaque mise à jour
void actor::reset_forces()
{
    foot.set_acc(0,0);
}

//Collisions avec le décor
bool actor::apply_collisions(const std::vector<phys_segment> &phys_seg)
{
    bool collision = false;
    vec2 tangent(0,0);
    wallpush = false;
    hanging = false;

    //Pied arrière (sol)
    for(unsigned int k=0, kmax = phys_seg.size(); k<kmax; k++)
    {
        if(tangent == vec2(0,0))
            tangent = foot.seg_collision(phys_seg[k]);
        if(tangent != vec2(0,0))
        {
            gfriction_buffer = tangent;
            ground_normal = phys_seg[k].N();
            ground_mvt = phys_seg[k].get_dpos();
            collision = true;
            tangent = vec2(0,0);
        }
    }

    //Main (pour s'accrocher)
    hand.set_pos(foot.get_pos() + height*normalize(stance) + q_rotate(5*normalize(stance),facing_dir));
    hand.set_speed(foot.get_speed());
    if(equipment.current == HANDS)
    {

        for(unsigned int k=0, kmax = phys_seg.size(); k<kmax; k++)
        {
            tangent = hand.seg_collision(phys_seg[k]);
            if( tangent != vec2(0,0))
            {
                if(angle(tangent, vec2(-1,0))>max_stand_angle
                        || angle(tangent, vec2(-1,0))<-max_stand_angle)
                {
                }
                else
                {
                    foot.cancel_move();
                    hanging = true;
                }
                break;
            }
        }
    }

    //Pied avant (mur)
    front_foot.set_pos(foot.get_pos() + q_rotate(5*normalize(stance),facing_dir));
    front_foot.set_speed(foot.get_speed());
    for(unsigned int k=0, kmax = phys_seg.size(); k<kmax; k++)
    {
        tangent = front_foot.seg_collision(phys_seg[k]);
        if( tangent != vec2(0,0))
        {
            if(angle(tangent, vec2(-1,0))>max_stand_angle
                    || angle(tangent, vec2(-1,0))<-max_stand_angle)
            {
                wallpush = true;
                foot.set_pos(front_foot.get_pos() - q_rotate(5*normalize(stance),facing_dir));
                foot.set_speed(front_foot.get_speed());
            }
            break;
        }
    }

    //Tête (plafond/mur haut)
    head.set_pos(foot.get_pos() + stance);
    head.set_speed(foot.get_speed());
    tangent = vec2(0,0);
    for(unsigned int k=0, kmax = phys_seg.size(); k<kmax; k++)
    {
        if(phys_seg[k].N().y >= 0)
        {
            tangent = head.seg_collision(phys_seg[k]);
            if(tangent != vec2(0,0))
            {
                if(collision)
                {
                    ground_mvt = vec2(0,0);
                    foot.cancel_move(); head.cancel_move();
                    return true;
                }
                else
                {
                    foot.set_pos(head.get_pos()-stance);
                    foot.set_speed(head.get_speed());
                    return false;
                }
            }
        }
    }

    //Pour se relever
    if(ducking)
    {
        tangent = vec2(0,0);
        phys_point next_head = foot;
        next_head.set_pos(foot.get_pos()+ height*normalize(stance));
        for(unsigned int k=0, kmax = phys_seg.size(); k<kmax; k++)
        {
            if(tangent == vec2(0,0))
            {
                tangent = next_head.seg_collision(phys_seg[k]);
            }
            if(tangent != vec2(0,0))
            {
                unduck_allowed = false;
                break;
            }
        }
        if(tangent == vec2(0,0)) unduck_allowed = true;
    }

    return collision;
}

//Marcher
void actor::walk(bool dir)
{
    //Si le joueur marche contre un mur, la commande est ignorée
    if(wallpush)
        return;

    //Orientation du personnage
    facing_dir = dir;
    float a = angle(ground_friction, vec2(-1,0));

    //Vitesse de déplacement
    float force = walk_speed;
    if(running) force = run_speed;
    if(ducking && !sliding) force = duck_speed;

    //Compensation de la pente (si l'angle n'est pas trop fort, la force de marche augmente avec la pente)
    if(a<max_stand_angle && a>-max_stand_angle && norm(ground_friction)>1)
        force += walk_speed*fabs(a/max_stand_angle);

    //Vecteur de déplacement
    vec2 step = force*ground_friction;
    //En l'air, le joueur a un controle réduit des mouvements
    if(ground_friction == vec2(0,0))
        step = vec2(-airwalk_speed, 0);

    //Direction
    if(dir==1)
        step *= -1;

    //Application
    foot.add_force(step);
    stand_timer = 0;
}

//Course (non utilisé)
void actor::run(bool on)
{
    running = on;
}

//S'accrouppir et se relever (il manque l'animation) --------------------------------------------------|
void actor::duck(bool on)
{
    //Si le joueur est suspendu à un rebord, le personnage lâche prise
    if(hanging && on == true)
    {
        hand.set_pos(head.get_pos());
        hand.set_speed(head.get_speed());
        duck_allowed = false;
        return;
    }

    //Sinon, le personnage s'accroupit (on == true)
    if(on == true && duck_allowed == true)
    {
        ducking = true;
        return;
    }

    //Ou se relève (on == false) si le plafond n'est pas trop bas
    if(on == false && unduck_allowed == true)
    {
        foot.set_friction(stand_friction);
        ducking = false;
        return;
    }
}

void actor::allow_duck(bool on)
{
    duck_allowed = on;
}

//Saut----------------------------------------------------------------------------|
void actor::jump()
{
    //Vecteur de saut
    vec2 hop;

    //Contrôle réduit si le personnage ne touche pas le sol
    if(norm(ground_friction) == 0 && foot.get_speed().y <0)
    {
        hop = airjump_force*vec2(0,-1);
        foot.add_force(hop);
    }

    //Saut d'appui contre un mur
    if(wallpush)
    {
        hop = jump_force*vec2(0,-1);
        hop = 0.5*hop + 0.5*q_rotate(hop, !facing_dir);
        foot.add_force(hop);
        return;
    }

    //Sinon, on vérifie que la touche de saut est relâchée entre chaque saut
    if(jump_allowed == false) return;
    else jump_allowed = false;

    //Si le personnage est accroupi, il se relève juste
    if(ducking)
    {
        duck(false);
        return;
    }

    //S'il est suspendu à un rebord, le saut est vertical et réduit
    if(hanging)
    {
        hop = 0.8*jump_force*vec2(0,-1);
        stand_timer = 0;
        foot.add_force(hop);
        hanging = false;
        return;
    }

    //Cas d'un saut normal, orienté selon la pente au sol
    hop = ground_normal;
    hop = jump_force*normalize(hop);

    //Apllication
    foot.add_force(hop);
    stand_timer = 0;
}

void actor::allow_jump(bool on)
{
    jump_allowed = on;
}

//Pour ne pas glisser lorsque le joueur veut rester immoble sur une pente
void actor::stand_ground()
{
    //Cas ou le joueur ne touche pas le sol
    if(norm(ground_friction) == 0)
    {
        stand_timer = 0;
        return;
    }

    //Cas ou la pente est trop glissante ou trop raide
    if(angle(ground_friction, vec2(-1,0))>max_stand_angle
            || angle(ground_friction, vec2(-1,0))<-max_stand_angle
            || norm(ground_friction)<=min_stand_friction)
        return;

    //Délai avant immobilisation (dérapage)
    if(stand_timer <= stand_delay)
    {
        foot.set_speed(foot.damp()*foot.get_speed());
        stand_timer++;
    }
    else
        foot.cancel_move(ground_mvt); //Immobilise le personnage
}

//Délai sur le vecteur sol: sans cela, le vecteur n'est pas stable car le joueur n'est pas en contact absolument constant avec le sol
//Je filtre donc les variations du vecteur en ignorant les écarts trop courts
void actor::compute_delayed_gf()
{
    //Si le buffer est non nul depuis suffisament longtemps, le vecteur sol est mis à jour
    if(gfriction_buffer != vec2(0,0))
    {
        ground_friction = gfriction_buffer;
        ground_timer = 0;
    }
    else // Sinon, le vecteur sol n'est pas modifié
    {
        if(ground_timer<ground_delay)
            ground_timer++;
        else
        {
            ground_friction = vec2(0,0);
            ground_normal = vec2(0,0);
        }
    }
    gfriction_buffer = vec2(0,0);
}

//Orientation du personnage en fonction de la pente, et ajustement de sa taille si il est accroupi
void actor::set_stance()
{
    //Vitesse de rétablissement à la verticale lorsque le personnage est en l'air
    float air_balance = 0.02;

    //Vecteur sol "filtré"
    compute_delayed_gf();

    //Hauteur du personnage
    float h;
    //Orientation actuelle
    stance = normalize(stance);

    //Accrouppi, la taille est réduite et le personnage glisse plus facilement
    if(ducking)
    {
        h = height/2;
        sliding = ( norm(foot.get_speed()) > slide_speed );
    }
    else
    {
        h = height;
        sliding = false;
    }

    //Si le personnage n'est pas sur un sol sur lequel il peut se tenir debout, il s'oriente progressivement à la verticale
    if(angle(ground_friction, vec2(-1,0))>max_stand_angle
            || angle(ground_friction, vec2(-1,0))<-max_stand_angle
            || norm(ground_friction)<=min_stand_friction)
    {
        stance = h*normalize(stance);
        float a = angle(stance, vec2(0,-1));
        if(a < 0)
        {
            if(a <=- air_balance*M_PI)
                stance = rotate(stance, air_balance*M_PI);
            else
                stance = vec2(0,-1);
        }
        else
        {
            if(a >= air_balance*M_PI)
                stance = rotate(stance, -air_balance*M_PI);
            else
                stance = vec2(0,-1);
        }
    }
    else // Sinon, il s'oriente perpendiculairement au sol
    {
        if(sliding)
            foot.set_friction(slide_friction);
        else
            foot.set_friction(stand_friction);

        stance = ground_normal;

    }

    //Taille du personnage
    stance = h*normalize(stance);
}

//Mise à jour de la position de la hitbox ----------------------------------------------------------------------------------|
void actor::set_hitbox()
{
    if(ducking)
        hitbox.set_pos(foot.get_pos() + vec2(0,-height/4), angle(stance, vec2(0,1)), 1, 0.5);
    else
        hitbox.set_pos(foot.get_pos() + vec2(0,-height/2), angle(stance, vec2(0,1)));
}

//Vérifie si le point bullet_pos touche la hitbox du personnage ----------------------------------------------------------------------------------|
bool actor::is_hit(const vec2 &bullet_pos) const
{
    return hitbox.is_hit(bullet_pos);
}

//Dégâts ----------------------------------------------------------------------------------|
bool actor::take_damage(const int &damage, const vec2 &force)
{
    //Si délai d'immunité, le dégât est ignoré
    if(taking_damage) return false;

    //Saut en arrière
    foot.add_force(force);

    //Mise à jour de la santé
    health -= damage;

    //Commencement du délai d'immunité
    damage_time = timer;
    taking_damage = true;

    return true;
}

//Délai d'immunité
bool actor::is_taking_damage() const
{
    return taking_damage;
}

//Positionnement de l'arme et portée
void actor::set_weapon(const Land &l, const std::vector<npc> &creature)
{
    equipment.set(foot.get_pos() + vec2(0,-10), aim_angle);
    //Calcule les points touché par l'arme si le personnage tire (décor et ennemis)
    equipment.apply_collisions(l, creature);
}

//Viser ----------------------------------------------------------------------------------|
void actor::move_aim(float dx, float dy)
{
    if(dx == 0 && dy == 0) return;

    //Volte-face
    if(dx < -0.05 && (aim_angle<M_PI/2 || aim_angle>3*M_PI/2))
        aim_angle = M_PI-aim_angle;
    if(dx > 0.05 && aim_angle>M_PI/2 && aim_angle<3*M_PI/2)
        aim_angle = M_PI-aim_angle;

    //Visée verticale
    if(aim_angle < 2*M_PI/5 || aim_angle>8*M_PI/5)
    {
        aim_angle += dy;
    }
    if(aim_angle > 3*M_PI/5 && aim_angle < 7*M_PI/5)
    {
        aim_angle -= dy;
    }

    //Visée horizontale
    if(aim_angle > M_PI/5 && aim_angle < 4*M_PI/5)
    {
        aim_angle -= dx;
    }
    if(aim_angle >6*M_PI/5 && aim_angle < 9*M_PI/5)
    {
        aim_angle += dx;
    }

    //Modulo 2PI
    if(aim_angle<0)         aim_angle += 2*M_PI;
    if(aim_angle>=2*M_PI)   aim_angle -= 2*M_PI;

    aiming_dir = (aim_angle < M_PI/2 || aim_angle> 3*M_PI/2);
}

//Tirer  ----------------------------------------------------------------------------------|
void actor::fire_weapon(std::vector<npc> &creature)
{
    //Les armes ont un comportement spécifique au niveau de l'effet de recul
    vec2 recoil = equipment.fire(creature),
            current_speed = foot.get_speed();
    switch(equipment.current)
    {
    case SHOTGUN:
    {
        foot.add_force(recoil);
    }
        break;
    case SMG:
        if(norm(ground_friction) == 0 && current_speed.y > 0)  //Effet du recul (sur la chute)
        {
            foot.add_force(recoil);
        }
        else if(norm(ground_friction) != 0 && current_speed.x != 0)  //Effet du recul (sur la course)
        {
            if(norm(ground_friction)>min_stand_friction && current_speed.x * recoil.x <0)
                return;
            else
            {
                recoil = 0.2*projected(recoil, ground_friction);
                foot.add_force(recoil);
            }
        }
        break;

    default: break;
    }
}

//Recharger ----------------------------------------------------------------------------------|
void actor::reload_weapon()
{
    equipment.reload();
}

//Changer d'arme  ----------------------------------------------------------------------------------|
void actor::draw_weapon(const weapon_name &wn)
{
    if(wn != equipment.current)
        equipment.current = wn;
    else
        equipment.current = HANDS;
}

//Permet de modifier le chargeur (munitions, capacité)
void actor::getto_magazine(int &mag, int &magsize) const
{
    equipment.getto_magazine(mag, magsize);
}
