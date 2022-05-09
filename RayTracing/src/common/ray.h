#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray {
public:
    Ray() : m_o(Point3()), m_d(Dir3()) {};
    Ray(const Point3 p, const Dir3 d) : m_o(p), m_d(d) {};

    Point3 Origin() const { return m_o; };
    Dir3 Direction() const { return m_d; };
    Dir3 UnitDirection() const { return Unit(m_d); };
    Point3 At(const float t) const { return m_o + t * m_d; };

private:
    Point3 m_o;
    Dir3 m_d;
};

#endif