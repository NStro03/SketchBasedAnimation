#include "utils.h"

// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>

#include <array>

// #include <glm/vec3.hpp> // glm::vec3
// #include <glm/vec4.hpp> // glm::vec4
// #include <glm/mat4x4.hpp> // glm::mat4
// #include <glm/gtc/matrix_transform.hpp> 
// #include <glm/glm.hpp>
// #include <glm/gtc/type_ptr.hpp>
// About Desktop OpenGL function loaders:
//  Modern desktop OpenGL doesn't have a standard portable header file to load OpenGL function pointers.
//  Helper libraries are often used for this purpose! Here we are supporting a few common ones (gl3w, glew, glad).
//  You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.

static int ObjectCount = -1;
static float transX = 0.0f, transY = 0.0f, ScaleX = 1.0f, ScaleY = 1.0f, Rotate = 0.0f;

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
        glm::mat4 trans = glm::mat4(1.0f);
    }
    
    ImVec2* getPoints(){
        return &objectPoints[0];
    }

    int getSize(){
        return size;
    }

    void translate(float x, float y)
    {
        x = x * 480;
        y = y * 360; 


        //glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(x, y, 0.0f));
       // vec = trans * vec;
        //std::cout << vec.x  << " " << vec.y  << " " << vec.z << std::endl;

        // translate Every point of our object
        for(int i=0;i<size;i++)
        {
            float x0 = objectPoints[i].x;
            float y0 = objectPoints[i].y;
           // glm::vec4 vec(x0, y0, 0.0f);

            glm::vec4 vec(x0, y0, 0.0f,1.0f);

            vec = trans * vec;
            objectPoints[i].x = vec.x;
            objectPoints[i].y = vec.y;
        }
    }

    void rotate( int X, int Y, float r)
    {

        // Rotation Matrix
        glm::mat4 rot_mat = glm::rotate(glm::mat4(1.0f), r, glm::vec3(0.0, 0.0,1.0));

        
         X = X * -1;
         Y = Y * -1;
        // Translation Matrix
        glm::mat4 trans = glm::mat4(1.0f);
        glm::mat4 trans_mat = glm::translate(trans, glm::vec3(X, Y, 0.0f));


         X = X * -1;
         Y = Y * -1;
        // Invers Translation Matrix
        glm::mat4 invtrans_mat = glm::translate(trans, glm::vec3(X, Y, 0.0f));
        glm::mat4 result_mat = invtrans_mat *  rot_mat * trans_mat;


        // rotate Every point of our object
        for(int i=0;i<size;i++)
        {
            float x0 = objectPoints[i].x;
            float y0 = objectPoints[i].y;
            

            glm::vec4 vec(x0, y0, 0.0f,1.0f);

            // Traslate * rotate * transalte
            vec = result_mat * vec;
            //  objectPoints[i].x = vec.x;
           // objectPoints[i].y = vec.y;

            
            objectPoints[i].x = vec.x;
            objectPoints[i].y = vec.y;

        }
        
    }   


    void scale(int anchorX, int anchorY, float x, float y)
    {
        // Scaling Matrix
        anchorX = anchorX * -1;
        anchorY = anchorY * -1;
        // Translation Matrix
        glm::mat4 trans = glm::mat4(1.0f);
        glm::mat4 trans_mat = glm::translate(trans, glm::vec3(anchorX, anchorY, 0.0f));


        anchorX = anchorX * -1;
        anchorY = anchorY * -1;
        // Invers Translation Matrix
        glm::mat4 invtrans_mat = glm::translate(trans, glm::vec3(anchorX, anchorY, 0.0f));
        glm::mat4 scale_mat = glm::scale(glm::mat4(1.0f), glm::vec3((float)x,(float)y,1.0));
        glm::mat4 result_mat = invtrans_mat *  scale_mat * trans_mat;

         for(int i=0;i<size;i++)
        {
            float x0 = objectPoints[i].x;
            float y0 = objectPoints[i].y;
           // glm::vec4 vec(x0, y0, 0.0f);

            glm::vec4 vec(x0, y0, 0.0f,1.0f);

            vec = result_mat * vec;
            objectPoints[i].x = vec.x;
            objectPoints[i].y = vec.y;
        }

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
