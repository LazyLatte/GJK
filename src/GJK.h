#ifndef GJK_H
#define GJK_H
#include <vector>
#include <array>
#include <initializer_list>
#include <glm/glm.hpp>

namespace GJK{
    class GJK2D {
        public: virtual glm::vec2 furthestPoint(glm::vec2 dir) const = 0;
    };
    class GJK3D {
        public: virtual glm::vec3 furthestPoint(glm::vec3 dir) const = 0;
    };
    class GJK4D {
        public: virtual glm::vec4 furthestPoint(glm::vec4 dir) const = 0;
    };

    template<typename T, std::size_t N>
    struct static_vector {
        std::array<T, N> storage;
        std::size_t m_size = 0;
        static_vector& operator=(const std::initializer_list<T>& list){
            this->m_size = 0;
            for(auto& e : list){
                if(this->m_size >= N) break;
                this->storage[this->m_size++] = e;
            }
            return *this;
        }
        T& operator[](std::size_t index){
            return this->storage[index];
        }
        void push_back(const T& val){
            if(this->m_size < N){
                this->storage[this->m_size++] = val;
            }
        }
        std::size_t size() const {
            return this->m_size;
        }
    };

    typedef static_vector<glm::vec2, 3> Simplex2D;
    typedef static_vector<glm::vec3, 4> Simplex3D;
    typedef static_vector<glm::vec4, 5> Simplex4D;

    bool TestCollision(GJK2D *s1, GJK2D *s2);
    bool TestCollision(GJK3D *s1, GJK3D *s2);
    bool TestCollision(GJK4D *s1, GJK4D *s2);
}



//----------------2D---------------------
bool simplex2(GJK::Simplex2D &simplex, glm::vec2 &dir){
    glm::vec2 B = simplex[0];
    glm::vec2 A = simplex[1];
    glm::vec2 AB = B - A;
    float z = A.x * AB.y - A.y * AB.x;
    if(z == 0.0f){
        //origin on AB
        return true;
    }else{
        dir.x = -z * AB.y;
        dir.y = z * AB.x;
        return false;
    }
}

bool simplex3(GJK::Simplex2D &simplex, glm::vec2 &dir){
    glm::vec2 C = simplex[0];
    glm::vec2 B = simplex[1];
    glm::vec2 A = simplex[2];

    glm::vec2 AB = B - A;
    glm::vec2 AC = C - A;
    float z_ABxAC = AB.x * AC.y - AB.y * AC.x;
    glm::vec2 NAB = glm::vec2(z_ABxAC * AB.y, -z_ABxAC * AB.x);
    glm::vec2 NAC = glm::vec2(-z_ABxAC * AC.y, z_ABxAC * AC.x);
    if(glm::dot(NAB, A) < 0){
        simplex = {B, A};
        dir = NAB;
        return false;
    }
    if(glm::dot(NAC, A) < 0){
        simplex = {C, A};
        dir = NAC;
        return false;
    }
    return true;
}

//----------------3D---------------------
bool simplex2(GJK::Simplex3D &simplex, glm::vec3 &dir){
    glm::vec3 B = simplex[0];
    glm::vec3 A = simplex[1];
    glm::vec3 AB = B - A;

    dir = glm::cross(glm::cross(A, AB), AB);
    return dir.x == 0.0f && dir.y == 0.0f && dir.z == 0.0f;
}

bool simplex3(GJK::Simplex3D &simplex, glm::vec3 &dir){
    glm::vec3 C = simplex[0];
    glm::vec3 B = simplex[1];
    glm::vec3 A = simplex[2];

    glm::vec3 AB = B - A;
    glm::vec3 AC = C - A;
    glm::vec3 N = glm::cross(AB, AC);
    glm::vec3 NAB = glm::cross(AB, N);
    glm::vec3 NAC = glm::cross(N, AC);
    if(glm::dot(NAB, A) < 0){
        //AB voronoi
        return simplex2(simplex = {B, A}, dir);
    }
    if(glm::dot(NAC, A) < 0){
        //AC voronoi
        return simplex2(simplex = {C, A}, dir);
    }
    //ABC voronoi
    float N_dot_A = glm::dot(N, A);
    if(N_dot_A < 0.0f){
        //above
        dir = N;
        return false;
    }
    if(N_dot_A > 0.0f){
        //below
        simplex = {B, C, A};
        dir = -N;
        return false;
    }
    //on
    return true;
}

