#ifndef LIGHT_H
#define LIGHT_H

#include "vec3.h"
#include "colour3.h"

class Light {
public:
    Light(Point3 p, Colour3 c) : m_p(p), m_c(c) {};
    Point3 Position() const { return m_p; };
    Colour3 Colour() const { return m_c; };

public:
    Colour3 Ambient = Colour3(0.2f, 0.2f, 0.2f);
    Colour3 Diffuse = Colour3(0.5f, 0.5f, 0.5f);
    Colour3 Specular = Colour3(1.0f, 1.0f, 1.0f);
    bool IsAtInfinity = false;
    float Power = 200.0f;

private:
    Point3 m_p;
    Colour3 m_c;
};

/*
    Description of light ambient, diffuse and specular
    
    Ambient:
        The ambient light is usually set to a low intensity because we don't want the ambient color to be too dominant

    Diffuse:
        The diffuse component of a light source is usually set to the exact color we'd like a light to have. 
        Often a bright white color

    Specular:
        The specular component is usually kept at (1.0, 1.0, 1.0) shining at full intensity
*/

#endif