#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"
#include "vec3.h"

class Camera {
public:
    Camera() {
        // Camera
        const float aspect_ratio = 16.0f / 9.0f;
        m_p = Point3(0.0f, 0.0f, -5.0f);
        float focalLength = 1.0f;
        // Viewport
        float viewPortHeight = 3.0f;
        float viewPortWidth =  aspect_ratio * viewPortHeight;
        m_h = Dir3(viewPortWidth, 0, 0);
        m_v = Dir3(0, viewPortHeight, 0);
        m_lc = m_p - m_h/2 - m_v/2 - Point3(0, 0, m_p.z() - focalLength);
    }

    Ray CameraRay(const float u, const float v) { return Ray(m_p, m_lc + u*m_h + v*m_v - m_p); };
private:
    Point3 m_p;
    Dir3 m_h;
    Dir3 m_v;
    Point3 m_lc;
};

#endif