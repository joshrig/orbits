#include <iostream>
#include <cstdio>
#include <memory>
#include <cstdint>

#include "BMPLoader.h"


using namespace std;



BMPLoader::BMPLoader(const string& bmpfile) :
    data(nullptr)
{
    FILE *fp = fopen(bmpfile.c_str(), "r");
    if (!fp)
        throw runtime_error("can't open file");

    if (fread(header, 1, 54, fp) != 54)
        throw runtime_error("short file");

    if (header[0] != 'B' || header[1] != 'M')
        throw runtime_error("invalid bmp file");

    data_offset = *(uint32_t *)&(header[0x0A]);
    image_size = *(uint32_t *)&(header[0x22]);
    width = *(uint32_t *)&(header[0x12]);
    height = *(uint32_t *)&(header[0x16]);

    if (image_size == 0)
        image_size = width * height * 3;
    if (data_offset == 0)
        data_offset = 54;

    cout << "data_offset: " << data_offset << endl;
    cout << "image_size: " << image_size << endl;
    cout << "width: " << width << endl;
    cout << "height: " << height << endl;
    
    data.reset<uint8_t>(new uint8_t[image_size], default_delete<uint8_t[]>());

    fread(data.get(), 1, image_size, fp);

    fclose(fp);
}
