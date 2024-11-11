#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <cstdint>
#include <vector>
#include <string>

bool positive(uint8_t pixelVal) {
    return pixelVal > 128;
}

char hextab[] = {
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    'a',
    'b',
    'c',
    'd',
    'e',
    'f',
};

int main(int argc ,char const* argv[]) {
    if(argc < 2) {
        return 0;
    }
    int w, h, c;
    auto* bitmap = stbi_load(argv[1], &w, &h, &c, 1);
    
    std::vector<uint8_t> exportData;
    int bitpos = 1;
    uint8_t by = 0xff;
    for(int i = 0; i<h; ++i) {
        by = 0xff;
        bitpos = 0;
        for(int j = 0; j<w; ++j) {
            if(positive(bitmap[i*w + j])) {
                by |= 1 << bitpos;
            } else {
                by &= ~(1 << bitpos);
            }
            ++bitpos;
            if(bitpos > 7) {
                exportData.push_back(by);
                by = 0xff;
                bitpos = 0;
            }
        }
    }

    std::string exportCode = "constexpr uint8_t splash[] = {\n";
    int counter = 0;
    for(auto b : exportData) { //
        exportCode.append("0x");
        exportCode.push_back(hextab[b>>4]);
        exportCode.push_back(hextab[b&0xf]);
        exportCode.push_back(',');
        ++counter;
        if((counter & 0xf) == 0) {
            exportCode.push_back('\n');
        }
    }
    exportCode.push_back('}');
    exportCode.push_back(';');
    
    std::string exportPath = std::string(argv[1]) + ".txt";
    auto f = fopen(exportPath.c_str(), "wb+");
    fwrite(exportCode.c_str(), 1, exportCode.length(), f);
    fclose(f);
    return 0;
}