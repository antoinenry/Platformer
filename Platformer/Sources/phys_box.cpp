/* CLASSE "PHYS_BOX" ----------------------------------------------------------------------------------------*/
/*Classe permettant surtout de créer des hitboxs...*/

#include "phys_box.h"

phys_box::phys_box()
    :nbpt(0), H_flipped(false){}

phys_box::phys_box(const vec2 &c)
    :center(c), H_flipped(false){}

phys_box::phys_box(const float &cx, const float &cy)
    :center(vec2(cx,cy)), H_flipped(false){}

phys_box::~phys_box()
{
//    std::cout<<"phys_box deleted / "<<std::endl;
}

void phys_box::add_pt(const vec2 &npt)
{
    rel_pt.push_back(npt);
    abs_pt.push_back(center+npt);
    nbpt++;
}

void phys_box::add_pt(const float &x, const float &y)
{
    add_pt(vec2(x,y));
}

vec2 phys_box::get_pos() const
{
    return center;
}

void phys_box::set_pos(const vec2 &pos)
{
    center = pos;
    for(unsigned int i=0; i<nbpt; i++)
        abs_pt[i] = center+rel_pt[i];
}

void phys_box::set_pos(const vec2 &pos, const float &angle)
{
    center = pos;
    for(unsigned int i=0; i<nbpt; i++)
        abs_pt[i] = center+rotate(rel_pt[i], angle);
}

void phys_box::set_pos(const vec2 &pos, const float &angle, const float &v_scale, const float &h_scale)
{
    center = pos;
    for(unsigned int i=0; i<nbpt; i++)
        abs_pt[i] = center+rotate(vec2(v_scale*rel_pt[i].x, h_scale*rel_pt[i].y), angle);
}

void phys_box::set_abs_pt(unsigned int i, const vec2 &pos)
{
    abs_pt[i] = pos;
}

void phys_box::flip_H(bool dir)
{
    if(H_flipped != dir)
    {
        for(unsigned int i=0; i<nbpt; i++)
            rel_pt[i].x *= -1;
        H_flipped = dir;
    }
}

bool phys_box::is_hit(const vec2 &pos) const
{
    vec2 v1, v2;
    float det = 0;
    bool looped_once = false;
    unsigned int imax = nbpt-1;
    for(unsigned int i=0; i<imax; i++)
    {
        v1 = abs_pt[i+1] - abs_pt[i]; v1 = normalize(v1);
        v2 = pos - abs_pt[i]; v2 = normalize(v2);
        if(looped_once && det * determinant(v1, v2) <= 0)
            return false;
        looped_once = true;
        det = determinant(v1, v2);
    }

    v1 = abs_pt[0] - abs_pt[imax]; v1 = normalize(v1);
    v2 = pos - abs_pt[0]; v2 = normalize(v2);
    if(det * determinant(v1, v2) < 0)
        return false;

    return true;
}

void phys_box::display()
{
    glColor3ub(0,255,0);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2i(center.x, center.y);
        for(unsigned int i=0; i<nbpt; i++)
            glVertex2i(abs_pt[i].x, abs_pt[i].y);
        glVertex2i(abs_pt[0].x, abs_pt[0].y);
    glEnd();
}

void phys_box::display(int R, int G, int B)
{
    glColor3ub(R,G,B);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2i(center.x, center.y);
        for(unsigned int i=0; i<nbpt; i++)
            glVertex2i(abs_pt[i].x, abs_pt[i].y);
        glVertex2i(abs_pt[0].x, abs_pt[0].y);
    glEnd();
}

/*---------------------------------------------------------------------*/
//Classe non utilisée
phys_bloc::phys_bloc()
{
    float f = 8, margin = 2;
    vec2   M(300, 400);
    vec2 A(-50, -20), B(-50, 20), C(50, 20), D(50, -20);

    hitbox.set_pos(M);
    hitbox.add_pt(A+vec2(margin,margin));
    hitbox.add_pt(B+vec2(margin,-margin));
    hitbox.add_pt(C+vec2(-margin,-margin));
    hitbox.add_pt(D+vec2(-margin,margin));

    edge.push_back(phys_segment(M+A, M+B, f)); edge.push_back(phys_segment(M+B, M+C, f));
    edge.push_back(phys_segment(M+C, M+D, f)); edge.push_back(phys_segment(M+D, M+A, f));
}

phys_bloc::~phys_bloc()
{}

void phys_bloc::display()
{
    hitbox.display(100,75,50);
    for(unsigned int i=0, imax = edge.size(); i<imax; i++)
        edge[i].display();
}

phys_box phys_bloc::get_box() const
{
    return hitbox;
}

phys_segment phys_bloc::get_edge(unsigned int i) const
{
    return edge[i];
}

int phys_bloc::get_nb_edges() const
{
    return edge.size();
}






















