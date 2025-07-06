#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>

// --- ESTRUTURAS DE DADOS ---
typedef struct {
    float x, y, z;
} Vec3;

typedef struct {
    int v_indices[3];
    int vn_indices[3];
} Face;

// --- VARIÁVEIS GLOBAIS PARA O MODELO E INTERAÇÃO ---
// Armazenamento dinâmico do modelo
Vec3* vertices = NULL;
Vec3* normals = NULL;
Face* faces = NULL;

int vertex_count = 0;
int vertex_capacity = 0;
int normal_count = 0;
int normal_capacity = 0;
int face_count = 0;
int face_capacity = 0;

// Transformações do modelo
float scaleFactor = 1.0f;
Vec3 modelCenter = {0.0f, 0.0f, 0.0f};

// Controle da janela e da interação com o mouse
int win_id;
float rotationX = 0.0f;
float rotationY = 0.0f;
float zoomFactor = 250.0f; // Variável para controlar o zoom (distância da câmera)
int lastMouseX, lastMouseY;
int isDragging = 0; // 0 para não, 1 para sim


// --- FUNÇÕES DE GERENCIAMENTO DE MEMÓRIA ---
void add_vertex(Vec3 v) {
    if (vertex_count >= vertex_capacity) {
        vertex_capacity = (vertex_capacity == 0) ? 100 : vertex_capacity * 2;
        vertices = (Vec3*) realloc(vertices, vertex_capacity * sizeof(Vec3));
        if (!vertices) {
            fprintf(stderr, "Falha ao alocar memória para vértices.\n");
            exit(1);
        }
    }
    vertices[vertex_count++] = v;
}

void add_normal(Vec3 n) {
    if (normal_count >= normal_capacity) {
        normal_capacity = (normal_capacity == 0) ? 100 : normal_capacity * 2;
        normals = (Vec3*) realloc(normals, normal_capacity * sizeof(Vec3));
        if (!normals) {
            fprintf(stderr, "Falha ao alocar memória para normais.\n");
            exit(1);
        }
    }
    normals[normal_count++] = n;
}

void add_face(Face f) {
    if (face_count >= face_capacity) {
        face_capacity = (face_capacity == 0) ? 100 : face_capacity * 2;
        faces = (Face*) realloc(faces, face_capacity * sizeof(Face));
        if (!faces) {
            fprintf(stderr, "Falha ao alocar memória para faces.\n");
            exit(1);
        }
    }
    faces[face_count++] = f;
}

// Libera toda a memória alocada dinamicamente
void cleanup() {
    free(vertices);
    free(normals);
    free(faces);
    printf("Memória liberada.\n");
}


