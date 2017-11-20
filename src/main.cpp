#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <iostream>
#include <vector>

#include <cmath>
#include <cstdio>

#include "glutil.h"
#include "Floor.h"
#include "Triangle.h"
#include "Earth.h"

using namespace glutil;
using namespace std;
using namespace Shapes;

// static int g_gl_width = 1280;
// static int g_gl_height = 1024;


void _update_fps_counter(GLFWwindow *window)
{
    static double previous_seconds = 0;
    static int frame_count = 0;
    double current_seconds;
    double elapsed_seconds;


    current_seconds = glfwGetTime();
    elapsed_seconds = current_seconds - previous_seconds;

    // limit the display cadence
    if (elapsed_seconds > 0.25)
    {
        previous_seconds = current_seconds;
        char tmp[100];

        double fps = (double)frame_count / elapsed_seconds;
        sprintf(tmp, "opengl @ fps: %.2f", fps);
        glfwSetWindowTitle(window, tmp);

        frame_count = 0;
    }

    frame_count++;
}


void glfw_error_callback(int error, const char *desc)
{
    log::err("GLFW code %d msg %s\n", error, desc);
}

// void glfw_window_size_callback(GLFWwindow *window, int width, int height)
// {
//     g_gl_width = width;
//     g_gl_height = height;

//     /* XXX update perspective matrices here */
// }


int main()
{
    glfwSetErrorCallback(glfw_error_callback);
    
    if (!glfwInit())
    {
        log::err("Could not start GLFW\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, 4);


    // full screen
    // GLFWmonitor *mon = glfwGetPrimaryMonitor();
    // const GLFWvidmode *vmode = glfwGetVideoMode(mon);
    // GLFWwindow *window = glfwCreateWindow(vmode->width,
    //                                    vmode->height,
    //                                    "Hello Triangle",
    //                                    mon,
    //                                    NULL);
    GLFWwindow *window = glfwCreateWindow(1280,
                                          1280,
                                          "Hello Triangle",
                                          NULL,
                                          NULL);
    if (!window)
    {
        log::err("Could not open window with GLFW\n");
        glfwTerminate();
        return 1;
    }

    //glfwSetWindowSizeCallback(window, glfw_window_size_callback));

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    log::params();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);


    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    
#if 0
    Floor floor;
    Triangle triangle;
#endif
    Earth earth;

    
    int params = -1;
    string vs_src = read_shader("scene1.vert");
    string fs_src = read_shader("scene1.frag");

    const char *vs_src_p = vs_src.c_str();
    const char *fs_src_p = fs_src.c_str();
    

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_src_p, NULL);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &params);
    if (params != GL_TRUE)
    {
        log::err("Vertex shader did not compile.\n");
//        return -1;
    }

    
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_src_p, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &params);
    if (params != GL_TRUE)
    {
        log::err("Fragment shader did not compile.\n");
//        return -1;
    }


    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vs);
    glAttachShader(shader_program, fs);



    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &params);
    if (params != GL_TRUE)
    {
        log::err("Could not link shader program.");
//        return -1;
    }

    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    // glFrontFace(GL_CCW);


    log::printall(shader_program);

    glm::vec3 camera_pos(0, 0, -2);
    glm::vec3 camera_foc = glm::normalize(glm::vec3(0, 0, 1));
    glm::vec3 camera_up = glm::normalize(glm::vec3(0, 1, 0));

    // move the camera to the right location
    glm::mat4 camera = glm::lookAt(camera_pos, camera_pos + camera_foc, camera_up);

    // create the frustrum
    glm::mat4 proj = glm::perspective<float>(90.0,      // fov angle
                                             1,         // aspect ratio
                                             0.0001,    // near
                                             1000.0);   // far

#if 0
    // translate 
    glm::mat4 floor_xlate = glm::translate(glm::mat4(1.0f), glm::vec3(-50, 0, -50));
    glm::mat4 tri_xlate = glm::translate(glm::mat4(1.0f), glm::vec3(-1, 0, 0));

    // combine the transformations
    glm::mat4 floor_mvp = proj * camera * floor_xlate;
    glm::mat4 tri_mvp = proj * camera * tri_xlate;
