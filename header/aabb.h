#ifndef AABB_H
#define AABB_H

#include "macro.h"
#include "ray.h"
#include "vec3.h"
#include <cmath>

using namespace std;

class aabb {

    public:
        aabb() {};
        aabb( const point3& a, const point3& b) : min(a) , max(b) {};

        point3 get_min() const {return min;}
        point3 get_max() const {return max;}


        bool hit(const ray& r, double t_min, double t_max) const {
            for (int a = 0; a < 3; a++) {
                auto t0 = fmin((min[a] - r.origin()[a]) / r.direction()[a],
                               (max[a] - r.origin()[a]) / r.direction()[a]);
                auto t1 = fmax((min[a] - r.origin()[a]) / r.direction()[a],
                               (max[a] - r.origin()[a]) / r.direction()[a]);
                t_min = fmax(t0, t_min);
                t_max = fmin(t1, t_max);
                if (t_max <= t_min)
                    return false;
            }
            return true;
        }

    public:
        point3 min;
        point3 max;

};

aabb surrounding_box(aabb box0, aabb box1) {
    vec3 small(fmin(box0.get_min().x(), box1.get_min().x()),
               fmin(box0.get_min().y(), box1.get_min().y()),
               fmin(box0.get_min().z(), box1.get_min().z()));

    vec3 big  (fmax(box0.get_max().x(), box1.get_max().x()),
               fmax(box0.get_max().y(), box1.get_max().y()),
               fmax(box0.get_max().z(), box1.get_max().z()));

    return aabb(small,big);
}

#endif