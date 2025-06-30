#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef struct {
    double x1, y1, x2, y2;
} Linha;

Linha linhasAleatorias[10];
int etapaAtual = 0;
const int NUM_LINHAS = 10;

int windowWidth = 800, windowHeight = 600;
float rectWidth = 400.0f, rectHeight = 300.0f;

const int DENTRO   = 0; // 0000
const int ESQUERDA = 1; // 0001
const int DIREITA  = 2; // 0010
const int BAIXO    = 4; // 0100
const int CIMA     = 8; // 1000

int codificador(double x, double y, float left, float right, float bottom, float top) {
    int code = DENTRO;
    if (x < left) {
        code = code | ESQUERDA;
    } else if (x > right) {
        code = code | DIREITA;
    }
    if (y < bottom) {
        code = code | BAIXO;
    } else if (y > top) {
        code = code | CIMA;
    }
    return code;
}

void cohenSutherland(double x1, double y1, double x2, double y2, float left, float right, float bottom, float top) {
    double original_x1 = x1, original_y1 = y1;
    double original_x2 = x2, original_y2 = y2;

    int code1 = codificador(x1, y1, left, right, bottom, top);
    int code2 = codificador(x2, y2, left, right, bottom, top);
    bool accept = false;

    while (true) {
        if ((code1 == 0) && (code2 == 0)) {
            accept = true;
            break;
        } else if (code1 & code2) {
            break;
        } else {
            int codigoFora = (code1 != 0) ? code1 : code2;
            double x, y;

            if (codigoFora & CIMA) {
                x = x1 + (x2 - x1) * (top - y1) / (y2 - y1);
                y = top;
            } else if (codigoFora & BAIXO) {
                x = x1 + (x2 - x1) * (bottom - y1) / (y2 - y1);
                y = bottom;
            } else if (codigoFora & DIREITA) {
                y = y1 + (y2 - y1) * (right - x1) / (x2 - x1);
                x = right;
            } else if (codigoFora & ESQUERDA) {
                y = y1 + (y2 - y1) * (left - x1) / (x2 - x1);
                x = left;
            }

            if (codigoFora == code1) {
                x1 = x; y1 = y;
                code1 = codificador(x1, y1, left, right, bottom, top);
            } else {
                x2 = x; y2 = y;
                code2 = codificador(x2, y2, left, right, bottom, top);
            }
        }
    }

    // Desenha a linha original completa em vermelho
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
        glVertex2d(original_x1, original_y1);
        glVertex2d(original_x2, original_y2);
    glEnd();

    // Desenha a parte vis├¡vel em azul por cima
    if (accept) {
        glColor3f(0.0f, 0.0f, 1.0f);
        glBegin(GL_LINES);
            glVertex2d(x1, y1);
            glVertex2d(x2, y2);
        glEnd();
    }
}

void gerarLinhas() {
    for (int i = 0; i < NUM_LINHAS; i++) {
        linhasAleatorias[i].x1 = rand() % windowWidth;
        linhasAleatorias[i].y1 = rand() % windowHeight;
        linhasAleatorias[i].x2 = rand() % windowWidth;
        linhasAleatorias[i].y2 = rand() % windowHeight;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    float centerX = windowWidth / 2.0f;
    float centerY = windowHeight / 2.0f;
    float left = centerX - rectWidth / 2.0f;
    float right = centerX + rectWidth / 2.0f;
    float bottom = centerY - rectHeight / 2.0f;
    float top = centerY + rectHeight / 2.0f;

    glLineWidth(3.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(left, bottom);
        glVertex2f(right, bottom);
        glVertex2f(right, top);
        glVertex2f(left, top);
    glEnd();

    glLineWidth(1.0f);
    
    for (int i = 0; i < NUM_LINHAS; i++) {
        if (i < etapaAtual) {
            cohenSutherland(linhasAleatorias[i].x1, linhasAleatorias[i].y1, linhasAleatorias[i].x2, linhasAleatorias[i].y2, left, right, bottom, top);
        } else {
            glColor3f(0.5f, 0.5f, 0.5f);
            glBegin(GL_LINES);
                glVertex2d(linhasAleatorias[i].x1, linhasAleatorias[i].y1);
                glVertex2d(linhasAleatorias[i].x2, linhasAleatorias[i].y2);
            glEnd();
        }
    }
    
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27: exit(0); break;
        case 'r':
        case 'R': // Tecla 'r' ou 'R' reinicia a visualiza├º├úo
            etapaAtual = 0;
            gerarLinhas();
            glutPostRedisplay();
            break;
    }
}

void update(int value) {
    if (etapaAtual < NUM_LINHAS) {
        etapaAtual++;
        glutPostRedisplay();
    }
    glutTimerFunc(500, update, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Ret├óngulo base de recorte - Cohen-Sutherland");
    
    srand(time(NULL));
    gerarLinhas();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    
    glutTimerFunc(500, update, 0);
    
    glClearColor(0.75f, 0.75f, 0.75f, 1.0f);
    glutMainLoop();
    return 0;
}
