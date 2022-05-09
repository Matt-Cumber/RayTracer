#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <algorithm>
#include <vector>

#include "camera.h"
#include "colour3.h"
#include "image.h"
#include "light.h"
#include "montecarlo.h"
#include "object.h"
#include "objectlist.h"
#include "vec3.h"

const float infinity = std::numeric_limits<float>::infinity();

class RayTracer {
public:
    RayTracer(ObjectList objl, Camera cam, Image img) : m_objl(objl), m_cam(cam), m_img(img) {};
    int Exec();

private:
    Colour3 TraceRay(Ray r, float min, float max, int depth);
    Colour3 DirectLighting(Surfel s, Dir3 out, Light l);
    Colour3 IndirectLighting(Surfel s, Dir3 out, int curDepth);

private:
    ObjectList m_objl;
    Camera m_cam;
    Image m_img;
    std::vector<Light> m_lights;
};

Colour3 RayTracer::DirectLighting(Surfel s, Dir3 out, Light l) {
    // Find incoming light direction
    Dir3 in = l.Position() - s.Point;
    float distance = in.Length();
    distance = distance * distance;

    // Shadow ray
    if (m_objl.DoesRayIntersectSurface(Ray(s.Point+0.0001*s.Normal, in), 0, Unit(in).Length(), s))
        return Colour3(0.0f, 0.0f, 0.0f);

    // Now can normalise the vectors
    s.Normal = Unit(s.Normal);
    in = Unit(in);
    out = Unit(out);

    if (l.IsAtInfinity)
        float distance = 1.0f;

    Colour3 diffuse = s.Diffuse3(in);
    Colour3 specular = s.Specular3(out, in);
    return (l.Diffuse * diffuse + l.Specular * specular);
}

Colour3 RayTracer::IndirectLighting(Surfel s, Dir3 out, int curDepth) {
    // return l.Colour() * s.Ambient;
    // return l.Ambient * s.Ambient;
    Dir3 in;
    float albedo = 0.0f;

    // Decide in direction by impulse refelction
    if (MonteCarlo::RandomFloat() < s.Impulse) {
        // impulse reflection
        in = 2.0f * s.Normal - out;
        albedo = s.ImpulseAlbedo;
    } else {
        // Use monte carlo to get random vec
        in = MonteCarlo::RandomVec();
        albedo = s.BRDF(Unit(out), Unit(in));
    }

    // Find incoming light
    Colour3 inLight = TraceRay(Ray(s.Point, in), 0, infinity, curDepth-1);

    // Apply brdf
    return inLight * albedo;
}

Colour3 RayTracer::TraceRay(Ray r, float min, float max, int depth) {

    // Start with no light
    Colour3 totalRadiance(0.0f, 0.0f, 0.0f);

    if (depth <= 0)
        return totalRadiance;

    // Find intersection point from ray into the world
    Surfel s;
    if (m_objl.DoesRayIntersectSurface(r, min, max, s)) {
        // The ray intersects a surface
        
        // Add the emissive colour of the surface
        totalRadiance += s.Emission;

        // Loop each light source
        for (unsigned int i = 0; i < m_lights.size(); ++i) {
            // Gather Direct light
            totalRadiance += DirectLighting(s, -r.Direction(), m_lights[i]);
        }

        totalRadiance += IndirectLighting(s, -r.Direction(), depth);
    }

    // Return collected light
    return totalRadiance;
}

int RayTracer::Exec() {
    // Init with one light only
    Light light(Point3(0.0f, 0.95f, 0.0f), Colour3(1.0f, 1.0f, 1.0f));
    light.Diffuse = light.Colour() * 0.5f;
    light.Ambient = light.Diffuse * 0.2f;
    m_lights.emplace_back(light);

    // Write header of image file
    std::cout << "P3\n" << m_img.Width() << ' ' << m_img.Height() << "\n255\n";

    // Store the colours in an array
    std::vector<Colour3> buffer;

    // Start with no colour
    for (int j = m_img.Height()-1; j >= 0; --j) {
        for (int i = 0; i < m_img.Width(); ++i) {
            Colour3 c(0.0f, 0.0f, 0.0f);
            buffer.emplace_back(c);
        }
    }

    // For each pixel, trace a ray from the camera position
    for (int j = m_img.Height()-1; j >= 0; --j) {
        // std::cerr << j << " lines left" << std::endl;
        for (int i = 0; i < m_img.Width(); ++i) {
            // Somewhere to store collected colour
            float r = 0.0f;
            float g = 0.0f;
            float b = 0.0f;

            // Collect samples
            for (int n = 0; n < m_img.NumberOfSamples(); ++n) {
                float u = float(i + MonteCarlo::RandomFloat()) / (m_img.Width()-1);
                float v = float(j + MonteCarlo::RandomFloat()) / (m_img.Height()-1);
                Colour3 c = TraceRay(m_cam.CameraRay(u, v), -infinity, infinity, 10);
                r += c.r();
                g += c.g();
                b += c.b();
            }
            // Now average out sample collection to get the final colour
            float scale = 1.0f / m_img.NumberOfSamples();
            r *= scale;
            g *= scale;
            b *= scale;

            // Set this colour for the pixel
            buffer[(m_img.Height()-j-1)*m_img.Width()+i] = Colour3(r, g, b);
        }
    }

    // Write colour
    for (unsigned int i = 0; i < buffer.size(); ++i) {
        buffer[i].WriteColor(std::cout);
    }
    return 0;
}

#endif