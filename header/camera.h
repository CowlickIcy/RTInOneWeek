#ifndef CAMERA_H
#define CAMERA_H

#include "macro.h"
#include "vec3.h"
#include "ray.h"

class camera {

    public:
        camera(
            vec3 lookfrom,
            vec3 lookat,
            vec3 vup,
            double fov,
            double aspect
        ){
            origin = lookfrom;
            vec3 u,v,w;
            // fov && aspect
            auto theta = degrees_to_radians(fov);
            auto half_height = tan(theta / 2);
            auto half_width = aspect * half_height;

            // u, v, w
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            // horizontal && vertical
            lower_left_corner = origin - half_height * v - half_width * u - w;
            horizontal = 2 * half_width * u;
            vertical = 2 * half_height * v;

        }
    ray get_ray(double s, double t) {
        return ray(origin, lower_left_corner + s * horizontal + t * vertical - origin);
    }

    public:
        vec3 origin;
        vec3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
};

#endif