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

#define HASH_SIZE 2000
#define GRAPH_SIZE 10
#define X_OFFSET 5.5
#define Y_OFFSET 5.5

// Grafo?
typedef struct nodo_vertice {
    int uid;
    float x, y, z;
    struct lista_vertices *inicio; 
} NodoVertice;

// Lista de vertices
typedef struct lista_vertices {
    struct nodo_vertice *nodo;
    struct lista_vertices *sig;
} ListaVertices;

// Lista de triangulos
// typedef struct triangulo {
//     int uid;
//     NodoVertice *v1, *v2, *v3;
//     struct triangulo *sig;
// } Triangulo;

// Tabla Ha-Ash de Nodos
typedef struct tabla_haash {
    NodoVertice **exdeverdad;
} TablaHaash;

void crearVertice(float x, float y, float z);
void crearElementoLista(NodoVertice *nodo);
void imprimirLista();

unsigned int funcionHaash(float x, float y, float z);
void inicializarTabla();
void crearElementoTabla();
NodoVertice* buscarElementoTabla(float x, float y, float z);

void crearTriangulos();

void liberarTabla();
void liberarTriangulos();
void liberarVertices();

void display();

ListaVertices *vertices = NULL;
// Triangulo *triangulos = NULL;
TablaHaash *tabla = NULL;
int cantVertices = 0;
int cantTriangulos = 0;

int main(int argc, char** argv) {
    printf("~~~Mallado V3.4~~~\n");
    inicializarTabla();

    crearVertice(3, 2, 0);
    crearVertice(5, 3, 0);
    crearVertice(1, 6, 0);
    crearVertice(6, 4, 0);
    crearVertice(6, 7, 0);
    crearVertice(7, 5, 0);
    crearVertice(11, 6, 0);
    crearVertice(3, 4, 0);
    crearVertice(7, 8, 0);
    crearVertice(12, 15, 0);

    // imprimirLista();
    // buscarElementoTabla(5, 3, 0);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Mallado V3");
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glutDisplayFunc(display);
    glutMainLoop();

    // liberarTabla();
    // liberarVertices();
    // liberarTriangulos();

    return 0;
}

void crearVertice(float x, float y, float z) {
    NodoVertice *nuevo = (NodoVertice*)calloc(1, sizeof(NodoVertice));

    if (!nuevo) {
        printf("\e[41m[ERROR] No se pudo asignar memoria!\e[0m\n");
        exit(0);
    }

    nuevo->uid = cantVertices + 1000;
    cantVertices++;
    nuevo->x = x;
    nuevo->y = y;
    nuevo->z = z;
    nuevo->inicio = NULL;
    crearElementoLista(nuevo);

    printf("Vertice %d creado en las coordenas %G, %G, %G\n", nuevo->uid, nuevo->x, nuevo->y, nuevo->z);
    crearElementoTabla();
}

void crearElementoLista(NodoVertice *nodo) {
    ListaVertices *nuevo = (ListaVertices*)calloc(1, sizeof(ListaVertices));

    if(!nuevo) {
        printf("\e[41m[ERROR] No se pudo asignar memoria!\e[0m\n");
        exit(0);
    }

    nuevo->nodo = nodo;
    nuevo->sig = vertices;
    vertices = nuevo;

    printf("Vertice %d almacenado en %p\n", nodo->uid, vertices->nodo);
    }

void imprimirLista() {
    ListaVertices *actual = vertices;
    while (actual->sig) {
        printf("Vertice %d apunta a %d\n", actual->nodo->uid, actual->sig->nodo->uid);
        printf("Dirección Memoria %p a %p\n\n", actual->nodo, actual->sig->nodo);
        actual = actual->sig;
    }

    // printf("Vertice %d apunta a %d\n", inicio->nodo->uid, inicio->sig->nodo->uid);
    // printf("Dirección Memoria %p a %p\n", inicio->nodo, inicio->sig->nodo);
    // if (inicio->sig->sig) {
    //     imprimirLista(inicio->sig);
    // }
}

unsigned int funcionHaash(float x, float y, float z) {
    unsigned int hash = 0;
    hash = x+y+z + GRAPH_SIZE;
    if (x != 0) {
        hash *= x;
    } else if (y != 0) {
        hash *= y;
    } else {
        hash *= z;
    }
    return hash % HASH_SIZE;
}

void inicializarTabla() {
    tabla = (TablaHaash*)calloc(1, sizeof(TablaHaash));
    tabla->exdeverdad = (NodoVertice**)calloc(HASH_SIZE, sizeof(NodoVertice*));
    
    if (!tabla || !tabla->exdeverdad) {
        printf("\e[41m[ERROR] No se pudo asignar memoria para la tabla hash!\e[0m\n");
        exit(0);
    }
}

void crearElementoTabla() {
    unsigned int index = funcionHaash(vertices->nodo->x, vertices->nodo->y, vertices->nodo->z);
    tabla->exdeverdad[index] = vertices->nodo;
    printf("Vertice %d almacenado en la tabla hash en el índice %d\n\n", vertices->nodo->uid, index);
}

NodoVertice* buscarElementoTabla(float x, float y, float z) {
    unsigned int index = funcionHaash(x, y, z);
    if (index >= HASH_SIZE) {
        printf("\e[41m[ERROR] El índice de hash excede el tamaño de la tabla!\e[0m\n");
        return NULL;
    }

    NodoVertice *nodo = tabla->exdeverdad[index];
    if (nodo) {
        printf("Vertice %d %p encontrado en indice: %d\n", nodo->uid, nodo, index);
    } else {
        printf("No se encontró ningún nodo en el índice %d\n", index);
    }
    return nodo;
}

// void crearTriangulos(){
// }

// This is where text is removed

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0, 1.0, 0.0);
    glPointSize(5.0);
    glBegin(GL_POINTS);
    ListaVertices *v = vertices;
    while (v != NULL) {
        glVertex2f((v->nodo->x - X_OFFSET) / GRAPH_SIZE, (v->nodo->y - Y_OFFSET) / GRAPH_SIZE);
        v = v->sig;
    }
    glEnd();
    glFlush();
}

// This is a test for using git