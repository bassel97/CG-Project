// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GL/glfw.h>

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	//The following line specifies that we will use 4x anti aliasing. Anti-aliasing is the technique of minimizing the distortion artifacts known as aliasing when representing a high-resolution image at a lower resolution.
	//FSAA stands for full-scene anti-aliasing which makes the image appear softer, and should also appear more realistic.   
	//FSAA means that each full frame is rendered at double (2x) or quadruple (4x) the display resolution, and then down-sampled to match the display resolution.
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);


	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 1);

	// Open a window and create its OpenGL context
	//glfwOpenWindow( int width, int height, int redbits,int greenbits, int bluebits, int alphabits, int depthbits,int stencilbits, int mode ) 
	//redbits,greenbits,bluebits: number of bits to use for each color.(0 means default color depth) 
	//mode: specifies whether we will open a GLFW_FullScreen or a GLFW_WINDOW
	if( !glfwOpenWindow( 1024, 768, 0,0,0,0, 32,0, GLFW_WINDOW ) )
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		return -1;
	}

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	glfwSetWindowTitle( "First Window" );

	// Ensure we can capture the escape key being pressed below
	glfwEnable( GLFW_STICKY_KEYS );

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	do{
		
		glClear( GL_COLOR_BUFFER_BIT );//buffers are currently enabled for color writing
		// Swap buffers
		glfwSwapBuffers();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
		   glfwGetWindowParam( GLFW_OPENED ) );

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

