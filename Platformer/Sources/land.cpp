/* CLASSE "LAND" ----------------------------------------------------------------------------------------*/
/*Classe dédiée à l'environnemet, au décor*/

#include "land.h"

texture_info::texture_info() : id(0), filesize(vec2(0,0)) {}

Land::Land()
{
    //L'adhérence du sol peut être réglée. Ici, j'ai pris une valeur fixe.
    float friction = 8;

    //Murs de fond (inutiles au joueur, mais les ennemis peuvent s'y déplacer
    std::vector<vec2> pt;

    pt.push_back(vec2(-200,-50)); pt.push_back(vec2(-110,-50)); pt.push_back(vec2(-110,300)); pt.push_back(vec2(-200,300));
    add_wall(pt);
    pt.clear();

    pt.push_back(vec2(200,-50)); pt.push_back(vec2(110,-50)); pt.push_back(vec2(110,300)); pt.push_back(vec2(200,300));
    add_wall(pt);
    pt.clear();

    pt.push_back(vec2(-110,-50)); pt.push_back(vec2(110,-50)); pt.push_back(vec2(110,150)); pt.push_back(vec2(-110,150));
    add_wall(pt);
    pt.clear();

    //Blocs solides pour le joueur et les ennemis.
    //Cadre
    add_bloc(-200,-200,200,-200, friction, 50);
    add_bloc(-250,300,250,300, friction, 50);
    add_bloc(-250,-200,-200,-200, friction, 500);
    add_bloc(200,-200,250,-200, friction, 500);

    //Blocs
    add_bloc(-160,200,160,200, friction, 20);
    add_bloc(110,-50,160,-50, friction, 200);
    add_bloc(-160,-50,-110,-50, friction, 220);
    add_bloc(-50,-50,50,-50, friction, 10);
    add_bloc(-110,110,40,150, friction, 20);
    add_bloc(160,280,200,260, friction, 40);

    //Plates-formes : peuvent être traversée par dessous
    add_platform(-20,25,20,25, friction);
    add_platform(-110,50,-80,50, friction);
    add_platform(80,0,110,0, friction);
    add_platform(80,100,110,100, friction);
}


Land::~Land()
{
//    std::cout<<"land deleted / "<<std::endl;
}

//Affichage -----------------------------------------------------------------------------|
void Land::display()
{
    for(unsigned int i=0, imax=visual.size(); i<imax; i++)
        visual[i].display();

    //Affichage des segments
//    for(unsigned int i=0, imax=seg.size(); i<imax; i++)
//        seg[i].display();

//    for(unsigned int i=0, imax=wall.size(); i<imax; i++)
//        wall[i].display();
}

//Ajout d'élements
void Land::add_seg(phys_segment s)
{
    seg.push_back(s);
}

void Land::add_seg(float xA, float yA, float xB, float yB, float f)
{
    seg.push_back(phys_segment(xA, yA, xB, yB, f));
}

void Land::add_seg(float xA, float yA, float xB, float yB, float f, bool normal_dir)
{
    seg.push_back(phys_segment(xA, yA, xB, yB, f, normal_dir));
}

//Un bloc est constitué de 4 segments
void Land::add_bloc(float xA, float yA, float xB, float yB, float f, float h)
{
    float yh = fmax(yA, yB) + h;
    textured_area cover;
    vec2 margin;

    //Visuel
    cover.add_pt(xA,yA); cover.add_pt(xB,yB); cover.add_pt(xB,yh); cover.add_pt(xA,yh);    
    if(txbloc.filesize != vec2(0,0))
    {
        cover.load(txbloc.id, txbloc.filesize);
    }
    else
    {
        txbloc.id = cover.load("stone.png", 20, 20);
        txbloc.filesize = vec2(20,20);
    }
    visual.push_back(cover);

    //Physique
    margin = vec2(xA,yA) - vec2(xB,yB);
    margin = 2*normalize(margin);

    add_seg(xA+margin.x, yA+margin.y, xB-margin.x, yB-margin.y, f, 1);
    add_seg(xA,yA,xA,yh,f,0);
    add_seg(xB,yB,xB,yh,f,1);
    add_seg(xA,yh,xB,yh,f,0);
}

//Un plate-forme est un simple segment
void Land::add_platform(float xA, float yA, float xB, float yB, float f)
{
    float h = 10;
    textured_area cover;
    vec2 margin;

    //Visuel
    cover.add_pt(xA,yA); cover.add_pt(xB,yB); cover.add_pt(xB,yB+h); cover.add_pt(xA,yA+h);
    if(txplatform.filesize != vec2(0,0))
    {
        cover.load(txplatform.id, txplatform.filesize);
    }
    else
    {
        txplatform.id = cover.load("floor.png", 40, 10);
        txplatform.filesize = vec2(40,10);
    }
    visual.push_back(cover);

    //Physique
    margin = vec2(xA,yA) - vec2(xB,yB);
    margin = 2*normalize(margin);

    add_seg(xA+margin.x, yA+margin.y, xB-margin.x, yB-margin.y, f, 1);
}

//Ajout de murs de fond
void Land::add_wall(std::vector<vec2> pt)
{
    textured_area cover;
    phys_box surface;
    vec2 center = vec2(0,0);

    for(unsigned int i=0, imax = pt.size(); i<imax; i++)
    {
        cover.add_pt(pt[i].x, pt[i].y);
        center += pt[i]/imax;
    }
    surface.set_pos(center, 0, 1, 1);
    for(unsigned int i=0, imax = pt.size(); i<imax; i++)
        surface.add_pt(pt[i]-center);
    wall.push_back(surface);

    if(txwall.filesize != vec2(0,0))
    {
        cover.load(txwall.id, txwall.filesize);
    }
    else
    {
        txwall.id = cover.load("brick.png", 20, 20);
        txwall.filesize = vec2(20,20);
    }
    visual.push_back(cover);
}

void Land::animate()
{
//    mvt.run();
}
