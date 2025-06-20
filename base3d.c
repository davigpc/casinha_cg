#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

int win_id;
GLUquadric *quadric;

float left_arm_angle = 0.0f;
float right_arm_angle = 0.0f;
const float depth = 10.0f;

void myKeyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 'a':
        case 'A':
            left_arm_angle += 5.0f;
            if (left_arm_angle > 180.0f)
                left_arm_angle = 180.0f;
            printf("Left arm angle: %.1f\n", left_arm_angle);
            break;
        case 'd':
        case 'D':
            left_arm_angle -= 5.0f;
            if (left_arm_angle < -180.0f)
                left_arm_angle = -180.0f;
            printf("Left arm angle: %.1f\n", left_arm_angle);
            break;
        case 'w':
        case 'W':
            right_arm_angle += 5.0f;
            if (right_arm_angle > 180.0f)
                right_arm_angle = 180.0f;
            printf("Right arm angle: %.1f\n", right_arm_angle);
            break;
        case 's':
        case 'S':
            right_arm_angle -= 5.0f;
            if (right_arm_angle < -180.0f)
                right_arm_angle = -180.0f;
            printf("Right arm angle: %.1f\n", right_arm_angle);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void myKeyboardSpecial(int key, int x, int y) {
    if (key == GLUT_KEY_UP) glutFullScreen();
    else if (key == GLUT_KEY_DOWN) glutReshapeWindow(640, 480);
}

void myMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float r = (float)rand() / RAND_MAX;
        float g = (float)rand() / RAND_MAX;
        float b = (float)rand() / RAND_MAX;
        glColor3f(r, g, b);
        printf("Cor: %.2f, %.2f, %.2f - posição: %d, %d\n", r, g, b, x, y);
    }
    glutPostRedisplay();
}

void init(void)
{
    glClearColor(0.7f, 0.8f, 1.0f, 1.0f);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat light_ambient[]  = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 50.0, 50.0, 50.0, 0.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_FILL);
}

void drawCube(float x1, float y1, float x2, float y2, float z1, float z2)
{
    glBegin(GL_QUADS);
    // Frente
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y1, z1);
    glVertex3f(x2, y2, z1);
    glVertex3f(x1, y2, z1);
    // Fundo
    glVertex3f(x1, y1, z2);
    glVertex3f(x2, y1, z2);
    glVertex3f(x2, y2, z2);
    glVertex3f(x1, y2, z2);
    // Topo
    glVertex3f(x1, y2, z2);
    glVertex3f(x2, y2, z2);
    glVertex3f(x2, y2, z1);
    glVertex3f(x1, y2, z1);
    // Base
    glVertex3f(x1, y1, z2);
    glVertex3f(x2, y1, z2);
    glVertex3f(x2, y1, z1);
    glVertex3f(x1, y1, z1);
    // Direita
    glVertex3f(x2, y1, z2);
    glVertex3f(x2, y2, z2);
    glVertex3f(x2, y2, z1);
    glVertex3f(x2, y1, z1);
    // Esquerda
    glVertex3f(x1, y1, z2);
    glVertex3f(x1, y2, z2);
    glVertex3f(x1, y2, z1);
    glVertex3f(x1, y1, z1);
    glEnd();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 85.0f,
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);

    // Braço Esquerdo
    glPushMatrix();
    glTranslatef(-15.0f, 5.0f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);  // alinhamento cilindro no eixo Z
    glRotatef(left_arm_angle, 0.0f, 1.0f, 0.0f);  // rotaciona em Y
    glColor3f(1.0f, 1.0f, 0.0f);
    gluCylinder(quadric, 2.0, 2.0, 15.0, 20, 5);
    gluDisk(quadric, 0.0, 2.0, 20, 1);
    glPopMatrix();

    // Braço Direito
    glPushMatrix();
    glTranslatef(15.0f, 5.0f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(-right_arm_angle, 0.0f, 1.0f, 0.0f);
    glColor3f(1.0f, 1.0f, 0.0f);
    gluCylinder(quadric, 2.0, 2.0, 15.0, 20, 5);
    gluDisk(quadric, 0.0, 2.0, 20, 1);
    glPopMatrix();

    // Corpo principal (cubo)
    glColor3f(1.0f, 0.9f, 0.0f);
    drawCube(-15.0f, 25.0f, 15.0f, -10.0f, 0.0f, -depth);

    // Calças (cubo)
    glColor3f(0.2f, 0.0f, 0.0f);
    drawCube(-15.0f, -10.0f, 15.0f, -25.0f, 0.0f, -depth);

    // Pernas (cubos)
    glColor3f(1.0f, 0.9f, 0.0f);
    drawCube(-9.0f, -25.0f, -5.0f, -35.0f, 0.0f, -depth);
    drawCube(5.0f, -25.0f, 9.0f, -35.0f, 0.0f, -depth);

    // Olhos (esferas)
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix(); glTranslatef(-7.0f, 15.0f, 0.1f); glutSolidSphere(5.0, 20, 20); glPopMatrix();
    glPushMatrix(); glTranslatef(7.0f, 15.0f, 0.1f); glutSolidSphere(5.0, 20, 20); glPopMatrix();

    // Pupilas (esferas)
    glColor3f(0.1f, 0.1f, 0.1f);
    glPushMatrix(); glTranslatef(-6.0f, 15.0f, 4.5f); glutSolidSphere(2.0, 20, 20); glPopMatrix();
    glPushMatrix(); glTranslatef(8.0f, 15.0f, 4.5f); glutSolidSphere(2.0, 20, 20); glPopMatrix();

    // Nariz (cubo)
    glColor3f(1.0f, 0.8f, 0.3f);
    drawCube(-3.0f, 7.0f, 3.0f, 3.0f, 0.1f, -depth/2);

    // Dentes (cubos)
    glColor3f(1.0f, 1.0f, 1.0f);
    drawCube(-7.0f, -1.0f, -1.0f, -5.0f, 0.1f, -depth/2);
    drawCube(1.0f, -1.0f, 7.0f, -5.0f, 0.1f, -depth/2);

    // Boca 
    glColor3f(0.8f, 0.1f, 0.1f);
    drawCube(-7.0f, 0.2f, 7.0f, -1.0f, 0.1f, -depth/2);

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-50, 50, -50 * (GLfloat)h/w, 50 * (GLfloat)h/w, -100.0, 100.0);
    else
        glOrtho(-50 * (GLfloat)w/h, 50 * (GLfloat)w/h, -50, 50, -100.0, 100.0);
}

void cleanup() {
    gluDeleteQuadric(quadric);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowSize(600, 800);
    glutInitWindowPosition(100, 100);
    win_id = glutCreateWindow("Bob Esponja");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(myKeyboard);
    glutSpecialFunc(myKeyboardSpecial);
    glutMouseFunc(myMouse);
    atexit(cleanup);
    glutMainLoop();
    return 0;
}
