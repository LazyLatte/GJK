#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
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

typedef struct _Transform4D {
    glm::vec4 position;        
    glm::vec4 scale;
    glm::quat left_isoclinic;
    glm::quat right_isoclinic;
    _Transform4D(){
        this->position = glm::vec4(0.0);
        this->scale = glm::vec4(1.0);
        this->left_isoclinic = glm::quat(1.0, 0.0, 0.0, 0.0);
        this->right_isoclinic = glm::quat(1.0, 0.0, 0.0, 0.0);
    }
    _Transform4D(glm::vec4 position){
        this->position = position;
        this->scale = glm::vec4(1.0);
        this->left_isoclinic = glm::quat(1.0, 0.0, 0.0, 0.0);
        this->right_isoclinic = glm::quat(1.0, 0.0, 0.0, 0.0); 
    }
    _Transform4D(glm::vec4 position, glm::vec4 scale){
        this->position = position;
        this->scale = scale;
        this->left_isoclinic = glm::quat(1.0, 0.0, 0.0, 0.0);
        this->right_isoclinic = glm::quat(1.0, 0.0, 0.0, 0.0); 
    }
    _Transform4D(glm::vec4 position, glm::vec4 scale, glm::quat left_isoclinic, glm::quat right_isoclinic){
        this->position = position;
        this->scale = scale;
        this->left_isoclinic = left_isoclinic;
        this->right_isoclinic = right_isoclinic; 
    }
}Transform4D;

#endif