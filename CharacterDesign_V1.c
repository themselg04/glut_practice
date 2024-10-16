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

#define HASH_SIZE 1000
#define GRAPH_SIZE 12
#define X_OFFSET 16
#define Y_OFFSET 12

typedef struct nodo_vertice {
    int uid;
    float x, y, z;
    struct lista_vertices *inicio; 
} NodoVertice;

typedef struct lista_vertices {
    struct nodo_vertice *nodo;
    struct lista_vertices *sig;
} ListaVertices;

typedef struct linea{
    int uid;
    struct nodo_vertice *vertice_a, *vertice_b;
    struct linea *sig;
} Linea;

typedef struct tabla_haash {
    NodoVertice **exdeverdad;
} TablaHaash;

void dibujarLinea(float xa, float ya, float za, float xb, float yb, float zb);

void crearVertice(float x, float y, float z);
void crearElementoLista(NodoVertice *nodo);
void imprimirListaVertices();

void crearLinea(NodoVertice *vertice_a, NodoVertice *vertice_b);
void imprimirListaLineas();

void crearConexion(NodoVertice *vertice_a, NodoVertice *vertice_b);
void imprimirListaConexiones(NodoVertice *nodo);

unsigned int funcionHaash(float x, float y, float z);
void inicializarTabla();
void crearElementoTabla();
NodoVertice* buscarElementoTabla(float x, float y, float z);

void liberarTabla();
void liberarLineas();
void liberarVertices();

void display();

ListaVertices *vertices = NULL;
Linea *lineas = NULL;
TablaHaash *tabla = NULL;
int cantVertices = 0;
int cantLineas = 0;

