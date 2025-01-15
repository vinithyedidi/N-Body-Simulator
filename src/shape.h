#ifndef SHAPE_H
#define SHAPE_H

class Shape {
public:
    virtual void load() = 0;
    virtual void draw() = 0;
    virtual void destroy() = 0;
    vec3 pos;
};

#endif //SHAPE_H
