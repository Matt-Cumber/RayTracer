#ifndef OBJECT_H
#define OBJECT_H

#include "ray.h"
#include "surfel.h"
#include "colour3.h"

class Object {
public:
    virtual bool Intersects(const Ray& r, float min, float max, Surfel& s) = 0;
    void TransferMaterialProperties(Surfel& s) const {
        s.Emission = Emission;
        s.AmbientAlbedo = AmbientAlbedo;
        s.LambertAlbedo = LambertAlbedo;
        s.GlossyAlbedo = GlossyAlbedo;
        s.Exponent = Exponent;
        s.Impulse = Impulse;
        s.ImpulseAlbedo = ImpulseAlbedo;

        s.Ambient = Ambient;
        s.Diffuse = Diffuse;
        s.Specular = Specular;
    }
    
public:
    Colour3 Emission = Colour3(0.0f, 0.0f, 0.0f);
    float AmbientAlbedo = 0.7f;
    float LambertAlbedo = 0.7;
    float GlossyAlbedo = 0.7;
    float Exponent = 20.0f;
    float Impulse = 0.1;
    float ImpulseAlbedo = 1.0f;

    Colour3 Ambient = Colour3(0.2f, 0.2f, 0.2f);
    Colour3 Diffuse = Colour3(0.7f, 0.7f, 0.7f);
    Colour3 Specular = Colour3(1.0f, 1.0f, 1.0f);
};

/*
    Lighting and material explanation
    
    Ambient:
        The ambient material vector defines what color the surface reflects under ambient lighting. 
        This is usually the same as the surface's color
   
    Diffuse:
        The diffuse material vector defines the color of the surface under diffuse lighting. 
        The diffuse color is (just like ambient lighting) set to the desired surface's color.

    Specular:
        The specular material vector sets the color of the specular highlight on the surface 
        (or possibly even reflect a surface-specific color).

    Exponent:
        The exponent impacts the scattering/radius of the specular highlight.

    Example Materials:
        Emerald 0.0215	0.1745	0.0215	0.07568	0.61424	0.07568	0.633	0.727811	0.633	0.6
*/

#endif