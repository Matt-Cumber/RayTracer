#ifndef PLANE_H
#define PLANE_H

#include <vector>

#include "object.h"
#include "vec3.h"
#include "simpletriangle.h"

class Plane : public Object {
public:
    Plane() {};
    Plane(const Point3 ll, const Point3 lu, const Point3 rl, const Point3 ru, const Dir3 n) {
                // Construct two triangles for the plane
                m_tri.emplace_back(SimpleTriangle(ll, ru, lu, n));
                m_tri.emplace_back(SimpleTriangle(ll, rl, ru, n));
            };
    virtual bool Intersects(const Ray& r, float min, float max, Surfel& s) override;

private:
    std::vector<SimpleTriangle> m_tri;
};

bool Plane::Intersects(const Ray& r, float min, float max, Surfel& s) {
    // Test for each triangle
    bool doesIntersect = false;
    for (unsigned int i = 0; i < m_tri.size(); ++i) {
        doesIntersect = m_tri[i].Intersects(r, min, max, s);
        if (doesIntersect) {
            TransferMaterialProperties(s);
            return true;
        }
    }
    return false;
}

#endif