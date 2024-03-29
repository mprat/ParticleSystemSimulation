#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <vector>

#include <cstring>

#include <GL/glut.h>
#include <vecmath.h>
#include "camera.h"

#include "TimeStepper.hpp"
#include "simpleSystem.h"
#include "pendulumSystem.h"
#include "ClothSystem.h"
#include "CubeSystem.h"

using namespace std;

// Globals here.
namespace
{

    ParticleSystem *system;
	ParticleSystem *simplesystem;
	ParticleSystem *pendulumsystem;
	ParticleSystem *particlechain;
	ParticleSystem *clothsystem;
	ParticleSystem *cubesystem;
	vector<ParticleSystem*> system_toggle;
    int system_index = 0;
	TimeStepper * timeStepper;
	double h;
	bool swingon = false;
	bool positive = true;
	int timercounter = 0;
	bool wire = false;

  // initialize your particle systems
  void initSystem(int argc, char * argv[])
  {
    // seed the random number generator with the current time
    srand( time( NULL ) );

	if (argc != 3){
		cout<<"Must have two arguments"<<endl;
		exit(0);
	} else {
		if (strcmp(argv[1], "e") == 0){	
			timeStepper = new ForwardEuler();
			cout<<"Initializing Forward Euler time stepper"<<endl;
		} else if (strcmp(argv[1], "t") == 0){
			timeStepper = new Trapezoidal();
			cout<<"Initializing Trapezoidal time stepper"<<endl;
		} else if (strcmp(argv[1], "r") == 0){	
			timeStepper = new RK4();
			cout<<"Initializing RK4 time stepper"<<endl;
		} else {
			timeStepper = new RK4();
			cout<<"Not a valid time stepper - has been initialized to RK4"<<endl;
		}
		simplesystem= new SimpleSystem();
		pendulumsystem = new PendulumSystem(2);
		particlechain = new PendulumSystem(4);
		clothsystem = new ClothSystem(8, 8);
		cubesystem = new CubeSystem(5, 5, 5);
		system_toggle.push_back(simplesystem);
		system_toggle.push_back(pendulumsystem);
		system_toggle.push_back(particlechain);
		system_toggle.push_back(clothsystem);
		system_toggle.push_back(cubesystem);
		system = simplesystem; 	

		h = atof(argv[2]);
	}
  }

  // Take a step forward for the particle shower
  void stepSystem()
  {
	if(timeStepper!=0){
		timeStepper->takeStep(system,h);
    }
  }

  // Draw the current particle positions
  void drawSystem()
  {
    
    // Base material colors (they don't change)
    GLfloat particleColor[] = {0.4f, 0.7f, 1.0f, 1.0f};
    GLfloat floorColor[] = {1.0f, 0.0f, 0.0f, 1.0f};
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, particleColor);

