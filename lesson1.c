// This code was created by Jeff Molofee '99 (ported to Linux/GLUT by Richard Campbell '99)
//
// If you've found this code useful, please let me know.
//
// Visit me at www.demonews.com/hosted/nehe
// (email Richard Campbell at ulmont@bellsouth.net)
//
#include <GL/glut.h>    // Header File For The GLUT Library
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#include <GL/glu.h>     // Header File For The GLu32 Library
#include <unistd.h>     // Header file for sleeping.
#include <math.h>

/* ascii code for the escape key */
#define ESCAPE 27

/* The number of our GLUT window */
int window;

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void InitGL(int Width, int Height)              // We call this right after our OpenGL window is created.
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);               // This Will Clear The Background Color To Black
    glClearDepth(1.0);                          // Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LESS);                               // The Type Of Depth Test To Do
    glEnable(GL_DEPTH_TEST);                    // Enables Depth Testing
    glShadeModel(GL_SMOOTH);                    // Enables Smooth Color Shading

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();                           // Reset The Projection Matrix

    // Set up an orthogonal projection
    glOrtho(-Width / 200.0f, Width / 200.0f, -Height / 200.0f, Height / 200.0f, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
}

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
void ReSizeGLScene(int Width, int Height)
{
    if (Height == 0)                            // Prevent A Divide By Zero If The Window Is Too Small
        Height = 1;

    glViewport(0, 0, Width, Height);            // Reset The Current Viewport And Perspective Transformation

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Adjust the orthogonal projection to match the new window size
    glOrtho(-Width / 200.0f, Width / 200.0f, -Height / 200.0f, Height / 200.0f, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
}

/* The main drawing function. */
void DrawGLScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    // Clear The Screen And The Depth Buffer
    glLoadIdentity();                                      // Reset The View

    // Desenhar o quadrado vermelho
    glColor3f(1.0f, 0.4f, 0.0f);
    glBegin(GL_QUADS);
        glVertex3f(-1.0f, -0.5f, -5.0f);
        glVertex3f(1.0f, -0.5f, -5.0f);
        glVertex3f(1.0f, 0.5f, -5.0f);
        glVertex3f(-1.0f, 0.5f, -5.0f);
    glEnd();

    // Desenhar o triângulo amarelo
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLES);
        glVertex3f(-1.0f, 0.55f, -5.0f);
        glVertex3f(-0.5f, 1.2f, -5.0f);
        glVertex3f(0.0f, 0.55f, -5.0f);
    glEnd();

    // Calcular o centro do triângulo
    float centerX = (-1.0f + (-0.5f) + 0.0f) / 3.0f;
    float centerY = (0.55f + 1.2f + 0.55f) / 3.0f;

    // Desenhar o círculo vermelho dentro do triângulo
    glColor3f(1.0f, 0.4f, 0.0f);
    glBegin(GL_POLYGON);
        for (int i = 0; i < 50; i++) {
            float angle = 2 * 3.14159f * i / 50;
            glVertex3f(centerX + cos(angle) * 0.15f, centerY + sin(angle) * 0.15f, -4.9f);
        }
    glEnd();

    // Desenhar o paralelogramo
    glColor3f(1.0f, 0.8f, 0.0f);
    glBegin(GL_QUADS);

        glVertex3f(0.1f, 0.55f, -5.0f);
        glVertex3f(-0.4f, 1.2f, -5.0f);
        glVertex3f(0.5f, 1.2f, -5.0f);
        glVertex3f(1.0f, 0.55f, -5.0f);
    glEnd();

    // Desenhar porta
    glColor3f(1.0f, 0.8f, 0.0f);
    glBegin(GL_QUADS);
       glVertex3f(-0.35f, -0.4f, -4.9f);
       glVertex3f(-0.35f, 0.0f, -4.9f);
       glVertex3f(-0.7f, 0.0f, -4.9f);
       glVertex3f(-0.7f, -0.4f, -4.9f);
    glEnd();

    // Desenhar janela
            glColor3f(1.0f, 1.0f, 1.0f);
            glBegin(GL_QUADS);
                glVertex3f(0.3f, -0.1f, -4.0f);  // Traseira inferior
                glVertex3f(0.7f, -0.1f, -4.0f);  // Dianteira inferior
                glVertex3f(0.7f, 0.3f, -4.0f);   // Dianteira superior
                glVertex3f(0.3f, 0.3f, -4.0f);   // Traseira superior
            glEnd();

    // Corpo do carrinho (retângulo azul)
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex3f(1.5f, -0.3f, -5.0f);  // Traseira inferior
        glVertex3f(2.0f, -0.3f, -5.0f);  // Dianteira inferior
        glVertex3f(2.0f, 0.3f, -5.0f);   // Dianteira superior
        glVertex3f(1.5f, 0.3f, -5.0f);   // Traseira superior
    glEnd();

     // Corpo do carrinho (retângulo azul)
        glColor3f(0.0f, 0.0f, 1.0f);
        glBegin(GL_QUADS);
            glVertex3f(1.4f, -0.3f, -5.0f);  // Traseira inferior
            glVertex3f(2.2f, -0.3f, -5.0f);  // Dianteira inferior
            glVertex3f(2.2f, 0.0f, -5.0f);   // Dianteira superior
            glVertex3f(1.4f, 0.0f, -5.0f);   // Traseira superior
        glEnd();

    // Roda traseira (círculo preto)
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
        for (int i = 0; i < 50; i++) {
            float angle = 2 * 3.14159f * i / 50;
            glVertex3f(1.6f + cos(angle) * 0.1f, -0.4f + sin(angle) * 0.1f, -4.9f);
        }
    glEnd();

    // Roda dianteira (círculo preto)
    glBegin(GL_POLYGON);
        for (int i = 0; i < 50; i++) {
            float angle = 2 * 3.14159f * i / 50;
            glVertex3f(2.0f + cos(angle) * 0.1f, -0.4f + sin(angle) * 0.1f, -4.9f);
        }
    glEnd();

    glutSwapBuffers();
}

/* The function called whenever a key is pressed. */
void keyPressed(unsigned char key, int x, int y)
{
    /* avoid thrashing this procedure */
    usleep(100);

    /* If escape is pressed, kill everything. */
    if (key == ESCAPE)
    {
        /* shut down our window */
        glutDestroyWindow(window);

        /* exit the program...normal termination. */
        exit(0);
    }
}

int main(int argc, char **argv)
{
  /* Initialize GLUT state - glut will take any command line arguments that pertain to it or
     X Windows - look at its documentation at http://reality.sgi.com/mjk/spec3/spec3.html */
  glutInit(&argc, argv);

  /* Select type of Display mode:
     Double buffer
     RGBA color
     Alpha components supported
     Depth buffer */
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

  /* get a 640 x 480 window */
  glutInitWindowSize(640, 480);

  /* the window starts at the upper left corner of the screen */
  glutInitWindowPosition(100, 100);

  /* Open a window */
  window = glutCreateWindow("Jeff Molofee's GL Code Tutorial ... NeHe '99");

  /* Register the function to do all our OpenGL drawing. */
  glutDisplayFunc(&DrawGLScene);

  /* Even if there are no events, redraw our gl scene. */
  glutIdleFunc(&DrawGLScene);

  /* Register the function called when our window is resized. */
  glutReshapeFunc(&ReSizeGLScene);

  /* Register the function called when the keyboard is pressed. */
  glutKeyboardFunc(&keyPressed);

  /* Initialize our window. */
  InitGL(640, 480);

  /* Start Event Processing Engine */
  glutMainLoop();

  return 1;
}