// --- FUNÇÃO PARA CARREGAR O MODELO .OBJ ---
void loadOBJ(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Erro ao abrir o arquivo: %s\n", filename);
        exit(1);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "v ", 2) == 0) {
            Vec3 vertex;
            sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            add_vertex(vertex);
        } else if (strncmp(line, "vn ", 3) == 0) {
            Vec3 normal;
            sscanf(line, "vn %f %f %f", &normal.x, &normal.y, &normal.z);
            add_normal(normal);
        } else if (strncmp(line, "f ", 2) == 0) {
            int v_indices[4], vn_indices[4], vt_indices[4];
            int items_read;

            // Tenta analisar o formato f v//vn
            items_read = sscanf(line, "f %d//%d %d//%d %d//%d %d//%d",
                                    &v_indices[0], &vn_indices[0],
                                    &v_indices[1], &vn_indices[1],
                                    &v_indices[2], &vn_indices[2],
                                    &v_indices[3], &vn_indices[3]);
            if (items_read >= 6) { // Pelo menos um triângulo
                Face f1;
                f1.v_indices[0] = v_indices[0] - 1; f1.vn_indices[0] = vn_indices[0] - 1;
                f1.v_indices[1] = v_indices[1] - 1; f1.vn_indices[1] = vn_indices[1] - 1;
                f1.v_indices[2] = v_indices[2] - 1; f1.vn_indices[2] = vn_indices[2] - 1;
                add_face(f1);

                if (items_read == 8) { // É um quad, triangula
                    Face f2;
                    f2.v_indices[0] = v_indices[0] - 1; f2.vn_indices[0] = vn_indices[0] - 1;
                    f2.v_indices[1] = v_indices[2] - 1; f2.vn_indices[1] = vn_indices[2] - 1;
                    f2.v_indices[2] = v_indices[3] - 1; f2.vn_indices[2] = vn_indices[3] - 1;
                    add_face(f2);
                }
            } else { // Tenta analisar o formato f v/vt/vn
                items_read = sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
                                    &v_indices[0], &vt_indices[0], &vn_indices[0],
                                    &v_indices[1], &vt_indices[1], &vn_indices[1],
                                    &v_indices[2], &vt_indices[2], &vn_indices[2],
                                    &v_indices[3], &vt_indices[3], &vn_indices[3]);
                if (items_read >= 9) { // Pelo menos um triângulo
                    Face f1;
                    f1.v_indices[0] = v_indices[0] - 1; f1.vn_indices[0] = vn_indices[0] - 1;
                    f1.v_indices[1] = v_indices[1] - 1; f1.vn_indices[1] = vn_indices[1] - 1;
                    f1.v_indices[2] = v_indices[2] - 1; f1.vn_indices[2] = vn_indices[2] - 1;
                    add_face(f1);
                    if (items_read == 12) { // É um quad
                        Face f2;
                        f2.v_indices[0] = v_indices[0] - 1; f2.vn_indices[0] = vn_indices[0] - 1;
                        f2.v_indices[1] = v_indices[2] - 1; f2.vn_indices[1] = vn_indices[2] - 1;
                        f2.v_indices[2] = v_indices[3] - 1; f2.vn_indices[2] = vn_indices[3] - 1;
                        add_face(f2);
                    }
                }
            }
        }
    }
    fclose(file);
    printf("Modelo carregado: %d vértices, %d normais, %d faces.\n", vertex_count, normal_count, face_count);
}

// --- FUNÇÃO PARA CALCULAR A ESCALA AUTOMÁTICA ---
void calculateBoundsAndScale() {
    if (vertex_count == 0) return;

    float min_x = FLT_MAX, max_x = -FLT_MAX;
    float min_y = FLT_MAX, max_y = -FLT_MAX;
    float min_z = FLT_MAX, max_z = -FLT_MAX;
    int i;
    for (i = 0; i < vertex_count; ++i) {
        if (vertices[i].x < min_x) min_x = vertices[i].x;
        if (vertices[i].x > max_x) max_x = vertices[i].x;
        if (vertices[i].y < min_y) min_y = vertices[i].y;
        if (vertices[i].y > max_y) max_y = vertices[i].y;
        if (vertices[i].z < min_z) min_z = vertices[i].z;
        if (vertices[i].z > max_z) max_z = vertices[i].z;
    }

    modelCenter.x = (min_x + max_x) / 2.0f;
    modelCenter.y = (min_y + max_y) / 2.0f;
    modelCenter.z = (min_z + max_z) / 2.0f;

    float dx = max_x - min_x;
    float dy = max_y - min_y;
    float dz = max_z - min_z;

    float max_dim = fmax(dx, fmax(dy, dz));
    float desired_size = 90.0f;
    if (max_dim > 0) {
        scaleFactor = desired_size / max_dim;
    }
}


// --- FUNÇÕES DE CALLBACK DO GLUT ---
void myKeyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 'R': case 'r': glColor3f(1.0f, 0.0f, 0.0f); break;
    case 'G': case 'g': glColor3f(0.0f, 1.0f, 0.0f); break;
    case 'B': case 'b': glColor3f(0.0f, 0.0f, 1.0f); break;

    // Controles de zoom pelo teclado
    case '+':
        zoomFactor -= 10.0f;
        if (zoomFactor < 10.0f) zoomFactor = 10.0f; // Limite para não atravessar o objeto
        break;
    case '-':
        zoomFactor += 10.0f;
        break;

    case 27: // Tecla ESC
        glutDestroyWindow(win_id);
        exit(0);
        break;
  }
  glutPostRedisplay();
}

