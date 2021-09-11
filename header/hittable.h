#ifndef HITTABLE_H
#define HITTABLE_H


#include "macro.h"
#include "vec3.h"
#include "ray.h"
#include "aabb.h"

using namespace std;

class material;


struct hit_record {
    point3 p;
    vec3 normal;
    shared_ptr<material> mat_ptr;
    // Hit timing
    double t;                       
    // Texture coordinate
    double u;                       
    double v;
    // Normal direction
    bool front_face;

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal :-outward_normal;
    }
};


class hittable {
    public:
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
        virtual bool bounding_box(double time0, double time1, aabb& output_box) const = 0;
};

class translate : public hittable {
    public:
        translate() {}
        translate(shared_ptr<hittable> p, const vec3& displacement) : hptr(p), offset(displacement) {}

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

    public:
        shared_ptr<hittable> hptr;
        vec3 offset;
};

bool translate::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    ray moved_r(r.origin() - offset, r.direction(), r.time());
    if(!hptr->hit(moved_r, t_min, t_max, rec)){
        return false;
    }

    rec.p += offset;
    rec.set_face_normal(moved_r, rec.normal);

    return true;
}

bool translate::bounding_box(double time0 ,double time1, aabb& output_box) const {
    if(!hptr->bounding_box(time0, time1, output_box)){
        return false;
    }

    output_box = aabb(output_box.get_min() + offset, output_box.get_max() + offset);

    return true;
}

class rotate_y : public hittable {

    public:
        rotate_y() {}
        rotate_y(shared_ptr<hittable> p, double angle);

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override{
            output_box = rbox;
            return hasbox;
        }

    public:
        shared_ptr<hittable> hptr;
        double sin_theta;
        double cos_theta;
        bool hasbox;
        aabb rbox;
};

rotate_y::rotate_y(shared_ptr<hittable> p, double angle) : hptr(p) {
    auto radians = degrees_to_radians(angle);
    sin_theta = sin(radians);
    cos_theta = cos(radians);
    hasbox = hptr->bounding_box(0, 1, rbox);

    point3 min(infinity, infinity, infinity);
    point3 max(-infinity, -infinity, -infinity);

    for(int i = 0; i < 2; ++i) {
        for(int j = 0; j < 2; ++j) {
            for(int k = 0; k < 2; ++k) {
                auto x = i * rbox.get_max().x() + (1 - i) * rbox.get_min().x();
                auto y = j * rbox.get_max().y() + (1 - j) * rbox.get_min().y();
                auto z = k * rbox.get_max().z() + (1 - k) * rbox.get_min().z();

                auto newx =  cos_theta * x + sin_theta * z;
                auto newz = -sin_theta * x + cos_theta * z;

                vec3 tester(newx, y, newz);

                for (int c = 0; c < 3; c++) {
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmax(max[c], tester[c]);
                }
            }
        }
    }
    rbox = aabb(min, max);
}

bool rotate_y::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    auto origin = r.origin();
    auto direction = r.direction();

    origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[2];
    origin[2] = sin_theta * r.origin()[0] + cos_theta * r.origin()[2];

    direction[0] = cos_theta * r.direction()[0] - sin_theta * r.direction()[2];
    direction[2] = sin_theta * r.direction()[0] + cos_theta * r.direction()[2];

    ray rotated_r(origin, direction, r.time());

    if(!hptr->hit(rotated_r, t_min, t_max, rec)){
        return false;
    }

    auto p = rec.p;
    auto normal = rec.normal;

    p[0] =  cos_theta * rec.p[0] + sin_theta * rec.p[2];
    p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2] ;

    normal[0] =  cos_theta*rec.normal[0] + sin_theta*rec.normal[2];
    normal[2] = -sin_theta*rec.normal[0] + cos_theta*rec.normal[2];

    rec.p = p;
    rec.set_face_normal(rotated_r, normal);

    return true;

}

#endif