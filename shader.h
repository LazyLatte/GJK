#include <fstream>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

std::string readShaderCode(const char *filename){
    std::ifstream in;
    in.open(filename);
    if(!in.is_open()){
        std::cout << "Failed to open file.\n";
        exit(1);
    }
    return std::string(
        std::istreambuf_iterator<char>(in),
        std::istreambuf_iterator<char>()
    );
}

class Shader {
    private:
        GLuint program;
    public:
        Shader(const char *vs_filename, const char *fs_filename){
            GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
            GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            std::string shaderSource;
            shaderSource = readShaderCode(vs_filename);
            const GLchar* s[1];
            s[0] = shaderSource.c_str();
            glShaderSource(vertexShader, 1, s, NULL);
            shaderSource = readShaderCode(fs_filename);
            s[0] = shaderSource.c_str();
            glShaderSource(fragmentShader, 1, s, NULL);
            
            glCompileShader(vertexShader);
            glCompileShader(fragmentShader);

            this->program = glCreateProgram();
            glAttachShader(this->program, vertexShader);
            glAttachShader(this->program, fragmentShader);
            glLinkProgram(this->program);

            glUseProgram(this->program);
            
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
        }
        void setUniformInt(const GLchar *name, GLint val){
            glUseProgram(this->program);
            glUniform1i(glGetUniformLocation(this->program, name), val);
        }
        void setUniformMatrix3x3(const GLchar *name, const GLfloat *matrix){
            glUseProgram(this->program);
            glUniformMatrix3fv(glGetUniformLocation(this->program, name), 1, GL_FALSE, matrix);
        }
        void setUniformMatrix4x4(const GLchar *name, const GLfloat *matrix){
            glUseProgram(this->program);
            glUniformMatrix4fv(glGetUniformLocation(this->program, name), 1, GL_FALSE, matrix);
        }
};