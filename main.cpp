#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <utils/shader.h>
#include <utils/physics.h>
#include <utils/camera.h>
#include <utils/tree.h>
#include <utils/object.h>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtx/quaternion.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#include <glm/gtc/type_ptr.hpp>

// Call backs
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, Physics* physics);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera cam = Camera(glm::vec3(0.0f,10.0f,3.0f), SCR_WIDTH, SCR_HEIGHT);
Tree ground = Tree(glm::vec3(0.0f,0.0f,0.0f));



int cubes = 50;
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{

	Tree* objects = new Tree[cubes];

	Physics* physics = new Physics();

	    for (int i = 0; i < cubes; i++) {
		 float x = rand() % 100;
		 float y = 0.0f;
		 float z = rand() % 100;
       		 objects[i] = Tree(glm::vec3(x,y,z));
   	 	}
	    
            // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    glEnable(GL_DEPTH_TEST);

    Shader s("vert.txt","frag.txt"); 
 for (int i = 0; i < cubes; i++) {
       		 objects[i].prepare();
   	 	}

    ground.prepare();

	cam.getCollision(physics->collisionShapes, physics->dynamicsWorld, 1.0f);
 
for (int i = 0; i < cubes; i++) {
	objects[i].getCollision(physics->collisionShapes, physics->dynamicsWorld, 0.0f);
   	 }

	ground.getCollision(physics->collisionShapes, physics->dynamicsWorld, 0.0f, glm::vec3(500.0f, 1.0f, 500.0f));
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
	float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        // -----
       
                physics->dynamicsWorld->stepSimulation(deltaTime, 10);

                //print positions of all objects

        
		cam.runPhysics(physics->dynamicsWorld);
		ground.runPhysics(physics->dynamicsWorld);
        ///-----stepsimulation_end-----

        //cleanup in the reverse order of creation/initialization

        ///-----cleanup_start-----

        //remove the rigidbodies from the dynamics world and delete them
               processInput(window, physics);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < cubes; i++) {
		 objects[i].runPhysics(physics->dynamicsWorld);
       		 objects[i].draw(cam,s);
   	 }

	ground.draw(cam, s);


        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
 
 physics->cleanUp();


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window, Physics* physics)
{
    float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	cam.applyForce(physics->dynamicsWorld, 0.1f, glm::vec3(1.0f,0.0f,0.0f));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	cam.applyForce(physics->dynamicsWorld, -0.1f, glm::vec3(1.0f,0.0f,0.0f));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)

	cam.applyForce(physics->dynamicsWorld, -0.2f, glm::vec3(0.0f,0.0f,1.0f));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)

	cam.applyForce(physics->dynamicsWorld, 0.2f, glm::vec3(0.0f,0.0f,1.0f));

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)

	cam.applyForce(physics->dynamicsWorld, 10.0f, glm::vec3(0.0f,1.0f,0.0f));
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    cam.look(xpos,ypos);
    cam.updateCam();

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
