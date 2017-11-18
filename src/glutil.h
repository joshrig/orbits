#ifndef GLUTIL_H
#define GLUTIL_H

#include <iostream>

#include <GL/glew.h>

using namespace std;

namespace glutil
{
    string read_shader(string);
    const char *type_to_string(GLenum);
    bool is_program_valid(GLuint);

    namespace log
    {
	void params();
	void program(GLuint);
	void printall(GLuint);

	bool dbg(const char *, ...);
	bool err(const char *, ...);
    }
};

#endif
