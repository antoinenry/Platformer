/* CLASSE "SPRITE" ----------------------------------------------------------------------------------------*/
/*Classe permettant de gérer les visuels 2D*/

#include "sprite.h"

sprite::sprite()
{
    texture_id = 0;
}

sprite::~sprite(){}

sprite::sprite(const vec2& v_pos, const vec2& v_size)
    :pos(v_pos),size(v_size),texture_id(0){}

void sprite::load(const char *file, GLuint &txt_id,
             const unsigned int &file_w, const unsigned int &file_h,
             const unsigned int &frame_w, const unsigned int &frame_h,
             const unsigned int &nw, const unsigned int &nh)
{
    frame_nx = nw; frame_ny = nh;
    frame_uvw = float(frame_w)/float(file_w); frame_uvh = float(frame_h)/float(file_h);
    period = 40; timer = 0; current_frame = 0;
    h_mirrored = false;
    timer = 0;

    if(txt_id == 0)
        texture_id = png_texture_load(file, file_w, file_h);
    else
        texture_id = txt_id;
}

void sprite::define_seq(const unsigned int &start, const unsigned int &end)
{
    sequence.push_back(start);
    sequence.push_back(end);
}

void sprite::resize(const float &w_val, const float &h_val)
{
    size.x = w_val; size.y = h_val;
}

void sprite::set_pos(const vec2 &v_pos)
{
    pos=v_pos;
}

void sprite::set_pos(const vec2 &v_pos, const float &v_angle)
{
    pos = v_pos;
    angle = v_angle;
}

void sprite::h_mirror(const bool &dir)
{
    h_mirrored = dir;
}

bool sprite::animate(unsigned int seq, const bool &once)
{
    int lseq = sequence[2*seq+1] - sequence[2*seq], f;

    if(seq != current_seq)
    {
        timer = 0;
        current_seq = seq;
    }

    f = ((timer*CALLBACK_TIME)/period)%lseq;
    timer++;

    if(once == true && seq == current_seq && current_frame == sequence[2*seq+1]-1)
        return false;
    else
        current_frame = sequence[2*seq] + f;

    return true;
}

void sprite::reset_timer()
{
    timer = 0;
}

void sprite::idle(unsigned int frame)
{
    timer++;
    current_frame = frame;
}

