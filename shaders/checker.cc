// Inspired by Tsoding
// https://gist.github.com/rexim/ef86bf70918034a5a57881456c0a0ccf

#include "shaders.hh"

void CheckerShader::main()
{
    int x = static_cast<int>(fragCoord.x);
    int y = static_cast<int>(fragCoord.y);
    int frame = u.iFrame;
    bool is_checker = ((x + frame) / 60 + (y + frame) / 60) % 2 != 0;

    if (is_checker)
    {
        fragColor = glm::vec4(1.0f, 0.0f, 0.0f, 0.1f);
    }
    else
    {
        fragColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.1f);
    }
}
