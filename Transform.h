#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
typedef struct _Transform2D {
        glm::vec2 position;        
        glm::vec2 scale;
        float rad;
        _Transform2D(){
            this->position = glm::vec2(0.0);
            this->scale = glm::vec2(1.0);
            this->rad = 0.0f;
        }
        _Transform2D(glm::vec2 position){
            this->position = position;
            this->scale = glm::vec2(1.0);
            this->rad = 0.0f;
        }
        _Transform2D(glm::vec2 position, glm::vec2 scale){
            this->position = position;
            this->scale = scale;
            this->rad = 0.0f;
        }
        _Transform2D(glm::vec2 position, glm::vec2 scale, float rad){
            this->position = position;
            this->scale = scale;
            this->rad = rad;
        }
}Transform2D;

typedef struct _Transform3D {
        glm::vec3 position;        
        glm::vec3 scale;
        glm::quat rotation;
        _Transform3D(){
            this->position = glm::vec3(0.0);
            this->scale = glm::vec3(1.0);
            this->rotation = glm::quat(1.0, 0.0, 0.0, 0.0);
        }
        _Transform3D(glm::vec3 position){
            this->position = position;
            this->scale = glm::vec3(0.5);
            this->rotation = glm::quat(1.0, 0.0, 0.0, 0.0);
        }
        _Transform3D(glm::vec3 position, glm::vec3 scale){
            this->position = position;
            this->scale = scale;
            this->rotation = glm::quat(1.0, 0.0, 0.0, 0.0);
        }
        _Transform3D(glm::vec3 position, glm::vec3 scale, glm::quat rotation){
            this->position = position;
            this->scale = scale;
            this->rotation = rotation;
        }
}Transform3D;

#endif