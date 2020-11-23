#include "utils.h"

// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>

#include <array>

static int ObjectCount = -1;

class PlObject
{
private:
    std::vector<ImVec2> objectPoints;
    int size;
    int Objectid;
public:
    PlObject(){
        size = 0;
        ObjectCount++;
        Objectid = ObjectCount+1;
    }
    
    void addPoint(ImVec2 p){
        objectPoints.push_back(p);
        size++;
        pixelObjectMap[(int)(p.x)][(int)(p.y)] = Objectid;
        // printf("%d %d\n", (int)p.x, (int)p.y);
    }
    
    ImVec2* getPoints(){
        return &objectPoints[0];
    }

    int getSize(){
        return size;
    }
};

// struct Object
// {
//     std::vector<ImVec2> currObjectPoints;
//     int size;
//     int Objectid;
//     Object(int i) 
//     { 
//         size = 0;
//         Objectid = i;
//     }
// };

// static void cursorPositionCallback(GLFWwindow *canvas, double xPos, double yPos);
// void cursorOnCanvasCallback(GLFWwindow *canvas, int entered);
// bool insideCanvas = false;
// void mouseButtonCallback(GLFWwindow *canvas, int button, int action, int mods);
bool leftMouseDown = false;

// std::vector<ImVec2> currObjectPoints;
// int size = 0;

std::vector<PlObject> PlObjects;

double x,y;
double prevX,prevY;
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
        if(x == prevX && y == prevY)
            return;
        PlObjects[ObjectCount].addPoint(s);
        //size++;
        // PlObjects[ObjectCount].size++;
    }
}
void setleftmouseDown(ImGuiIO io){
    if(ImGui::GetIO().MouseDown[0]){
        // std::cout << "Location "<< io.MousePos.x << " "
        //      <<" " << io.MousePos.y << " " << std::endl  ;
        if(io.MousePos.x>0 && io.MousePos.x<960 && io.MousePos.y>0 && io.MousePos.y<720){
            if(!leftMouseDown){
                //printf("%d %d\n", Objects.size(), Objectcount);
                // ObjectCount++;
                
                //PlObject o;
                PlObjects.push_back(PlObject());
            }
            prevX = io.MousePos.x;
            prevY = io.MousePos.y;
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
