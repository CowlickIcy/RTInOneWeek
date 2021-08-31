#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <cmath>
#include "macro.h"

using namespace std;


class vec3{
    public:
        // 构造函数
        vec3() : e{0,0,0}{}
        vec3(double e0,double e1, double e2) : e{ e0 , e1 , e2}{}
        // 取分量
        double x() const {return e[0];}
        double y() const {return e[1];}
        double z() const {return e[2];}
        // 重载操作符、运算符
        vec3 operator-() const { return vec3( -e[0] , -e[1] , -e[2]) ;}
        double operator[](int i ) const { return e[i]; }
        double& operator[](int i ) { return e[i]; }

        vec3& operator+=( const vec3 &v ){
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        vec3& operator*= ( const double t ){
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        vec3& operator/=( const double t ){
            return *this *= 1/t;
        }
        // random generate

        inline static vec3 random(){
            return vec3(random_double(), random_double(), random_double());
        }

        inline static vec3 random(double min, double max) {
            return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
        }

        // 模运算
        double length_squared() const {
            return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
        }
        double length() const {
            return sqrt(length_squared());
        }
        // 输出  
        void write_color(std::ostream &out , int samples_per_pixel) {
            auto scale = 1.0 / samples_per_pixel;
            auto r = pow(scale * e[0], 1 / gamma_coff );
            auto g = pow(scale * e[1], 1 / gamma_coff );
            auto b = pow(scale * e[2], 1 / gamma_coff );

            out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
                << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
                << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
        }

    public:
        double e[3];
};

// other utility functions
inline std::ostream& operator<<( std::ostream&out , const vec3 &v ) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+( const vec3 &u , const vec3 &v ) {
    return vec3( u.e[0] + v.e[0] , u.e[1] + v.e[1] , u.e[2] + v.e[2] );
}

inline vec3 operator-( const vec3 &u , const vec3 &v ) {
   return vec3( u.e[0] - v.e[0] , u.e[1] - v.e[1] , u.e[2] - v.e[2] );
}

inline vec3 operator*( const vec3 &u, const vec3 &v ) {
    return vec3( u.e[0] * v.e[0] , u.e[1] * v.e[1] , u.e[2] * v.e[2] );
}

inline vec3 operator*( const vec3 &v , double t ) {
    return vec3( v.e[0] * t , v.e[1] * t , v.e[2] * t );
}

inline vec3 operator*( double t , const vec3 &u ) {
    return u * t;
}

inline vec3 operator/( vec3 v , double t){
    return (1/t) * v;
}

inline double dot( const vec3 &u , const vec3 &v ) {
    return u.e[0] * v.e[0] +
           u.e[1] * v.e[1] +
           u.e[2] * v.e[2] ;
}

inline vec3 cross( const vec3 &u , const vec3 &v ) {
    return vec3( u.e[1] * v.e[2] - u.e[2] * v.e[1] ,
                 u.e[2] * v.e[0] - u.e[0] * v.e[2] ,
                 u.e[0] * v.e[1] - u.e[1] - v.e[0] );
}

inline vec3 unit_vector( vec3 v ) {
    return v / v.length();
}

// random reflection generate
vec3 random_in_unit_shpere() {
    while(true){
        auto p = vec3::random(-1,1);
        if(p.length_squared() >= 1) continue;
        return p;
    }
}

vec3 random_unity_vector() {
    auto a = random_double(0, 2*PI);
    auto z = random_double(-1, 1);
    auto r = sqrt(1 - z*z);
    return vec3(r*cos(a), r*sin(a), z);

}

vec3 random_in_hemisphere(const vec3& normal) {
    vec3 in_unit_sphere = random_in_unit_shpere();
    if(dot(in_unit_sphere, normal) > 0){
        return in_unit_sphere;
    }else{
        return -in_unit_sphere;
    }
}

// refection diretion
vec3 reflect(const vec3& v, const vec3& n){
    return v - 2.0 * dot(v, n) * n;

}

// refraction 
vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    auto cos_theta = dot(-uv, n);
    vec3 r_out_parallel =  etai_over_etat * (uv + cos_theta*n);
    vec3 r_out_perp = -sqrt(1.0 - r_out_parallel.length_squared()) * n;
    return r_out_parallel + r_out_perp;
}
// schlick fresnell appromix
double schlick(double cosine, double ref_idx) {
    auto r0 = (1-ref_idx) / (1+ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cosine),5);
}
#endif