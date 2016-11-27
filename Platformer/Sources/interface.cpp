/*ELEMENTS D'UI*/

#include "interface.h"

//Jauge dynamique (barre de vie, munitions) ----------------------------------------------------------------------------------|
indic_bar::indic_bar()
{}

indic_bar::indic_bar(const vec2 &vpos, const vec2 &vsize, const int &vmax)
    :pos(vpos), size(vsize), max(vmax)
{}

indic_bar::~indic_bar()
{}

void indic_bar::display()
{
    if(max == 0) return;

    float uvl = val*size.x/max;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glColor3ub(100, 100, 100);
    glBegin(GL_QUADS);
        glVertex2f(pos.x, pos.y);
        glVertex2f(pos.x+size.x, pos.y);
        glVertex2f(pos.x+size.x, pos.y+size.y);
        glVertex2f(pos.x, pos.y+size.y);
    glEnd();

    glColor3ub(255-255*val/max, 255*val/max, 0);
    glBegin(GL_QUADS);
        glVertex2f(pos.x, pos.y);
        glVertex2f(pos.x+uvl, pos.y);
        glVertex2f(pos.x+uvl, pos.y+size.y);
        glVertex2f(pos.x, pos.y+size.y);
    glEnd();

    glPopMatrix();
}

void indic_bar::set_val(const int &a)
{
    val = a;
}

void indic_bar::set_max(const int &a)
{
    max = a;
}


 //Compteur numérique (score)----------------------------------------------------------------------------------|
indic_meter::indic_meter()
{}

indic_meter::indic_meter(const vec2 &vpos, const vec2 &vsize, const int &n)
    :pos(vpos), size(vsize), ndigits(n)
{}

indic_meter::~indic_meter()
{}

void indic_meter::load_font(const char* file, const unsigned int &file_w, const unsigned int &file_h, GLuint &txt_id)
{
    if(txt_id == 0)
        txt_id = png_texture_load(file, file_w, file_h);

    font = txt_id;
}

void indic_meter::display()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glBindTexture(GL_TEXTURE_2D, font);
    glEnable(GL_TEXTURE_2D);
    glColor3ub(255,255,255);

    for(int i= 0; i<ndigits; i++)
    {
        int p = pow(10, i), digit = (val%(10*p) - val%p)/p, xdigit = ndigits-1 - i;
        float uvx=digit*1.0/13, uvy=3*1.0/5;

        glBegin(GL_QUADS);
            glTexCoord2f(uvx, uvy+1.0/5); glVertex2f(pos.x+xdigit*size.x/ndigits, pos.y);
            glTexCoord2f(uvx+1.0/13, uvy+1.0/5); glVertex2f(pos.x+(xdigit+1)*size.x/ndigits, pos.y);
            glTexCoord2f(uvx+1.0/13, uvy); glVertex2f(pos.x+(xdigit+1)*size.x/ndigits, pos.y+size.y);
            glTexCoord2f(uvx, uvy); glVertex2f(pos.x+xdigit*size.x/ndigits, pos.y+size.y);
        glEnd();
    }

    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

void indic_meter::set_val(const int &a)
{
    val = a;
}

void indic_meter::set_pos(const vec2 &p)
{
    pos = p;
}


//Texte  (Game Over, etc)----------------------------------------------------------------------------------|
indic_text::indic_text()
{}

indic_text::indic_text(const vec2 &vpos, const vec2 &vsize, const std::string &text)
    :pos(vpos), size(vsize), txt(text)
{}

indic_text::~indic_text()
{}

void indic_text::load_font(const char* file, const unsigned int &file_w, const unsigned int &file_h, GLuint &txt_id)
{
    if(txt_id == 0)
        txt_id = png_texture_load(file, file_w, file_h);

    font = txt_id;
}

void indic_text::display()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glBindTexture(GL_TEXTURE_2D, font);
    glEnable(GL_TEXTURE_2D);
    glColor3ub(255,255,255);

    for(unsigned int i= 0, imax = txt.length(); i<imax; i++)
    {
        float uvx = 0, uvy = 4.0;

        if(txt[i] >= 'A' && txt[i] <= 'Z')
        {
            uvx = (txt[i] - 'A');
            uvx /= 13;
            if(txt[i] > 'M')
                uvy = 1.0/5;
            else
                uvy = 0;

            glBegin(GL_QUADS);
                glTexCoord2f(uvx, uvy+1.0/5); glVertex2f(pos.x+i*size.x/imax, pos.y);
                glTexCoord2f(uvx+1.0/13, uvy+1.0/5); glVertex2f(pos.x+(i+1)*size.x/imax, pos.y);
                glTexCoord2f(uvx+1.0/13, uvy); glVertex2f(pos.x+(i+1)*size.x/imax, pos.y+size.y);
                glTexCoord2f(uvx, uvy); glVertex2f(pos.x+i*size.x/imax, pos.y+size.y);
            glEnd();
        }
        else if (txt[i] >= 'a' && txt[i] <= 'z')
        {
            uvx = (txt[i] - 'a');
            uvx /= 26;
            uvy = 2.0/5;

            glBegin(GL_QUADS);
                glTexCoord2f(uvx, uvy+1.0/5); glVertex2f(pos.x+i*size.x/imax, pos.y);
                glTexCoord2f(uvx+1.0/26, uvy+1.0/5); glVertex2f(pos.x+(i+1)*size.x/imax, pos.y);
                glTexCoord2f(uvx+1.0/26, uvy); glVertex2f(pos.x+(i+1)*size.x/imax, pos.y+size.y);
                glTexCoord2f(uvx, uvy); glVertex2f(pos.x+i*size.x/imax, pos.y+size.y);
            glEnd();
        }
    }

    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

void indic_text::set_pos(const vec2 &p)
{
    pos = p;
}
