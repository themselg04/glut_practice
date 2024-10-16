#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>

#define size 10

typedef struct vertice {
    int uid;
    float x, y, z;
    struct vertice *sig;
} Vertice;

typedef struct triangulo {
    int uid;
    Vertice *v1, *v2, *v3;
    struct triangulo *sig;
} Triangulo;

void crearVertice(Vertice **inicio, float x, float y);
void liberarVertices(Vertice *inicio);
void crearTriangulo(Triangulo **triangulos, Vertice *actual);
void liberarTriangulos(Triangulo *inicio);

// Variables globales
Vertice *vertices = NULL;
Triangulo *triangulos = NULL;
int cantVertices = 0;
int cantTriangulos = 0;

void crearVertice(Vertice **inicio, float x, float y) {
    // Vertice *nuevo = (Vertice*)malloc(sizeof(Vertice));
    Vertice *nuevo = (Vertice*)calloc(1, sizeof(Vertice));

    if (!nuevo) {
        printf("\e[41m[ERROR] No se pudo asignar memoria!\e[0m\n");
        exit(0);
    }
    nuevo->uid = cantVertices + 1000;
    cantVertices++;
    nuevo->x = x;
    nuevo->y = y;
    nuevo->z = 0;
    nuevo->sig = *inicio;
    *inicio = nuevo;

    printf("Vertice %d creado en las coordenas %G, %G, %G\n", nuevo->uid, nuevo->x, nuevo->y, nuevo->z);
}

void liberarVertices(Vertice *inicio) {
    if (inicio->sig != NULL)
        liberarVertices(inicio->sig);
    free(inicio);
}

void crearTriangulo(Triangulo **triangulos, Vertice *actual) {
    if (actual == NULL || actual->sig == NULL || actual->sig->sig == NULL)
        return;

    Vertice *v1 = actual;
    Vertice *v2 = actual->sig;
    Vertice *v3 = actual->sig->sig;

    // Triangulo *nuevo = (Triangulo*)malloc(sizeof(Triangulo));
    Triangulo *nuevo = (Triangulo*)calloc(1, sizeof(Triangulo));
    if (!nuevo) {
        printf("\e[41m[ERROR] No se pudo asignar memoria para el triángulo!\e[0m\n");
        exit(0);
    }
    nuevo->uid = cantTriangulos + 2000;
    cantTriangulos++;
    nuevo->v1 = v1;
    nuevo->v2 = v2;
    nuevo->v3 = v3;
    nuevo->sig = *triangulos;
    *triangulos = nuevo;

    printf("Triángulo %d creado con vértices %d, %d, %d\n",nuevo->uid ,v1->uid, v2->uid, v3->uid);

    crearTriangulo(triangulos, actual->sig);
}

void liberarTriangulos(Triangulo *inicio) {
    if (inicio->sig != NULL)
        liberarTriangulos(inicio->sig);
    free(inicio);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Ajustar coordenadas para centrar los vértices
    float xOffset = 5.5;  // Desplazamiento para centrar los vértices en X
    float yOffset = 5.5;  // Desplazamiento para centrar los vértices en Y
    
    // Dibujar vértices
    glColor3f(0.0, 1.0, 0.0); // Color verde para los vértices
    glPointSize(5.0); // Tamaño del punto
    glBegin(GL_POINTS);
    Vertice *v = vertices;
    while (v != NULL) {
        glVertex2f((v->x - xOffset) / size, (v->y - yOffset) / size); // Centrando y dibujando cada vértice
        v = v->sig;
    }
    glEnd();
    
    glColor3f(1.0, 0.0, 0.0); // Color azul para las lineas
    glBegin(GL_LINES);
    Triangulo *t = triangulos;
    while (t != NULL) {
        // Dibujar línea entre v1 y v2
        glVertex2f((t->v1->x - xOffset) / size, (t->v1->y - yOffset) / size);
        glVertex2f((t->v2->x - xOffset) / size, (t->v2->y - yOffset) / size);
        
        // Dibujar línea entre v2 y v3
        glVertex2f((t->v2->x - xOffset) / size, (t->v2->y - yOffset) / size);
        glVertex2f((t->v3->x - xOffset) / size, (t->v3->y - yOffset) / size);
        
        // Dibujar línea entre v3 y v1
        glVertex2f((t->v3->x - xOffset) / size, (t->v3->y - yOffset) / size);
        glVertex2f((t->v1->x - xOffset) / size, (t->v1->y - yOffset) / size);
        
        t = t->sig;
    }
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    crearVertice(&vertices, 3, 2);
    crearVertice(&vertices, 5, 3);
    crearVertice(&vertices, 1, 6);
    crearVertice(&vertices, 6, 4);
    crearVertice(&vertices, 6, 7);
    crearVertice(&vertices, 7, 5);
    crearVertice(&vertices, 11, 6);
    crearVertice(&vertices, 3, 4);
    crearVertice(&vertices, 7, 8);
    crearVertice(&vertices, 12, 15);

    crearTriangulo(&triangulos, vertices);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Triangulos y Vertices");
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glutDisplayFunc(display);
    glutMainLoop();

    liberarVertices(vertices);
    liberarTriangulos(triangulos);
    return 0;
}
