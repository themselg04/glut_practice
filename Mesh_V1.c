// https://www.desmos.com/calculator/moaviacn04/
// Ploteo de los puntos

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

void crearVertice(Vertice **inicio, int x, int y); // Crea un nodo de una lista
void liberarVertices(Vertice *inicio);
// float calcDistPuntos(Vertice *v1, Vertice *v2);
void crearTriangulo(Triangulo **triangulos, Vertice *inicio, Vertice *actual);
void liberarTriangulos(Triangulo *inicio);

int cantVertices = 0;
int cantTriangulos = 0;

int main() {
    Vertice *inicio = NULL;
    Triangulo *triangulos = NULL;

    // Crear vértices
    crearVertice(&inicio, 3, 2);
    crearVertice(&inicio, 5, 3);
    crearVertice(&inicio, 1, 6);
    crearVertice(&inicio, 4, 3);
    crearVertice(&inicio, 8, 9);
    crearVertice(&inicio, 7, 5);
    crearVertice(&inicio, 11, 6);

    // Crear triángulos recursivamente
    crearTriangulo(&triangulos, inicio, inicio);

    // Liberar memoria
    liberarVertices(inicio);
    liberarTriangulos(triangulos);

    return 0;
}

void crearVertice(Vertice **inicio, int x, int y) {
    Vertice *nuevo = (Vertice*)malloc(sizeof(Vertice));
    if (!nuevo) {
        printf("\e[41m[ERROR] No se pudo asignar memoria!\e[0m\n");
        exit(0);
    }
    nuevo->uid = cantVertices + 1000; // Los vertices empiezan como 1xxx
    cantVertices++;
    nuevo->x = x;
    nuevo->y = y;
    nuevo->z = 0; // Aunque el vertice tiene datos 3D, en este programa solo se usa X y Y
    nuevo->sig = *inicio;
    *inicio = nuevo;
}

void liberarVertices(Vertice *inicio) {
    if (inicio->sig != NULL)
        liberarVertices(inicio->sig);
    free(inicio);
}

// float calcDistPuntos(Vertice *v1, Vertice *v2) {
//     return sqrt(pow(v2->x - v1->x, 2) + pow(v2->y - v1->y, 2));
// }

// Función recursiva para crear triángulos
void crearTriangulo(Triangulo **triangulos, Vertice *inicio, Vertice *actual) {
    if (actual == NULL || actual->sig == NULL || actual->sig->sig == NULL)
        return;

    Vertice *v1 = actual;
    Vertice *v2 = actual->sig;
    Vertice *v3 = actual->sig->sig;

    Triangulo *nuevo = (Triangulo*)malloc(sizeof(Triangulo));
    if (!nuevo) {
        printf("\e[41m[ERROR] No se pudo asignar memoria para el triángulo!\e[0m\n");
        exit(0);
    }
    nuevo->uid = cantTriangulos + 2000; // Los triangulos empiezan como 2xxx
    cantTriangulos++;
    nuevo->v1 = v1;
    nuevo->v2 = v2;
    nuevo->v3 = v3;
    nuevo->sig = *triangulos;
    *triangulos = nuevo;

    printf("Triángulo creado con vértices %d, %d, %d\n", v1->uid, v2->uid, v3->uid);

    crearTriangulo(triangulos, inicio, actual->sig);
}

void liberarTriangulos(Triangulo *inicio) {
    if (inicio->sig != NULL)
        liberarTriangulos(inicio->sig);
    free(inicio);
}
