#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    // The program ID
    unsigned int ID;

    // Constructor reads and builds the shader from file paths
    Shader(const char* vertexPath, const char* fragmentPath);

    // Use/activate the shader program
    void use() const;

    // Utility functions for setting uniform variables
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;

private:
    // Helper function to compile shaders
    unsigned int compileShader(unsigned int type, const std::string& source) const;

    // Helper function to load shader source code from a file
    std::string loadShaderSource(const char* filePath) const;
};

#endif

