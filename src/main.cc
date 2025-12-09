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
    const int frames = 144;

    // User input width and height
    if (argc >= 3)
    {
        w = std::atoi(argv[1]);
        h = std::atoi(argv[2]);
    }

    // Command to pipe raw RGB data to ffmpeg
    std::string dimensions = std::to_string(w) + "x" + std::to_string(h);
    std::string cmd = "ffmpeg -y -f rawvideo -vcodec rawvideo -s " + dimensions +
                      " -pix_fmt rgb24 -r 144 -i - -c:v libx264 -pix_fmt yuv420p output.mp4";

    FILE* pipe = popen(cmd.c_str(), "w");
    if (!pipe)
    {
        std::cerr << "Error: Could not open pipe to ffmpeg." << std::endl;
        return 1;
    }

    // Buffer for one frame (Width * Height * 3 bytes for RGB)
    std::vector<unsigned char> frame(w * h * 3);

    for (int i = 0; i < frames; ++i)
    {
        for (int y = 0; y < h; ++y)
        {
            for (int x = 0; x < w; ++x)
            {
                int index = (y * w + x) * 3;

                checker(x, y, i, frame[index], frame[index + 1], frame[index + 2]);
            }
        }

        // Write the frame to the pipe
        fwrite(frame.data(), 1, frame.size(), pipe);
    }

    pclose(pipe);

    return 0;
}