int main(int argc, char** argv){
    inicializarTabla();

    // Oreja izquierda
    dibujarLinea(16, 1, 0, 16.85, 2.75, 0);
    dibujarLinea(16, 1, 0, 14.8, 3.25, 0);
    dibujarLinea(14.8, 3.25, 0, 16.59, 7.55, 0);
    dibujarLinea(16.59, 7.55, 0, 16.85, 2.75, 0);

    // Oreja derecha
    dibujarLinea(21.25, 2, 0, 18.75, 3, 0);
    dibujarLinea(18.75, 3, 0, 16.75, 7.59, 0);
    dibujarLinea(16.75, 7.59, 0, 20.2, 4.45, 0);
    dibujarLinea(20.2, 4.45, 0, 21.25, 2, 0);

    // Ojo izquierdo
    dibujarLinea(14.68, 6.66, 0, 14.45, 7.29, 0);
    dibujarLinea(14.45, 7.29, 0, 14.75, 7.32, 0);
    dibujarLinea(14.75, 7.32, 0, 14.68, 6.66, 0);

    dibujarLinea(14.75, 7.32, 0, 14.8, 7.5, 0);
    dibujarLinea(14.8, 7.5, 0, 15.2, 6.9, 0);
    dibujarLinea(15.2, 6.9, 0, 14.75, 7.32, 0);

    dibujarLinea(14.8, 7.5, 0, 14.79, 7.98, 0);
    dibujarLinea(14.79, 7.98, 0, 15.39, 7.8, 0);
    dibujarLinea(15.39, 7.8, 0, 14.8, 7.5, 0);

    dibujarLinea(14.79, 7.98, 0, 15.1, 8.8, 0);
    dibujarLinea(15.1, 8.8, 0, 14.52, 8.98, 0);
    dibujarLinea(14.52, 8.98, 0, 14.79, 7.98, 0);

    dibujarLinea(14.52, 8.98, 0, 13.9, 8.4, 0);
    dibujarLinea(13.9, 8.4, 0, 14.45, 8.09, 0);
    dibujarLinea(14.45, 8.09, 0, 14.52, 8.98, 0);

    dibujarLinea(13.9, 8.4, 0, 13.8, 7.7, 0);
    dibujarLinea(13.8, 7.7, 0, 14.27, 7.86, 0);
    dibujarLinea(14.27, 7.86, 0, 13.9, 8.4, 0);

    dibujarLinea(13.8, 7.7, 0, 14.25, 7.45, 0);
    dibujarLinea(14.25, 7.45, 0, 14, 6.8, 0);
    dibujarLinea(14, 6.8, 0, 13.8, 7.7, 0);

    dibujarLinea(14.25, 7.45, 0, 14.45, 7.29, 0);
    dibujarLinea(14.45, 7.29, 0, 14, 6.8, 0);
    dibujarLinea(14, 6.8, 0, 14.68, 6.66, 0);

    dibujarLinea(14.68, 6.66, 0, 15.2, 6.9, 0);
    dibujarLinea(15.2, 6.9, 0, 15.32, 7.35, 0);
    dibujarLinea(15.32, 7.35, 0, 15.39, 7.8, 0);

    dibujarLinea(15.39, 7.8, 0, 15.1, 8.8, 0);
    dibujarLinea(14.25, 7.45, 0, 14.27, 7.86, 0);
    dibujarLinea(14.27, 7.86, 0, 14.45, 8.09, 0);

    dibujarLinea(14.45, 8.09, 0, 14.7, 8.11, 0);
    dibujarLinea(14.7, 8.11, 0, 14.79, 7.98, 0);

    // Ojo derecho
    dibujarLinea(17.5, 7.5, 0, 17.6, 7, 0);
    dibujarLinea(17.6, 7, 0, 18, 7.5, 0);
    dibujarLinea(18, 7.5, 0, 17.54, 7.75, 0);
    dibujarLinea(17.54, 7.75, 0, 17.5, 7.5, 0);

    dibujarLinea(17.6, 7, 0, 18.25, 6.4, 0);
    dibujarLinea(18.25, 6.4, 0, 18.2, 7.2, 0);
    dibujarLinea(18.2, 7.2, 0, 18, 7.5, 0);
    dibujarLinea(18.2, 7.2, 0, 17.6, 7, 0);

    dibujarLinea(18.2, 7.2, 0, 18.4, 7.2, 0);
    dibujarLinea(18.4, 7.2, 0, 18.25, 6.4, 0);
    dibujarLinea(18.4, 7.2, 0, 18.9, 6.65, 0);

    dibujarLinea(18.9, 6.65, 0, 18.25, 6.4, 0);
    dibujarLinea(18.4, 7.2, 0, 18.6, 7.3, 0);
    dibujarLinea(18.6, 7.3, 0, 18.9, 6.65, 0);

    dibujarLinea(18.6, 7.3, 0, 19.15, 7.6, 0);
    dibujarLinea(19.15, 7.6, 0, 18.9, 6.65, 0);
    dibujarLinea(19.15, 7.6, 0, 19, 8.5, 0);

    dibujarLinea(19, 8.5, 0, 18.69, 7.8, 0);
    dibujarLinea(18.69, 7.8, 0, 19.18, 7.6, 0);
    dibujarLinea(18.69, 7.8, 0, 18.6, 7.3, 0);

    dibujarLinea(17.54, 7.75, 0, 18, 7.8, 0);
    dibujarLinea(18, 7.8, 0, 18, 7.5, 0);
    dibujarLinea(18, 7.8, 0, 18.2, 8, 0);

    dibujarLinea(18.2, 8, 0, 18.3, 8, 0);
    dibujarLinea(18.3, 8, 0, 18.69, 7.8, 0);
    dibujarLinea(18.3, 8, 0, 19, 8.5, 0);

    dibujarLinea(19, 8.5, 0, 18.35, 8.91, 0);
    dibujarLinea(18, 8.91, 0, 18.3, 8, 0);
    dibujarLinea(18.35, 8.91, 0, 18.2, 8, 0);

    dibujarLinea(18.35, 8.91, 0, 17.69, 8.65, 0);
    dibujarLinea(18.35, 8.91, 0, 18.2, 8, 0);
    dibujarLinea(17.69, 8.65, 0, 18.2, 8, 0);

    dibujarLinea(17.69, 8.65, 0, 18, 7.8, 0);
    dibujarLinea(17.69, 8.65, 0, 17.54, 7.75, 0);

    // Cabeza
    // dibujarLinea(13.32, 7.35, 0, 16.59, 7.35, 0);
    dibujarLinea(16.59, 7.55, 0, 15.32, 7.7, 0);
    dibujarLinea(16.59, 7.55, 0, 16.75, 7.59, 0);
    dibujarLinea(16.75, 7.59, 0, 17.5, 7.5, 0);

    dibujarLinea(14.52, 8.98, 0, 17.69, 8.65, 0);
    dibujarLinea(14.52, 8.98, 0, 13.6, 9.1, 0);
    dibujarLinea(13.6, 9.1, 0, 13.9, 8.4, 0);

    dibujarLinea(19, 8.5, 0, 19.4, 9.32, 0);
    dibujarLinea(19.4, 9.32, 0, 18.59, 10.48, 0);
    dibujarLinea(18.59, 10.48, 0, 16.94, 11.05, 0);

    dibujarLinea(16.94, 11.05, 0, 16.45, 11.05, 0);
    dibujarLinea(16.45, 11.05, 0, 15.5, 11.05, 0);
    dibujarLinea(15.5, 11.05, 0, 17.69, 8.65, 0);

    // Brazo izquierdo
    dibujarLinea(16.2, 11.65, 0, 13.08, 10.78, 0);
    dibujarLinea(13.08, 10.78, 0, 13.5, 7.93, 0);

    dibujarLinea(13.5, 7.93, 0, 12.05, 7, 0);
    dibujarLinea(12.05, 7, 0, 12.9, 6.59, 0);
    dibujarLinea(12.9, 6.59, 0, 13.5, 7.93, 0);

    // Brazo derecho
    dibujarLinea(16.35, 11.35, 0, 18.08, 14.38, 0);
    dibujarLinea(18.08, 14.38, 0, 15.5, 15.58, 0);

    dibujarLinea(15.5, 15.58, 0, 16.92, 16.29, 0);
    dibujarLinea(16.92, 16.29, 0, 16.25, 16.75, 0);
    dibujarLinea(16.25, 16.75, 0, 15.5, 15.58, 0);

    // Cuerpo

    dibujarLinea(16.45, 11.05, 0, 16.35, 11.35, 0);
    dibujarLinea(16.35, 11.35, 0, 16.20, 11.65, 0);
    dibujarLinea(16.2, 11.65, 0, 14.5, 15.15, 0);

    dibujarLinea(16.35, 11.35, 0, 15.5, 15.58, 0);
    dibujarLinea(15.5, 15.58, 0, 15.08, 15.68, 0);
    dibujarLinea(15.08, 15.68, 0, 15, 15.6, 0);
    dibujarLinea(15, 15.6, 0, 14.5, 15.15, 0);

    dibujarLinea(15, 15.6, 0, 13.35, 18.75, 0);
    dibujarLinea(15.08, 15.68, 0, 14.98, 19.05, 0);
    dibujarLinea(14.98, 19.05, 0, 15.4, 22.9, 0);

    dibujarLinea(13.35, 18.75, 0, 17, 20, 0);
    dibujarLinea(17, 20, 0, 16.75, 23.2, 0);
    dibujarLinea(16.75, 23.2, 0, 15.6, 22.85, 0);
    dibujarLinea(17, 20, 0, 15.6, 22.85, 0);

    dibujarLinea(15.4, 22.9, 0, 13.6, 23.1, 0);
    dibujarLinea(13.6, 23.1, 0, 13.1, 21.8, 0);
    dibujarLinea(13.1, 21.8, 0, 15.4, 22.9, 0);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Vibri");
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glutDisplayFunc(display);
    glutMainLoop();

    // liberarLineas();
    // liberarTabla();
    // liberarVertices();

    return 0;
}


