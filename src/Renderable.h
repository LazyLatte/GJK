#ifndef RENDERABLE_H
#define RENDERABLE_H
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
class Renderable {
    private:
        GLuint vao;
        GLuint ebo;
        GLuint position_buffer;
        unsigned numOfVertex;
    public:
        Renderable(std::vector<glm::vec2> vertices, std::vector<unsigned> indices){
            assert(vertices.size() > 0);
            
            glGenVertexArrays(1, &this->vao);
            glGenBuffers(1, &this->position_buffer);

            glBindVertexArray(this->vao);
            glBindBuffer(GL_ARRAY_BUFFER, this->position_buffer);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * 2 * sizeof(float), &vertices[0].x, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
            glEnableVertexAttribArray(0);

            glGenBuffers(1, &this->ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

            this->numOfVertex = indices.size();
        }
        Renderable(std::vector<glm::vec3> vertices, std::vector<unsigned> indices){
            glGenVertexArrays(1, &this->vao);
            glGenBuffers(1, &this->position_buffer);

            glBindVertexArray(this->vao);
            glBindBuffer(GL_ARRAY_BUFFER, this->position_buffer);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(float), &vertices[0].x, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
            glEnableVertexAttribArray(0);

            glGenBuffers(1, &this->ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

            this->numOfVertex = indices.size();
        }
        Renderable(std::vector<glm::vec4> vertices, std::vector<unsigned> indices){
            glGenVertexArrays(1, &this->vao);
            glGenBuffers(1, &this->position_buffer);

            glBindVertexArray(this->vao);
            glBindBuffer(GL_ARRAY_BUFFER, this->position_buffer);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * 4 * sizeof(float), &vertices[0].x, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
            glEnableVertexAttribArray(0);

            glGenBuffers(1, &this->ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

            this->numOfVertex = indices.size();
        }
        void draw(){
            glBindVertexArray(this->vao);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
            glDrawElements(GL_TRIANGLES, this->numOfVertex, GL_UNSIGNED_INT, (void*)0);
            //glDrawArrays(GL_TRIANGLE_STRIP, 0, this->numOfVertex);
        }
};

#endif /* RENDERABLE_H */
