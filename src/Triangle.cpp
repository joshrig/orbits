#include "Triangle.h"

using namespace Shapes;

/*
  make a 100m by 100m checkerboard floor with 1m by 1m squares
*/
Triangle::Triangle()
{
    GLfloat points[] = {
        0.0, 0.0, 0.0,
        1.0, 2.0, 0.0,
        2.0, 0.0, 0.0
    };

    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };
    
    for (int i = 0; i < 9; i++)
    {
        m_vertices[i] = points[i];
        m_colors[i] = colors[i];
    }

    m_size = 9;
    

    glGenBuffers(1, &m_vertex_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), NULL, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &m_color_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_color_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), NULL, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    // cout << "size: " << m_size << endl;
}

Triangle::~Triangle()
{

}

void Triangle::render()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, m_color_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_colors), m_colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, m_size / 3);
}
