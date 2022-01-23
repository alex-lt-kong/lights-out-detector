#include <iostream>
#include <stdint.h>

#define STB_IMAGE_IMPLEMENTATION // This define is a must...
#include "stb_image.h"

using namespace std;

int main() {
    string path = "test.jpg";
    int width, height, channels;

    uint8_t* rgbImage = stbi_load(path.c_str(), &width, &height, &channels, 3);
    if (rgbImage == NULL) {
        cerr << "error!" << endl;
        return 0;
    }
    assert (channels <= 3);
    // This program is not designed to handle an alpha channel
    
    cout << width << "x" << height << "x" << channels << endl;
    
    // In RGB format, white is defined as 255,255,255 while black is defined as 0,0,0
    // the origin of rgbImage is at the upper-left corner;
    int x=10, y=30;
    unsigned bytePerPixel = channels;
    unsigned char* pixelOffset = rgbImage + (x + height * y) * bytePerPixel;
    unsigned char* pixelOffsetTest = &rgbImage[(x + height * y) * bytePerPixel];
    assert (pixelOffset == pixelOffsetTest);
    // Just to be sure that two expressions mean the same thing

    unsigned short r = pixelOffset[0];
    unsigned short g = pixelOffset[1];
    unsigned short b = pixelOffset[2];
    cout << "R" << (int)r << ",G" << g << ",B" << b << endl;

    stbi_image_free(rgbImage);

    return 0;
}