	if (system_index == 3 && wire == true){
		ClothSystem* clothsystemcast = static_cast<ClothSystem*>(clothsystem);
		clothsystemcast->drawSprings();
	} else {
		system->draw();
	}
 
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, floorColor);
    glPushMatrix();
    glTranslatef(0.0f,-5.0f,0.0f);
    glScaled(50.0f,0.01f,50.0f);
    glutSolidCube(1);
    glPopMatrix();
    
  }
        

    //-------------------------------------------------------------------
    
        
    // This is the camera
    Camera camera;

    // These are state variables for the UI
    bool g_mousePressed = false;

    // Declarations of functions whose implementations occur later.
    void arcballRotation(int endX, int endY);
    void keyboardFunc( unsigned char key, int x, int y);
    void specialFunc( int key, int x, int y );
    void mouseFunc(int button, int state, int x, int y);
    void motionFunc(int x, int y);
    void reshapeFunc(int w, int h);
    void drawScene(void);
    void initRendering();

    // This function is called whenever a "Normal" key press is
    // received.
    void keyboardFunc( unsigned char key, int x, int y )
    {
        switch ( key )
        {
        case 27: // Escape key
            exit(0);
            break;
        case ' ':
        {
            Matrix4f eye = Matrix4f::identity();
            camera.SetRotation( eye );
            camera.SetCenter( Vector3f::ZERO );
            break;
        }
		case 't':
		case 'T':
		{
			system_index = (system_index + 1) % (system_toggle.size());
			system = system_toggle[system_index];	
			
			string s;
			switch (system_index){
				case 0:
					s = "simple";
					break;
				case 1:
					s = "pendulum";
					break;
				case 2:
					s = "particle chain";
					break;
				case 3:
					s = "cloth system";
					break;
			}
			cout<<"Toggle: showing "<<s<<" system"<<endl; 
			break;
		}
		case 's':
		case 'S':
		{
			//only swing the cloth
			if (system_index == 3){
				swingon = !swingon;
				string swingstatus;
				if (swingon){
					swingstatus = "on";
					ClothSystem* clothsystemcast = static_cast<ClothSystem*>(clothsystem);
					for (unsigned i = 0; i < (clothsystemcast->fixedpoints).size(); i++){
						clothsystemcast->addExternalForce(ExternalForce((clothsystemcast->fixedpoints)[i], Vector3f(0,0, 1)));
						positive = true;
					}
				} else {
					swingstatus = "off";
					static_cast<ClothSystem*>(clothsystem)->clearExternalForces();
				}
				cout<<"Swing "<<swingstatus<<endl;
			} else if (system_index == 4){
				swingon = !swingon;
				string swingstatus;
				if (swingon){
					swingstatus = "on";
					CubeSystem* cubesystemcast = static_cast<CubeSystem*>(cubesystem);
					for (unsigned i = 0; i < (cubesystemcast->fixedpoints).size(); i++){
						cubesystemcast->addExternalForce(ExternalForce((cubesystemcast->fixedpoints)[i], Vector3f(0,0, .2)));
						positive = true;
					}
				} else {
					swingstatus = "off";
					static_cast<ClothSystem*>(clothsystem)->clearExternalForces();
				}
				cout<<"Swing "<<swingstatus<<endl;

			} else {
				cout<<"Unhandled key press "<<key<<"."<<endl;
			}
			break;
		}
		case 'r':
		case 'R':
		{
			//reset
			string s;
			switch(system_index){
				case 0:
					static_cast<SimpleSystem*>(simplesystem)->reset();
					s = "simple";
					break;
				case 1:
					static_cast<PendulumSystem*>(pendulumsystem)->reset(2);
					s = "pendulum";
					break;
				case 2:
					static_cast<PendulumSystem*>(particlechain)->reset(4);
					s = "particle";
					break;
				case 3:
					static_cast<ClothSystem*>(clothsystem)->reset(8, 8);
					s = "cloth";
					break;
				case 4:
					static_cast<CubeSystem*>(cubesystem)->reset(5, 5, 5);
					s = "cube";
					break;

			}	
			cout<<"Reset "<<s<<" system"<<endl;
			break;
		}
		case 'w':
		case 'W':
		{
			if (system_index == 3){
				wire = !wire;
			} else {
				cout<<"Unhandled key press "<<key<<"."<<endl;
			}
			break;
		}
        default:
            cout << "Unhandled key press " << key << "." << endl;        
        }

        glutPostRedisplay();
    }

    // This function is called whenever a "Special" key press is
    // received.  Right now, it's handling the arrow keys.
    void specialFunc( int key, int x, int y )
    {
        switch ( key )
        {

        }
        //glutPostRedisplay();
    }

    //  Called when mouse button is pressed.
    void mouseFunc(int button, int state, int x, int y)
    {
        if (state == GLUT_DOWN)
        {
            g_mousePressed = true;
            
            switch (button)
            {
            case GLUT_LEFT_BUTTON:
                camera.MouseClick(Camera::LEFT, x, y);
                break;
            case GLUT_MIDDLE_BUTTON:
                camera.MouseClick(Camera::MIDDLE, x, y);
                break;
            case GLUT_RIGHT_BUTTON:
                camera.MouseClick(Camera::RIGHT, x,y);
            default:
                break;
            }                       
        }
        else
        {
            camera.MouseRelease(x,y);
            g_mousePressed = false;
        }
        glutPostRedisplay();
    }

    // Called when mouse is moved while button pressed.
    void motionFunc(int x, int y)
    {
        camera.MouseDrag(x,y);        
    
        glutPostRedisplay();
    }

    // Called when the window is resized
    // w, h - width and height of the window in pixels.
    void reshapeFunc(int w, int h)
    {
        camera.SetDimensions(w,h);

        camera.SetViewport(0,0,w,h);
        camera.ApplyViewport();

        // Set up a perspective view, with square aspect ratio
        glMatrixMode(GL_PROJECTION);

        camera.SetPerspective(50);
        glLoadMatrixf( camera.projectionMatrix() );
    }

    // Initialize OpenGL's rendering modes
    void initRendering()
    {
        glEnable(GL_DEPTH_TEST);   // Depth testing must be turned on
        glEnable(GL_LIGHTING);     // Enable lighting calculations
        glEnable(GL_LIGHT0);       // Turn on light #0.

        glEnable(GL_NORMALIZE);

        // Setup polygon drawing
        glShadeModel(GL_SMOOTH);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

        // Clear to black
        glClearColor(0,0,0,1);
    }

    // This function is responsible for displaying the object.
    void drawScene(void)
    {
        // Clear the rendering window
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode( GL_MODELVIEW );  
        glLoadIdentity();              

        // Light color (RGBA)
        GLfloat Lt0diff[] = {1.0,1.0,1.0,1.0};
        GLfloat Lt0pos[] = {3.0,3.0,5.0,1.0};
        glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
        glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);

        glLoadMatrixf( camera.viewMatrix() );

        // THIS IS WHERE THE DRAW CODE GOES.

        drawSystem();

        // This draws the coordinate axes when you're rotating, to
        // keep yourself oriented.
        if( g_mousePressed )
        {
            glPushMatrix();
            Vector3f eye = camera.GetCenter();
            glTranslatef( eye[0], eye[1], eye[2] );

            // Save current state of OpenGL
            glPushAttrib(GL_ALL_ATTRIB_BITS);

            // This is to draw the axes when the mouse button is down
            glDisable(GL_LIGHTING);
            glLineWidth(3);
            glPushMatrix();
            glScaled(5.0,5.0,5.0);
            glBegin(GL_LINES);
            glColor4f(1,0.5,0.5,1); glVertex3f(0,0,0); glVertex3f(1,0,0);
            glColor4f(0.5,1,0.5,1); glVertex3f(0,0,0); glVertex3f(0,1,0);
            glColor4f(0.5,0.5,1,1); glVertex3f(0,0,0); glVertex3f(0,0,1);

            glColor4f(0.5,0.5,0.5,1);
            glVertex3f(0,0,0); glVertex3f(-1,0,0);
            glVertex3f(0,0,0); glVertex3f(0,-1,0);
            glVertex3f(0,0,0); glVertex3f(0,0,-1);

            glEnd();
            glPopMatrix();

            glPopAttrib();
            glPopMatrix();
        }
                 
        // Dump the image to the screen.
        glutSwapBuffers();
    }

    void timerFunc(int t)
    {
        stepSystem();
		timercounter++;
	
		if (swingon && timercounter % 500 == 0){
			if (system_index == 3){
				ClothSystem* clothsystemcast = static_cast<ClothSystem*>(clothsystem);
				clothsystemcast->clearExternalForces();
				for (unsigned i = 0; i < (clothsystemcast->fixedpoints).size(); i++){
					if (positive){
						clothsystemcast->addExternalForce(ExternalForce((clothsystemcast->fixedpoints)[i], Vector3f(0,0, -1)));
					} else {
						clothsystemcast->addExternalForce(ExternalForce((clothsystemcast->fixedpoints)[i], Vector3f(0,0, 1)));
					}
				}
			} else if (system_index == 4){
				CubeSystem* cubesystemcast = static_cast<CubeSystem*>(cubesystem);
				cubesystemcast->clearExternalForces();
				for (unsigned i = 0; i < (cubesystemcast->fixedpoints).size(); i++){
					if (positive){
						cubesystemcast->addExternalForce(ExternalForce((cubesystemcast->fixedpoints)[i], Vector3f(0,0, -0.2)));
					} else {
						cubesystemcast->addExternalForce(ExternalForce((cubesystemcast->fixedpoints)[i], Vector3f(0,0, 0.2)));
					}
				}
			}
			positive = !positive;
			timercounter = 0;
		}	
	
        glutPostRedisplay();

        glutTimerFunc(t, &timerFunc, t);
    }

    

    
    
}

