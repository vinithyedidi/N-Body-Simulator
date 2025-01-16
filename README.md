# N Body Simulator

This is a first person simulation engine with a moving camera that renders spheres in space that affect each other through the mechanics of gravity and collisions. There are multiple preloaded simulations stored in the enum "Scenarios" that can be inputted in the main method, such as a solar system, twin stars, or $N$ randomly generated bodies. The bodies spawn in space with their respective initial conditions, and then the simulation runs and the spheres move around according to their mechanics. 

See "images" to see some screenshots of the simulation. You can also download it, and link GLFW, GLM, and GLAD. If you copy the CMakeLists.txt from this repo and adjust the filenames in the main function, it should work.

## The Rendering

I have some experience in OpenGL from my Raytracer project. I used the same framework, and from there the only real challenge was rendering spheres. I implemented the Sphere class, which generates the object using trigonometry out of primitive triangles. You can also control the size of the sphere — the radius of a sphere is proportional to the cube root of its  mass. You can render any amount of spheres you want (though performance suffers after around 200 spheres) by entering initial position, velocity, color, resolution of primitive triangles, and mass. See the Sphere class for more details.

Once the simulation is running, the values of position and velocity are calculated according to the mechanics (see below) and set for the next frame. The renderer then adjusts the position accordingly, which then renders the sphere in the next frame.

## Gravitational Force

Each sphere exerts gravitational force on each other sphere in every frame, altering their orbit. This problem is called the N Body Problem, famous in Physics for being chaotic and impossible to solve analytically for large values of $N$. However, it's possible to plot the trajectories of $N$ bodies using numerical methods of solving the equations with respect to a time step of one frame, and adjusting the coordinates of rendered spheres accordingly.

We know that $\vec{F}^{\} = m\vec{a}^{\}$ and that $F = G\frac{m_1m_2}{r^2}\vec{u}^{\}$, where $\vec{F}^{\}$ is gravitational force, $\vec{a}^{\}$ is acceleration, $r$ is distance between the centers of masses, $m_1, m_2$ are the respective masses, $G$ is the graviatational constant, and $\vec{u}^{\}$ is the unit vector pointing from $m_1$ to $m_2$. Note that $\vec{F}^{\}$, $\vec{a}^{\}$, and $\vec{u}^{\}$ are vectors in 3D space.

We can set these equal, that $m_1\vec{a}^{\} = G\frac{m_1m_2}{r^2} \vec{u}^{\}$ and $m_2\vec{a}^{\} = -G\frac{m_1m_2}{r^2} \vec{u}^{\}$. This results in the set of second order ordinary differential equations $\vec{a}^{\} = G\frac{m_2}{r^3}$ and $\vec{a}^{\} = G\frac{m_1}{r^3}$. For $N=2$, this is possible analytically, but scaling this up for $N\ge3$ has to be done numerically.

To do so, we now have a formula for $\vec{a}^{\}$, which is the second derivative of position $\vec{x}^{\}$, so we can then take the integral of $\vec{a}^{\}$ to get velocity $\vec{v}^{\}$ and the integral of $\vec{v}^{\}$ to get position $\vec{x}^{\}$. These quantities all get updated frame by frame. Our method of integration is an adjustment to the Trapezoidal Rule, $\\int_{t_0}^{t_1} f(t) \,dt \ = (t_1 - t_0)f(t_1)$. Seeing as the simulation runs at 60 fps, $dt$ is small enough for this to be accurate. This calculates $\vec{v}^{\}$ and $\vec{x}^{\}$ for each sphere, which both get updated in each sphere and represent the initial conditions for the next frame.

## Collision Mechanics

We now analyze sphere-sphere collision mechanics. In 1D, we assume conservation of momentum, $m_1v_1 + m_2v_2 = m_1v_1' + m_2v_2'$ where $v_i'$ is the velocity of sphere $i$ calculated in the next frame. We also assume conservation of kinetic energy, or $\frac{1}{2}m_1v_1^2 + \frac{1}{2}m_2v_2^2 = \frac{1}{2}m_1v_1'^2 + \frac{1}{2}m_2v_2^2$. 

We make the same assumptions in $N$ spatial dimensions — conservation of momentum implies that $\vec{J_1}^{\} = -\vec{J_2}^{\}$ where $\vec{J_i}^{\} = \lambda \vec{\hat{n}}^{\}$ for $J_i$ impulse, some $\lambda$ and $\vec{\hat{n}}^{\} = \Delta\vec{x}^{\}$ normalized. We then arrive at $\vec{v_i'}^{\} = \vec{v_i}^{\} + \frac{\lambda}{m_i} \vec{\hat{n}}^{\}$. 

Conservation of kinetic energy implies $\lambda^2 \frac{m_1 + m_2}{m_1m_2} + 2\lambda \langle\vec{\hat{n}}^{\} , \Delta\vec{v}^{\}\rangle = 0$. Solving all of this out with the conservation of momentum results in $\vec{v_i'}^{\} = \vec{v_i}^{\} + \frac{2m_j}{m_i+m_j} * \frac{\langle\vec{v_i}^{\}-\vec{v_j}^{\}, \vec{x_i}^{\}-\vec{x_j}^{\} \rangle}{||\vec{x_j}^{\} - \vec{x_i}^{\}||^2} * (\vec{x_i}^{\} - \vec{x_j}^{\})$. This new $\vec{v_i'}^{\}$ can be integrated as above to find the new $\vec{x_i'}^{\}$, which is then updated and becomes the initial conditions for the next frame.

The elastic collision mechanics and gravitational force are the two main mechanics this simulation captures. To adjust the force of gravity, change $G$ to any value you wish — I chose 5 $\frac{m^3}{kg*s^2}$, a ridiculously large value to ensure a reasonable visual scale without too large values being sent to the GPU for rendering. Also, to reduce elasticity, change $2$ in the above equation with any $(1+e)$ for $e\in[0,1]$. I chose $e=1$, or perfectly elastic.

## Conclusions
This was a really fun project to make, and I'm really happy with the results. The interactions between spheres look really realistic and I was able to input scenarios we see in outer space, such as stellar orbit, twin stars, and a black hole sucking in asteroids. I'm also really excited about the randomized $N$ body scenarios, as they're never predictable until you actually run the simulation. Weird things happen sometimes, and it's all according to the math.

In the future, I'd like to make some of the algorithms more efficent and accurate. For instance, the collision checked checks each sphere with every other sphere, which scales up quadratically with $N$. Instead, I'd like to implement bounding boxes, so that a sphere doesn't need to check for collisions with other spheres miles away. I'd also like to improve the integration algorithm, as at the moment I'm using the Trapezoidal Rule. It works fairly well considering the very small timestep, but I think Simpson's Rule or Gauss Quadrature could add minimal performance loss and significant accuracy improvement.

I could also consider adding the effects of special relativity or other physical forces in this scenario, rendering a 4D spacetime grid and calculating event coordinates and velocity for each frame. Thank goodness GLM has a vec4 class.


## Sources:
https://www.songho.ca/opengl/gl_sphere.html <br />
https://www.youtube.com/watch?v=w4f_Z-_spZQ <br />
https://orbital-mechanics.space/intro.html <br />
https://en.wikipedia.org/wiki/Elastic_collision


- Vinith Yedidi, 2025
