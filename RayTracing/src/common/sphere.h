#ifndef SPHERE_H
#define SPHERE_H

#include <cmath>

#include "object.h"
#include "vec3.h"

class Sphere : public Object {
public:
    Sphere() : m_c(Point3()), m_r(1.0f) {};
    Sphere(const Point3 c, const float r) : m_c(c), m_r(r) {};

    virtual bool Intersects(const Ray& r, float min, float max, Surfel& s) override;
    // virtual void TransferMaterialProperties(Surfel& s) const override;

    Point3 Centre() const { return m_c; };
    float Radius() const { return m_r; };

private:
    Point3 m_c;
    float m_r;
};

bool Sphere::Intersects(const Ray& r, float min, float max, Surfel& s) {
    Vec3 oc = r.Origin() - m_c;
    float a = r.Direction().LengthSquared();
    float half_b = Dot(oc, r.Direction());
    float c = oc.LengthSquared() - m_r*m_r;

    float discriminant = half_b*half_b - a*c;
    if (discriminant < 0.0f) return false;
    float sqrtd = std::sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    float root = (-half_b - sqrtd) / a;
    if (root < min || max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < min || max < root)
            return false;
    }

    s.At = root;
    s.Point = r.At(s.At);
    s.Normal = (s.Point - m_c) / m_r;

    // Transfer material properties
    TransferMaterialProperties(s);

    return true;
}

#endif