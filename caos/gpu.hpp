#ifndef GPU_HPP
#define GPU_HPP

#include <string>

using namespace std;

namespace gpu {
    void details(const std::string& name, bool NVIDIA, int cores, const std::string& structure, int vram, 
                 const std::string& memType, int bits, int bandwidth, float freq, int tgp, const std::string& graphics) {
        string vendor = NVIDIA ? "NVIDIA" : "AMD";
        string gpudetails[] = {name, vendor, to_string(cores), structure, to_string(vram), memType, to_string(bits), to_string(bandwidth), 
                               to_string(freq), to_string(tgp), graphics};
    }
}

#endif // GPU_HPP