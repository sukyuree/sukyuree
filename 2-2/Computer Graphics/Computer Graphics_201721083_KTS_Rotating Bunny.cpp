// ConsoleApplication1.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
#ifdef WIN32
//  #include
    #define GLEW_STATIC
    #include <GL/glew.h>
    #define GLFW_INCLUDE_NONE
#elif defined __APPLE__
    #pragma clang diagnostic ignored "-Wdocumentation"
    #include <OpenGL/gl3.h>
#endif

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "j3a.hpp"
#include "j3a.cpp"

#ifdef WIN32
#pragma comment (lib, "glew32s")
#pragma comment (lib, "opengl32")
#pragma comment (lib, "glfw3")
#endif
#include <vector>
#define GLM_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "toys.h"
using namespace glm;

void render (GLFWwindow* window);
void init();

GLuint triangleVBO;
GLuint vertexArrayID;
GLuint indexVBOID;
Program program;

int main(int argc, const char * argv[]){
    glfwInit();
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow* window = glfwCreateWindow(1000,800,"Test",0,0);
    glfwMakeContextCurrent(window);
#ifdef WIN32
    glewInit();
#endif
    init();
    while(!glfwWindowShouldClose(window)){
        render(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

#ifdef WIN32
#define PATH "../../Assign2/Assign2/"
#else
#define PATH ""
#endif


void init(){
    loadJ3A("bunny.j3a");
    program.loadShaders("shader.vert","shader.frag");
    
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);
    
    glGenBuffers(1,&triangleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferData(GL_ARRAY_BUFFER, nVertices[0]*sizeof(glm::vec3), vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
    
    glGenBuffers(1,&indexVBOID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nTriangles[0]*sizeof(glm::u32vec3), triangles[0], GL_STATIC_DRAW);
}

float rotAngle = 0;

void render(GLFWwindow* window){
    int w,h;
    glfwGetFramebufferSize(window, &w,&h);
    glViewport(0,0,w,h);
    glClearColor(0,0,.5,0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glUseProgram( program.programID);
    glEnable(GL_DEPTH_TEST);
    
    rotAngle+=0.1/180.f*3.141592;
    GLuint loc = glGetUniformLocation (program.programID, "modelMat");
    mat4 rotMat = rotate(rotAngle, glm::vec3(0,1,0));
    glUniformMatrix4fv(loc,1,0,value_ptr(rotMat));
    
    glBindVertexArray(vertexArrayID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID);
    glDrawElements(GL_TRIANGLES, nTriangles[0]*3, GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);
}

