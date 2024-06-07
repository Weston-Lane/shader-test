
#pragma warning(disable : 4996)

#include <iostream>
#include <fstream>
#include <sstream>
//#include "raylib\raylib.h"
//#define RAYGUI_IMPLEMENTATION
//#include "raylib\raygui.h"


#include <glad\glad.h>//include first
#include <GLFW\glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
const char* readShader(const std::string& shader);

//reads the vertexShaderSource
//this is different than the tutorial, use the function to read the shader as a string and c_str() to cast as a const char* so it can go into the shader
//const char* vertexShaderSource = readShader("src/shaders/vertex.glsl");
//const char* fragmentShaderSource = readShader("src/shaders/fragment.fg");

const char* vertexShaderSource = "#version 420 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 420 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

int main()
{
    glfwInit();                                         //intitializes window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);      //hints check for openGl version  for user
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);   //loads a subset of GLFW that does not need the legacy features

    //creating a window object
    GLFWwindow* window = glfwCreateWindow(800, 600, "first window", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "failed to create window" << std::endl;
        glfwTerminate();                            //terminates openGL context, destroys all windows and must call glfwInit again for another
        return -1;
    }
    glfwMakeContextCurrent(window);         //window context needs to be changed so must you the glfwMakeContextCurrent() function//only current on one thread at a time
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//registers the callback function

    //intitailizing GLAD which finds the openGL functions and creates pointers to each so we dont have to
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))//passes GLAD the function to load the address of openGl function pointers which is OS specific, glfwGetProcAdress defines thhe correct function based on OS
    {
        std::cout << "failed to init GLAD" << std::endl;
        return -1;
    }

    //loading vertex shader
    unsigned int vertexShader=glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //checking for errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //loading fragment shader
    unsigned int fragmentShader;//create object
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);//create the shader
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);////link the source to object
    glCompileShader(fragmentShader);//compile shader

    //check
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //creating shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    //attaching and linking shaders to program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);//attach shaders
    glLinkProgram(shaderProgram);//link shaders

    //check
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    //deleting shader objects since they are no longer needed after linking to the program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //set up coords for triangle with float array of vetices
    float vertices[] = {
       -0.5f, -0.5f, -1.0f,  //left//.5 would be half of screen
        0.5f, -0.5f, 0.0f,  //right
        0.0f,  0.5f, 0.0f  //top
    };

    //openGL buffer object
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    //BindBuffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //copying vertices data into buffers memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //creating and binding VAO vertex Array Object before tthe attribute so it can store state
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //vetex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);





 
    //making openGl viewport, we have to tell openGl what we want the window size to be based on coordinates
    glViewport(0, 0, 800, 600);     //this alows for us to render smaller than the GLFW window if wanted, but is still required
    //this also maps the point from (-1 to1) for openGl use, so for (-.5,.5) it would be 

    //allows for resizing window by calling back to the GLFW window function and resizes


    //main render loop
    while (!glfwWindowShouldClose(window))
    {
        
        processInput(window);

        glClearColor(.2f, .3f, .3f, 1.f);//clears the frame and replaces with this color//RGBA channels
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);//swaps front buffer with back buffer, back buffer renders new frame while front buffer displays to prevent artifacting
        glfwPollEvents();//checks for any input events such as key presses or mouse, used with call back functions

    }
     
    glfwTerminate();

    return 0;                                      

}
                               
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);//recall the viewport function on resize
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

const char* readShader(const std::string& shader)
{
    std::ifstream file(shader);
        if (!file.is_open())
            return "did not find shader";
    
    std::stringstream shaderString{};
    shaderString << file.rdbuf();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
    file.close();

    return shaderString.str().c_str();
}

