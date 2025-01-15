#ifndef PRIMITIVE_H
#define PRIMITIVE_H

class Shape {
public:
    virtual void load() = 0;
    virtual void draw() = 0;
    virtual void destroy() = 0;
    vec3 pos;
};

#endif //PRIMITIVE_H
