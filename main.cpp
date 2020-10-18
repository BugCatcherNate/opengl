#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <utils/shader.h>
#include <utils/camera.h>
#include <utils/object.h>
#include "btBulletDynamicsCommon.h"
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtx/quaternion.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#include <glm/gtc/type_ptr.hpp>

// Call backs
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera cam = Camera(glm::vec3(0.0f,0.0f,3.0f), SCR_WIDTH, SCR_HEIGHT);
Object cube = Object(glm::vec3(2.0f,50.0f,0.0f));
Object ground = Object(glm::vec3(0.0f,0.0f,0.0f));

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{

        ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
        btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

        ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
        btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

        ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
        btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

        ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
        btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

        btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

        dynamicsWorld->setGravity(btVector3(0, -10, 0));

        ///-----initialization_end-----

        //keep track of the shapes, we release memory at exit.
        //make sure to re-use collision shapes among rigid bodies whenever possible!
        btAlignedObjectArray<btCollisionShape*> collisionShapes;

        ///create a few basic rigid bodies

        //the ground is a cube of side 100 at position y = -56.
        //the sphere will hit it at y = -6, with center at -5
  
	cube.getCollision(collisionShapes, dynamicsWorld, 0.2f);

	ground.getCollision(collisionShapes, dynamicsWorld, 0.0f);     
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

    cube.prepare();
    ground.prepare();
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
	float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        // -----
       
                dynamicsWorld->stepSimulation(deltaTime, 10);

                //print positions of all objects

        

		cube.runPhysics(dynamicsWorld);
		ground.runPhysics(dynamicsWorld);
        ///-----stepsimulation_end-----

        //cleanup in the reverse order of creation/initialization

        ///-----cleanup_start-----

        //remove the rigidbodies from the dynamics world and delete them
               processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	cube.draw(cam, s);
	ground.draw(cam, s);


        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
 for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
        {
                btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
                btRigidBody* body = btRigidBody::upcast(obj);
                if (body && body->getMotionState())
                {
                        delete body->getMotionState();
                }
                dynamicsWorld->removeCollisionObject(obj);
                delete obj;
        }

        //delete collision shapes
        for (int j = 0; j < collisionShapes.size(); j++)
        {
                btCollisionShape* shape = collisionShapes[j];
                collisionShapes[j] = 0;
                delete shape;
        }

        //delete dynamics world
        delete dynamicsWorld;

        //delete solver
        delete solver;

        //delete broadphase
        delete overlappingPairCache;

        //delete dispatcher
        delete dispatcher;

        delete collisionConfiguration;

        //next line is optional: it will be cleared by the destructor when the array goes out of scope
        collisionShapes.clear();



    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.incPosition(cameraSpeed * cam.getFront());

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.incPosition(-cameraSpeed * cam.getFront());

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.incPosition(-glm::normalize(glm::cross(cam.getFront(), cam.getUp())) * cameraSpeed);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.incPosition(glm::normalize(glm::cross(cam.getFront(), cam.getUp())) * cameraSpeed);

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
