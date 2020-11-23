#include "utils.h"

static void draw_callback(const ImDrawList* parent_list, const ImDrawCmd* cmd)
{
    printf("hello!\n");
    {
        // static GLuint programID = LoadShaders( "/Users/moritanaoki/Desktop/rendering-in-imgui/SimpleVertexShader.vert", "/Users/moritanaoki/Desktop/rendering-in-imgui/SimpleFragmentShader.frag" );
        // glViewport(0, 0, 300, 300);

        //glUseProgram(programID);

        GLuint VertexArrayID;
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        static const GLfloat g_vertex_buffer_data[] = {
                -1.0f, -1.0f, 0.0f,
                1.0f, -1.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
        };

        GLuint vertexbuffer;
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void *) 0            // array buffer offset
        );
        glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
        glDisableVertexAttribArray(0);
    }
}