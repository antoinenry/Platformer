/* CLASSE "VEC2" ----------------------------------------------------------------------------------------*/
/*Créé par CPE Lyon, légèrement modifiée par moi*/

#include "vec2.h"
#include <cmath>
#include <cassert>
#include <iostream>


vec2::vec2()
    :x(0.0f),y(0.0f)
{}

vec2::vec2(float x_param,float y_param)
    :x(x_param),y(y_param)
{}

vec2& vec2::operator+=(const vec2& v)
{
    x+=v.x;
    y+=v.y;
    return *this;
}

vec2& vec2::operator-=(const vec2& v)
{
    x-=v.x;
    y-=v.y;
    return *this;
}

vec2& vec2::operator*=(float s)
{
    x*=s; y*=s;
    return *this;
}

vec2& vec2::operator/=(float s)
{
    assert(fabs(s)>10e-6);
    x/=s; y/=s;
    return *this;
}

void print_screen(const vec2& v)
{
    std::cout<<v.x<<" "<<v.y<<std::endl;
}

float norm(const vec2& v)
{
    return std::sqrt(v.x*v.x+v.y*v.y);
}

float arg(const vec2& v)
{
    vec2 vn(v);
    vn = normalize(vn);

    if (vn.y>=0) return acos(vn.x);
    else return 2*M_PI - acos(vn.x);
}

float angle(const vec2& v0, const vec2& v1)
{
    return arg(v0) - arg(v1);
}

float min_abs_angle(const vec2& v0, const vec2& v1)
{
    float a = fabs(angle(v0, v1));
    if (a>=M_PI)
        a -= M_PI;

    return a;
}

float dot(const vec2& v0,const vec2& v1)
{
    return v0.x*v1.x+v0.y*v1.y;
}

float determinant(const vec2& v0,const vec2& v1)
{
    return v0.x*v1.y-v0.y*v1.x;
}

vec2 projected(const vec2& v0,const vec2& v1)
{
    vec2 v=v1;
    v=normalize(v);

    return dot(v0, v1)*v;
}

vec2 normalize(vec2& v)
{
    vec2 temp=v;
    float n=norm(v);
    if(fabs(n)>10e-6) temp/=norm(v);
    else temp=vec2(0,0);
    return temp;
}

vec2 rotate(const vec2& v, float angle)
{
    vec2 output;
    float n = norm(v), a = arg(v);
    a+=angle;
    while(a>=2*M_PI) a-=2*M_PI;
    output.x = n*cos(a);    output.y = n*sin(a);
    return output;
}

vec2 q_rotate(const vec2& v, bool s)
{
    vec2 output;
    if(s)
    {
        output.x = -(v.y);
        output.y = v.x;
    }
    else
    {
        output.x = v.y;
        output.y = -(v.x);
    }
    return output;
}

vec2 operator+(const vec2& v0,const vec2& v1)
{
    vec2 temp=v0;
    temp+=v1;
    return temp;
}

vec2 operator-(const vec2& v0,const vec2& v1)
{
    vec2 temp=v0;
    temp-=v1;
    return temp;
}

vec2 operator*(const vec2& v0,float s)
{
    vec2 temp=v0;
    temp*=s;
    return temp;
}

vec2 operator*(float s,const vec2& v0)
{
    return v0*s;
}

vec2 operator/(const vec2& v0,float s)
{
    vec2 temp=v0;temp/=s;return temp;
}

bool vec2::operator==(const vec2& right) const
{
    return (x == right.x && y == right.y);
}

bool vec2::operator!=(const vec2& right) const
{
    return (x != right.x || y != right.y);
}

bool vec2::operator<(const vec2& right) const
{
    return (x*x+y*y < right.x*right.x + right.y*right.y);
}

bool vec2::operator>(const vec2& right) const
{
    return (x*x+y*y > right.x*right.x + right.y*right.y);
}

bool vec2::isint()
{
    return (int)x==x && (int)y==y;
}

bool vec2::CompRel(const vec2& right, const float epsilon) const
{
    return fabs(x-right.x)<epsilon && fabs(y-right.y)<epsilon;
}

