#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "NBodySimulator.h"

class Triangle : public Shape {
public:
    vec3 pos;

    Triangle(vec3 position) : pos(position) {}

    void load() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void draw() {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void destroy() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
private:
    float vertices[18] = {
        // positions         // colors
        0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f,  // bottom right
       -0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f,  // bottom left
        0.0f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f   // top
   };

    unsigned int VBO, VAO;
};

#endif //TRIANGLE_H
