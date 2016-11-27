/* CLASSE "PHYS_SEGMENT" ----------------------------------------------------------------------------------------*/
/*Classe permettant de gérer les collisions physiques*/

#include "phys_segment.h"

phys_segment::phys_segment(){}

phys_segment::phys_segment(vec2 ptA, vec2 ptB, float f)
{
    friction = f;

    if(ptA.x > ptB.x){
        extr_A = ptB; extr_B = ptA; }
    else{
        extr_A = ptA; extr_B = ptB; }
    speed_A = vec2(0,0); speed_B = vec2(0,0);

    tangent = extr_A-extr_B;
    tangent = normalize(tangent);
    normal = vec2(0,0);
    traj = NULL;
}

phys_segment::phys_segment(vec2 ptA, vec2 ptB, float f, bool normal_dir)
{
    friction = f;

    if(ptA.x > ptB.x){
        extr_A = ptB; extr_B = ptA; }
    else{
        extr_A = ptA; extr_B = ptB; }
    speed_A = vec2(0,0); speed_B = vec2(0,0);

    tangent = extr_A-extr_B;
    tangent = normalize(tangent);

    normal = q_rotate(tangent, normal_dir);
    traj = NULL;
}

phys_segment::phys_segment(float xA, float yA, float xB, float yB, float f)
{
    friction = f;

    vec2 ptA(xA, yA), ptB(xB,yB);
    if(ptA.x > ptB.x){
        extr_A = ptB; extr_B = ptA; }
    else{
        extr_A = ptA; extr_B = ptB; }
    speed_A = vec2(0,0); speed_B = vec2(0,0);

    tangent = extr_A-extr_B;
    tangent = normalize(tangent);

    normal = vec2(0,0);
    traj = NULL;
}

phys_segment::phys_segment(float xA, float yA, float xB, float yB, float f, bool normal_dir)
    :extr_A(vec2(xA,yA)), extr_B(vec2(xB,yB)), friction(5)
{
    friction = f;

    vec2 ptA(xA, yA), ptB(xB,yB);
    if(ptA.x > ptB.x){
        extr_A = ptB; extr_B = ptA; }
    else{
        extr_A = ptA; extr_B = ptB; }
    speed_A = vec2(0,0); speed_B = vec2(0,0);

    tangent = extr_A-extr_B;
    tangent = normalize(tangent);

    normal = q_rotate(tangent, normal_dir);
    traj = NULL;
}

//phys_segment::~phys_segment()
//{
//    std::cout<<"phys_segment deleted / "<<std::endl;
//}

vec2 phys_segment::A() const
{
    if(traj == NULL)
        return extr_A;
    else
        return extr_A + traj->current_pos();
}

vec2 phys_segment::B() const
{
    if(traj == NULL)
        return extr_B;
    else
        return extr_B + traj->current_pos();
}

vec2 phys_segment::T() const
{
    return tangent;
}

vec2 phys_segment::N() const
{
    return normal;
}

float phys_segment::F() const
{
    return friction;
}

float phys_segment::damp() const
{
    return (100-friction)/100;
}

bool phys_segment::is_dynamic()
{
    return traj != NULL;
}

void phys_segment::set_mvt(mvt_routine *mvt)
{
    traj = mvt;
}

vec2 phys_segment::get_dpos() const
{
    if(traj == NULL) return vec2(0,0);
    return traj->current_dpos();
}

void phys_segment::display()
{
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
        glVertex2i(A().x, A().y);
        glVertex2i(B().x, B().y);
    glEnd();

    //Affichage de la normale
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_LINES);
        glVertex2i(A().x/2 + B().x/2, A().y/2 + B().y/2);
        glVertex2i(A().x/2 + B().x/2 + 10*normal.x, A().y/2 + B().y/2 + 10*normal.y);
    glEnd();
}
