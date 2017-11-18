#ifndef FLOOR_H
#define FLOOR_H

#include <iostream>
#include <GL/glew.h>

using namespace std;

namespace Shapes
{
    const int nverts = 18;

class Floor
{
private:
    GLfloat m_vertices[100 * 100 * 6 * 3];
    GLfloat m_colors[100 * 100 * 6 * 3];
    GLint m_width;
    GLint m_depth;
    int m_size;
    GLuint m_vertex_vbo;
    GLuint m_color_vbo;

    void make_square(int, int, int);
    void make_color(int, int, int);
public:
    Floor(int width = 100, int depth = 100);
    ~Floor();
    void render();
};
}

#endif
