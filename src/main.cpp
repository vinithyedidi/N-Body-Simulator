#include "NBodySimulator.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 800;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// camera
auto cam = Camera();

// mouse movement
bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

// preloaded scenarios
enum Scenario {
    TWIN_STARS,
    EARTH_MOON,
    SOLAR_SYSTEM,
    BLACK_HOLE,
    TWO_BODIES,
    THREE_BODIES,
    FIVE_BODIES,
    MANY_BODIES
};


World createWorld(Scenario s) {
    auto world = World();
    srand(time(0)); // for random number generators in some simulations

    switch (s) {
        case(TWIN_STARS): {
            world.enableBarrier = false;
            world.add(Sphere(100, 10000.0f, vec3(-25.0f, 0.0f, 0.0f),
                 vec3(0.0f, 0.0f, -25.0f), vec3(1.0f, 0.0f, 0.0f)));
            world.add(Sphere(100, 10000.0f, vec3(25.0f, 0.0f, 0.0f),
                 vec3(0.0f, 0.0f, 25.0f), vec3(0.0f, 1.0f, 1.0f)));
            break;
        }
        case(EARTH_MOON): {
            world.enableBarrier = false;
            world.add(Sphere(100, 10000.0f, vec3(0.0f, 0.0f, 0.0f),
                vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.6f, 0.8f)));
            world.add(Sphere(100, 100.0f, vec3(40.0f, 0.0f, 0.0f),
                vec3(0.0f, 0.0f, 30.0f), vec3(0.4f, 0.4f, 0.4f)));
            break;
        }
        case(SOLAR_SYSTEM): {
            world.enableBarrier = false;
            int N = 9;
            float dist = 100.0f;
            float v = -20.0f;
            world.add(Sphere(100, 100000.0f, vec3(0.0f, 0.0f, 0.0f),
                vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.4f, 0.0f)));
            for (int i = 0; i < N; i++) {
                float mass = (rand() % 900) + 100;
                auto loc = vec3(dist, 0.0f, 0.0f);
                auto vel = vec3(0.0f, 0.0f, v);
                auto col = vec3 (0.2f + (float)rand() /(RAND_MAX/(1.0f-0.2f))
                                ,0.2f + (float)rand() /(RAND_MAX/(1.0f-0.2f)),
                                 0.2f + (float)rand() /(RAND_MAX/(1.0f-0.2f)));
                world.add(Sphere(100, mass, loc, vel, col));
                dist *= 1.3;
                v *= 0.9;
            }
            break;
        }
        case(BLACK_HOLE): {
            world.enableBarrier = false;
            cam.position = vec3(0.0f, 0.0f, 1000.0f);
            world.add(Sphere(200, 10000000.0f, vec3(0.0f, 0.0f, 0.0f),
                vec3(0.0f, 0.0f, 0.0f), vec3(0.01f, 0.01f, 0.01f)));
            int N = 200;
            for (int i = 0; i < N; i++) {
                float mass = (rand() % 1500) + 500;
                auto loc = vec3((rand()%5000)-2500,
                                (rand()%5000)-2500,
                                (rand()%5000)-2500);
                auto vel = vec3((rand()%20)-10, (rand()%20)-10, (rand()%20)-10);
                auto col = vec3 (0.2f + (float)rand() /(RAND_MAX/(1.0f-0.2f))
                                ,0.2f + (float)rand() /(RAND_MAX/(1.0f-0.2f)),
                                 0.2f + (float)rand() /(RAND_MAX/(1.0f-0.2f)));
                world.add(Sphere(100, mass, loc, vel, col));
            }
            break;
        }
        case(TWO_BODIES): {
            world.enableBarrier = false;
            int N = 2;
            for (int i = 0; i < N; i++) {
                float mass = (rand() % 5000) + 100;
                auto loc = vec3((rand()%world.barrier)-world.barrier/2,
                                (rand()%world.barrier)-world.barrier/2,
                                (rand()%world.barrier)-world.barrier/2);
                auto vel = vec3((rand()%20)-10, (rand()%20)-10, (rand()%20)-10);
                auto col = vec3 (0.2f + (float)rand() /(RAND_MAX/(1.0f-0.2f))
                                ,0.2f + (float)rand() /(RAND_MAX/(1.0f-0.2f)),
                                 0.2f + (float)rand() /(RAND_MAX/(1.0f-0.2f)));
                world.add(Sphere(100, mass, loc, vel, col));
            }
            break;
        }
        case(THREE_BODIES): {
            world.enableBarrier = false;
            int N = 3;
            for (int i = 0; i < N; i++) {
                float mass = (rand() % 5000) + 100;
                auto loc = vec3((rand()%world.barrier)-world.barrier/2,
                                (rand()%world.barrier)-world.barrier/2,
                                (rand()%world.barrier)-world.barrier/2);
                auto vel = vec3((rand()%20)-10, (rand()%20)-10, (rand()%20)-10);
                auto col = vec3 (0.2f + (float)rand() /(RAND_MAX/(1.0f-0.2f))
                                ,0.2f + (float)rand() /(RAND_MAX/(1.0f-0.2f)),
                                 0.2f + (float)rand() /(RAND_MAX/(1.0f-0.2f)));
                world.add(Sphere(100, mass, loc, vel, col));
            }
            break;
        }
        case(FIVE_BODIES): {
            world.enableBarrier = false;
            int N = 5;
            for (int i = 0; i < N; i++) {
                float mass = (rand() % 5000) + 100;
                auto loc = vec3((rand()%world.barrier)-world.barrier/2,
                                (rand()%world.barrier)-world.barrier/2,
                                (rand()%world.barrier)-world.barrier/2);
                auto vel = vec3((rand()%20)-10, (rand()%20)-10, (rand()%20)-10);
                auto col = vec3 (0.2f + (float)rand() /(RAND_MAX/(1.0f-0.2f))
                                ,0.2f + (float)rand() /(RAND_MAX/(1.0f-0.2f)),
                                 0.2f + (float)rand() /(RAND_MAX/(1.0f-0.2f)));
                world.add(Sphere(100, mass, loc, vel, col));
            }
            break;
        }
        case(MANY_BODIES): {
            world.barrier = 500;
            cam.position = vec3(0.0f, 0.0f, 600.0f);
            int N = 200;
            for (int i = 0; i < N; i++) {
                float mass = (rand() % 5000) + 100;
                auto loc = vec3((rand()%world.barrier)-world.barrier/2,
                                (rand()%world.barrier)-world.barrier/2,
                                (rand()%world.barrier)-world.barrier/2);
                auto vel = vec3((rand()%20)-10, (rand()%20)-10, (rand()%20)-10);
                auto col = vec3 (0.2f + (float)rand() /(RAND_MAX/(1.0f-0.2f))
                                ,0.2f + (float)rand() /(RAND_MAX/(1.0f-0.2f)),
                                 0.2f + (float)rand() /(RAND_MAX/(1.0f-0.2f)));
                world.add(Sphere(100, mass, loc, vel, col));
            }
            break;
        }
    }
    return world;
}


