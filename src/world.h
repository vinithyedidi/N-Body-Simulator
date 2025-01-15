#ifndef WORLD_H
#define WORLD_H

#include "NBodySimulator.h"

class World {
public:
    std::vector<Sphere> objs;
    int barrier = 100;
    bool enableBarrier = true;
    float e = 1.0f;    // perfectly elastic collision
    const float G = 5.0;    // km^3/(kg * s^2)
    // we make gravity ridiculously strong to render objects to have a reasonable visual scale and still
    // have the force of gravity be significant.


    void add(Sphere sphere) {
        objs.push_back(sphere);
        objs[objs.size()-1].load();
    }


    int size() {
        return objs.size();
    }


    void draw(Shader shader) {
        for (int i = 0; i < size(); i++) {
            mat4 model = mat4(1.0f);
            model = translate(model, objs[i].pos);
            shader.setMat4("model", model);
            objs[i].draw();
        }
    }


    void destroy() {
        for (int i = 0; i < size(); i++) {
            objs[i].destroy();
        }
    }


    // this is where the actually interesting calculations occur: handle collisions, gravity, and existing motion.
    void moveObjs(float dt) {
        for (int i = 0; i < size(); i++) {
            for (int j = 0; j < size(); j++) {
                if (i != j) {
                    vec3 aj = vec3(0.0f, 0.0f, 0.0f); // accel of sphere j, changed by various forces.

                    // calculate effect of gravity
                    float d = distance(objs[i].pos, objs[j].pos);
                    aj = (objs[i].pos - objs[j].pos) * ((G * objs[i].m) / float(pow(d,3)));

                    if (enableBarrier) {
                        vec3 new_vj = objs[j].v;
                        // sphere collides with barrier
                        if (abs(objs[j].pos.x) > barrier)
                            new_vj = vec3((-e)*objs[j].v.x, objs[j].v.y, objs[j].v.z);
                        if (abs(objs[j].pos.y) > barrier)
                            new_vj = vec3(objs[j].v.x, (-e)*objs[j].v.y, objs[j].v.z);
                        if (abs(objs[j].pos.z) > barrier)
                            new_vj = vec3(objs[j].v.x, objs[j].v.y, (-e)*objs[j].v.z);
                        aj += (new_vj-objs[j].v)/dt;
                    }

                    // sphere to sphere collision
                    if (d <= objs[i].r + objs[j].r) {
                        vec3 new_vj;
                        vec3 new_vi;
                        new_vj = objs[j].v - ((((1.0f+e)*objs[i].m) * dot(objs[j].v-objs[i].v, objs[j].pos-objs[i].pos))
                                            / ((objs[i].m + objs[j].m) * dot(objs[j].pos-objs[i].pos, objs[j].pos-objs[i].pos))
                                            *  (objs[j].pos - objs[i].pos));
                        new_vi = objs[i].v - ((((1.0f+e)*objs[j].m) * dot(objs[i].v-objs[j].v, objs[i].pos-objs[j].pos))
                                            / ((objs[j].m + objs[i].m) * dot(objs[i].pos-objs[j].pos, objs[i].pos-objs[j].pos))
                                            *  (objs[i].pos - objs[j].pos));
                        aj += (new_vj-objs[j].v)/dt;
                        vec3 ai = (new_vi-objs[i].v)/dt;
                        objs[i].update(dt, ai); // sphere i also gets altered, opposite reaction
                    }

                    // update acceleration, velocity, and position through Sphere class.
                    objs[j].update(dt, aj);
                }
            }
        }
    }
};

#endif //WORLD_H
