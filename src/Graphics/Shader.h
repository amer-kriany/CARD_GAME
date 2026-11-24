#pragma once
#include <string>
#include <glm/glm.hpp>


class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    void use() const;  

    void setMat4(const std::string& name, const glm::mat4& value) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setInt(const std::string& name, int value) const;

private:
    unsigned int program_ = 0;  

    static std::string readFile(const std::string& path);
    static unsigned int compile(unsigned int type, const std::string& source,
                                const std::string& label);
};