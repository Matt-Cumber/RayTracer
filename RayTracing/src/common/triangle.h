#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <array>

#include "object.h"
#include "vec3.h"
#include "ray.h"

/*
 * Triangle class
 * Holds information about a triangle
 * A triangle has 3 vertices, 3 normals, and 3 texture coordinates
 */
class Triangle : public Object {
public:
    Triangle(std::array<Point3, 3> &p, std::array<Dir3, 3> &n, std::array<Tex2, 3> &tc) {
        m_p[0] = p[0];
        m_p[1] = p[1];
        m_p[2] = p[2];

        m_n[0] = n[0];
        m_n[1] = n[1];
        m_n[2] = n[2];

        m_tc[0] = tc[0];
        m_tc[1] = tc[1];
        m_tc[2] = tc[2];

        // Compute surface normal as points will not change
        Vec3 edgeA = m_p[1] - m_p[0];
        Vec3 edgeB = m_p[2] - m_p[0];
        m_surfaceNormal = Unit(Cross(edgeA, edgeB));

        // For small computational optimisation, compute the first part of barycentric coordiantes once
        // Rest is dependant on point position
        b0 = m_p[1] - m_p[0];
        b1 = m_p[2] - m_p[0];
        d00 = Dot(b0, b0);
        d01 = Dot(b0, b1);
        d11 = Dot(b1, b1);
    }

    virtual bool Intersects(const Ray& r, float min, float max, Surfel& s) override;


    /*
     * Compute Alpha, Beta, Gamma for Barycentric interpolation
     */
    void ComputeABG(Point3 p) {
        Vec3 b2 = p - m_p[0];
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
        if (Dot(Cross((m_p[1] - m_p[0]), (p - m_p[0])), m_surfaceNormal) < 0) return false;
        if (Dot(Cross((m_p[2] - m_p[1]), (p - m_p[1])), m_surfaceNormal) < 0) return false;
        if (Dot(Cross((m_p[0] - m_p[2]), (p - m_p[2])), m_surfaceNormal) < 0) return false;
        return true;
    }

    /*
     * Test for intersection between given ray and this triangle
     * Converts triangle into a plane, intersects ray with plane, then test if intersection is inside triangle
     */
    // bool Intersects(Ray r, const float mint, const float maxt) {
    //     // First check if ray and triangle are parallel to avoid division by zero problems
    //     if (Dot(m_surfaceNormal, r.Direction()) == 0) return false;

    //     // Each triangle vertex is on the plane so just pick one to compute d of the plane
    //     float d = Dot(m_surfaceNormal, m_p[0]);

    //     // Find where on ray intersection with plane lies
    //     float t = (d - Dot(m_surfaceNormal, r.Origin())) / (Dot(m_surfaceNormal, r.Direction()));

    //     // Check t is within boounds
    //     if (t > maxt || t < mint) return false;

    //     // Call point Q the point along the ray which intersects the plane from the triangle
    //     Point3 Q = r.At(t);

    //     // Does point Q lie inside the triangle
    //     if (!Inside(Q)) return false;

    //     return true;
    // }

    std::array<Point3, 3> GetPoints() const { return m_p; };
    std::array<Dir3, 3> GetNormals() const { return m_n; };
    std::array<Tex2, 3> GetTexCoords() const { return m_tc; };
    float Alpha() const { return m_alpha; };
    float Beta() const { return m_beta; };
    float Gamma() const { return m_gamma; };

private:
    std::array<Point3, 3> m_p;
    std::array<Dir3, 3> m_n;
    std::array<Tex2, 3> m_tc;
    Dir3 m_surfaceNormal;

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

bool Triangle::Intersects(const Ray& r, float min, float max, Surfel& s) {
    // First check if ray and triangle are parallel to avoid division by zero problems
    if (Dot(m_surfaceNormal, r.Direction()) == 0) return false;

    // Each triangle vertex is on the plane so just pick one to compute d of the plane
    float d = Dot(m_surfaceNormal, m_p[0]);

    // Find where on ray intersection with plane lies
    float t = (d - Dot(m_surfaceNormal, r.Origin())) / (Dot(m_surfaceNormal, r.Direction()));

    // Check t is within boounds
    if (t > max || t < min) return false;

    // Call point Q the point along the ray which intersects the plane from the triangle
    Point3 Q = r.At(t);

    // Does point Q lie inside the triangle
    if (!Inside(Q)) return false;

    ComputeABG(Q);

    s.At = t;
    s.Point = Q;
    s.Normal = m_alpha*m_n[0] + m_beta*m_n[1] + m_gamma*m_n[2];

    // Transfer material properties
    TransferMaterialProperties(s);

    return true;
}


inline std::ostream& operator<<(std::ostream &out, const Triangle &t) {
    out << "Pos: " << t.GetPoints()[0].x() << " " << t.GetPoints()[0].y() << " " << t.GetPoints()[0].z();
    out << ", " << t.GetPoints()[1].x() << " " << t.GetPoints()[1].y() << " " << t.GetPoints()[1].z();
    out << ", " << t.GetPoints()[2].x() << " " << t.GetPoints()[2].y() << " " << t.GetPoints()[2].z() << std::endl;
    out << "Normals: " << t.GetNormals()[0].x() << " " << t.GetNormals()[0].y() << " " << t.GetNormals()[0].z();
    out << ", " << t.GetNormals()[1].x() << " " << t.GetNormals()[1].y() << " " << t.GetNormals()[1].z();
    out << ", " << t.GetNormals()[2].x() << " " << t.GetNormals()[2].y() << " " << t.GetNormals()[2].z() << std::endl;
    out << "TexCoords: " << t.GetTexCoords()[0].x() << " " << t.GetTexCoords()[0].y();
    out << ", " << t.GetTexCoords()[1].x() << " " << t.GetTexCoords()[1].y();
    out << ", " << t.GetTexCoords()[2].x() << " " << t.GetTexCoords()[2].y();
    return out;
};

#endif