// Based on templates from learnopengl.com
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>
#include <algorithm>
#include "Eigen/Dense"
#include "Camera.h"
#include "Sphere.h"
#include "Triangle.h"
#include "PhongMat.h"
#include "Scene.h"
#include "PointLight.h"
#include "AmbientLight.h"
#include "DirectionalLight.h"

using Eigen::Array3f;
using Eigen::Vector2f;
using Eigen::Vector3f;
using Eigen::Vector4f;

Scene scene;
unsigned int *texture;
unsigned int pTex;
unsigned int oTex;
bool ortho = false;
Array3f shadeRay(Ray ray, float t0, float t1);
void renderImageP(unsigned int *texture);
void renderImageO(unsigned int *texture);
void switchTextures();
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "layout (location = 1) in vec3 aColor;\n"
                                 "layout (location = 2) in vec2 aTexCoord;\n"
                                 "out vec3 ourColor;\n"
                                 "out vec2 TexCoord;\n"
                                 "void main()\n"
                                 "{\n"
                                 "gl_Position = vec4(aPos, 1.0);\n"
                                 "ourColor = aColor;\n"
                                 "TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
                                 "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "in vec3 ourColor;\n"
                                   "in vec2 TexCoord;\n"
                                   "uniform sampler2D texture1;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = texture(texture1, TexCoord);\n"
                                   "}\n\0";

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Display RGB Array", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // // GLEW: load all OpenGL function pointers
    glewInit();

    // build and compile the shaders
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // // load and create a texture
    // // -------------------------
    // unsigned int texture;
    // glGenTextures(1, &texture);
    // glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // // set the texture wrapping parameters
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // // set texture filtering parameters
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    Material blue(Vector3f(0.0, 0.0, 1.0), Array3f(0.2, 0.2, 0.2));
    Material green(Vector3f(0.0, 1.0, 0.0), Array3f(0.2, 0.2, 0.2));
    Material white(Vector3f(1.0, 1.0, 1.0), Array3f(0.2, 0.2, 0.2));
    PhongMat red(Vector3f(1.0, 0.0, 0.0));
    blue.setMirror(true);

    // Create Spheres
    Vector3f c1(-1.0, -3.0, 2.0);
    Sphere sphere1(c1, 1.0, &red);
    Vector3f c2(0.5, -3.0, 1.0);
    Sphere sphere2(c2, 1.0, &blue);

    // Create Floor
    Triangle tetra1(Vector3f(-1, -5, 0), Vector3f(0, -5, 1), Vector3f(0, -4, 0), &green);
    Triangle tetra2(Vector3f(0, -4, 0), Vector3f(0, -5, 1), Vector3f(1, -5, 0), &green);
    Triangle tetra3(Vector3f(-1, -5, 0), Vector3f(0, -5, 1), Vector3f(0, -6, 0), &green);
    Triangle tetra4(Vector3f(0, -6, 0), Vector3f(0, -5, 1), Vector3f(1, -5, 0), &green);
    Triangle floor1(Vector3f(10.0, 10.0, 0.0), Vector3f(-10.0, 10.0, 0.0), Vector3f(10.0, -10.0, 0.0), &white);
    Triangle floor2(Vector3f(10.0, 10.0, 0.0), Vector3f(-10.0, -10.0, 0.0), Vector3f(10.0, -10.0, 0.0), &white);

    // Create Light
    PointLight light1(Vector3f(0.0, -3.0, 0.5), Array3f(1.0, 1.0, 1.0));
    AmbientLight ambLight(Vector3f(0.0, 0.0, 0.0), Array3f(1.0, 1.0, 1.0));
    DirectionalLight dirLight(Vector3f(0.0, -3.0, 10.0), Array3f(1.0, 1.0, 1.0), Array3f(1, 1, 1));

    // Create Scene
    scene.addSurface(&sphere1);
    scene.addSurface(&sphere2);
    scene.addSurface(&tetra1);
    scene.addSurface(&tetra2);
    scene.addSurface(&tetra3);
    scene.addSurface(&tetra4);
    scene.addSurface(&floor1);
    scene.addSurface(&floor2);
    scene.addLight(&light1);
    scene.addLight(&ambLight);
    scene.addLight(&dirLight);

    renderImageP(&pTex);

    renderImageO(&oTex);

    texture = &pTex;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind Texture
        glBindTexture(GL_TEXTURE_2D, *texture);

        // render container
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        switchTextures();
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

Array3f shadeRay(Ray ray, float t0, float t1)
{
    HitRecord hit = scene.hit(ray, t0, t1);
    if (hit.getT() > 0 && hit.getT() < std::numeric_limits<float>().max())
    {

        Array3f color = Array3f(0.0, 0.0, 0.0);
        for (Light *light : scene.getLights())
        {
            color = color + light->illuminate(ray, hit);
        }
        if (hit.getMat()->getMirror())
        {
            Vector3f r = ray.getDir() - 2 * (ray.getDir().dot(hit.getNormal()) * hit.getNormal());
            Ray newRay(ray.evaluate(hit.getT()) + r * 0.01, r);
            color = color + (0.8) * shadeRay(newRay, std::numeric_limits<float>::epsilon(), std::numeric_limits<float>().max());
        }
        return color;
    }
    else
    {
        return Array3f(0, 0, 0);
    }
}

void renderImageP(unsigned int *texture)
{
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    const int width = 1024;  // keep it in powers of 2!
    const int height = 1024; // keep it in powers of 2!

    unsigned char image[width * height * 3];

    // Create Camera
    Vector3f worldUp(0.0, 0.0, 1.0);
    Vector3f e(0, 0, 1.0);
    Vector3f w(0, 1.0, 0.0);
    Vector2f res(width, height);
    Vector4f boundaries(-1, 1, 1, -1);
    Camera mainCam(e, worldUp, 2.0, w.normalized(), res, boundaries);

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            Ray r = mainCam.CastRay(i, j);

            int idx = (i * width + j) * 3;
            Array3f color = shadeRay(r, (float)0, std::numeric_limits<float>().max());
            image[idx] = std::clamp(color.x(), (float)0, (float)1) * 255.0;
            image[idx + 1] = std::clamp(color.y(), (float)0, (float)1) * 255.0;
            image[idx + 2] = std::clamp(color.z(), (float)0, (float)1) * 255.0;
        }
    }

    unsigned char *data = &image[0];
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
}

void renderImageO(unsigned int *texture)
{
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    const int width = 128;  // keep it in powers of 2!
    const int height = 128; // keep it in powers of 2!

    unsigned char image[width * height * 3];

    // Create Camera
    Vector3f worldUp(0.0, 0.0, 1.0);
    Vector3f e(0.0, 0.0, 1.0);
    Vector3f w(0.0, 1.0, 0.0);
    Vector2f res(width, height);
    Vector4f boundaries(-1, 1, 1, -1);
    Camera mainCam(e, worldUp, 0.0, w.normalized(), res, boundaries);

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            Ray r = mainCam.CastRay(i, j);

            int idx = (i * width + j) * 3;
            Array3f color = shadeRay(r, (float)0, std::numeric_limits<float>().max());
            image[idx] = std::clamp(color.x(), (float)0, (float)1) * 255.0;
            image[idx + 1] = std::clamp(color.y(), (float)0, (float)1) * 255.0;
            image[idx + 2] = std::clamp(color.z(), (float)0, (float)1) * 255.0;
        }
    }

    unsigned char *data = &image[0];
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
}

void switchTextures()
{
    ortho = !ortho;
    if (ortho)
        texture = &oTex;
    else
        texture = &pTex;
}