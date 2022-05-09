#ifndef OBJECTLIST_H
#define OBJECTLIST_H

#include <memory>
#include <vector>

#include "object.h"

class ObjectList {
    public:
        ObjectList() {};
        ObjectList(std::shared_ptr<Object> object) { add(object); }

        void clear() { objects.clear(); }
        void add(std::shared_ptr<Object> object) { objects.emplace_back(object); }

        bool DoesRayIntersectSurface(const Ray& r, float min, float max, Surfel& s) const;

    public:
        std::vector<std::shared_ptr<Object>> objects;
};

bool ObjectList::DoesRayIntersectSurface(const Ray& r, float min, float max, Surfel& s) const {
    Surfel tempS;
    bool doesIntersect = false;
    float closest = max;

    for (const auto& object : objects) {
        if (object->Intersects(r, min, closest, tempS)) {
            doesIntersect = true;
            closest = tempS.At;
            s = tempS;
        }
    }
    return doesIntersect;
}

#endif