void dibujarLinea(float xa, float ya, float za, float xb, float yb, float zb) {
    NodoVertice *vertice_a = buscarElementoTabla(xa, ya, za);
    if (!vertice_a) {
        crearVertice(xa, ya, za);
        vertice_a = buscarElementoTabla(xa, ya, za);
    }
    printf("\n");
    NodoVertice *vertice_b = buscarElementoTabla(xb, yb, zb);
    if (!vertice_b) {
        crearVertice(xb, yb, zb);
        vertice_b = buscarElementoTabla(xb, yb, zb);
    }

    printf("\n");
    crearLinea(vertice_a, vertice_b);
}

void crearLinea(NodoVertice *vertice_a, NodoVertice *vertice_b) {
    printf("Creando linea entre %d y %d\n\n", vertice_a->uid, vertice_b->uid);
    Linea *nuevo = (Linea*)calloc(1, sizeof(Linea));

    if (!nuevo) {
        printf("Error\n");
        exit(0);
    }

    nuevo->uid = cantLineas + 2000;
    cantLineas++;
    nuevo->vertice_a = vertice_a;
    nuevo->vertice_b = vertice_b;
    nuevo->sig = lineas;
    lineas = nuevo;

    crearConexion(vertice_a, vertice_b);
}

void imprimirListaLineas() {
    Linea *actual = lineas;
    while (actual) {
        printf("Linea %d\nVertice_a: %d, Vertice_b: %d\n", actual->uid, actual->vertice_a->uid, actual->vertice_b->uid);
        actual = actual->sig;
    }
}

