
#pragma once

#ifndef VEC2_HPP
#define VEC2_HPP



struct vec2
{
    float x;
    float y;

    vec2();
    vec2(float x_param,float y_param);

    vec2& operator+=(const vec2& v);
    vec2& operator-=(const vec2& v);
    vec2& operator*=(float s);
    vec2& operator/=(float s);
    bool operator==(const vec2& right) const;
    bool operator!=(const vec2& right) const;
    bool operator<(const vec2& right) const;
    bool operator>(const vec2& right) const;

    bool isint();
    bool CompRel(const vec2& right, const float epsilon) const;
};
void print_screen(const vec2& v);
float norm(const vec2& v);
float arg(const vec2& v);
float dot(const vec2& v0,const vec2& v1);
float determinant(const vec2& v0,const vec2& v1);
vec2 projected(const vec2& v0,const vec2& v1);
float angle(const vec2& v0, const vec2& v1);
float min_abs_angle(const vec2& v0, const vec2& v1);
vec2 normalize(vec2& v);
vec2 rotate(const vec2& v, float angle);
vec2 q_rotate(const vec2& v, bool s);
vec2 operator+(const vec2& v0,const vec2& v1);
vec2 operator-(const vec2& v0,const vec2& v1);
vec2 operator*(const vec2& v0,float s);
vec2 operator*(float s,const vec2& v0);
vec2 operator/(const vec2& v0,float s);




#endif
