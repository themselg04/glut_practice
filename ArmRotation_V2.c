#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif
#include <math.h>
#include <stdio.h>

#define BODY_HEIGHT 5.0
#define BODY_WIDTH 3.0
#define BODY_LENGTH 3.0

#define ARM_HEIGHT 3.5
#define ARM_WIDTH 1.0
#define ARM_LENGTH 1.0

double anguloBrazo = 0.0;  // Ángulo de rotación del brazo

// Multiplicación de una matriz 4x4 por un vector de longitud 4
void Matriz_X_Vector(double matrix[4][4],  double vector[4], double *c)
{
    for (int i = 0; i < 4; i++)
        c[i] = matrix[i][0] * vector[0] +
               matrix[i][1] * vector[1] +
               matrix[i][2] * vector[2] +
               matrix[i][3] * vector[3];
}

// Rotación de un punto (vector) en un ángulo dado
void rotar(double *vector, double angle)
{
    double seno = sin(angle);
    double coseno = cos(angle);
  
    double vectortemp[4]; 

    double matrizRot[4][4] = {
        {coseno, -seno, 0, 0},
        {seno,  coseno, 0, 0},
        {0,       0, 1, 0},
        {0,       0, 0, 1}
    };

    // double matrizRot[4][4] = {
    //     {coseno, -seno, 0, 0},
    //     {seno,  coseno, 0, 0},
    //     {0,       0, 1, 0},
    //     {0,       0, 0, 1}
    // };

    Matriz_X_Vector(matrizRot, vector, vectortemp);
  
    for(int i = 0; i < 4; i++)
        vector[i] = vectortemp[i];
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Vector para el brazo derecho
    double brazoVector[4] = {-(BODY_WIDTH)/2, (BODY_HEIGHT-ARM_HEIGHT)/2, 0, 1};

    // Aplicamos la rotación usando el ángulo del brazo
    rotar(brazoVector, anguloBrazo);

    glLoadIdentity(); // Limpiar las matrices antes de dibujar?
    glTranslatef(0.0, -2.0, -12.0);

    // Dibujar el cuerpo
    glTranslatef(0, BODY_HEIGHT / 2, 0);
    glPushMatrix();
        glScalef(BODY_WIDTH, BODY_HEIGHT, BODY_LENGTH);
        glColor3f(0.0, 0.6, 0.3);
        glutSolidCube(1);
    glPopMatrix();

    // Dibujar el brazo derecho
    glPushMatrix();
        glTranslatef(brazoVector[0], brazoVector[1], brazoVector[2]);

        // Escalar y dibujar el brazo
        glPushMatrix();
            glScalef(ARM_WIDTH, ARM_HEIGHT, ARM_LENGTH);
            glutSolidCube(1);
        glPopMatrix();

        // Dibujar la mano
        glTranslatef(0, -(ARM_HEIGHT + ARM_WIDTH) / 2, 0);
        glColor3f(1, 0.6, 0.6);
        glScalef(ARM_WIDTH, ARM_WIDTH, ARM_LENGTH);
        glutSolidCube(1);
    glPopMatrix();

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 'r' || key == 'R') {
        anguloBrazo += 0.1; // Aumentar ángulo de rotación
    }
    if (key == 'l' || key == 'L') {
        anguloBrazo -= 0.1; // Disminuir ángulo de rotación
    }
    printf("Rotación del brazo: %G\n", anguloBrazo);
    glutPostRedisplay(); // Redibujar la escena
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowSize(600, 600);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Rotar Humanoide");

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glViewport(0, 0, 500, 500);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80.0, 1.0, 1.0, 100.00);

    glShadeModel(GL_SMOOTH);
    glMatrixMode(GL_MODELVIEW); // Necesario para que no se vea la pantalla negra al usar el glLoadIdentity dentro de display()
    glLoadIdentity();

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST); 
    glClearDepth(1.0);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);  // Vincular la función de teclado

    glutMainLoop();

    return 0;
}
