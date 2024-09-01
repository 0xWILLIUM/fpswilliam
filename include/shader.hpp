#ifndef SHADER_H 
#define SHADER_H    1

#include <string>
#include <glad/glad.h>

class Shader {
    public:
    unsigned int ID;
    // shader constructor
    Shader(const char* vertexPath, const char* fragmentPath);
    // shader activation
    void use();
    // utility uniform funcs
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
};

#endif
