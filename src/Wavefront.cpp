#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <cstdlib>
#include <cstring>

#include "Wavefront.h"


using namespace std;


Wavefront::Wavefront(const std::string& filename) :
    m_nvertices(0),
    m_nnormals(0),
    m_ntexels(0),
    m_nfaces(0),
    m_vertices(NULL),
    m_faces(NULL),
    m_texels(NULL),
    m_normals(NULL)
{
    m_filename = filename;

    Load_1();
    Load_2();
}


Wavefront::~Wavefront()
{
    if (m_vertices)
    {
        for (int i = 0; i < m_nvertices; i++)
            delete [] m_vertices[i];

        delete [] m_vertices;
    }

    if (m_faces)
    {
        for (int i = 0; i < m_nfaces; i++)
            delete [] m_faces[i];

        delete [] m_faces;
    }

    if (m_texels)
    {
        for (int i = 0; i < m_ntexels; i++)
            delete [] m_texels[i];

        delete [] m_texels;
    }

    if (m_normals)
    {
        for (int i = 0; i < m_nnormals; i++)
            delete [] m_normals[i];

        delete [] m_normals;
    }
}


void Wavefront::Load_1()
{
    ifstream is;
    

    is.open(m_filename);
    if (!is.good())
    {
        cout << "error opening file: " << m_filename << endl;
        throw WavefrontException();
        
        return;
    }

    while (!is.eof())
    {
        string line;
        string type;


        getline(is, line);

        type = line.substr(0, 2);

        if (type.compare("v ") == 0)
            m_nvertices++;
        else if (type.compare("vn") == 0)
            m_nnormals++;
        else if (type.compare("vt") == 0)
            m_ntexels++;
        else if (type.compare("f ") == 0)
            m_nfaces++;
    }

    is.close();


    m_vertices = new float *[m_nvertices];
    for (int i = 0; i < m_nvertices; i++)
        m_vertices[i] = new float[3];
    
    m_faces = new int *[m_nfaces];
    for (int i = 0; i < m_nfaces; i++)
        m_faces[i] = new int[9];

    if (m_ntexels > 0)
    {
        m_texels = new float *[m_ntexels];
        for (int i = 0; i < m_ntexels; i++)
            m_texels[i] = new float[2];
    }

    m_normals = new float *[m_nnormals];
    for (int i = 0; i < m_nnormals; i++)
        m_normals[i] = new float[3];
            

    
    cout << m_nvertices << " vertices" << endl;
    cout << m_nnormals << " normals" << endl;
    cout << m_ntexels << " texels" << endl;
    cout << m_nfaces << " faces" << endl;
}


void Wavefront::Load_2()
{
    ifstream is;
    int v_index = 0;
    int f_index = 0;
    int vt_index = 0;
    int vn_index = 0;



    is.open(m_filename);
    if (!is.good())
    {
        cout << "error opening file: " << m_filename << endl;
        return;
    }

    while (!is.eof())
    {
        string line;
        string type;
        const char *cline;


        getline(is, line);

        type = line.substr(0, 2);

        cline = new char [line.length() + 1];

        strcpy((char *)cline, line.c_str());

        strtok((char *)cline, " ");


        if (type.compare("v ") == 0)
        {
            for (int i = 0; i < 3; i++)
                m_vertices[v_index][i] = strtof(strtok(NULL, " "), NULL);

            v_index++;
        }
        else if (type.compare("f ") == 0)
        {
            for (int i = 0; i < 9; i++)
            {
                if (m_ntexels == 0 && (i == 1 || i == 4 || i == 7))
                {
                    m_faces[f_index][i] = 0.0;
                    continue;
                }
                        
                char *token = strtok(NULL, " /");
                
                m_faces[f_index][i] = strtol(token, NULL, 10);
            }

            f_index++;
        }
        else if (type.compare("vt") == 0)
        {
            for (int i = 0; i < 2; i++)
                m_texels[vt_index][i] = strtof(strtok(NULL, " "), NULL);

            vt_index++;
        }
        else if (type.compare("vn") == 0)
        {
            for (int i = 0; i < 3; i++)
                m_normals[vn_index][i] = strtof(strtok(NULL, " "), NULL);

            vn_index++;
        }
        
        delete cline;
    }

    is.close();


    cout << "loaded " << v_index << " vertices" << endl;
    cout << "loaded " << vn_index << " normals" << endl;
    cout << "loaded " << vt_index << " texels" << endl;
    cout << "loaded " << f_index << " faces" << endl;
}


vector<float> Wavefront::vertices()
{
    enum { X, Y, Z };
    vector<float> vertices;


    for (int i = 0; i < m_nfaces; i++)
    {
        int vA = m_faces[i][0] - 1;
        int vB = m_faces[i][3] - 1;
        int vC = m_faces[i][6] - 1;

        vertices.push_back(m_vertices[vA][X]);
        vertices.push_back(m_vertices[vA][Y]);
        vertices.push_back(m_vertices[vA][Z]);

        vertices.push_back(m_vertices[vB][X]);
        vertices.push_back(m_vertices[vB][Y]);
        vertices.push_back(m_vertices[vB][Z]);

        vertices.push_back(m_vertices[vC][X]);
        vertices.push_back(m_vertices[vC][Y]);
        vertices.push_back(m_vertices[vC][Z]);
    }


    cout << vertices.size() / 3 << " vertices" << endl;


    return vertices;
}

vector<float> Wavefront::uvdata()
{
    enum { U, V };
    vector<float> uvdata;


    for (int i = 0; i < m_nfaces; i++)
    {
        int vA = m_faces[i][1] - 1;
        int vB = m_faces[i][4] - 1;
        int vC = m_faces[i][7] - 1;

        uvdata.push_back(m_texels[vA][U]);
        uvdata.push_back(m_texels[vA][V]);

        uvdata.push_back(m_texels[vB][U]);
        uvdata.push_back(m_texels[vB][V]);

        uvdata.push_back(m_texels[vC][U]);
        uvdata.push_back(m_texels[vC][V]);
    }


    cout << uvdata.size() / 3 << " uv coords" << endl;


    return uvdata;
}
