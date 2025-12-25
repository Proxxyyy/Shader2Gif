#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "shaders.hh"

int main(int argc, char* argv[])
{
    int w = 1280;
    int h = 720;
    const int frames = 60;

    // User input width and height
    if (argc >= 3)
    {
        w = std::atoi(argv[1]);
        h = std::atoi(argv[2]);
    }

    // Command to pipe raw RGB data to ffmpeg
    std::string dimensions = std::to_string(w) + "x" + std::to_string(h);
    std::string cmd = "ffmpeg -y -f rawvideo -vcodec rawvideo -s " + dimensions +
                      " -pix_fmt rgb24 -r 60 -i - -c:v libx264 -pix_fmt yuv420p output.mp4";
    // For alpha support: -c:v libvpx-vp9 -pix_fmt yuva420p output.webm"

    FILE* pipe = popen(cmd.c_str(), "w");
    if (!pipe)
    {
        std::cerr << "Error: Could not open pipe to ffmpeg." << std::endl;
        return 1;
    }

    // Buffer for one frame (Width * Height * 3 bytes for RGBA)
    std::vector<unsigned char> frame(w * h * 3);

    for (int i = 0; i < frames; ++i)
    {
        for (int y = 0; y < h; ++y)
        {
            for (int x = 0; x < w; ++x)
            {
                int index = (y * w + x) * 3;

                PlasmaShader shader;
                shader.u.iResolution = glm::vec2(w, h);
                shader.u.iTime = i * (1.0f / 60.0f); // Assuming 60fps
                shader.u.iTimeDelta = 1.0f / 60.0f;
                shader.u.iFrame = i;
                shader.u.iFrameRate = 60.0f;
                shader.u.iDuration = frames * (1.0f / 60.0f);

                shader.fragCoord = glm::vec2(x, y);
                shader.main();
                glm::vec4 color = shader.fragColor;

                frame[index] = static_cast<unsigned char>(color.r * 255.0f);
                frame[index + 1] = static_cast<unsigned char>(color.g * 255.0f);
                frame[index + 2] = static_cast<unsigned char>(color.b * 255.0f);
                // frame[index + 3] = static_cast<unsigned char>(color.a * 255.0f);
            }
        }

        // Write the frame to the pipe
        fwrite(frame.data(), 1, frame.size(), pipe);

        // Debug: Save the first frame as a PPM file
        if (i == 0)
        {
            FILE* debug_file = fopen("first_frame.ppm", "wb");
            if (debug_file)
            {
                fprintf(debug_file, "P6\n%d %d\n255\n", w, h);
                fwrite(frame.data(), 1, frame.size(), debug_file);
                fclose(debug_file);
                std::cout << "Saved first frame to first_frame.ppm" << std::endl;
            }
        }
    }

    pclose(pipe);

    return 0;
}
