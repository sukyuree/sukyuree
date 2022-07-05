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
#include <glm/gtx/projection.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "toys.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace glm;

void render (GLFWwindow* window);
void init();
void cursorCallback(GLFWwindow* window, double x, double y);
void refreshCallback(GLFWwindow* window);
void scrollCallback(GLFWwindow* window,double dx,double dy);
void buttonCallback(GLFWwindow* window,int button, int action, int mods);


GLuint triangleVBO=0;
GLuint normalVBO=0;
GLuint vertexArrayID=0;
GLuint indexVBOID=0;

GLuint diffTexID=0;
GLuint texCoordVBO=0;
GLuint bumpTexID=0;


GLuint shadowTex = 0;
GLuint shadowFBO = 0;
GLuint shadowDepth = 0;

Program program;
Program shadowprogram;

float cameraDistance = 8;
float cameraYaw = 0;
float cameraPitch = 0;
float cameraFov = 30;
double lastX, lastY;

vec3 lightPos = vec3(3,3,3);
vec3 lightColor = vec3(1);
vec4 diffuseMaterial = vec4(0,0.7,0,1);
vec4 specularMaterial = vec4(1);
vec3 ambientLight = vec3(0.1);
float shine = 0.f;


int main(int argc, const char * argv[]){
    glfwInit();
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow* window = glfwCreateWindow(640,480,"Test",0,0);
    glfwMakeContextCurrent(window);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetMouseButtonCallback(window, buttonCallback);
    glfwSetCursorPosCallback(window, cursorCallback);
    glfwSetWindowRefreshCallback(window, refreshCallback);
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
    loadJ3A("dwarf.j3a");
    int texWidth, texHeight, texChannels;
    
    void *buffer = stbi_load(diffuseMap[0].c_str(),&texWidth,&texHeight,&texChannels,4);

    glGenTextures(1,&diffTexID);
    glBindTexture(GL_TEXTURE_2D,diffTexID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D,0,GL_SRGB8_ALPHA8,texWidth,texHeight,0,GL_RGBA,GL_UNSIGNED_BYTE,buffer);
    glGenerateMipmap(GL_TEXTURE_2D);


    stbi_image_free(buffer);

    buffer = stbi_load(bumpMap[0].c_str(),&texWidth,&texHeight,&texChannels,4);

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
    shadowprogram.loadShaders("shadow.vert","shadow.frag");

    
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);
    
    glGenBuffers(1,&triangleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferData(GL_ARRAY_BUFFER, nVertices[0]*sizeof(glm::vec3), vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
    
    glGenBuffers(1,&normalVBO);
    glBindBuffer(GL_ARRAY_BUFFER,normalVBO);
    glBufferData(GL_ARRAY_BUFFER,nVertices[0]*sizeof(glm::vec3), normals[0],GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER,normalVBO);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);
    
    glGenBuffers(1,&texCoordVBO);
    glBindBuffer(GL_ARRAY_BUFFER,texCoordVBO);
    glBufferData(GL_ARRAY_BUFFER,nVertices[0]*sizeof(glm::vec2), texCoords[0],GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER,texCoordVBO);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,0,0);
    
    glGenBuffers(1,&indexVBOID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nTriangles[0]*sizeof(glm::u32vec3), triangles[0], GL_STATIC_DRAW);
    
    
    
    glEnable(GL_DEPTH_TEST);//앞에 것이 먼저 그려져있을 때 뒤에 있는 것을 그리면 안덮어 씌워지게 해주는 것.
    glEnable(GL_FRAMEBUFFER_SRGB);

    glGenTextures(1, &shadowTex);
    glBindTexture(GL_TEXTURE_2D, shadowTex);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB32F, 1024, 1024, 0,GL_RGB, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glGenTextures(1, &shadowDepth);
    glBindTexture(GL_TEXTURE_2D, shadowDepth);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT32F, 1024, 1024, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glGenFramebuffers(1, &shadowFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, shadowTex, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowDepth, 0);
    GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) printf("FBO Error\n");
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);


}

void render(GLFWwindow* window){
    int w,h;
    GLuint loc;
    mat4 modelMat = mat4(1);
    glfwGetFramebufferSize(window, &w,&h);
    glViewport(0,0,w,h);
    glClearColor(0,0,.5,0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    

    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glViewport(0,0,1024,1024);
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glUseProgram(shadowprogram.programID);
    mat4 shadowProjMat = ortho(-2.f,2.f,-2.f,2.f,0.01f,10.f);
    mat4 shadowViewMat = lookAt(lightPos, vec3(0,0,0), vec3(0,1,0));
    mat4 shadowMVP = shadowProjMat * shadowViewMat * modelMat;
    loc = glGetUniformLocation (shadowprogram.programID, "shadowMVP");
    glUniformMatrix4fv(loc, 1, 0, value_ptr(shadowMVP));
    
    glBindVertexArray(vertexArrayID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID);
    glDrawElements(GL_TRIANGLES, nTriangles[0]*3, GL_UNSIGNED_INT, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);

    
    glUseProgram(program.programID);
    mat4 projMat = perspective(cameraFov/180.f*3.1415926535f, w/float(h), 0.01f, 100.f);
    loc = glGetUniformLocation (program.programID, "projMat");
    glUniformMatrix4fv(loc,1,0,value_ptr(projMat));
    
    loc = glGetUniformLocation (program.programID, "viewMat");
    
    vec3 cameraPosition = vec3(rotate(cameraYaw,vec3(0,1,0))*rotate(cameraPitch,vec3(1,0,0))*vec4(0,0,cameraDistance,0));
    mat4 viewMat = lookAt(cameraPosition,vec3(0,0,0),vec3(0,1,0));
    glUniformMatrix4fv(loc,1,0,value_ptr(viewMat));

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

    
    mat4 shadowBias = translate(vec3(0.5))*scale(vec3(0.5));
    mat4 shadowBiasMVP = shadowBias*shadowMVP;
    loc = glGetUniformLocation (program.programID, "shadowBiasMVP");
    glUniformMatrix4fv(loc,1,0,value_ptr(shadowBiasMVP));
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,diffTexID);
    loc = glGetUniformLocation(program.programID,"diffTex");
    glUniform1i(loc,1);
    
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D,bumpTexID);
    loc = glGetUniformLocation(program.programID,"bumpTex");
    glUniform1i(loc,2);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, shadowDepth);
    loc = glGetUniformLocation(program.programID, "shadowMap");
    glUniform1i(loc, 3);

    glBindVertexArray(vertexArrayID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID);
    glDrawElements(GL_TRIANGLES, nTriangles[0]*3, GL_UNSIGNED_INT, 0);


    glfwSwapBuffers(window);
}
void cursorCallback(GLFWwindow* window, double x, double y){
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1)){
        cameraPitch-=(y-lastY)/200;
        cameraYaw-=(x-lastX)/200;
        lastX=x;
        lastY=y;
    }
}

void buttonCallback(GLFWwindow* window,int button, int action, int mods){
    if(button==GLFW_MOUSE_BUTTON_1&&action==GLFW_PRESS){
        glfwGetCursorPos(window, &lastX, &lastY);
    }
}
void scrollCallback(GLFWwindow* window,double dx,double dy){
    cameraFov-=dy;
    if(cameraFov<5) cameraFov=5;
    if(cameraFov>170) cameraFov=170;
}
void refreshCallback(GLFWwindow* window){
    render(window);
}
