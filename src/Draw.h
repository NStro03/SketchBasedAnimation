#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>            // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>            // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>          // Initialize with gladLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
#include <glad/gl.h>            // Initialize with gladLoadGL(...) or gladLoaderLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
#define GLFW_INCLUDE_NONE       // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#include <glbinding/Binding.h>  // Initialize with glbinding::Binding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
#define GLFW_INCLUDE_NONE       // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#include <glbinding/glbinding.h>// Initialize with glbinding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <iostream>
#include <vector>
#include <array>

struct Object
{
    std::vector<ImVec2> currObjectPoints;
    int size;
    int Objectid;
    Object(int i) 
    { 
        size = 0;
        Objectid = i;
    }
};
// static void cursorPositionCallback(GLFWwindow *canvas, double xPos, double yPos);
// void cursorOnCanvasCallback(GLFWwindow *canvas, int entered);
// bool insideCanvas = false;
// void mouseButtonCallback(GLFWwindow *canvas, int button, int action, int mods);
bool leftMouseDown = false;

std::vector<ImVec2> currObjectPoints;
int size = 0;

std::vector<Object> Objects;
static int Objectcount=-1;

double x,y;
void draw(GLFWwindow *canvas){
    // glfwSetCursorPosCallback(canvas, cursorPositionCallback);
    // glfwSetInputMode(canvas, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    // glfwSetCursorEnterCallback(canvas, cursorOnCanvasCallback);
    // glfwSetMouseButtonCallback(canvas, mouseButtonCallback);
    // glfwSetInputMode(canvas, GLFW_STICKY_MOUSE_BUTTONS, 1);     // To make sure that a mouse eent is always detected. Done by glfw by setting and resetting flags.
    glfwGetCursorPos(canvas, &x, &y);
    // printf("%s\n", leftMouseDown ? "true" : "false");
    if(leftMouseDown){
        ImVec2 s(x,y);
        Objects[Objectcount].currObjectPoints.push_back(s);
        //size++;
        Objects[Objectcount].size++;
    }
}
void setleftmouseDown(ImGuiIO io){
    if(ImGui::GetIO().MouseDown[0]){
        // std::cout << "Location "<< io.MousePos.x << " "
        //      <<" " << io.MousePos.y << " " << std::endl  ;
        if(io.MousePos.x>0 && io.MousePos.x<960 && io.MousePos.y>0 && io.MousePos.y<720){
            if(!leftMouseDown){
                //printf("%d %d\n", Objects.size(), Objectcount);
                Objectcount++;
                
                Object o(Objectcount);
                Objects.push_back(o);
            }
            leftMouseDown = true;
        }
        else{
            leftMouseDown = false;
        }
    }
    else{
        leftMouseDown = false;
    }
}


// static void cursorPositionCallback(GLFWwindow *canvas, double xPos, double yPos){
   
// }

// void cursorOnCanvasCallback(GLFWwindow *canvas, int entered){
//     if(entered)
//         insideCanvas = true;
//     else
//         insideCanvas = false;
    
// }

// void mouseButtonCallback(GLFWwindow *canvas, int button, int action, int mods){
//     if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
//         leftMouseDown = true;
        
//         glfwGetCursorPos(canvas, &x, &y);
//         std::cout<<"Leftdown x: "<< x <<" , y: " << y << std::endl;
//     }

//     if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
//         glfwGetCursorPos(canvas, &x, &y);
//         std::cout<<"LeftUp x: "<< x <<" , y: " << y << std::endl;
//         leftMouseDown = false;
//     }
// }
