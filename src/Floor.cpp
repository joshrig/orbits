#include "Floor.h"

using namespace Shapes;

/*
  make a 100m by 100m checkerboard floor with 1m by 1m squares
*/
Floor::Floor(int width, int depth)
{
    m_width = width;
    m_depth = depth;

    if (m_width > 100)
        m_width = 100;

    if (m_depth > 100)
        m_width = 100;

    m_size = m_width * m_depth * 18;
    
    for (int z = 0; z < m_depth; z++)
    {
        for (int x = 0; x < m_width; x++)
        {
            make_square(x, 0, z);
            make_color(x, 0, z);
        }
    }


    glGenBuffers(1, &m_vertex_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_size * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &m_color_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_color_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_size * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    //cout << "size: " << m_size << endl;
}

Floor::~Floor()
{

}

void Floor::make_square(int x, int y, int z)
{
    GLfloat pattern[nverts] = {
        0.0, 0.0, 0.0,
        1.0, 0.0, 1.0,
        0.0, 0.0, 1.0,
        0.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 0.0, 1.0
    };

    int base = (x * nverts) + (z * m_width * nverts);

    //cout << "vertices" << endl;
    for (int p = 0; p < nverts; p += 3)
    {
        m_vertices[base + p + 0] = pattern[p + 0] + x;
        m_vertices[base + p + 1] = pattern[p + 1] + y;
        m_vertices[base + p + 2] = pattern[p + 2] + z;

        //cout << "(" << m_vertices[base + p + 0] << ", " << m_vertices[base + p + 1] << ", " << m_vertices[base + p + 2] << ")" << endl;
    }
}

void Floor::make_color(int x, int y, int z)
{
    int base = (x * nverts) + (z * m_width * nverts);

    //cout << "colors" << endl;
    for (int p = 0; p < nverts; p += 3)
    {
        if (x % 2 || z % 2)
        {
            m_colors[base + p + 0] = 0.243;
            m_colors[base + p + 1] = 0.623;
            m_colors[base + p + 2] = 0.8;
        }
        else
        {            
            m_colors[base + p + 0] = 0.886;
            m_colors[base + p + 1] = 0.890;
            m_colors[base + p + 2] = 0.427;
        }
        
        //cout << "(" << m_colors[base + p + 0] << ", " << m_colors[base + p + 1] << ", " << m_colors[base + p + 2] << ")" << endl;
    }
}

void Floor::render()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_size * sizeof(GLfloat), m_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, m_color_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_size * sizeof(GLfloat), m_colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);
    glDrawArrays(GL_TRIANGLES, 0, m_size / 3);
}


