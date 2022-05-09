#ifndef SURFEL_H
#define SURFEL_H

#include <memory>
#include <cmath>
#include <algorithm>

#include "object.h"
#include "vec3.h"
#include "colour3.h"

class Surfel {
public:
    Surfel() {};
    float BRDF(Dir3 in, Dir3 out);
    Colour3 BRDF3(Dir3 out, Dir3 in);
    Colour3 Diffuse3(Dir3 in) {
        return std::max(Dot(Normal, in), 0.0f) * Diffuse;
    }
    Colour3 Specular3(Dir3 out, Dir3 in) {
        return std::pow(std::max(Dot((out+in)/2.0f, Normal), 0.0f), Exponent) * Specular;
    }

public:
    float At;
    Point3 Point;
    Dir3 Normal;
    Colour3 Emission;
    float AmbientAlbedo;
    float LambertAlbedo;
    float GlossyAlbedo;
    float Exponent;
    float Impulse;
    float ImpulseAlbedo;
    Colour3 Ambient;
    Colour3 Diffuse;
    Colour3 Specular;
};

float Surfel::BRDF(Dir3 out, Dir3 in) {
    float L = Dot(Normal, in);
    float G = std::pow(Dot(Normal, (out + in)/2.0f), Exponent);
    return (L * LambertAlbedo + G * GlossyAlbedo);
}

/*
 * Blinn-Phong reflecting model
 * Assumes vectors already normalised
 */
Colour3 Surfel::BRDF3(Dir3 out, Dir3 in) {
    float L = std::max(Dot(Normal, in), 0.0f);
    float G = 0.0f;

    // Blinn-Phong model
    if (L > 0.0f) {
        G = std::pow(std::max(Dot((out+in)/2.0f, Normal), 0.0f), Exponent);
        // G = std::max(std::pow(Dot((out+in)/2.0f, Normal), Exponent), 0.0f);
    }

    // float G = std::max(std::pow(Dot(Normal, (out + in)/2.0f), Exponent), 0.0f);
    return (L * Diffuse + G * Specular);
}



#endif