bool simplex4(GJK::Simplex3D &simplex, glm::vec3 &dir){
    glm::vec3 D = simplex[0];
    glm::vec3 C = simplex[1];
    glm::vec3 B = simplex[2];
    glm::vec3 A = simplex[3];

    glm::vec3 AB = B - A;
    glm::vec3 AC = C - A;
    glm::vec3 AD = D - A;
    glm::vec3 NABC = glm::cross(AB, AC);
    glm::vec3 NACD = glm::cross(AC, AD);
    glm::vec3 NADB = glm::cross(AD, AB);
    if(glm::dot(NABC, A) < 0){
        return simplex3(simplex = {C, B, A}, dir);
    }
    if(glm::dot(NACD, A) < 0){
        return simplex3(simplex = {D, C, A}, dir);
    }
    if(glm::dot(NADB, A) < 0){
        return simplex3(simplex = {B, D, A}, dir);
    }
    return true;
}

//----------------4D---------------------

glm::vec4 wedge4D(glm::vec4 v, glm::vec4 u, glm::vec4 w){
    glm::mat3 m0 = glm::mat3(
        v[1], v[2], v[3],
        u[1], u[2], u[3],
        w[1], w[2], w[3]
    );

    glm::mat3 m1 = glm::mat3(
        v[0], v[2], v[3],
        u[0], u[2], u[3],
        w[0], w[2], w[3]
    );

    glm::mat3 m2 = glm::mat3(
        v[0], v[1], v[3],
        u[0], u[1], u[3],
        w[0], w[1], w[3]
    );

    glm::mat3 m3 = glm::mat3(
        v[0], v[1], v[2],
        u[0], u[1], u[2],
        w[0], w[1], w[2]
    );

    return glm::vec4(glm::determinant(m0), -glm::determinant(m1), glm::determinant(m2), -glm::determinant(m3));
}
bool simplex2(GJK::Simplex4D &simplex, glm::vec4 &dir){
    glm::vec4 B = simplex[0];
    glm::vec4 A = simplex[1];
    glm::vec4 AB = B - A;
    float t = glm::dot(A, AB) / glm::dot(AB, AB);
    dir = t * AB - A;
    return dir.x == 0.0f && dir.y == 0.0f && dir.z == 0.0f && dir.w == 0.0f;
}

bool simplex3(GJK::Simplex4D &simplex, glm::vec4 &dir){
    glm::vec4 C = simplex[0];
    glm::vec4 B = simplex[1];
    glm::vec4 A = simplex[2];

    glm::vec4 AO = -A;
    glm::vec4 AB = B - A;
    glm::vec4 AC = C - A;
    dir = wedge4D(wedge4D(AB, AC, AO), AC, AB);
    return dir.x == 0.0f && dir.y == 0.0f && dir.z == 0.0f && dir.w == 0.0f;
    // float sqLenAB = glm::dot(AB, AB);
    // float sqLenAC = glm::dot(AC, AC);
    // float dotABAC = glm::dot(AB, AC);
    // float dotOAAB = glm::dot(A, AB);
    // float dotOAAC = glm::dot(A, AC);
    // float t = (sqLenAC * dotOAAB - dotABAC * dotOAAC) / (sqLenAB * sqLenAC - dotABAC * dotABAC);
    // float s = (sqLenAB * dotOAAC - dotABAC * dotOAAB) / (sqLenAB * sqLenAC - dotABAC * dotABAC);
    // dir = (t * AB) + (s * AC) - A;
    
}

bool simplex4(GJK::Simplex4D &simplex, glm::vec4 &dir){
    glm::vec4 D = simplex[0];
    glm::vec4 C = simplex[1];
    glm::vec4 B = simplex[2];
    glm::vec4 A = simplex[3];

    glm::vec4 AB = B - A;
    glm::vec4 AC = C - A;
    glm::vec4 AD = D - A;
    glm::vec4 N = wedge4D(AB, AC, AD);
    glm::vec4 NABC = wedge4D(AB, AC, N);
    glm::vec4 NABD = wedge4D(AB, N, AD);
    glm::vec4 NACD = wedge4D(N, AC, AD);

    if(glm::dot(NABC, A) < 0){
        return simplex3(simplex = {C, B, A}, dir);
    }
    if(glm::dot(NABD, A) < 0){
        return simplex3(simplex = {D, B, A}, dir);
    }
    if(glm::dot(NACD, A) < 0){
        return simplex3(simplex = {D, C, A}, dir);
    }

    float N_dot_A = glm::dot(N, A);
    if(N_dot_A < 0.0f){
        //above
        dir = N;
        return false;
    }
    if(N_dot_A > 0.0f){
        //below
        simplex = {C, D, B, A};
        dir = -N;
        return false;
    }
    //on
    return true;
}

