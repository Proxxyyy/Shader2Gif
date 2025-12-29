#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>

#include "shaders.hh"

// Default options values
int w = 1280;
int h = 720;
int fps = 60;
int duration = 1;
bool debug = false;

void print_help(const char* program_name)
{
    std::cout << "Usage: " << program_name << " [OPTIONS]\n"
              << "Options:\n"
              << "  -f, --framerate <fps>    Set frame rate (default: 60)\n"
              << "  -s, --size <W>x<H>       Set frame size (default: 1280x720)\n"
              << "  -d, --duration <sec>     Set duration in seconds (default: 1)\n"
              << "  --debug                  Enable debug mode\n"
              << "  -h, --help               Show this help message\n";
}

int parse_options(int argc, char* argv[])
{
    static struct option long_options[] = {{"framerate", required_argument, 0, 'f'},
                                           {"size", required_argument, 0, 's'},
                                           {"duration", required_argument, 0, 'd'},
                                           {"debug", no_argument, 0, 0},
                                           {"help", no_argument, 0, 'h'},
                                           {0, 0, 0, 0}};

    int opt;
    int option_index = 0;
    while ((opt = getopt_long(argc, argv, "f:s:d:h", long_options, &option_index)) != -1)
    {
        switch (opt)
        {
            case 'f':
                fps = std::atoi(optarg);
                if (fps <= 0)
                {
                    std::cerr << "Invalid framerate. Use a positive integer." << std::endl;
                    return 1;
                }
                break;
            case 's':
                if (sscanf(optarg, "%dx%d", &w, &h) != 2)
                {
                    std::cerr << "Invalid size format. Use WxH (e.g., 1280x720)." << std::endl;
                    return 1;
                }
                break;
            case 'd':
                duration = std::atoi(optarg);
                if (duration <= 0)
                {
                    std::cerr << "Invalid duration. Use a positive integer." << std::endl;
                    return 1;
                }
                break;
            case 'h':
                print_help(argv[0]);
                return 2; // Exit with success

            case 0: // Long option that have no short option
                if (std::string(long_options[option_index].name) == "debug")
                {
                    debug = true;
                }
                break;

            default:
                std::cerr << "Unrecognized option -" << char(optopt) << ". Use --help for usage information."
                          << std::endl;
                return 1;
        }
    }
    return 0;
}

int main(int argc, char* argv[])
{
    int opt_status = parse_options(argc, argv);
    if (opt_status == 1)
        return 1; // Error while parsing options
    if (opt_status == 2)
        return 0; // Usage/Help requested. Exit with success

    // Command to pipe raw RGB data to ffmpeg
    std::string cmd = "ffmpeg -y -f rawvideo -vcodec rawvideo -s " + std::to_string(w) + "x" + std::to_string(h) +
                      " -pix_fmt rgb24 -r " + std::to_string(fps) + " -i - -c:v libx264 -pix_fmt yuv420p output.mp4";
    // For alpha support: -c:v libvpx-vp9 -pix_fmt yuva420p output.webm"

    // Open pipe to ffmpeg
    FILE* pipe = popen(cmd.c_str(), "w");
    if (!pipe)
    {
        std::cerr << "Error: Could not open pipe to ffmpeg." << std::endl;
        return 1;
    }

    int frames = duration * fps;
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
                shader.u.iTime = i * (1.0f / fps);
                shader.u.iTimeDelta = 1.0f / fps;
                shader.u.iFrame = i;
                shader.u.iFrameRate = static_cast<float>(fps);
                shader.u.iDuration = static_cast<float>(duration);

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
        if (debug && i == 0)
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