void crearConexion(NodoVertice *vertice_a, NodoVertice *vertice_b) {
    ListaVertices *nuevo_a = (ListaVertices*)calloc(1, sizeof(ListaVertices));
    ListaVertices *nuevo_b = (ListaVertices*)calloc(1, sizeof(ListaVertices));

    if(!nuevo_a || !nuevo_b) {
        printf("Explotó\n");
        exit(0);
    }

    nuevo_a->nodo = vertice_b;
    nuevo_b->nodo = vertice_a;

    nuevo_a->sig = vertice_a->inicio;
    nuevo_b->sig = vertice_b->inicio;

    vertice_a->inicio = nuevo_a;
    vertice_b->inicio = nuevo_b;
}

void imprimirListaConexiones(NodoVertice *nodo) {
    ListaVertices *actual = nodo->inicio;

    printf("Vertice %d apunta a los vertices:\n", nodo->uid);
    while (actual) {
        printf("Vertice %d\n", actual->nodo->uid);
        actual = actual->sig;
    }
    printf("\n");
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

void imprimirListaVertices() {
    ListaVertices *actual = vertices;
    while (actual->sig) {
        printf("Vertice %d apunta a %d\n", actual->nodo->uid, actual->sig->nodo->uid);
        printf("Dirección Memoria %p a %p\n\n", actual->nodo, actual->sig->nodo);
        actual = actual->sig;
    }
}

unsigned int funcionHaash(float x, float y, float z) {
    unsigned int hash = 0;
    hash = (x*10+y*5+z*3) + X_OFFSET + Y_OFFSET - 2;
    if (x != 0) {
        hash = hash*x;
    } else if (y != 0) {
        hash = hash*y;
    } else {
        hash = hash*z;
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
    printf("Vertice %d almacenado en la tabla hash en el índice %d\n", vertices->nodo->uid, index);
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

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(2);
    glBegin(GL_LINES);
    Linea *l = lineas;
    while (l) {
        glVertex2f((l->vertice_a->x - X_OFFSET) / GRAPH_SIZE, -(l->vertice_a->y - Y_OFFSET) / GRAPH_SIZE);
        glVertex2f((l->vertice_b->x - X_OFFSET) / GRAPH_SIZE, -(l->vertice_b->y - Y_OFFSET) / GRAPH_SIZE);
        l = l->sig;
    }
    glEnd();
    glFlush();
}