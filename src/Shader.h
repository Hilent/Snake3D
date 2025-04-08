#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class Shader
{
public:

    unsigned int progId;

    Shader(string vertShaderPath, string fragShaderPath, string progMame);

    void use();

    //Uniforms
    void setFloat(string name, float value);
    void setFloat3(string name, float v1, float v2, float v3);
    void setInt(string name, int value);
    void setBool(string name, bool value);
    void setMat4(string name, glm::mat4 value);

private:

    unsigned int vertShaderId, fragShaderId;


    string readShader(string path);



    //Error checking
    void checkShaderCompilation(unsigned int shader, string shaderPath, string progMame);
    void checkProgramLinking(unsigned int prog, string progMame);

};

