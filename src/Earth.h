#ifndef EARTH_H
#define EARTH_H

#include <iostream>
#include <vector>
#include <GL/glew.h>

#include "Wavefront.h"
#include "BMPLoader.h"

using namespace std;

namespace Shapes
{
class Earth
{
private:
    BMPLoader m_ldr;
    Wavefront m_wf;
    vector<GLfloat> m_vertices;
    vector<GLfloat> m_uvdata;
    GLuint m_vertex_vbo;
    GLuint m_uv_vbo;
    GLuint m_texture;

public:
    Earth();
    ~Earth();
    void render();
};
}

#endif
