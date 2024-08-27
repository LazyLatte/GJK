#ifndef POLYGON_H
#define POLYGON_H
#include "GJK.h"
#include "Renderable.h"
#include "Transform.h"
class ConvexPolygon2D : public GJK::GJK2D, public Renderable {
    private:
        Transform2D *transform;
        std::vector<glm::vec2> vertices;
    public:
        ConvexPolygon2D(std::vector<glm::vec2> vertices, std::vector<unsigned> indices, glm::vec2 position): Renderable(vertices, indices){
            this->vertices = vertices;
            this->transform = new Transform2D(position);
        }
        ~ConvexPolygon2D(){
            delete this->transform;
        }
        glm::vec2 furthestPoint(glm::vec2 dir) const override {
            glm::vec2 target = glm::vec2(0.0);
            float max_dot = -std::numeric_limits<float>::max();
            glm::mat3 model_matrix = this->getModelMatrix();
            for(auto &vertex : this->vertices){
                glm::vec2 v = model_matrix * glm::vec3(vertex, 1.0);
                float d = glm::dot(v, dir);
                if(d > max_dot){
                    max_dot = d;
                    target = v;
                }
            }
            return target;
        }
        void move(glm::vec2 d){this->transform->position +=d;}
        void scale(glm::vec2 s){this->transform->scale *= s;}
        void rotate(float rad){this->transform->rad += rad;}
        glm::mat3 getModelMatrix() const {
            return glm::translate(glm::rotate(glm::scale(glm::mat3(1.0), this->transform->scale), this->transform->rad), this->transform->position);
        }
};

class ConvexPolygon3D : public GJK::GJK3D, public Renderable {
    private:
        Transform3D *transform;
        std::vector<glm::vec3> vertices;
    public:
        ConvexPolygon3D(std::vector<glm::vec3> vertices, std::vector<unsigned> indices, glm::vec3 position): Renderable(vertices, indices){
            this->vertices = vertices;
            this->transform = new Transform3D(position);
        }
        ~ConvexPolygon3D(){
            delete this->transform;
        }
        glm::vec3 furthestPoint(glm::vec3 dir) const override {
            glm::vec3 target = glm::vec3(0.0);
            float max_dot = -std::numeric_limits<float>::max();
            glm::mat4 model_matrix = this->getModelMatrix();
            for(auto &vertex : this->vertices){
                glm::vec3 v = model_matrix * glm::vec4(vertex, 1.0);
                float d = glm::dot(v, dir);
                if(d > max_dot){
                    max_dot = d;
                    target = v;
                }
            }
            return target;
        }
        void move(glm::vec3 d){this->transform->position +=d;}
        void scale(glm::vec3 s){this->transform->scale *= s;}
        void rotate(glm::quat q){this->transform->rotation *= q;}
        glm::mat4 getModelMatrix() const {
            return glm::translate(glm::mat4(1.0f), this->transform->position) * glm::mat4_cast(this->transform->rotation) * glm::scale(glm::mat4(1.0f), this->transform->scale);
        }
};

class ConvexPolygon4D : public GJK::GJK4D, public Renderable {
    private:
        Transform4D *transform;
        std::vector<glm::vec4> vertices;
    public:
        ConvexPolygon4D(std::vector<glm::vec4> vertices, std::vector<unsigned> indices, glm::vec4 position): Renderable(vertices, indices){
            this->vertices = vertices;
            this->transform = new Transform4D(position);
        }
        ~ConvexPolygon4D(){
            delete this->transform;
        }
        glm::vec4 furthestPoint(glm::vec4 dir) const override {
            glm::vec4 target = glm::vec4(0.0);
            float max_dot = -std::numeric_limits<float>::max();
            glm::mat4 left_rotation_matrix = this->getLeftRotationMatrix();
            glm::mat4 right_rotation_matrix = this->getRightRotationMatrix();
            for(auto &vertex : this->vertices){
                glm::vec4 v = left_rotation_matrix * (vertex * this->transform->scale) * right_rotation_matrix + this->transform->position;
                float d = glm::dot(v, dir);
                if(d > max_dot){
                    max_dot = d;
                    target = v;
                }
            }
            return target;
        }
        void move(glm::vec3 d){this->transform->position +=d;}
        void scale(glm::vec3 s){this->transform->scale *= s;}
        //void rotate(glm::quat q){this->transform->rotation *= q;}
        glm::mat4 getLeftRotationMatrix() const {
            glm::quat left = this->transform->left_isoclinic;
            return glm::mat4(
                left.w, left.x, left.y, left.z,
                -left.x, left.w, left.z, -left.y,
                -left.y, -left.z, left.w, left.x,
                -left.z, left.y, -left.x, left.w
            );
        }
        glm::mat4 getRightRotationMatrix() const {
            glm::quat right = this->transform->right_isoclinic;
            return glm::mat4(
                right.w, right.x, right.y, right.z,
                -right.x, right.w, -right.z, right.y,
                -right.y, right.z, right.w, -right.x,
                -right.z, -right.y, right.x, right.w
            );
        }
};
#endif /* POLYGON_H */
