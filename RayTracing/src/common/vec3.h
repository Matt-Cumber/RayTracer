#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

/*
 * Vec3
 * Stores x, y, z values of a vector
 */
class Vec3 {
public:
    Vec3() : m_x(0.0f), m_y(0.0f), m_z(0.0f) {};
    Vec3(const float x, const float y, const float z) : m_x(x), m_y(y), m_z(z) {};
    float x() const { return m_x; };
    float y() const { return m_y; };
    float z() const { return m_z; };

    // Negate values
    Vec3 operator-() const { return Vec3(-m_x, -m_y, -m_z); };

    // Summation to itself
    Vec3& operator+=(const Vec3 &v) {
        m_x += v.x();
        m_y += v.y();
        m_z += v.z();
        return *this;
    };

    // Multiplication to itself
    Vec3& operator*=(const float t) {
        m_x *= t;
        m_y *= t;
        m_z *= t;
        return *this;
    };

    // Division to itself (a special multiplication)
    Vec3& operator/=(const float t) { return *this *= 1.0f/t; };

    // Functions
    float Length() const { return std::sqrt(LengthSquared()); };
    float LengthSquared() const { return m_x*m_x + m_y*m_y + m_z*m_z; };

private:
    float m_x;
    float m_y;
    float m_z;
};

// Utilities
inline std::ostream& operator<<(std::ostream &out, const Vec3 &v) {
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
};

inline Vec3 operator*(float t, const Vec3 &v) { return Vec3(t*v.x(), t*v.y(), t*v.z()); };
inline Vec3 operator*(const Vec3 &v, float t) { return t * v; };
inline Vec3 operator/(Vec3 v, float t) { return (1.0f/t) * v; };

inline Vec3 operator+(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
};

inline Vec3 operator-(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
};

inline Vec3 operator*(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
};

inline float Dot(const Vec3 &u, const Vec3 &v) {
    return u.x() * v.x()
        + u.y() * v.y()
        + u.z() * v.z();
};

inline Vec3 Cross(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.y() * v.z() - u.z() * v.y(),
                u.z() * v.x() - u.x() * v.z(),
                u.x() * v.y() - u.y() * v.x());
};

inline Vec3 Unit(Vec3 v) { return v / v.Length(); };

// Type aliases for Vec3
using Point3 = Vec3; // 3D point
using Dir3 = Vec3; // 3D direction vector
using Tex2 = Vec3; // 2D Texture coordinate 

#endif