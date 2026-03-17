#ifndef GPU_HPP
#define GPU_HPP

#include <string>

namespace gpu {
    void details(const std::string& name, bool NVIDIA, int cores, const std::string& structure, int vram, 
                 const std::string& memType, int bits, int bandwidth, float freq, int tgp, const std::string& graphics) {
        std::string vendor = NVIDIA ? "NVIDIA" : "AMD";
        std::string gpudetails[] = {name, vendor, std::to_string(cores), structure, std::to_string(vram), memType, std::to_string(bits), std::to_string(bandwidth), 
                                   std::to_string(freq), std::to_string(tgp), graphics};
    }
}

class pixel_cal {
    public:
    static int calculate_monitor(int width, int height, float refresh_rate) {
        int total_pixels = width * height;
        float total_pixels_per_second = total_pixels * refresh_rate;
        return static_cast<int>(total_pixels_per_second);
    }
};

pixel_cal PNV;
pixel_cal PAMD;

#endif // GPU_HPP