// Main routine.
// Set up OpenGL, define the callbacks and start the main loop
int main( int argc, char* argv[] )
{
    glutInit( &argc, argv );

    // We're going to animate it, so double buffer 
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

    // Initial parameters for window position and size
    glutInitWindowPosition( 60, 60 );
    glutInitWindowSize( 600, 600 );
    
    camera.SetDimensions( 600, 600 );

    camera.SetDistance( 10 );
    camera.SetCenter( Vector3f::ZERO );
    
    glutCreateWindow("Assignment 4");

    // Initialize OpenGL parameters.
    initRendering();

    // Setup particle system
    initSystem(argc,argv);

    // Set up callback functions for key presses
    glutKeyboardFunc(keyboardFunc); // Handles "normal" ascii symbols
    glutSpecialFunc(specialFunc);   // Handles "special" keyboard keys

    // Set up callback functions for mouse
    glutMouseFunc(mouseFunc);
    glutMotionFunc(motionFunc);

    // Set up the callback function for resizing windows
    glutReshapeFunc( reshapeFunc );

    // Call this whenever window needs redrawing
    glutDisplayFunc( drawScene );

    // Trigger timerFunc every 20 msec
    glutTimerFunc(20, timerFunc, 20);

        
    // Start the main loop.  glutMainLoop never returns.
    glutMainLoop();

    return 0;	// This line is never reached.
}
