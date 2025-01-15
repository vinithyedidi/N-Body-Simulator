#ifndef SPHERE_H
#define SPHERE_H

#include "NBodySimulator.h"

class Sphere : public Shape {
public:
    float m; // mass corresponds to size rendered on screen for visualization purposes.
    float r;
    vec3 pos;
    vec3 v;
    vec3 a;
    vec3 col;

    Sphere(unsigned int resolution, float mass, vec3 position, vec3 velocity, vec3 color) :
    res(resolution), pos(position), m(mass), v(velocity), col(color) {
        // we procedurally render a sphere from primitive triangles using the resolution.

        float slicestep = two_pi<float>() / float(res);
        float stackstep = pi<float>() / float(res);
        int row = 0;
        int numVertices = 0;
        float phi = -half_pi<float>();

        // radius/scale_factor is proportional to cube root of mass.
        r = pow((3.0f * mass)/(4.0f * pi<float>()), 1.0f/3.0f);

        for (; phi < half_pi<float>() + stackstep; phi += stackstep, row++) {
            float radius = cos(phi);
            int cell = 0;
            for (float theta = 0; theta < two_pi<float>(); theta += slicestep, cell++) {
                // add vertex
                vertices.push_back(r * vec3(radius * cos(theta), sin(phi), radius * sin(theta)));
                // add color
                vertices.push_back(col);
                // add indices if not bottom row
                if (row) {
                    int nextCell = (cell+1) % res;
                    indices.push_back(numVertices - res); // bottom left
                    indices.push_back((row-1)*res + nextCell); // bottom right
                    indices.push_back(row*res + nextCell); // top right

                    indices.push_back(numVertices - res); // bottom left
                    indices.push_back(numVertices); // top left
                    indices.push_back(row*res + nextCell); // top right
                }
                numVertices++;
            }
        }
    }


    void update(float dt, vec3 acc) {
        a = acc;
        v += a*dt;  // integrate velocity from acceleration * time
        pos += v*dt;  // update position from velocity
        if (pos.x > 10000.0f || pos.y > 10000.0f || pos.z > 10000.0f) {
            destroy();
        }
    }


    void load() {
        // generate and bind buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), &indices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }


    void draw() {
        // draw elements instanced to save space
        glBindVertexArray(VAO);
        glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0, 1);
    }


    void destroy() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }


private:
    std::vector<vec3> vertices;
    std::vector<unsigned int> indices;
    unsigned int VBO, VAO, EBO;
    unsigned int res;
};

#endif //SPHERE_H
