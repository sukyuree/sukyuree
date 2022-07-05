#ifdef WIN32
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
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace glm;

void render (GLFWwindow* window);
void init();
void mouseButtonCallback(GLFWwindow* window,int button,int action,int mods);
void cursorMotionCallback(GLFWwindow* window,double xpos,double ypos);

GLuint triangleVBO=0;
GLuint normalVBO=0;
GLuint vertexArrayID=0;
GLuint indexVBOID=0;

GLuint diffTexID=0;
GLuint texCoordVBO=0;
GLuint bumpTexID=0;

Program program;

float cameraDistance = 8;
glm::vec3 sceneCenter =glm::vec3(0,0,0);
float cameraYaw = 0;
float cameraPitch = 0;
float cameraFov = 30;
int lastX=0,lastY=0;

 
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
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorMotionCallback);
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



void init(){
    loadJ3A("apple.j3a");
    int texWidth, texHeight, texChannels;
    
    void *buffer = stbi_load("appleD.jpg",&texWidth,&texHeight,&texChannels,4);
    
    glGenTextures(1,&diffTexID);
    glBindTexture(GL_TEXTURE_2D,diffTexID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D,0,GL_SRGB8_ALPHA8,texWidth,texHeight,0,GL_RGBA,GL_UNSIGNED_BYTE,buffer);
    glGenerateMipmap(GL_TEXTURE_2D);

    
    stbi_image_free(buffer);
    
    buffer = stbi_load("appleD.jpg",&texWidth,&texHeight,&texChannels,4);
    
    glGenTextures(1,&bumpTexID);
    glBindTexture(GL_TEXTURE_2D,bumpTexID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D,0,GL_SRGB8_ALPHA8,texWidth,texHeight,0,GL_RGBA,GL_UNSIGNED_BYTE,buffer);
    glGenerateMipmap(GL_TEXTURE_2D);

    
    stbi_image_free(buffer);
    
    program.loadShaders("shader.vert","shader.frag");
    
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);
    
    glGenBuffers(1,&triangleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferData(GL_ARRAY_BUFFER, nVertices[0]*sizeof(glm::vec3), vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
    
    glGenBuffers(1,&normalVBO);
    glBindBuffer(GL_ARRAY_BUFFER,normalVBO);
    glBufferData(GL_ARRAY_BUFFER,nVertices[0]*sizeof(glm::vec3), normals[0],GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,normalVBO);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);
    
    glGenBuffers(1,&indexVBOID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nTriangles[0]*sizeof(glm::u32vec3), triangles[0], GL_STATIC_DRAW);
    
    glGenBuffers(1,&texCoordVBO);
    glBindBuffer(GL_ARRAY_BUFFER,texCoordVBO);
    glBufferData(GL_ARRAY_BUFFER,nVertices[0]*sizeof(glm::vec2), texCoords[0],GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER,texCoordVBO);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,0,0);
    
    glEnable(GL_DEPTH_TEST);//앞에 것이 먼저 그려져있을 때 뒤에 있는 것을 그리면 안덮어 씌워지게 해주는 것.
    glEnable(GL_FRAMEBUFFER_SRGB);
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
    
    glUseProgram(program.programID);
    mat4 projMat = perspective(cameraFov*3.141592f/180, w/float(h), 0.01f, 100.f);
    
    vec3 cameraPosition = vec3(0,0,cameraDistance);
    cameraPosition = vec3(rotate(cameraPitch,vec3(-1,0,0))*vec4(cameraPosition,1));
    cameraPosition = vec3(rotate(cameraYaw,vec3(0,1,0))*vec4(cameraPosition,1));
    mat4 viewMat = lookAt(cameraPosition, sceneCenter, vec3(0,1,0));
    
    rotAngle+=0.1/180.f*3.141592;
    
    vec3 lightPos = vec3(3,3,3);
    vec3 lightColor = vec3(1);
    vec4 diffuseMaterial = vec4(0,0.7,0,1);
    vec4 specularMaterial = vec4(1);
    vec3 ambientLight = vec3(0.1);
    float shine = 0.f;
    
    GLuint loc = glGetUniformLocation (program.programID, "modelMat");
    glUniformMatrix4fv(loc,1,0,value_ptr(rotate(90/180.f*3.14592f,vec3(1,0,0))));
    
    loc = glGetUniformLocation (program.programID, "viewMat");
    glUniformMatrix4fv(loc,1,0,value_ptr(viewMat));
    
    loc = glGetUniformLocation (program.programID, "projMat");
    glUniformMatrix4fv(loc,1,0,value_ptr(projMat));
    
    loc = glGetUniformLocation(program.programID,"cameraPos");
    glUniform3fv(loc,1,value_ptr(cameraPosition));
    
    loc = glGetUniformLocation(program.programID,"lightPos");
    glUniform3fv(loc,1,value_ptr(lightPos));

    loc = glGetUniformLocation(program.programID,"lightColor");
    glUniform3fv(loc,1,value_ptr(lightColor));

    loc = glGetUniformLocation(program.programID,"diffuseMaterial");
    glUniform4fv(loc,1,value_ptr(diffuseMaterial));

    loc = glGetUniformLocation(program.programID,"specularMaterial");
    glUniform4fv(loc,1,value_ptr(specularMaterial));

    loc = glGetUniformLocation(program.programID,"ambientLight");
    glUniform3fv(loc,1,value_ptr(ambientLight));

    loc = glGetUniformLocation(program.programID,"shine");
    glUniform1f(loc,shine);
    
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D,diffTexID);
    loc = glGetUniformLocation(program.programID,"diffTex");
    glUniform1i(loc,2);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,bumpTexID);
    loc = glGetUniformLocation(program.programID,"bump");
    glUniform1i(loc,1);
    
    
    glBindVertexArray(vertexArrayID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID);
    glDrawElements(GL_TRIANGLES, nTriangles[0]*3, GL_UNSIGNED_INT, 0);
    
    
    glfwSwapBuffers(window);
}

void mouseButtonCallback(GLFWwindow* window,int button,int action,int mods){
    if(button==GLFW_MOUSE_BUTTON_1 && action==GLFW_PRESS){
        double xpos,ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        lastX = int(xpos);
        lastY = int(ypos);
    }
    
}

void cursorMotionCallback(GLFWwindow* window,double xpos,double ypos){
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1)==GLFW_PRESS){
        if(glfwGetKey(window,  GLFW_KEY_LEFT_CONTROL)==GLFW_PRESS){
            cameraFov+=(ypos-lastY)/100;

        }
        else{
            cameraPitch+=(ypos-lastY)/300;
            cameraPitch = glm::clamp(cameraPitch,-1.f,1.f);
            cameraYaw-=(xpos-lastX)/300;
        }
        lastX = int(xpos);
        lastY = int(ypos);
    }
}
