#ifndef _LOAD_HPP_
#define _LOAD_HPP_

#include <iostream>


class WavefrontException : public std::runtime_error
{
public:
    WavefrontException() : std::runtime_error("WavefrontException") { }
};



class Wavefront
{
private:
    std::string m_filename;
    
    float **m_vertices;
    int   **m_faces;
    float **m_texels;
    float **m_normals;

    void Load_1();
    void Load_2();

public:
    Wavefront(const std::string&);
    ~Wavefront();


    std::vector<float> vertices();
    std::vector<float> uvdata();
    std::vector<float> normals();

    int m_nvertices;
    int m_nnormals;
    int m_ntexels;
    int m_nfaces;
};



#endif
