#ifndef BVH_H
#define BVH_H

#include "macro.h"

#include "ray.h"
#include "vec3.h"
#include "hittable.h"
#include "hittable_list.h"
#include <algorithm>

using namespace std;

class bvh_node : public hittable{
    public:
        bvh_node(){};
        bvh_node(const hittable_list& list, double time0, double time1)
            : bvh_node(list.objects, 0, list.objects.size(), time0, time1){};

        bvh_node(const vector<shared_ptr<hittable>>& src_objects, size_t start,
        size_t end, double time0 ,double time1);

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

    public:
        shared_ptr<hittable> left;
        shared_ptr<hittable> right;
        aabb box;
};

// construct the BVH tree

bvh_node::bvh_node(
    const vector<shared_ptr<hittable>>& src_objects, 
    size_t start, size_t end,double time0, double time1) {

        auto objects = src_objects;      // Create a modifiable array of the source scene objects

        int axis = random_int(0, 2);
        auto comparator = (axis == 0) ? box_x_comparator
                        : (axis == 1) ? box_y_comparator
                                      : box_z_comparator ; 

        size_t objects_span = end - start;

        if(objects_span == 1) {         // Only 1 object , insert it into left and right children
            left = right = objects[start];
        }else(objects_span == 2) {

        }
    }


bool bvh_node::bounding_box(double time0, double time1, aabb& output_box) const {
    output_box = box;
    return true;
}

bool bvh_node::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    if(!box.hit(r, t_min, t_max)) return false;

    bool hit_left = left -> hit(r, t_min, t_max, rec);
    bool hit_right = right -> hit(r, t_min, hit_left ? rec.t : t_max, rec);

    return hit_left || hit_right;
}

// Comparator function
inline bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis) {
    aabb box_a;
    aabb box_b;
    
}

#endif