void run_window(Scenario s) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    auto window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "N Body Simulator    -Vinith Yedidi, 2025", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // anti-aliasing
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    // initialize shaders
    auto shader = Shader("/Users/vinithyedidi/CodingProjects/CLion/NBodySimulator/shaders/vt.glsl",
        "/Users/vinithyedidi/CodingProjects/CLion/NBodySimulator/shaders/ft.glsl");

    // create a world with as many objects as you want — edit in the createWorld() function above.
    auto world = createWorld(s);

    // render loop
    while (!glfwWindowShouldClose(window))  {
        // make sure we run at 60 frames per second
        auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        auto inv_fps = 1.0f / 60.0f;
        if (deltaTime < inv_fps) {
            glfwWaitEventsTimeout(inv_fps - deltaTime);
        }
        std::cout << "\r" << 1.0f / (static_cast<float>(glfwGetTime()) - lastFrame) << " fps";
        lastFrame = currentFrame;

        // clear screen
        glClearColor(0.05f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();

        // Projection transformation based on zoom.
        mat4 projection = perspective(radians(cam.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
        shader.setMat4("projection", projection);
        // camera/view transformation
        mat4 view = cam.getViewMatrix();
        shader.setMat4("view", view);

        // draw each object (see world.h)
        world.draw(shader);

        // process changes from input and object movement
        world.moveObjs(deltaTime);
        processInput(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // clear memory after loop is terminated
    world.destroy();
    glfwTerminate();
}


int main() {
    run_window(BLACK_HOLE);
    return 0;
}


void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.processKeyboard(Camera_Movement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.processKeyboard(Camera_Movement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.processKeyboard(Camera_Movement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.processKeyboard(Camera_Movement::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cam.processKeyboard(Camera_Movement::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cam.processKeyboard(Camera_Movement::DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        cam.processKeyboard(Camera_Movement::RESET, deltaTime);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    cam.processMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    cam.processMouseScroll(static_cast<float>(yoffset));
}
