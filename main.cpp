#include <fstream>
#include <iostream>
#include <limits>
#include <cassert>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GJK.h"
#include "Polygon.h"
#include "shader.h"
#define deg2rad(x) ((x)*((3.1415926f)/(180.0f)))

glm::vec2 triangle_vertices[3] = {
    glm::vec2(0.0, 1.0),
    glm::vec2(0.85, -0.5),
    glm::vec2(-0.85, -0.5)
};
unsigned triangle_indices[3] = {0, 1, 2};

glm::vec2 square_vertices[4] = {
    glm::vec2(1.0, 1.0),
    glm::vec2(1.0, -1.0),
    glm::vec2(-1.0, 1.0),
    glm::vec2(-1.0, -1.0)
};

unsigned square_indices[6] = {0, 1, 2, 1, 2, 3};

glm::vec3 tetrahedron_vertices[4] = {
    glm::vec3(0.0, 1.0, 0.0),
    glm::vec3(0.9428, -1.0f / 3.0f, 0.0),
    glm::vec3(-0.4714, -1.0f / 3.0f, 0.8165),
    glm::vec3(-0.4714, -1.0f / 3.0f, -0.8165)
};

unsigned tetrahedron_indices[12] = {0, 1, 2, 0, 1, 3, 0, 2, 3, 1, 2, 3};

glm::vec3 cube_vertices[8] = {
    glm::vec3(1.0, 1.0, -1.0),
    glm::vec3(1.0, -1.0, -1.0),
    glm::vec3(-1.0, -1.0, -1.0),
    glm::vec3(-1.0, 1.0, -1.0),

    glm::vec3(1.0, 1.0, 1.0),
    glm::vec3(1.0, -1.0, 1.0),
    glm::vec3(-1.0, -1.0, 1.0),
    glm::vec3(-1.0, 1.0, 1.0),
 
};

unsigned cube_indices[36] = {
    0, 1, 2, 0, 2, 3,
    4, 5, 6, 4, 6, 7,
    0, 4, 1, 4, 1, 5,
    1, 5, 6, 1, 6 ,2,
    2, 6, 7, 2, 7, 3,
    3, 7, 4, 3, 4, 0
};

Shader *shader2D, *shader3D;
glm::vec3 camera = glm::vec3(0.0, 0.0, -3.0);
float prevRad = 0.0;
const GLuint WIDTH = 1600, HEIGHT = 1600;
void framebuffer_size_callback(GLFWwindow* window, int height, int width);
int main(int argc, char** argv){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GJK", NULL, NULL);    
    if (!window){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (glewInit() != GLEW_OK){
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }    
    
    shader2D = new Shader("./shaders/vs-2d.glsl", "./shaders/fs.glsl");
    shader3D = new Shader("./shaders/vs-3d.glsl", "./shaders/fs.glsl");
    ConvexPolygon2D *triangle = new ConvexPolygon2D(std::vector<glm::vec2>(triangle_vertices, triangle_vertices+3), std::vector<unsigned>(triangle_indices, triangle_indices+3), glm::vec2(-1.0, 0.0));
    ConvexPolygon2D *square = new ConvexPolygon2D(std::vector<glm::vec2>(square_vertices, square_vertices+4), std::vector<unsigned>(square_indices, square_indices+6), glm::vec2(1.0, 0.0));

    ConvexPolygon3D *tetrahedron = new ConvexPolygon3D(std::vector<glm::vec3>(tetrahedron_vertices, tetrahedron_vertices+4), std::vector<unsigned>(tetrahedron_indices, tetrahedron_indices+12), glm::vec3(-2.0, 0.0, 0.0));
    ConvexPolygon3D *cube = new ConvexPolygon3D(std::vector<glm::vec3>(cube_vertices, cube_vertices+8), std::vector<unsigned>(cube_indices, cube_indices+36), glm::vec3(2, 0.0, 0.0));
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    framebuffer_size_callback(window, WIDTH, HEIGHT);
    while (!glfwWindowShouldClose(window)){
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        double timeValue = glfwGetTime();
        float rad = deg2rad(timeValue) * 40.0f;

        triangle->move(glm::vec2(cos(timeValue * 0.3) * 0.01, sin(timeValue) * 0.01));
        triangle->rotate(rad - prevRad);
        square->move(glm::vec2(cos(timeValue * 2) * sin(timeValue) * 0.1, sin(-timeValue + 314) * sin((timeValue - 1) * 2) * 0.05));
        square->rotate(rad - prevRad);

        shader2D->setUniformInt("trigger", GJK::TestCollision(triangle, square));
        shader2D->setUniformMatrix3x3("M", &triangle->getModelMatrix()[0][0]);
        triangle->draw();

        shader2D->setUniformMatrix3x3("M", &square->getModelMatrix()[0][0]);
        square->draw();
        // tetrahedron->rotate(glm::quat(cos(rad - prevRad), sin(rad - prevRad) * 0.5, sin(rad - prevRad) * 0.866, 0.0));
        // cube->rotate(glm::quat(cos(rad - prevRad), sin(rad - prevRad) * -0.866, sin(rad - prevRad) * -0.5, 0.0));

        // tetrahedron->move(glm::vec3((rad - prevRad)*0.5, 0.0, 0.0));
        // cube->move(glm::vec3((rad - prevRad)*-0.5, 0.0, 0.0));

        // shader3D->setUniformInt("trigger", GJK::TestCollision(tetrahedron, cube));
        // glm::mat4 view_matrix = glm::lookAt(camera, glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));
        // shader3D->setUniformMatrix4x4("V", &view_matrix[0][0]);

        
        // shader3D->setUniformMatrix4x4("M", &tetrahedron->getModelMatrix()[0][0]);
        // tetrahedron->draw();

        // shader3D->setUniformMatrix4x4("M", &cube->getModelMatrix()[0][0]);
        // cube->draw();

        prevRad = rad;
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    delete triangle;
    delete square;
    delete tetrahedron;
    delete cube;
    delete shader2D;
    delete shader3D;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
    float viewportAspect = (float)width / (float)height;
    glm::mat4 proj_matrix = glm::perspective(deg2rad(60.0f), viewportAspect, 0.1f, 1000.0f);
    shader3D->setUniformMatrix4x4("P", &proj_matrix[0][0]);

    glm::mat4 ortho_proj_matrix = glm::ortho(-5.0, 5.0, -5.0, 5.0);
    shader2D->setUniformMatrix4x4("P", &ortho_proj_matrix[0][0]);
}



