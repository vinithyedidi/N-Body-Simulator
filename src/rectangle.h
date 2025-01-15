#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "NBodySimulator.h"

class Rectangle : public Shape {
public:
    vec3 pos;

    Rectangle(vec3 position) : pos(position) {}

    void load() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    void draw() {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    void destroy() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }


private:
    float vertices[24] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f, // bottom right
       -0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f, // bottom left
       -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f, // top left
   };
    unsigned int indices[6] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
};

#endif //RECTANGLE_H
