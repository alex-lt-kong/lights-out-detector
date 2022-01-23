#include <iostream>
#include <stdint.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION // This define is a must...
#include "stb_image.h"

using namespace std;

int main(int argc, char *argv[]) {

    string path = "test.jpg";
    if (argc == 2) {
        path = argv[1];
        // argv[0] is the path of the program;
    }    
    int width, height, channels;

    uint8_t* rgbImage = stbi_load(path.c_str(), &width, &height, &channels, 3);
    if (rgbImage == NULL) {
        cerr << "Error, image array is NULL" << endl;
        return 2;
    }
    assert (channels <= 3);
    // This program is not designed to handle an alpha channel
    
    cout << "Metadata: H" << height << "W" << width << "C" << channels << endl;
    
    // In RGB format, white is defined as 255,255,255 while black is defined as 0,0,0
    // This makes sense if we consider the value as "level of energy", so black 
    // implies 0 enery.
    // the origin of rgbImage is at the upper-left corner;
    // Original logic comes from: https://stackoverflow.com/questions/48235421/get-rgb-of-a-pixel-in-stb-image
    int x=1, y=3;
    unsigned bytePerPixel = channels;
    // Note this special variable assignment: each byte in memory has its own
    // address. While it is possible for JPEG to have color depth other than
    // 8 bits, stbi_load always returns 8-bit (i.e. 1-byte) deep colors.

    unsigned char* pixelOffset = rgbImage + (x + height * y) * bytePerPixel;
    uint8_t* pixelOffsetTest = &rgbImage[(x + height * y) * bytePerPixel];
    assert (pixelOffset == pixelOffsetTest);
    // Just to be sure that two expressions mean the same thing

    unsigned char r = pixelOffset[0];
    uint8_t g = pixelOffset[1];
    unsigned short b = pixelOffset[2];
    // By definition, chars are just small (8-bit) integers so the above 3
    // definitions are all okay. However, it appears that cout cannot directly
    // print uint8_t and unsigned char, so the original version of this snippet
    // uses unsigned short (16-bit) instead.
    //cout << "R" << (unsigned short)r << ",G" << (unsigned short)g << ",B" << b << endl;
    
    // acout << sizeof(rgbImage) << endl;
    // sizeof() only works if rgbImage is defined as an array--while somehow
    // arrays can be considered as a pointer, if you explicitly define an array
    // as a pointer pointing to a few blocks of memory, sizeof() won't work--it
    // will, instead, only return the size of that pointer.
    // ICYW: The loss of type and dimensions of an array is known as decay of an array.
    
    int imageSize = width * height * channels;
    int absBrightness = 0;
    float relativeBrightness = 0.0, threshold = 0.1;
    // >>> 1920 * 1080 * 3 / 2147483647
    // 0.00289678
    for (int i = 0; i < imageSize; i++) {
        absBrightness += rgbImage[i];
    }
    relativeBrightness = absBrightness / (imageSize * 255.0);
    cout << "Brightness: " << absBrightness << "(Absolute), " << relativeBrightness << "(Relative)" << endl;
    
    // experiment results:
    // dark: <= 0.1
    stbi_image_free(rgbImage);

    cout << "Status: " << (relativeBrightness > threshold ? "On" : "Off") << endl;
    // The definition of cout follows the pattern of JPEG's color scheme
    return 0;
    
}