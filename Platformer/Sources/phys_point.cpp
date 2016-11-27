/* CLASSE "PHYS_POINT" ----------------------------------------------------------------------------------------*/
/*Classe permettant de gérer les collisions physiques*/

#include "phys_point.h"

float const_dt = 0.2, const_g = 8, const_dampling = 0.98;

phys_point::phys_point(){}

phys_point::phys_point(const vec2 &p)
{
    vec2 v0(0,0);
    pos = p; speed = vec2(0,0); acc = v0;
    prev_pos = p; prev_speed = v0; prev_acc = v0;
    radius = 2;
    mass = 1;
}

phys_point::~phys_point()
{
//    std::cout<<"phys_point deleted"<<std::endl;
}

void phys_point::add_force(const vec2 &f)
{
    if(mass == 0) return;

    prev_acc = acc;
    acc+=f/mass;
}

void phys_point::add_weight()
{
    if(mass == 0) return;

    vec2 g(0,const_g);
    add_force(mass*g);
}

void phys_point::add_dampling()
{
    speed *= const_dampling;
}

void phys_point::apply_forces()
{
    prev_speed = speed;
    speed += const_dt*acc;

    prev_pos = pos;
    pos += const_dt*speed;

    if(norm(speed)<1) speed = vec2(0,0);
}

void phys_point::set_acc(const float &x, const float &y)
{
    acc = vec2(x,y);
}

void phys_point::set_pos(const vec2 &pn, bool set_prev)
{
    if(set_prev) prev_pos = pos;
    pos = pn;
}

void phys_point::translate(const vec2 &v)
{
    pos += v;
}

void phys_point::set_mass(const float &m)
{
    mass = m;
}

vec2 phys_point::get_pos() const
{
    return pos;
}

void phys_point::set_speed(const vec2 &s, bool set_prev)
{
    if(set_prev) prev_speed = speed;
    speed = s;
}

vec2 phys_point::get_speed() const
{
    return speed;
}

void phys_point::set_bounce(const float &b)
{
    bounce = b;
}

void phys_point::set_friction(const float &f)
{
    friction = f;
}

float phys_point::damp() const
{
    return (100-friction)/100;
}

//Annuler la dernière étape de la simulation
void phys_point::cancel_move()
{
    float margin = 0.1;
    pos = prev_pos;
    speed = margin*normalize(speed);
}

//Annuler la dernière étape de la simulation
void phys_point::cancel_move(const vec2 &dpos)
{
    float margin = 0.1;
    vec2 dp = dpos;
    if(dp.y>0) dp.y -= margin;
    pos = prev_pos + dp;
    speed = margin*normalize(speed);
}

void phys_point::display(unsigned int R, unsigned int G, unsigned int B)
{
    glColor3ub(R,G,B);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(pos.x, pos.y);
    for(unsigned int i=0; i<8*M_PI; i++)
    {
        glVertex2f(pos.x+radius*cos(i/4.0), pos.y+radius*sin(i/4.0));
    }
    glVertex2f(pos.x+radius, pos.y);
    glEnd();
}

