#include "BMPLoader.h"

#include "Earth.h"

using namespace Shapes;

/*
  render a textured earth!
*/
Earth::Earth() :
    m_wf("earth.obj"),
    m_ldr("earth.bmp")
{
    m_vertices = m_wf.vertices();
    m_uvdata = m_wf.uvdata();
    m_normals = m_wf.normals();

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 m_ldr.width,
                 m_ldr.height,
                 0,
                 GL_BGR,
                 GL_UNSIGNED_BYTE,
                 m_ldr.data.get());
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glGenBuffers(1, &m_vertex_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 m_vertices.size() * sizeof(GLfloat),
                 NULL,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &m_uv_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_uv_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 m_uvdata.size() * sizeof(GLfloat),
                 NULL,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &m_normal_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_normal_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 m_normals.size() * sizeof(GLfloat),
                 NULL,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);

    // cout << "size: " << m_size << endl;
}

Earth::~Earth()
{

}

void Earth::render()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 m_vertices.size() * sizeof(GLfloat),
                 m_vertices.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, m_uv_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 m_uvdata.size() * sizeof(GLfloat),
                 m_uvdata.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, m_normal_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 m_normals.size() * sizeof(GLfloat),
                 m_normals.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);

    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size() / 3);
}
