#ifndef SIMPLETRIANGLE_H
#define SIMPLETRIANGLE_H

#include "object.h"
#include "vec3.h"

class SimpleTriangle : public Object {
public:
    SimpleTriangle() : m_v0(Point3()), m_v1(Point3()), m_v2(Point3()), m_n(Dir3()) {};
    SimpleTriangle(Point3 v0, Point3 v1, Point3 v2) : m_v0(v0), m_v1(v1), m_v2(v2) {
        // Compute normal
        m_n = Unit(Cross(m_v1-m_v0, m_v2-m_v0));

        // For small benefit compute part of bary coords
        b0 = m_v1 - m_v0;
        b1 = m_v2 - m_v0;
        d00 = Dot(b0, b0);
        d01 = Dot(b0, b1);
        d11 = Dot(b1, b1);
    };

    SimpleTriangle(Point3 v0, Point3 v1, Point3 v2, Dir3 n) : m_v0(v0), m_v1(v1), m_v2(v2), m_n(n) {
        // For small benefit compute part of bary coords
        b0 = m_v1 - m_v0;
        b1 = m_v2 - m_v0;
        d00 = Dot(b0, b0);
        d01 = Dot(b0, b1);
        d11 = Dot(b1, b1);
    }

    virtual bool Intersects(const Ray& r, float min, float max, Surfel& s) override;

    /*
     * Compute Alpha, Beta, Gamma for Barycentric interpolation
     */
    void ComputeABG(Point3 p) {
        Vec3 b2 = p - m_v0;
        float d20 = Dot(b2, b0);
        float d21 = Dot(b2, b1);
        float denom = d00 * d11 - d01 * d01;
        m_beta = (d11 * d20 - d01 * d21) / denom;
        m_gamma = (d00 * d21 - d01 * d20) / denom;
        m_alpha = 1.0f - m_gamma - m_beta;
    }

    /*
     * Return true if p is inside this triangle
     */
    bool Inside(Point3 p) const {
        Dir3 n = Unit(Cross(m_v1-m_v0, m_v2-m_v0));
        if (Dot(Cross((m_v1 - m_v0), (p - m_v0)), n) < 0) return false;
        if (Dot(Cross((m_v2 - m_v1), (p - m_v1)), n) < 0) return false;
        if (Dot(Cross((m_v0 - m_v2), (p - m_v2)), n) < 0) return false;
        return true;
    }

private:
    // Defining triangle
    Point3 m_v0;
    Point3 m_v1;
    Point3 m_v2;
    Dir3 m_n;

    // For Barycentric coords calculation
    Vec3 b0;
    Vec3 b1;
    float d00;
    float d01;
    float d11;

    // For Barycentric coords
    float m_alpha;
    float m_beta;
    float m_gamma;
};

bool SimpleTriangle::Intersects(const Ray& r, float min, float max, Surfel& s) {

    // First check if ray and triangle are parallel to avoid division by zero problems
    if (Dot(m_n, r.Direction()) == 0) return false;

    // std::cerr << "not parallel" << std::endl;

    // Each triangle vertex is on the plane so just pick one to compute d of the plane
    float d = Dot(m_n, m_v0);

    // Find where on ray intersection with plane lies
    float t = (d - Dot(m_n, r.Origin())) / (Dot(m_n, r.Direction()));

    // Check t is within boounds
    if (t > max || t < min) return false;

    // std::cerr << "t fine" << std::endl;

    // Call point Q the point along the ray which intersects the plane from the triangle
    Point3 Q = r.At(t);

    // Does point Q lie inside the triangle
    if (!Inside(Q)) return false;

    // std::cerr << "Intersects" << std::endl;
    // std::cerr << t << std::endl;
    // std::cerr << Q << std::endl;
    // std::cerr << m_n << std::endl;

    ComputeABG(Q);

    s.At = t;
    s.Point = Q;
    // s.Normal = m_alpha*m_n + m_beta*m_n + m_gamma*m_n;
    s.Normal = m_n;

    // Transfer material properties
    TransferMaterialProperties(s);

    return true;
}

#endif