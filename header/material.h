#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class material {
    public:
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered
        ) const = 0;
};
 
// lambert diffuse material

class lambertian : public material{
    public:
        lambertian(const vec3& a) : albedo(a) {};

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered
        ) const override {
            vec3 scatter_direction = rec.normal + random_unity_vector();
            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        } 

    public:
        vec3 albedo;
};

class metal : public material{
    public:
        metal(const vec3& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {};

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered
    ) const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_shpere());
        attenuation = albedo;
        return dot(scattered.direction(), rec.normal) > 0;

    }
    public:
        vec3 albedo;
        double fuzz;
};

class dielectric : public material {
    public:

        dielectric(double refi) : ref_idx(refi) {};

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered
        ) const override {
            attenuation = vec3(1.0, 1.0, 1.0);
            double etai_over_etat = rec.front_face ? (1.0 / ref_idx) : ref_idx;

            vec3 unit_direction = unit_vector(r_in.direction());

            double cos_theta = ffmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);
            if(etai_over_etat * sin_theta > 1.0) {
                vec3 reflected = reflect(unit_direction, rec.normal);              // theta_prime > 1.0 reflection
                scattered = ray(rec.p , reflected);
                return true;
            }
            
            double refract_prob = schlick(cos_theta, etai_over_etat);
            if(random_double() < refract_prob){
                vec3 reflected = reflect(unit_direction, rec.normal);              // fresnell reflection
                scattered = ray(rec.p, reflected);
                return true;
            }
            vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);  // else refraction
            scattered = ray(rec.p, refracted);
            return true;
        }

    public:
        double ref_idx;
};
#endif