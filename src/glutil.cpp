#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdarg>
#include <cstdio>
#include <cstring>

#include "glutil.h"



using namespace glutil;



string glutil::read_shader(string filename)
{
    string shader_code;
    ifstream shader_stream(filename, ios::in);

    if (shader_stream.is_open())
    {
        string line = "";

        while (getline(shader_stream, line))
            shader_code += "\n" + line;

        shader_stream.close();
    }

    return shader_code;
}


void glutil::log::printall(GLuint program)
{
    dbg("shader program %i info:\n", program);
    int params = -1;

    glGetProgramiv(program, GL_LINK_STATUS, &params);
    dbg("GL_LINK_STATUS = %i\n", params);

    glGetProgramiv(program, GL_ATTACHED_SHADERS, &params);
    dbg("GL_ATTACHED_SHADERS = %i\n", params);

    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &params);
    dbg("GL_ACTIVE_ATTRIBUTES = %i\n", params);

    for (GLuint i = 0; i < (GLuint)params; i++)
    {
	int max_length = 100;
	char name[max_length];
	int actual_length = 0;
	int size = 0;
	GLenum type;

	glGetActiveAttrib(program,
			  i,
			  max_length,
			  &actual_length,
			  &size,
			  &type,
			  name);
	if (size > 1)
	{
	    for (int j = 0; j < size; j++)
	    {
		char long_name[64];

		sprintf(long_name, "%s[%i]", name, j);

		int location = glGetAttribLocation(program, long_name);

		dbg("   %i) type: %s  name: %s  location: %i\n", i,
		    type_to_string(type), long_name, location);
	    }
	}
	else
	{
	    int location = glGetAttribLocation(program, name);
	    dbg("   %i) type: %s  name: %s  location: %i\n", i,
		type_to_string(type), name, location);
	}
    }


    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &params);
    dbg("GL_ACTIVE_UNIFORMS = %i\n", params);

    for (GLuint i = 0; i < (GLuint)params; i++)
    {
	int max_length = 100;
	char name[max_length];
	int actual_length = 0;
	int size = 0;
	GLenum type;

	glGetActiveUniform(program,
			   i,
			   max_length,
			   &actual_length,
			   &size,
			   &type,
			   name);

	if (size > 1)
	{
	    for (int j = 0; j < size; j++)
	    {
		char long_name[64];

		sprintf(long_name, "%s[%i]", name, j);

		int location = glGetUniformLocation(program, long_name);
		dbg("   %i) type: %s  name: %s  location: %i\n", i,
		    type_to_string(type), long_name, location);
	    }
	}
	else
	{
	    int location = glGetUniformLocation(program, name);

	    dbg("   %i), type: %s  name: %s  location: %i\n", i,
		type_to_string(type), name, location);
	}
    }

    dbg("----------------------------------------------------\n");
    log::program(program);
}


const char *glutil::type_to_string(GLenum type)
{
    switch (type)
    {
    case GL_BOOL: return "bool";
    case GL_INT: return "int";
    case GL_FLOAT: return "float";
    case GL_FLOAT_VEC2: return "vec2";
    case GL_FLOAT_VEC3: return "vec3";
    case GL_FLOAT_VEC4: return "vec4";
    case GL_FLOAT_MAT2: return "mat2";
    case GL_FLOAT_MAT3: return "mat3";
    case GL_FLOAT_MAT4: return "mat4";
    case GL_SAMPLER_2D: return "sampler2D";
    case GL_SAMPLER_3D: return "sampler3D";
    case GL_SAMPLER_CUBE: return "samplerCube";
    case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";
    default: break;
    }

    return "other";
}


bool glutil::is_program_valid(GLuint program)
{
    int params = -1;

    
    glValidateProgram(program);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &params);
    log::dbg("program %i GL_VALIDATE_STATUS = %i\n", program, params);
    if (params != GL_TRUE)
    {
	log::program(program);
	return false;
    }

    return true;
}


void glutil::log::program(GLuint program)
{
    int max_length = 2048;
    int actual_length = 0;
    char log[max_length];


    glGetProgramInfoLog(program, max_length, &actual_length, log);

    if (strlen(log) > 0)
    {
	dbg("program info log for GL index %u:\n%s", program, log);
	dbg("----------------------------------------------------\n");
    }
}


void glutil::log::params()
{
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);

    dbg("Renderer: %s\n", renderer);
    dbg("OpenGL version supported: %s\n\n", version);


    GLenum params[] = { GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
			GL_MAX_CUBE_MAP_TEXTURE_SIZE,
			GL_MAX_DRAW_BUFFERS,
			GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
			GL_MAX_TEXTURE_IMAGE_UNITS,
			GL_MAX_TEXTURE_SIZE,
			GL_MAX_VARYING_FLOATS,
			GL_MAX_VERTEX_ATTRIBS,
			GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
			GL_MAX_VERTEX_UNIFORM_COMPONENTS,
			GL_MAX_VIEWPORT_DIMS,
			GL_STEREO, };

    const char* names[] = { "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
			    "GL_MAX_CUBE_MAP_TEXTURE_SIZE",
			    "GL_MAX_DRAW_BUFFERS",
			    "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
			    "GL_MAX_TEXTURE_IMAGE_UNITS",
			    "GL_MAX_TEXTURE_SIZE",
			    "GL_MAX_VARYING_FLOATS",
			    "GL_MAX_VERTEX_ATTRIBS",
			    "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
			    "GL_MAX_VERTEX_UNIFORM_COMPONENTS",
			    "GL_MAX_VIEWPORT_DIMS",
			    "GL_STEREO", };

    dbg("GL Context Params:\n");
    
    // integers - only works if the order is 0-10 integer return types
    for (int i = 0; i < 10; i++)
    {
	int v = 0;

	glGetIntegerv (params[i], &v);

	dbg("%35s %i\n", names[i], v);
    }

    // others
    int v[2];
    v[0] = v[1] = 0;

    glGetIntegerv(params[10], v);

    dbg("%35s %i %i\n", names[10], v[0], v[1]);

    unsigned char s = 0;

    glGetBooleanv(params[11], &s);

    dbg("%35s %u\n", names[11], (unsigned int)s);
    dbg("----------------------------------------------------\n");
}


bool glutil::log::dbg(const char *message, ...)
{
    va_list argptr;

    fprintf(stdout, "DEBUG: ");
    va_start(argptr, message);
    vfprintf(stdout, message, argptr);
    va_end(argptr);

    return true;
}


bool glutil::log::err(const char *message, ...)
{
    va_list argptr;

    fprintf(stdout, "ERROR: ");
    va_start(argptr, message);
    vfprintf(stderr, message, argptr);
    va_end(argptr);

    return true;
}
