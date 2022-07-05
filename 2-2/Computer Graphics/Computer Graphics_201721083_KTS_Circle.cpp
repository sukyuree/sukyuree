// ConsoleApplication1.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.

#if __APPLE__
#include <OpenGL/gl3.h>
#else
#include <gl/glew.h>
#include <GL
#endif
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <math.h>
#include "toys.h"
#include <vector>

void render (GLFWwindow* window);
void init();

int main(int argc, const char * argv[]){
    glfwInit();
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow* window = glfwCreateWindow(700,700,"Test",0,0);
    
    glfwMakeContextCurrent(window);
//#ifndef __APPLE__
//    glewInit();
//#endif
    
    init();
    
    while(!glfwWindowShouldClose(window)){
        render(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

GLuint triBuffer = 0;
GLuint va = 0;
GLuint vao = 0;
GLuint elementArray = 0;
int how_many = 0;
Program program;

using namespace glm;
using namespace std;
void init(){
 
    
    program.loadShaders("shader.vert","shader.frag");
    double rad = 1.0;
    std::vector<glm::vec3> triVertData;
    std::vector<glm::uvec3> triData;
    triVertData.push_back(vec3(0,0,0));
    how_many +=3;
    for(double i=0;i<=360;i++){
        double angle = (i/180)*3.149258;
        double x = (double)rad*cos(angle);
        double y = (double)rad*sin(angle);
        triVertData.push_back(vec3(x,y,0));
        triData.push_back(uvec3(0,i,i+1));
        how_many+=3;
    }
    
    glGenBuffers(1,&triBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, triBuffer);
    glBufferData(GL_ARRAY_BUFFER, triVertData.size()*sizeof(vec3),triVertData.data(),GL_STATIC_DRAW);
    
    glGenVertexArrays(1,&va);
    glBindVertexArray(va);
    glBindBuffer(GL_ARRAY_BUFFER,triBuffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,nullptr);
    
    glGenBuffers(1,&elementArray);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArray);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triData.size()*sizeof(uvec3),triData.data(), GL_STATIC_DRAW);
}

void render(GLFWwindow* window){
    int w,h;
    glfwGetFramebufferSize(window,&w, &h);
    glViewport(0,0,w,h);
    glClearColor(0, 0, .5, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program.programID);
    glBindVertexArray(va);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,elementArray);
    glDrawElements(GL_TRIANGLES,how_many,GL_UNSIGNED_INT,0);
    glfwSwapBuffers(window);
}

