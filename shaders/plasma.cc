// Inspired by Tsoding
// https://gist.github.com/rexim/ef86bf70918034a5a57881456c0a0ccf

#include "shaders.hh"


void PlasmaShader::main()
{
    glm::vec2 FC = fragCoord;
    glm::vec2 r = u.iResolution;
    float t = u.iTime;
    glm::vec2 p = (FC * 2.0f - r) / r.y;
    glm::vec2 l(0.0f), i(0.0f);
    glm::vec2 v = p * (l += 4.0f - 4.0f * glm::abs(0.7f - glm::dot(p, p)));
    for (; i.y++ < 8.0f; fragColor += (glm::sin(v.xyyx()) + 1.0f) * glm::abs(v.x - v.y))
    {
        v += glm::cos(v.yx() * i.y + i + t) / i.y + 0.7f;
    }
    fragColor = glm::tanh(5.0f * glm::exp(l.x - 4.0f - p.y * glm::vec4(-1.0f, 1.0f, 2.0f, 0.0f)) / fragColor);
}
