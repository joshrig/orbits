#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <iostream>
#include <GL/glew.h>

using namespace std;

namespace Shapes
{
class Triangle
{
private:
    GLfloat m_vertices[3 * 3];
    GLfloat m_colors[3 * 3];
    int m_size;
    GLuint m_vertex_vbo;
    GLuint m_color_vbo;

public:
    Triangle();
    ~Triangle();
    void render();
};
}

#endif
