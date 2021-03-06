#ifndef _BMPLOADER_H_
#define _BMPLOADER_H_


#include <iostream>


class BMPLoader
{
    uint8_t header[54];
    uint32_t data_offset;
    uint32_t image_size;

public:
    BMPLoader(const std::string& bmpfile);

    uint32_t width;
    uint32_t height;

    std::shared_ptr<uint8_t> data;
};

#endif
