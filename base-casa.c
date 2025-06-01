#include <GL/glut.h>
#include <math.h>

// Largura e altura da janela
GLfloat windowWidth;
GLfloat windowHeight;

//Desenha um quadrado de lado 1 centrado na origem
void desenhaQuadrado() {
    glBegin(GL_QUADS);
        glVertex2f(-0.5, -0.5);
        glVertex2f(0.5, -0.5);
        glVertex2f(0.5, 0.5);
        glVertex2f(-0.5, 0.5);
    glEnd();
}

//Desenha um triângulo equilátero de lado 1 centrado na origem
void desenhaTriangulo() {
    glBegin(GL_TRIANGLES);
        glVertex2d(0.0, 0.5);
        glVertex2d(-0.5, -0.5);
        glVertex2d(0.5, -0.5);
    glEnd();
}

//Desenha um círculo de raio 1 centrado na origem
void desenhaCirculo() {
    glBegin(GL_POLYGON);
        for (int i = 0; i < 30; i++) {
            glVertex2f(cos(i), sin(i));
        }
    glEnd();
}

//Faz o cisalhamento de um objeto em relação ao eixo X
//Basta utilizar como uma das funções do OpenGL como glTranslatef, glRotatef, glScalef
void skewX(GLfloat angle) {
    GLfloat m[16] = {
        1, 0, 0, 0,
        tan(angle), 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0 ,1
    };
    glMultMatrixf(m);
}

