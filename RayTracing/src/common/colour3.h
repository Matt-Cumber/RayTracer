#ifndef COLOUR3_H
#define COLOUR3_H

#include <iostream>

/*
 * Colour Class
 * Stores r, g, b values of a colour
 */
class Colour3 {
public:
    Colour3() : m_r(0.0f), m_g(0.0f), m_b(0.0f) {};
    Colour3(float r, float g, float b) {
        m_r = Clamp(r);
        m_g = Clamp(g);
        m_b = Clamp(b);
    }

    float r() const { return m_r; };
    float g() const { return m_g; };
    float b() const { return m_b; };

    // Write the translated [0,255] value of each color component.
    void WriteColor(std::ostream &out) {
        out << static_cast<int>(255.999 * m_r) << ' '
            << static_cast<int>(255.999 * m_g) << ' '
            << static_cast<int>(255.999 * m_b) << '\n';
    };

    // Summation to itself
    Colour3& operator+=(const Colour3 &c) {
        m_r = Clamp(m_r + c.r());
        m_g = Clamp(m_g + c.g());
        m_b = Clamp(m_b + c.b());
        return *this;
    };

    // Multiplication to itself
    Colour3& operator*=(const float t) {
        m_r = Clamp(m_r * t);
        m_g = Clamp(m_g * t);
        m_b = Clamp(m_b * t);
        return *this;
    };

private:
    float Clamp(float c) {
        if (c < 0.0f) c = 0.0f;
        if (c > 1.0f) c = 1.0f;
        return c;
    };
    float m_r;
    float m_g;
    float m_b;
};

// Utilities
inline std::ostream& operator<<(std::ostream &out, const Colour3 &v) {
    return out << v.r() << ' ' << v.g() << ' ' << v.b();
};

inline Colour3 operator*(float t, const Colour3 &c) { return Colour3(t*c.r(), t*c.g(), t*c.b()); };
inline Colour3 operator*(const Colour3 &c, float t) { return t * c; };
inline Colour3 operator/(Colour3 c, float t) { return (1.0f/t) * c; };

inline Colour3 operator+(const Colour3 &u, const Colour3 &v) {
    return Colour3(u.r() + v.r(), u.g() + v.g(), u.b() + v.b());
};

inline Colour3 operator-(const Colour3 &u, const Colour3 &v) {
    return Colour3(u.r() - v.r(), u.g() - v.g(), u.b() - v.b());
};

// inline Colour3 operator*(const Colour3 &u, const Colour3 &v) {
//     return Colour3((u.r() * v.r()) / 255.0f, (u.g() * v.g()) / 255.0f, (u.b() * v.b()) / 255.0f);
// };

inline Colour3 operator*(const Colour3 &u, const Colour3 &v) {
    return Colour3((u.r() * v.r()), (u.g() * v.g()), (u.b() * v.b()));
};

#endif