//Collision avec un segment
vec2 phys_point::seg_collision(const phys_segment &s)
{
    vec2 extr_A = s.A(), extr_B = s.B(), prev_pos_comp = s.get_dpos();
    float slip = fmax(s.damp(), damp()), damp = fmin(s.F(), friction);
    float margin = 0.1;

    if(prev_pos_comp.y>0)
        prev_pos_comp.y = 0;
    prev_pos_comp += prev_pos;

    //Cas particulier : trajectoire et segment verticaux
    if(prev_pos_comp.x==pos.x && extr_A.x==extr_B.x) return vec2(0,0);

    //Vecteurs propres au segment
    vec2 seg_tangent = s.T();
    vec2 seg_normal = s.N();
    if(seg_normal==vec2(0,0))
    {
        seg_normal=q_rotate(seg_tangent, 1);
        if(fabs(angle(seg_normal, pos-prev_pos_comp))<M_PI/2)
            seg_normal *= -1;
    }
    else
    {
        float a = fabs(angle(speed,seg_normal));
//        std::cout<<a<<std::endl;
        if(a <= M_PI/2 || a>= 3*M_PI/2)
            return vec2(0,0);
    }

    //Equation du déplacement (si non vertical)
    float   dpos_slope = 0, dpos_0val = 0,
            seg_slope = 0, seg_0val = 0;

    if(prev_pos_comp.x!=pos.x)
    {
        dpos_slope = (pos.y-prev_pos_comp.y)/(pos.x-prev_pos_comp.x);
        dpos_0val = pos.y - dpos_slope*pos.x;
    }

    //Equation du segment (si non vertical)
    if(extr_A.x!=extr_B.x)
    {
        seg_slope = (extr_B.y-extr_A.y)/(extr_B.x-extr_A.x);
        seg_0val = extr_A.y - seg_slope*extr_A.x;
    }

    //Calcul de collision
    vec2 intersection;
    bool collision;

    if(prev_pos_comp.x==pos.x)  //Cas d'un déplacement vertical
    {
        intersection.x = prev_pos_comp.x;
        intersection.y = seg_slope*intersection.x + seg_0val;

        if(prev_pos_comp.y< pos.y)
            collision = intersection.y<pos.y && intersection.y>=prev_pos_comp.y;
        else
            collision = intersection.y<=prev_pos_comp.y && intersection.y>pos.y;
        collision = collision && prev_pos_comp.x<=fmax(extr_A.x, extr_B.x) && prev_pos_comp.x>=fmin(extr_A.x, extr_B.x)
                    && fabs(angle(pos-prev_pos_comp, seg_normal))>M_PI/2;

        //Effet de la collision
        if(collision)
        {
            vec2 new_speed , new_pos;
            new_pos = intersection + projected(pos-intersection, seg_tangent) + seg_normal*margin + s.get_dpos();
            new_speed = slip*projected(speed,seg_tangent) - bounce*projected(speed, seg_normal);
            pos = new_pos;
            speed = new_speed;

            return damp*seg_tangent;
        }
        return vec2(0,0);
    }

    if(extr_A.x==extr_B.x)      //Cas d'un segment vertical
    {
        intersection.x = extr_A.x;
        intersection.y = dpos_slope*intersection.x + dpos_0val;

        if(prev_pos_comp.x< pos.x)
        {
            collision = intersection.x<pos.x && intersection.x>=prev_pos_comp.x;
            seg_normal.x = -fabs(seg_normal.x);
            seg_tangent.y = fabs(seg_tangent.y);
        }
        else
            collision = intersection.x<=prev_pos_comp.x && intersection.x>pos.x;

        collision = collision && prev_pos_comp.y<=fmax(extr_A.y, extr_B.y) && prev_pos_comp.y>=fmin(extr_A.y, extr_B.y)
                && fabs(angle(pos-prev_pos_comp, seg_normal))>M_PI/2;

        //Effet de la collision
        if(collision)
        {
            speed = slip*projected(speed,seg_tangent) - bounce*projected(speed, seg_normal);
            pos = intersection + projected(pos-intersection, seg_tangent) + seg_normal*margin + s.get_dpos();

            return damp*seg_tangent;
        }
        return vec2(0,0);
    }

    //Cas général
    intersection.x = (seg_0val-dpos_0val)/(dpos_slope-seg_slope);
    intersection.y = seg_slope*intersection.x + seg_0val;
    if(prev_pos_comp.y< pos.y)
        collision = intersection.y<pos.y && intersection.y>=prev_pos_comp.y;
    else
        collision = intersection.y<=prev_pos_comp.y && intersection.y>pos.y;

    if(prev_pos_comp.x< pos.x)
        collision = intersection.x<pos.x && intersection.x>=prev_pos_comp.x;
    else
        collision = intersection.x<=prev_pos_comp.x && intersection.x>pos.x;

    collision = collision && prev_pos_comp.x<=fmax(extr_A.x, extr_B.x) && prev_pos_comp.x>=fmin(extr_A.x, extr_B.x)
            && fabs(angle(pos-prev_pos_comp, seg_normal))>M_PI/2;

    //Effet de la collision
    if(collision)
    {
        vec2 new_speed , new_pos;
        new_pos = intersection + projected(pos-intersection, seg_tangent) + seg_normal*margin + s.get_dpos();
        new_speed = slip*projected(speed,seg_tangent) - bounce*projected(speed, seg_normal);
        pos = new_pos;
        speed = new_speed;

        return damp*seg_tangent;
    }
    return vec2(0,0);
}

//Collision avec hitbox
bool phys_point::in_box(const phys_box &box)
{
    return box.is_hit(get_pos());
}

//Non utilisé
vec2 phys_point::bloc_collision(const phys_bloc &b)
{
    vec2 tangent(0,0);
    if(in_box(b.get_box()) == false || 1)
    {
        for(unsigned int i=0, imax=b.get_nb_edges(); i<imax; i++)
        {
            tangent = seg_collision(b.get_edge(i));
            if(tangent != vec2(0,0))
                return tangent;
        }
    }
//    else
//    {
//        vec2 temp_prev_pos = prev_pos, dpos = prev_pos-pos;
//        prev_pos += margin*normalize(dpos);
//        for(unsigned int i=0, imax=b.get_nb_edges(); i<imax; i++)
//        {
//            tangent = seg_collision(b.get_edge(i));
//            if(tangent != vec2(0,0))
//            {
//                prev_pos = temp_prev_pos;
//                return tangent;
//            }
//        }
//    }

    return vec2(0,0);
}







