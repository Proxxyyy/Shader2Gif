// Inspired by Tsoding
// https://gist.github.com/rexim/ef86bf70918034a5a57881456c0a0ccf

#include "shaders.hh"

void checker(int x, int y, int frame, glm::vec4& color)
{
    bool is_checker = ((x + frame) / 60 + (y + frame) / 60) % 2 != 0;

    if (is_checker)
    {
        color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    }
    else
    {
        color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
}
