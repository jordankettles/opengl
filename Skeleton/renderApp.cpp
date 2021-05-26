/*
 * renderApp.cpp
 *
 * by Stefanie Zollmann
 *
 * Basic model loading with assimp library
 *
 */


// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

// Include GLEW
#include <GL/glew.h>


// Include GLFW
#include <glfw3.h>
GLFWwindow* window;


// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/Shader.hpp>
#include <common/Texture.hpp>
#include <common/Object.hpp>
#include <common/MTLShader.hpp> //Why would they add this line. common/ColorShader.hpp
#include <common/Scene.hpp>
#include <common/Triangle.hpp>
#include <common/BasicMaterialShader.hpp>
#include <common/Mesh.hpp>
#include <common/Controls.hpp>
#include <common/Group.hpp>
#include <common/Objloader.hpp>
#include <common/Quad.hpp>


bool initWindow(std::string windowName){
    
    // Initialise GLFW
    if( !glfwInit() ){
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return false;
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, windowName.c_str(), NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.\n" );
        getchar();
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    
    return true;
    
}




int main( int argc, char *argv[] )
{
    
    initWindow("renderEngine Skeleton");
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);
    
    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Enable Blending.
    glEnable(GL_BLEND);
    // Blend function.
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    
    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

    int windowWidth = 1024;
	int windowHeight = 768;
	// Get the size of the window in case we are using MacOS.
	glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
    
    
    //create a Vertex Array Object and set it as the current one
    //we will not go into detail here. but this can be used to optimise the performance by storing all of the state needed to supply vertex data
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
    /*
     * Create Scene
     * With a object and texture loaded from loader
     */
    
    Scene* myScene = new Scene();
    
    // Read our .obj files - this is hard coded for testing - you can pass obj file names as arguments instead to make the code more flexible
    
    if(argc==1){
        // if nothing specified we load default objects
        // person obj
        Group* person = new Group();
        bool res = loadOBJMTL("person.obj", person);
        person->init();
        
        //earth obj
        Group* sphere = new Group();
        res = loadOBJMTL("earthobj.obj", sphere);
        sphere->init();
        
        //add objects to the scene
        myScene->addObject(person);
        myScene->addObject(sphere);
        
    }
    else{
        
        for (int a = 1; a < argc; ++a) {
            Group* objGroup = new Group();
            bool res = loadOBJMTL(argv[a], objGroup);
            if(res){
                objGroup->init();
                //add objects to the scene
                myScene->addObject(objGroup);
            }
        }
    }
    
    //add code for render to texture here.
    //Create a frame buffer called my Framebuffer
    GLuint MyFramebuffer = 0;
    glGenFramebuffers(1, &MyFramebuffer);
    //Bind my frame buffer.
    glBindFramebuffer(GL_FRAMEBUFFER, MyFramebuffer);

    // Create a texture to render to.
    GLuint textureBeforeEffect;
    glGenTextures(1, &textureBeforeEffect);
    //Bind the texture.
    glBindTexture(GL_TEXTURE_2D, textureBeforeEffect);

    //Give an empty image to OpenGL to render the texture to.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    // Poor filtering 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
    // Dont forget the depth buffer otherwise depth testing will fail
    GLuint depthrenderbuffer;
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowWidth, windowHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

    //set textureBeforeEffect to color attachment 0.
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureBeforeEffect, 0);

    /*// Create a list of draw buffers.
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers	*/

    
    //Check the frame buffer is ok.
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        return 0;
    }

    MTLShader* postShader = new MTLShader("passthrough.vert", "specialeffect.frag");
    //No effect.
    postShader->setRenderMode(1.0);


    //create my Quad.
    Quad* outputQuad = new Quad();
    
    Camera* myCamera = new Camera();
    myCamera->setPosition(glm::vec3(0,100,200)); //set camera to show the models
    Controls* myControls = new Controls(myCamera);
    myControls->setSpeed(60);
    
    // For speed computation
	double lastTime = glfwGetTime();
	int nbFrames = 0;
    //Render loop
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 ){// Clear the screen
        
        //Render to my Frame Buffer.
        glBindFramebuffer(GL_FRAMEBUFFER, MyFramebuffer);
        glViewport(0,0, windowWidth, windowHeight);

        // Measure speed
		double currentTime = glfwGetTime();
		nbFrames++;
		if ( currentTime - lastTime >= 1.0 ){ // If last printf() was more than 1sec ago
			// printf and reset
			printf("%f ms/frame\n", 1000.0/double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}
        
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Also clear the depth buffer!!!
        
        // update camera controls with mouse input
        myControls->update();
        // takes care of all the rendering automatically
        myScene->render(myCamera);

        // Render to the screen
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, windowWidth, windowHeight);

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Also clear the depth buffer!!!

        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
            // Turn effect off.
            postShader->setRenderMode(1.0);
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
            // Turn on blank effect.
            postShader->setRenderMode(2.0);
        }
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
            // Turn on blank effect.
            postShader->setRenderMode(3.0);
        }
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
            // Turn on blank effect.
            postShader->setRenderMode(4.0);
        }
        postShader->setTime((float) currentTime*10.0f);
        postShader->bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureBeforeEffect);
        outputQuad->directRender();

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    
    glDeleteVertexArrays(1, &VertexArrayID);
    //delete texture;
    delete myScene;
    delete myCamera;
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    
    return 0;
}

