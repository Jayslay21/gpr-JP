#include <stdio.h>
#include <math.h>
#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// Vertex and Fragment Shader source code
const char* vertexShaderSource = R"(
    #version 330 core
    layout(location = 0) in vec3 aPos;
    layout(location = 1) in vec4 aColor;
    out vec4 ourColor;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main() {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
        ourColor = aColor;
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    in vec4 ourColor;
    out vec4 FragColor;
    uniform float uTime;
    void main() {
        float greenValue = (sin(uTime) / 2.0f) + 0.5f;
        FragColor = vec4(ourColor.r, greenValue, ourColor.b, ourColor.a);
    }
)";

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

// Vertices for a cube
float vertices[] = {
    // positions          // colors
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, // Front bottom-left
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, // Front bottom-right
     0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f, // Front top-right
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f, // Front top-left
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 1.0f, // Back bottom-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.5f, 0.0f, 1.0f, // Back bottom-right
     0.5f,  0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, // Back top-right
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f  // Back top-left
};

// Indices for the cube's triangles
unsigned int indices[] = {
    // Front face
    0, 1, 2,
    0, 2, 3,
    // Back face
    4, 5, 6,
    4, 6, 7,
    // Left face
    0, 3, 7,
    0, 7, 4,
    // Right face
    1, 5, 6,
    1, 6, 2,
    // Top face
    3, 2, 6,
    3, 6, 7,
    // Bottom face
    0, 4, 5,
    0, 5, 1
};

unsigned int compileShader(unsigned int type, const char* source) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("ERROR::SHADER::COMPILATION_FAILED\n%s", infoLog);
    }
    return shader;
}

unsigned int setupShaders() {
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::PROGRAM::LINKING_FAILED\n%s", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

unsigned int setupVertexArrayObject() {
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    return VAO;
}

int main() {
    printf("Initializing...\n");

    if (!glfwInit()) {
        printf("GLFW failed to init!\n");
        return 1;
    }

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Cube", NULL, NULL);
    if (!window) {
        printf("GLFW failed to create window\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGL(glfwGetProcAddress)) {
        printf("GLAD Failed to load GL headers");
        return 1;
    }

    glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D rendering

    unsigned int shaderProgram = setupShaders();
    unsigned int VAO = setupVertexArrayObject();

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Set the background color to a light blue
        glClearColor(0.678f, 0.847f, 0.902f, 1.0f); // Light blue color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers

        glUseProgram(shaderProgram);
        float time = (float)glfwGetTime();
        int timeLoc = glGetUniformLocation(shaderProgram, "uTime");
        glUniform1f(timeLoc, time);

        for (unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X-axis
            model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y-axis

            int modelLoc = glGetUniformLocation(shaderProgram, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            int viewLoc = glGetUniformLocation(shaderProgram, "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            int projLoc = glGetUniformLocation(shaderProgram, "projection");
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); // Draw the cube
        }

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}
