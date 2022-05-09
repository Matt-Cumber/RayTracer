#ifndef MONTECARLO_H
#define MONTECARLO_H

#include <random>

#include "vec3.h"

namespace MonteCarlo {

inline float RandomFloat() {
    static std::uniform_real_distribution<float> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline Vec3 RandomVec() {
    while (true) {
        Vec3 v(RandomFloat(), RandomFloat(), RandomFloat());
        if (v.LengthSquared() >= 1.0f) continue;
        return v;
    }
}

}

#endif