bool simplex5(GJK::Simplex4D &simplex, glm::vec4 &dir){
    glm::vec4 E = simplex[0];
    glm::vec4 D = simplex[1];
    glm::vec4 C = simplex[2];
    glm::vec4 B = simplex[3];
    glm::vec4 A = simplex[4];

    glm::vec4 AB = B - A;
    glm::vec4 AC = C - A;
    glm::vec4 AD = D - A;
    glm::vec4 AE = E - A;
    glm::vec4 NABCD = wedge4D(AB, AC, AD);
    glm::vec4 NACDE = wedge4D(AC, AD, AE);
    glm::vec4 NADEB = wedge4D(AD, AE, AB);
    glm::vec4 NAEBC = wedge4D(AE, AB, AC);
    if(glm::dot(NABCD, A) < 0){
        return simplex4(simplex = {D, C, B, A}, dir);
    }
    if(glm::dot(NACDE, A) < 0){
        return simplex4(simplex = {E, D, C, A}, dir);
    }
    if(glm::dot(NADEB, A) < 0){
        return simplex4(simplex = {B, E, D, A}, dir);
    }
    if(glm::dot(NAEBC, A) < 0){
        return simplex4(simplex = {C, B, E, A}, dir);
    }
    return true;
}

//---------------------------------------

bool handleSimplex(GJK::Simplex2D &simplex, glm::vec2 &dir){
    return simplex.size() == 2 ? simplex2(simplex, dir) : simplex3(simplex, dir);
}

bool handleSimplex(GJK::Simplex3D &simplex, glm::vec3 &dir){
    switch(simplex.size()){
        case 2:
            return simplex2(simplex, dir);
        case 3:
            return simplex3(simplex, dir);
        case 4:
            return simplex4(simplex, dir);
        default:
            return false;
    }
}

bool handleSimplex(GJK::Simplex4D &simplex, glm::vec4 &dir){
    switch(simplex.size()){
        case 2:
            return simplex2(simplex, dir);
        case 3:
            return simplex3(simplex, dir);
        case 4:
            return simplex4(simplex, dir);
        case 5:
            return simplex5(simplex, dir);
        default:
            return false;
    }
}

glm::vec2 getMinkowskiDiff(GJK::GJK2D *s1, GJK::GJK2D *s2, glm::vec2 dir){
    return s1->furthestPoint(dir) - s2->furthestPoint(-dir);
}

glm::vec3 getMinkowskiDiff(GJK::GJK3D *s1, GJK::GJK3D *s2, glm::vec3 dir){
    return s1->furthestPoint(dir) - s2->furthestPoint(-dir);
}

glm::vec4 getMinkowskiDiff(GJK::GJK4D *s1, GJK::GJK4D *s2, glm::vec4 dir){
    return s1->furthestPoint(dir) - s2->furthestPoint(-dir);
}

bool GJK::TestCollision(GJK::GJK2D *s1, GJK::GJK2D *s2){
    GJK::Simplex2D simplex;
    glm::vec2 dir = glm::vec2(1.0, 0.0);
    simplex.push_back(getMinkowskiDiff(s1, s2, dir));
    dir = -simplex[0];
    while(true){
        glm::vec2 A = getMinkowskiDiff(s1, s2, dir);
        if(glm::dot(A, dir) < 0){
            return false;
        }
        simplex.push_back(A);
        if(handleSimplex(simplex, dir)){
            return true;
        }
    }
    return false;
}

bool GJK::TestCollision(GJK::GJK3D *s1, GJK::GJK3D *s2){
    GJK::Simplex3D simplex;
    glm::vec3 dir = glm::vec3(1.0, 0.0, 0.0);
    simplex.push_back(getMinkowskiDiff(s1, s2, dir));
    dir = -simplex[0];
    while(true){
        glm::vec3 A = getMinkowskiDiff(s1, s2, dir);
        if(glm::dot(A, dir) < 0){
            return false;
        }
        simplex.push_back(A);
        if(handleSimplex(simplex, dir)){
            return true;
        }
    }
    return false;
}

bool GJK::TestCollision(GJK::GJK4D *s1, GJK::GJK4D *s2){
    GJK::Simplex4D simplex;
    glm::vec4 dir = glm::vec4(1.0, 0.0, 0.0, 0.0);
    simplex.push_back(getMinkowskiDiff(s1, s2, dir));
    dir = -simplex[0];
    while(true){
        glm::vec4 A = getMinkowskiDiff(s1, s2, dir);
        if(glm::dot(A, dir) < 0){
            return false;
        }
        simplex.push_back(A);
        if(handleSimplex(simplex, dir)){
            return true;
        }
    }
    return false;
}

#endif