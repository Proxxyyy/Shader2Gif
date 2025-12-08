#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

int main()
{
    for (int i = 0; i < 60; i++)
    {
        std::stringstream ss;
        ss << "output-" << std::setw(2) << std::setfill('0') << i << ".ppm";
        std::string output_path = ss.str();

        std::ofstream f(output_path, std::ios::binary);
        if (!f)
        {
            std::cerr << "Failed to open " << output_path << " for writing." << std::endl;
            continue;
        }

        int w = 16 * 60;
        int h = 9 * 60;
        f << "P6\n";
        f << w << " " << h << "\n";
        f << "255\n";

        for (int y = 0; y < h; y++)
        {
            for (int x = 0; x < w; x++)
            {
                if (((x + i) / 60 + (y + i) / 60) % 2)
                {
                    f.put(static_cast<char>(0xFF));
                    f.put(static_cast<char>(0x00));
                    f.put(static_cast<char>(0x00));
                }
                else
                {
                    f.put(static_cast<char>(0x00));
                    f.put(static_cast<char>(0x00));
                    f.put(static_cast<char>(0x00));
                }
            }
        }
        f.close();
        std::cout << "Generated " << output_path << std::endl;
    }
    return 0;
}
