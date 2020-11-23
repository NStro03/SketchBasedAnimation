#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <iostream>
#include <vector>
// About Desktop OpenGL function loaders:
//  Modern desktop OpenGL doesn't have a standard portable header file to load OpenGL function pointers.
//  Helper libraries are often used for this purpose! Here we are supporting a few common ones (gl3w, glew, glad).
//  You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
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

int pixelObjectMap[961][721];
int selected = 0, hovered = 0;
static int getObjectid(int x, int y){
	int i = x-1, s=0,j=y-1;
	for(;i<=x+1;i++){
		for(;j<=y+1;j++)
			if(pixelObjectMap[i][j]>s)
				s=pixelObjectMap[i][j];
	}
    return s;
}
static void selectCurve(ImGuiIO io)
{
    int x,y;
    if(ImGui::GetIO().MouseClicked[0]){
        ImVec2 distFromClick(io.MouseClickedPos[0]);
        x = (int)(distFromClick.x);
        y = (int)(distFromClick.y);
        if(x>0 && x<960 && y>0 && y<720){
        	// printf("%d %d\n", x, y);
        	int objid = getObjectid(x,y);
        	if(objid!=0){
            	selected = objid;
        	}
    	}
    }
    x = (int)(io.MousePos.x);
    y = (int)(io.MousePos.y);
    hovered = 0;
    if(x>0 && x<960 && y>0 && y<720){
    	int objid = getObjectid(x,y);
    	if(objid!=0){
        	hovered = objid;
    	}
	}
}