//Faz o cisalhamento de um objeto em relação ao eixo Y
//Basta utilizar como uma das funções do OpenGL como glTranslatef, glRotatef, glScalef
void skewY(GLfloat angle) {
    GLfloat m[16] = {
        1, tan(angle), 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    glMultMatrixf(m);
}

// Função callback chamada para fazer o desenho
void Desenha(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT);

    GLfloat cW = (windowWidth > 0) ? windowWidth : 250.0f;
    GLfloat cH = (windowHeight > 0) ? windowHeight : 250.0f;

    GLfloat groundLevel = cH * 0.2f;
    glColor3f(0.2f, 0.6f, 0.1f);
    glPushMatrix();
        glTranslatef(cW / 2.0f, groundLevel / 2.0f, 0.0f);
        glScalef(cW, groundLevel, 1.0f);
        desenhaQuadrado();
    glPopMatrix();

    GLfloat houseCenterX = cW / 2.0f;
    GLfloat houseBaseWidth = 70.0f;
    GLfloat houseBaseHeight = 60.0f;
    GLfloat houseBaseBottomY = groundLevel;
    GLfloat houseBaseCenterY = houseBaseBottomY + houseBaseHeight / 2.0f;

    glColor3f(0.85f, 0.7f, 0.4f);
    glPushMatrix();
        glTranslatef(houseCenterX, houseBaseCenterY, 0.0f);
        glScalef(houseBaseWidth, houseBaseHeight, 1.0f);
        desenhaQuadrado();
    glPopMatrix();

    GLfloat roofWidth = houseBaseWidth * 1.1f;
    GLfloat roofHeight = 40.0f;
    GLfloat roofCenterY = houseBaseBottomY + houseBaseHeight + roofHeight / 2.0f;

    glColor3f(0.7f, 0.15f, 0.1f);
    glPushMatrix();
        glTranslatef(houseCenterX, roofCenterY, 0.0f);
        glScalef(roofWidth, roofHeight, 1.0f);
        desenhaTriangulo();
    glPopMatrix();

    GLfloat doorWidth = 18.0f;
    GLfloat doorHeight = 30.0f;
    GLfloat doorCenterY = houseBaseBottomY + doorHeight / 2.0f;

    glColor3f(0.4f, 0.25f, 0.1f);
    glPushMatrix();
        glTranslatef(houseCenterX, doorCenterY, 0.0f);
        glScalef(doorWidth, doorHeight, 1.0f);
        desenhaQuadrado();
    glPopMatrix();
    
    GLfloat windowSize = 15.0f;
    GLfloat windowX = houseCenterX + houseBaseWidth * 0.25f;
    GLfloat windowY = houseBaseCenterY + houseBaseHeight * 0.1f;

    glColor3f(0.5f, 0.75f, 0.9f);
    glPushMatrix();
        glTranslatef(windowX, windowY, 0.0f);
        glScalef(windowSize, windowSize, 1.0f);
        desenhaQuadrado();
    glPopMatrix();

    GLfloat sunRadius = 20.0f;
    glColor3f(1.0f, 0.9f, 0.1f);
    glPushMatrix();
        glTranslatef(cW * 0.8f, cH * 0.8f, 0.0f);
        glScalef(sunRadius, sunRadius, 1.0f);
        desenhaCirculo();
    glPopMatrix();

    // Carrinho Azul
    GLfloat carBodyWidth = 45.0f;
    GLfloat carBodyHeight = 22.0f;
    GLfloat carWheelRadius = 6.0f;
    GLfloat carBaseX = cW * 0.75f; // Posição X do centro do carro
    GLfloat carBaseY = groundLevel + carWheelRadius; // Base das rodas no nível do chão

    // Corpo do carro
    glColor3f(0.1f, 0.3f, 0.8f); // Azul
    glPushMatrix();
        glTranslatef(carBaseX, carBaseY + carBodyHeight/2.0f - carWheelRadius*0.1f, 0.0f);
        glScalef(carBodyWidth, carBodyHeight, 1.0f);
        desenhaQuadrado();
    glPopMatrix();
    
    // Topo do carro (cabine)
    GLfloat carTopWidth = carBodyWidth * 0.55f;
    GLfloat carTopHeight = carBodyHeight * 0.65f;
    glPushMatrix();
        glTranslatef(carBaseX - carBodyWidth * 0.05f, carBaseY + carBodyHeight - carWheelRadius*0.1f + carTopHeight/2.0f, 0.0f);
        glScalef(carTopWidth, carTopHeight, 1.0f);
        desenhaQuadrado();
    glPopMatrix();

    // Rodas do carro
    glColor3f(0.2f, 0.2f, 0.2f); // Cinza escuro para as rodas
    glPushMatrix(); // Roda traseira
        glTranslatef(carBaseX - carBodyWidth/2.0f + carWheelRadius * 1.3f, carBaseY, 0.0f);
        glScalef(carWheelRadius, carWheelRadius, 1.0f);
        desenhaCirculo();
    glPopMatrix();

    glPushMatrix(); // Roda dianteira
        glTranslatef(carBaseX + carBodyWidth/2.0f - carWheelRadius * 1.3f, carBaseY, 0.0f);
        glScalef(carWheelRadius, carWheelRadius, 1.0f);
        desenhaCirculo();
    glPopMatrix();


    glutSwapBuffers();
}

// Inicializa parâmetros de rendering
void Inicializa (void)
{
    // Define a cor de fundo da janela de visualização como preta
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

// Função callback chamada quando o tamanho da janela é alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
    // Evita a divisao por zero
    if(h == 0) h = 1;

    // Especifica as dimensões da Viewport
    glViewport(0, 0, w, h);

    // Inicializa o sistema de coordenadas
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Estabelece a janela de seleção (left, right, bottom, top)
    // Se a largura da janela, em pixels, for menor que a altura, a largura da viewport é fixada em 250 e a altura é escalada para o necessário
    if (w <= h)  {
        windowHeight = 250.0f*h/w;
                windowWidth = 250.0f;
    }
    else  {
    // Se a altura da janela, em pixels, for menor que a largura, a altura da viewport é fixada em 250 e a largura é escalada para o necessário
                windowWidth = 250.0f*w/h;
                windowHeight = 250.0f;
    }

    // As dimensões da janela vão de (0.0, 0.0) até (windowWidth, windowHeight), essas são as coordenadas da tela
    gluOrtho2D(0.0f, windowWidth, 0.0f, windowHeight);
}

// Programa Principal
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400,350);
    glutInitWindowPosition(10,10);
    glutCreateWindow("Minha Casa");
    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    Inicializa();
    glutMainLoop();
    return 0;
}
