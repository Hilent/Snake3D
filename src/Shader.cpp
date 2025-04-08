#include "Shader.h"

Shader::Shader(string vertShaderPath, string fragShaderPath, string progName)
{
    //Reading the shaders in
    string vertShaderSrc = readShader(vertShaderPath);
    string fragShaderSrc = readShader(fragShaderPath);
    
    //Converting into c string because of glShaderSource()
    const char* vSrc = vertShaderSrc.c_str();
    const char* fSrc = fragShaderSrc.c_str();



    //Compiling vertex shader
    vertShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShaderId, 1, &vSrc, NULL);
    glCompileShader(vertShaderId);

    checkShaderCompilation(vertShaderId, vertShaderPath, progName);

    //Compiling fragment shader
    fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShaderId, 1, &fSrc, NULL);
    glCompileShader(fragShaderId);

    checkShaderCompilation(fragShaderId, fragShaderPath, progName);


    //Attaching shaders and linking the program
    progId = glCreateProgram();
    glAttachShader(progId, fragShaderId);
    glAttachShader(progId, vertShaderId);
    glLinkProgram(progId);

    checkProgramLinking(progId, progName);


    //Deleting because shaders are already attached to the program
    glDeleteShader(vertShaderId);
    glDeleteShader(fragShaderId);
}


string Shader::readShader(string path)
{
    ifstream fin(path);

    string result;
    string line;

    while(getline(fin, line)){
        result += line + "\n";
    }


    return result;
}


void Shader::use()
{
    glUseProgram(progId);
}


void Shader::setFloat(string name, float value)
{
    string n = name;
    glUniform1f(glGetUniformLocation(progId, n.c_str()), value);
}

void Shader::setFloat3(string name, float v1, float v2, float v3)
{
    string n = name;
    glUniform3f(glGetUniformLocation(progId, n.c_str()), v1, v2, v3);
}

void Shader::setInt(string name, int value)
{
    string n = name;
    glUniform1i(glGetUniformLocation(progId, n.c_str()), value);
}

void Shader::setBool(string name, bool value)
{
    string n = name;
    glUniform1i(glGetUniformLocation(progId, n.c_str()), (int)value);
}

void Shader::setMat4(string name, glm::mat4 value)
{
    string n = name;
    glUniformMatrix4fv(glGetUniformLocation(progId, n.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}


void Shader::checkShaderCompilation(unsigned int shader, string shaderPath, string progName)
{
    int success;
    char info[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(shader, 512, NULL, info);
        cerr<<"Error compiling "<< shaderPath <<" shader in program "<< progName <<"\nerror log: "<<endl<<info<<endl;
    }

}

void Shader::checkProgramLinking(unsigned int prog, string progName){
    int success;
    char info[512];
    glGetProgramiv(prog, GL_LINK_STATUS, &success);
    if (!success) {
        int type;
        glGetProgramInfoLog(prog, 512, NULL, info);
        cerr << "Error linking program "<< progName << "\nerror log: " << endl << info << endl;
    }
}
