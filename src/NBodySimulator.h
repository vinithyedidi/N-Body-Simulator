#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#define GLM_FORCE_SWIZZLE
#define GLM_ENABLE_EXPERIMENTAL

// Contains include statements for all relevant imports.
#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glad/glad.c>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>

using namespace glm;

// include statements for each file in this library
#include "shader.h"
#include "shape.h"
#include "triangle.h"
#include "rectangle.h"
#include "sphere.h"
#include "camera.h"
#include "world.h"

#endif
