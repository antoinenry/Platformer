#ifndef LAND_H
#define LAND_H

//Librairies graphiques
#include <GL/glew.h>
#include <GL/glut.h>

#include <stdlib.h>
#include <vector>

#include "vec2.h"
#include "phys_segment.h"
#include "phys_point.h"
#include "phys_box.h"
#include "sprite.h"

struct texture_info
{
    texture_info();
    GLuint id; vec2 filesize;
};

class Land
{
public:
    Land();
    ~Land();
    void display();

    void add_seg(phys_segment s);
    void add_seg(float xA, float yA, float xB, float yB, float f);
    void add_seg(float xA, float yA, float xB, float yB, float f, bool normal_dir);

    void add_bloc(float xA, float yA, float xB, float yB, float f, float h);
    void add_wall(std::vector<vec2> pt);
    void add_platform(float xA, float yA, float xB, float yB, float f);

    void animate();

    std::vector<phys_segment> seg;
    std::vector<phys_box> wall;
    std::vector<textured_area> visual;
    texture_info txbloc, txwall, txplatform;
};

#endif // LAND_H