void myKeyboardSpecial(int key, int x, int y ) {
  switch ( key ) {
    case GLUT_KEY_UP: glutFullScreen(); break;
    case GLUT_KEY_DOWN: glutReshapeWindow(800, 600); break;
    default: break;
  }
}

void myMouse(int button, int state, int x, int y) {
    // Botão esquerdo para arrastar e girar
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = 1;
            lastMouseX = x;
            lastMouseY = y;
        } else {
            isDragging = 0;
        }
    }
    // Botões 3 e 4 são a roda do mouse na maioria dos sistemas
    else if (button == 3) { // Roda para cima (Aproximar)
        zoomFactor -= 10.0f;
        if (zoomFactor < 10.0f) zoomFactor = 10.0f; // Limite
        glutPostRedisplay();
    }
    else if (button == 4) { // Roda para baixo (Afastar)
        zoomFactor += 10.0f;
        glutPostRedisplay();
    }
    // Botão direito para cor aleatória
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        float r = (float)rand() / (float)RAND_MAX;
        float g = (float)rand() / (float)RAND_MAX;
        float b = (float)rand() / (float)RAND_MAX;
        glColor3f(r, g, b);
        glutPostRedisplay();
    }
}

void myMotion(int x, int y) {
    if (isDragging) {
        int deltaX = x - lastMouseX;
        int deltaY = y - lastMouseY;
        rotationY += deltaX * 0.5f;
        rotationX += deltaY * 0.5f;
        lastMouseX = x;
        lastMouseY = y;
        glutPostRedisplay();
    }
}

// --- FUNÇÕES DE CONFIGURAÇÃO E RENDERIZAÇÃO DO OPENGL ---
void init(void) {
  glClearColor (0.1, 0.1, 0.1, 0.0);
  glShadeModel (GL_SMOOTH);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_NORMALIZE);
  GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
  GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light_position[] = { 100.0, 100.0, 100.0, 0.0 };
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  glColor3f(1.0, 1.0, 1.0);
}

void display(void) {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Usa a variável zoomFactor para definir a distância da câmera
  gluLookAt(0.0f, 0.0f, zoomFactor, // Posição da câmera
            0.0f, 0.0f, 0.0f,   // Ponto para onde a câmera olha
            0.0f, 1.0f, 0.0f);  // Vetor "up"

  glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
  glRotatef(rotationY, 0.0f, 1.0f, 0.0f);

  glPushMatrix();
  glScalef(scaleFactor, scaleFactor, scaleFactor);
  glTranslatef(-modelCenter.x, -modelCenter.y, -modelCenter.z);

  glBegin(GL_TRIANGLES);
  int i;
  for(i = 0; i < face_count; ++i) {
      int j;
      for(j = 0; j < 3; ++j) {
          if (normal_count > 0) {
            glNormal3fv((GLfloat*)&normals[faces[i].vn_indices[j]]);
          }
          glVertex3fv((GLfloat*)&vertices[faces[i].v_indices[j]]);
      }
  }
  glEnd();
  glPopMatrix();

  glutSwapBuffers();
}

void reshape (int w, int h) {
  if (h == 0) h = 1; // Previne a divisão por zero
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();

  // MUDANÇA: Troca glOrtho por gluPerspective para uma visão 3D natural
  gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 1.0f, 1000.0f);

  // Retorna para a matriz de visão do modelo para que o display() funcione corretamente
  glMatrixMode(GL_MODELVIEW);
}


// --- FUNÇÃO PRINCIPAL ---
int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Uso: %s <caminho_para_arquivo.obj>\n", argv[0]);
        return 1;
    }

    atexit(cleanup); // Registra a função de limpeza para ser chamada na saída

    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowSize (800, 600);
    glutInitWindowPosition (100, 100);
    win_id = glutCreateWindow ("Visualizador de Modelos .OBJ");

    init();
    loadOBJ(argv[1]);
    calculateBoundsAndScale();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(myKeyboard);
    glutSpecialFunc(myKeyboardSpecial);
    glutMouseFunc(myMouse);
    glutMotionFunc(myMotion);

    glutMainLoop();

    return 0;
}
