#pragma once

#define GLM_FORCE_SWIZZLE

#include <glm/glm.hpp>

struct Uniforms
{
    glm::vec2 iResolution;
    float iTime;
    float iTimeDelta;
    int iFrame;
    float iFrameRate;
    float iDuration;
};

struct Shader
{
    Uniforms u;
    glm::vec2 fragCoord;
    glm::vec4 fragColor;

    virtual void main() = 0;
    virtual ~Shader() = default;
};

struct CheckerShader : public Shader
{
    void main() override;
};

struct PlasmaShader : public Shader
{
    void main() override;
};