void sprite::display(const float &flicker_period)
{
    if(flicker_period != 0)
    {
        int flick_timer = timer/(flicker_period*CALLBACK_TIME);
        if(flick_timer%2 == 0)
            return;
    }

    vec2 uv(current_frame%frame_nx,current_frame/frame_nx);
    uv.x *= frame_uvw; uv.y *= frame_uvh;
    vec2 a(size.x/2, size.y/2), b(-size.x/2, size.y/2), c(-size.x/2, -size.y/2), d(size.x/2, -size.y/2);
    a = rotate(a,angle); b = rotate(b,angle); c = rotate(c,angle); d = rotate(d,angle);

    if(h_mirrored)
    {
        vec2 s;
        s = a; a = b; b = s;
        s = c; c = d; d = s;
    }

    glBindTexture(GL_TEXTURE_2D, texture_id);
    glEnable(GL_TEXTURE_2D);
    glColor4f(1.0,1.0,1.0,1.0);
    glBegin(GL_QUADS);
        glTexCoord2f(uv.x,uv.y);                     glVertex2d(pos.x+a.x, pos.y+a.y);
        glTexCoord2f(uv.x+frame_uvw,uv.y);           glVertex2d(pos.x+b.x, pos.y+b.y);
        glTexCoord2f(uv.x+frame_uvw,uv.y+frame_uvh); glVertex2d(pos.x+c.x, pos.y+c.y);
        glTexCoord2f(uv.x,uv.y+frame_uvh);           glVertex2d(pos.x+d.x, pos.y+d.y);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

GLuint png_texture_load(const char *filename, const unsigned int &filewidth, const unsigned int &fileheight)
{
    unsigned int w = filewidth, h = fileheight;
    GLuint id;
    std::vector<unsigned char> buffer, bufferout;
    unsigned int s;
    GLubyte *image;

    lodepng::load_file(buffer, filename);
    lodepng::decode(bufferout, w, h, buffer);
    s = bufferout.size();

    image = (GLubyte*)malloc(s*sizeof(GLubyte));

    for(unsigned int i=0; i<s; i++)
        image[i] = bufferout[i];

    glGenTextures(1,&id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(
                GL_TEXTURE_2D,
                0,
                4,
                w, h, 0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                image
                );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return id;
}

/* CLASSE "TEXTURED _AREA" ----------------------------------------------------------------------------------------*/
/*Classe permettant de créer des zones texturées*/
textured_area::textured_area()
{
    texture_id = 0;
    txsize = vec2(0,0);
    txoffset = vec2(0,0);
}

textured_area::~textured_area(){}

void textured_area::add_pt(const float &x, const float &y)
{
    pt.push_back(vec2(x,y));
}

GLuint textured_area::load(const char* file, const unsigned int &file_w, const unsigned int &file_h)
{
    texture_id = png_texture_load(file, file_w, file_h);
    txsize = vec2(file_w, file_h);

    return texture_id;
}

void textured_area::load(const GLuint &id, const vec2 &size)
{
    texture_id = id;
    txsize = size;
}

void textured_area::rescale(const float &w, const float &h)
{
    txsize.x *= w; txsize.y *= h;
}

void textured_area::translate(const float &w, const float &h)
{
    txoffset.x += w; txoffset.y += h;
}

GLuint textured_area::get_txid() const
{
    return texture_id;
}

vec2 textured_area::get_txsize() const
{
    return txsize;
}

void textured_area::display() const
{
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glEnable(GL_TEXTURE_2D);
    glColor4f(1.0,1.0,1.0,1.0);
    glBegin(GL_TRIANGLE_FAN);

    int     x0 = pt[0].x,
            y0 = pt[0].y;
    for(unsigned int i=0, imax=pt.size(); i<imax; i++)
    {
        int     x = pt[i].x,
                y = pt[i].y;
        glTexCoord2f((x-x0+txoffset.x)/txsize.x, (y-y0+txoffset.y)/txsize.y);    glVertex2d(x,y);
    }


    glEnd();
    glDisable(GL_TEXTURE_2D);
}


//Non utilisé
decoy::decoy()
{
    z = 1;
    txscale = vec2(1,1);
    txsize = vec2(0,0);
    txoffset = vec2(0,0);
}

decoy::~decoy()
{}

void decoy::load(const char *file, const unsigned int &file_w, const unsigned int &file_h)
{
    texture_id = png_texture_load(file, file_w, file_h);
    txsize = vec2(file_w, file_h);

    txscale.x /= 0.2; txscale.y /= 0.2;
}

void decoy::rescale(const float &w, const float &h)
{
    txsize.x *= w; txsize.y *= h;
}

void decoy::translate(const float &w, const float &h)
{
    surface.translate(w, h);
}

void decoy::display(const vec2 &pos, const float &zoom, const float &angle)
{
    float z2 = 0.5*z;

    glBindTexture(GL_TEXTURE_2D, texture_id);
    glEnable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glColor4f(1.0,1.0,1.0,1.0);
    glBegin(GL_QUADS);
        glTexCoord2f(txscale.x*(pos.x/txsize.x-z2)/(zoom+z2), txscale.y*(pos.y/txsize.y+z2)/(zoom+z2));    glVertex2d(-1,-1);
        glTexCoord2f(txscale.x*(pos.x/txsize.x+z2)/(zoom+z2), txscale.y*(pos.y/txsize.y+z2)/(zoom+z2));    glVertex2d(1,-1);
        glTexCoord2f(txscale.x*(pos.x/txsize.x+z2)/(zoom+z2), txscale.y*(pos.y/txsize.y-z2)/(zoom+z2));    glVertex2d(1,1);
        glTexCoord2f(txscale.x*(pos.x/txsize.x-z2)/(zoom+z2), txscale.y*(pos.y/txsize.y-z2)/(zoom+z2));    glVertex2d(-1,1);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}













