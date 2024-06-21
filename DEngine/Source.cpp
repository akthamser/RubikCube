#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include"stb_image.h"
#include"shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

float deltatime = 0.0f;
float lastframe = 0.0f;

glm::vec3 cameraPosNorm = glm::vec3(0, 0, 1);
float cameraPosMag = 10;
glm::vec3 cameraUp = glm::vec3(0, 1, 0);

glm::mat4 models[3][3][3] = {glm::mat4(1.0f)};
glm::vec3 realmodels[3][3][3];
glm::vec3 newrealmodels[3][3][3];

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE);

    for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) 
        for (int k = 0; k < 3; k++)
        {
            models[i][j][k] = glm::mat4(1.0f);
            realmodels[i][j][k] = glm::vec3(i,j,k);
            newrealmodels[i][j][k]= glm::vec3(i, j, k);
        }


    std::cout << "USE Q - W - E - A - S - D - Z - X - C To rotate " << std::endl;


    GLFWwindow* window = glfwCreateWindow(800, 600, "Rubik Cube", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader shader = Shader("src\\textureshader.vs", "src\\textureshader.frags");


     
   

      

    float vertices[] = {
        // Front face
        -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f, // white
         0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, 0.0f, // white
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 0.0f, // white
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 0.0f, // white
        -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 0.0f, // white
        -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f, // white

        // Back face
        -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f, // red
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 1.0f, // red
         0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f, // red
         0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f, // red
        -0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 1.0f, // red
        -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f, // red

        // Left face
        -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 2.0f, // green
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 2.0f, // green
        -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, 2.0f, // green
        -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, 2.0f, // green
        -0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 2.0f, // green
        -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 2.0f, // green

        // Right face
         0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 3.0f, // blue
         0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, 3.0f, // blue
         0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, 3.0f, // blue
         0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, 3.0f, // blue
         0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f, 3.0f, // blue
         0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 3.0f, // blue
        
        // Bottom face
        -0.5f, -0.5f, -0.5f,   1.0f, 0.5f, 0.0f,   0.0f, 0.0f, 4.0f, // orange
         0.5f, -0.5f, -0.5f,   1.0f, 0.5f, 0.0f,   1.0f, 0.0f, 4.0f, // orange
         0.5f, -0.5f,  0.5f,   1.0f, 0.5f, 0.0f,   1.0f, 1.0f, 4.0f, // orange
         0.5f, -0.5f,  0.5f,   1.0f, 0.5f, 0.0f,   1.0f, 1.0f, 4.0f, // orange
        -0.5f, -0.5f,  0.5f,   1.0f, 0.5f, 0.0f,   0.0f, 1.0f, 4.0f, // orange
        -0.5f, -0.5f, -0.5f,   1.0f, 0.5f, 0.0f,   0.0f, 0.0f, 4.0f, // orange
        
        // Top face
        -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f, 5.0f, // yellow
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f, 5.0f, // yellow
         0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f, 5.0f, // yellow
         0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f, 5.0f, // yellow
        -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f, 5.0f, // yellow
        -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f, 5.0f  // yellow
    };




    unsigned int VBO;
    glGenBuffers(1, &VBO);



    unsigned int VAO;
    glGenVertexArrays(1, &VAO);


    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float)  , (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float)  , (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float)  , (void*)(6 * sizeof(float)));
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float)  , (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    unsigned int texture1;
    glGenTextures(1,&texture1);
    glBindTexture(GL_TEXTURE_2D,texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("src\\oneFaceTexture.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0, GL_RGB,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glViewport(0, 0, 800, 600);


    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    shader.use();
    shader.setInt("texture1", 0);

    unsigned int modelloc = glGetUniformLocation(shader.ID,"model");
    unsigned int viewloc = glGetUniformLocation(shader.ID,"view");
    unsigned int projectionloc = glGetUniformLocation(shader.ID,"projection");
    unsigned int facecolorloc = glGetUniformLocation(shader.ID, "facecolor");
    unsigned int maskloc = glGetUniformLocation(shader.ID, "faceMask");

    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        float currentframe = glfwGetTime();
        deltatime = currentframe - lastframe;
        lastframe = currentframe;


        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        
        glClear(GL_COLOR_BUFFER_BIT  |  GL_DEPTH_BUFFER_BIT);

        shader.use();
        glActiveTexture(GL_TEXTURE0); 
        glBindTexture(GL_TEXTURE_2D, texture1);
        


        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(
                             cameraPosNorm ,
                             glm::vec3(0,0,0),
                             cameraUp
                             );

        for (int i = 0; i < 3;i++)
        {
            int f = 0, bot = 0, l=0, r=0, back=0, t=0;
      
            if (i == 0)
            {
                l = 1;
            }
            if (i == 2)
            {
                r = 1;
            }

            for (int j = 0; j < 3;j++)
            {
                bot = 0; t = 0;
                if (j == 0)
                {
                    bot = 1;
                }
                if (j == 2)
                {
                    t = 1;
                }

                for (int k = 0; k < 3;k++)
                {
                    back = 0; f = 0;
                    if (k == 0)
                    {
                        f = 1;
                    }
                    if (k == 2)
                    {
                        back = 1;
                    }

                    glm::mat4 model = glm::mat4(1.0f);
                    glm::mat4 projection = glm::mat4(1.0f);
                    model = glm::translate( models[i][j][k], glm::vec3(-1 + i, -1 + j, -1 + k));
                    model = glm::scale(model, glm::vec3(.95, .95, .95));
                 
                    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, .1f, 100.0f);

                    int mask[6] = { f, back, l, r, bot, t };
                    glUniform1iv(maskloc, 6, mask);

                    glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(model));
                    glUniformMatrix4fv(viewloc, 1, GL_FALSE, glm::value_ptr(view));
                    glUniformMatrix4fv(projectionloc, 1, GL_FALSE, glm::value_ptr(projection));
                    glBindVertexArray(VAO);
                    glBindTexture(GL_TEXTURE_2D, texture1);
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }
            }
        
        }


      

        glfwPollEvents();
        glfwSwapBuffers(window);


    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

float anim[3][3][3] = { 0 };
glm::vec3 axis[3][3][3] = { glm::vec3(0, 0, 0) };
float pressed = 0.0f;
float animtime = .5f;



void Rotate_i(int w)
{
    glm::vec3 daxis = glm::vec3(1, 0, 0);
    for (int i = 0;i < 3;i++)
    {
        for (int j = 0;j < 3;j++)
        {
            anim[w][i][j] = 1;

            int ni, nj;
            ni = j; nj = 2 - i;
            newrealmodels[w][i][j] = realmodels[w][ni][nj];

            glm::vec3 ri = realmodels[w][i][j];
            int x = ri.x;
            int y = ri.y;
            int z = ri.z;
            glm::mat4 b;
            b = models[x][y][z];

            axis[w][i][j] = daxis * glm::mat3(b);




        }

    }
}

void Rotate_j(int w)
{
    glm::vec3 daxis = glm::vec3(0, -1, 0);
    for (int i = 0;i < 3;i++)
    {
        for (int j = 0;j < 3;j++)
        {
            anim[i][w][j] = 1;
            int ni, nj;
            ni = j; nj = 2 - i;
            newrealmodels[i][w][j] = realmodels[ni][w][nj];
            glm::vec3 ri = realmodels[i][w][j];
            int x = ri.x;
            int y = ri.y;
            int z = ri.z;
            glm::mat4 b;
            b = models[x][y][z];
            axis[i][w][j] = daxis * glm::mat3(b);
        }

    }
}

void Rotate_k(int w)
{
    glm::vec3 daxis = glm::vec3(0, 0, 1);
    for (int i = 0;i < 3;i++)
    {
        for (int j = 0;j < 3;j++)
        {
            anim[i][j][w] = 1;
            int ni, nj;
            ni = j; nj = 2 - i;
            newrealmodels[i][j][w] = realmodels[ni][nj][w];
            glm::vec3 ri = realmodels[i][j][w];
            int x = ri.x;
            int y = ri.y;
            int z = ri.z;
            glm::mat4 b;
            b = models[x][y][z];
            axis[i][j][w] = daxis * glm::mat3(b);
        }

    }
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetInputMode(window, GLFW_CURSOR,  GLFW_CURSOR_NORMAL);

    if(GLFW_FOCUSED && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)== GLFW_PRESS)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    

   

    pressed -= deltatime;

    if (pressed < 0) {
    
        for (int i = 0;i < 3;i++)
        {
            for (int j = 0;j < 3;j++)
            {
                for (int k = 0;k < 3;k++)
                {
                    anim[i][j][k] = 0;
                    realmodels[i][j][k] = newrealmodels[i][j][k];
                    axis[i][j][k] = glm::vec3(0, 0, 0);
                }

            }

        }
       
        
    
    }


    
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && pressed < 0)
    {
        pressed = animtime;
        Rotate_j(0);

    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && pressed < 0)
    {
        pressed = animtime;
        Rotate_j(1);

    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && pressed < 0)
    {
        pressed = animtime;
        Rotate_j(2);

    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS &&  pressed < 0)
    {
        pressed = animtime;
        Rotate_i(0);

    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && pressed < 0)
    {
        pressed = animtime;
      
        Rotate_i(1);

    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && pressed < 0)
    {
        pressed = animtime;

        Rotate_i(2);
        
    }
    
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && pressed < 0)
    {
        pressed = animtime;

        Rotate_k(0);
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && pressed < 0)
    {
        pressed = animtime;

        Rotate_k(1);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && pressed < 0)
    {
        pressed = animtime;

        Rotate_k(2);
    }




    if (pressed > 0)
    {
    for (int i = 0;i < 3;i++)
    {
        for (int j = 0;j < 3;j++)
        {
            for (int k = 0;k < 3;k++)
            {
                glm::vec3 ri = realmodels[i][j][k];
                int x = ri.x;
                int y = ri.y;
                int z = ri.z;
                
                if(anim[i][j][k] > 0)
                models[x][y][z] = glm::rotate(models[x][y][z],     glm::radians(90.0f*deltatime*(1/animtime)),  axis[i][j][k]);
                
                
            }

        }

    }
    }

  
        


    
    
}


bool firstMouse = true;
float lastX = 400, lastY = 300;
float yaw = -90.0f;
float pitch;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch < -89)
        pitch =- 89;
    if (pitch > 89)
        pitch = 89;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraPosNorm = glm::normalize(direction) * cameraPosMag;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    cameraPosMag -= (float)yoffset;
    if (cameraPosMag < 2.0f)
        cameraPosMag = 2.0f;
    if (cameraPosMag > 30.0f)
        cameraPosMag = 30.0f;
    cameraPosNorm = glm::normalize(cameraPosNorm) * cameraPosMag;
}