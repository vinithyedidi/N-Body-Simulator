#ifndef CAMERA_H
#define CAMERA_H

#include "NBodySimulator.h"

// Defines several possible options for camera movement.
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    RESET
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  250.0f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;
const auto DEFAULT_POS = vec3(0.0f, 0.0f, 200.0f);


class Camera {
public:
    // camera attributes
    vec3 position;
    vec3 camFront;
    vec3 camUp;
    vec3 camRight;
    vec3 worldUp;

    // euler angles
    float yaw;
    float pitch;

    // camera options
    float camSpeed;
    float mouseSensitivity;
    float zoom;

    // constructor with vectors
    Camera(vec3 pos) :
        camFront(vec3(0.0f, 0.0f, -1.0f)), camSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM),
        pitch(PITCH), yaw(YAW)
    {
        position = pos;
        worldUp = vec3(0.0f, 1.0f, 0.0f);
        updateCameraVectors();
    }

    // default camera position
    Camera() :
    camFront(vec3(0.0f, 0.0f, -1.0f)), position(DEFAULT_POS), camSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM),
    pitch(PITCH), yaw(YAW)
    {
        worldUp = vec3(0.0f, 1.0f, 0.0f);
        updateCameraVectors();
    }


    mat4 getViewMatrix() {
        return lookAt(position, position + camFront, camUp);
    }


    void processKeyboard(Camera_Movement direction, float deltaTime) {
        float velocity = camSpeed * deltaTime;
        if (direction == FORWARD)
            position += camFront * velocity;
        if (direction == BACKWARD)
            position -= camFront * velocity;
        if (direction == LEFT)
            position -= camRight * velocity;
        if (direction == RIGHT)
            position += camRight * velocity;
        if (direction == UP)
            position += camUp * velocity;
        if (direction == DOWN)
            position -= camUp * velocity;
        if (direction == RESET) {
            position = DEFAULT_POS;
            yaw = YAW;
            pitch = PITCH;
            updateCameraVectors();
        }
    }


    void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw   += xoffset;
        pitch += yoffset;

        // make sure that when pitch is out of bounds, you don't break your neck
        if (constrainPitch) {
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }
        updateCameraVectors();
    }


    void processMouseScroll(float yoffset) {
        zoom -= (float)yoffset;
        if (zoom < 1.0f)
            zoom = 1.0f;
        if (zoom > 45.0f)
            zoom = 45.0f;
    }


private:
    // calculates the camera front, right, and up vector from the Camera's (updated) Euler Angles
    void updateCameraVectors() {
        vec3 front;
        front.x = cos(radians(yaw)) * cos(radians(pitch));
        front.y = sin(radians(pitch));
        front.z = sin(radians(yaw)) * cos(radians(pitch));
        camFront = normalize(front);
        camRight = normalize(cross(camFront, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        camUp    = normalize(cross(camRight, camFront));
    }
};
#endif