#endif
    glm::mat4 earth_xlate(1.0);
    glm::mat4 earth_mvp = proj * camera;
    
    // get the location of the uniform variable
    int mvp_loc = glGetUniformLocation(shader_program, "mvp");


    double curtime;
    double prevtime = glfwGetTime();
    double dt;
    bool moved = false;
    float camera_lv = 10.0; // m/s
    float camera_rv = 0.125; // rad/s
    while (!glfwWindowShouldClose(window))
    {
        curtime = glfwGetTime();
        dt = curtime - prevtime;
        prevtime = curtime;


        _update_fps_counter(window);

        
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, 1);
            break;
        }

        int ps4_cntl_present = glfwJoystickPresent(GLFW_JOYSTICK_1);

        if (ps4_cntl_present)
        {
            int count;
            const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);


            float x_val = axes[0] * -1;
            float y_val_dn = axes[4] * -1;
            float y_val_up = axes[5];
            float z_val = axes[1] * -1;
            float rot_val = axes[2] * -1;
            // float rot_val2 = axes[3] * -1;

            if (x_val >= 0.1)
            {
                // find the vector orthogonal to the up/focus plane
                // and normalize it.
                glm::vec3 left = glm::normalize(glm::cross(camera_foc, camera_up));
                // now scale it by the distance traveled in this dt
                left *= camera_lv * x_val * (float)dt;
                // now move the camera to where "left" points.
                camera_pos -= left;
                moved = true;
            }
            else if (x_val <= -0.1)
            {
                glm::vec3 right = glm::normalize(glm::cross(camera_up, camera_foc));
                right *= camera_lv * x_val * (float)dt;
                camera_pos += right;
                moved = true;
            }
            if (y_val_up >= 0.1)
            {
                camera_pos += camera_up * camera_lv * y_val_up * (float)dt;
                moved = true;
            }
            if (y_val_dn <= -0.1)
            {
                camera_pos += camera_up * camera_lv * y_val_dn * (float)dt;
                moved = true;
            }
            if (z_val >= 0.1 || z_val <= -0.1)
            {
                camera_pos += camera_foc * camera_lv * z_val * (float)dt;
                moved = true;
            }
            if (rot_val >= 0.1 || rot_val <= -0.1)
            {
                camera_foc = glm::rotate(camera_foc, camera_rv * rot_val, camera_up);
                moved = true;
            }
            // if (rot_val2 >= 0.1 || rot_val2 <= -0.1)
            // {
            //     glm::vec3 right = glm::normalize(glm::cross(camera_up, camera_foc));
            //     camera_foc = glm::rotate(right, camera_rv * rot_val, camera_foc);
            //     camera_up = glm::normalize(glm::cross(right, camera_foc));
            //     moved = true;
            // }
        }
        else
        {
            if (glfwGetKey(window, GLFW_KEY_UP))
            {
                camera_pos += camera_foc * camera_lv * (float)dt;
                moved = true;
            }
            if (glfwGetKey(window, GLFW_KEY_DOWN))
            {
                camera_pos -= camera_foc * camera_lv * (float)dt;
                moved = true;
            }
            if (glfwGetKey(window, GLFW_KEY_RIGHT))
            {
                glm::vec3 left = glm::normalize(glm::cross(camera_foc, camera_up));
                left *= camera_lv * (float)dt;
                camera_pos += left;
                moved = true;
            }
            if (glfwGetKey(window, GLFW_KEY_LEFT))
            {
                glm::vec3 right = glm::normalize(glm::cross(camera_up, camera_foc));
                right *= camera_lv * (float)dt;
                camera_pos += right;
                moved = true;
            }
        }

        
        if (moved)
        {
            moved = false;

            camera = glm::lookAt(camera_pos, camera_pos + camera_foc, camera_up);

#if 0            
            floor_mvp = proj * camera * floor_xlate;
            tri_mvp = proj * camera * tri_xlate;
#endif
        }        
        earth_mvp = proj * camera * earth_xlate;
        earth_xlate = glm::rotate(glm::mat4(1.0),
                                  camera_rv * (float)curtime * 5.0f,
                                  glm::vec3(0.0, 1.0, 0.0));

        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // glViewport(0, 0, g_gl_width, g_gl_height);


        // drawing code
        glUseProgram(shader_program);

        glBindVertexArray(vao);

#if 0
        glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(floor_mvp));
        floor.render();

        glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(tri_mvp));
        triangle.render();
#endif
        glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(earth_mvp));
        earth.render();
        
        glfwSwapBuffers(window);
    }
    

    glfwTerminate();

    return 0;
}
