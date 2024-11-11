// ANIMACIÓN DE CAMINADO

// Controles: 
// W: Caminar hacia delante
// S: Caminar hacia atrás
// E: Activar bucle de caminado
// R(ight): Mover hacia la derecha brazo izquierdo
// L(left): Mover hacia la izquierda brazo izquierdo
// U(p): Mover hacia la derecha brazo derecho
// D(own): Mover hacia la izquierda brazo derecho
// A: Activar rotación cámara alrededor
// Q(uit): Cerrar programa

// creo notará en cual le puse más esfuerzo

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif
#include <stdio.h>

#define BODY_HEIGHT 5.0
#define BODY_WIDTH 3.0
#define BODY_LENGTH 3.0

#define ARM_HEIGHT 3.5
#define ARM_WIDTH 1.0
#define ARM_LENGTH 1.0

#define LEG_HEIGHT 4.0
#define LEG_WIDTH 1.0
#define LEG_LENGTH 1.0

#define HEAD_HEIGHT 2.0
#define HEAD_WIDTH 2.0
#define HEAD_LENGTH 2.0

int armRotationAngleR = -15;
int armRotationAngleL = 15;
int rotateCamera = 0;
int walkingAngle = 0;
int walkingDirection = 1; // 1 pa'lante, -1 pa'tra

// Le robé la idea a Denisse :p
typedef enum _bool_{
    false,
    true
} Bool;

Bool isWalking = false;
Bool isRotating = false;

const GLfloat light_ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
const GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat light_position[] = {2.0f, 5.0f, 5.0f, 0.0f}; // Posición de la cámara

const GLfloat mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};
const GLfloat mat_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
const GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat high_shininess[] = {100.0f};

void idle();
void display();
void keyboard(unsigned char key, int x, int y);
void resize(int width, int height);

int main(int argc, char **argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Walking on Shunshine");

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST); 

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80.0, 1.0, 1.0, 100.00);

    glShadeModel(GL_SMOOTH); 

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDepthFunc(GL_LEQUAL);
    glClearDepth(1.0);

    printf("Renderizador: %s\n", glGetString(GL_RENDERER));
    printf("Versión de OpenGL soportada: %s\n", glGetString(GL_VERSION));

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    // Función que se ejecuta cada que NO se reciben eventos
    // De forma que es ideal para animaciones
    // Se recomienda que no sea muy intensivo para no alentar el programa

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glutMainLoop();
    return 0;
}

void idle() {
    if (isWalking) { // Verifica si isWalking == true
        walkingAngle += 3 * walkingDirection; // Aumenta (o disminuye) 3 al angulo de caminado
        printf("Angulo de caminar: %d\n", walkingAngle);
        if (walkingAngle >= 40.0 || walkingAngle <= -40.0) { // Llega a 30 (o -30)
            walkingDirection *= -1; // Invierte la dirección de caminado
        }
        glutPostRedisplay(); // Redibuja la escena
    }

    if (isRotating) {
        rotateCamera += 2;
        glutPostRedisplay();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0, -2.0, -16.0);
    glColor3f(1.0, 0.0, 0.0);

    glRotatef(rotateCamera, 0, 1, 0);

    // Cuerpo
    glTranslatef(0, BODY_HEIGHT / 2, 0);
    glPushMatrix();
        glScalef(BODY_WIDTH, BODY_HEIGHT, BODY_LENGTH);
        glColor3f(0.0, 0.6, 0.3);
        glutSolidCube(1);
    glPopMatrix();

    // Brazo derecho
    glPushMatrix();
        glTranslatef(-(BODY_WIDTH) / 2, (BODY_HEIGHT - ARM_HEIGHT) / 2, 0);
        glTranslatef(0, ARM_HEIGHT / 2, 0);
        glRotatef(armRotationAngleR, 0, 0, 1); // Para alzarlo
        glRotatef(walkingAngle, 1, 0, 0); // Para caminar
        glTranslatef(0, -ARM_HEIGHT / 2, 0);

        glPushMatrix();
            glScalef(ARM_WIDTH, ARM_HEIGHT, ARM_LENGTH);
            glColor3f(0.0, 0.6, 0.3);
            glutSolidCube(1);
        glPopMatrix();

        // Mano derecha
        glTranslatef(0, -(ARM_HEIGHT + ARM_WIDTH) / 2, 0);
        glScalef(ARM_WIDTH, ARM_WIDTH, ARM_LENGTH);
        glColor3f(1.0, 1.0, 0.0);
        glutSolidCube(1);
    glPopMatrix();

    // Brazo izquierdo
    glPushMatrix();
        glTranslatef((BODY_WIDTH) / 2, (BODY_HEIGHT - ARM_HEIGHT) / 2, 0);
        glTranslatef(0, ARM_HEIGHT / 2, 0);
        glRotatef(armRotationAngleL, 0, 0, 1); // Para alzarlo
        glRotatef(-walkingAngle, 1, 0, 0); // Para caminar
        glTranslatef(0, -ARM_HEIGHT / 2, 0);

        glPushMatrix();
            glScalef(ARM_WIDTH, ARM_HEIGHT, ARM_LENGTH);
            glColor3f(0.0, 0.6, 0.3);
            glutSolidCube(1);
        glPopMatrix();

        // Mano izquierda
        glTranslatef(0, -(ARM_HEIGHT + ARM_WIDTH) / 2, 0);
        glScalef(ARM_WIDTH, ARM_WIDTH, ARM_LENGTH);
        glColor3f(1.0, 1.0, 0.0);
        glutSolidCube(1);
    glPopMatrix();

    // Pierna derecha
    glPushMatrix();
        glTranslatef(-(BODY_WIDTH) / 4, -(BODY_HEIGHT + LEG_HEIGHT) / 2, 0);
        glTranslatef(0, LEG_HEIGHT / 2, 0);
        glRotatef(-walkingAngle, 1, 0, 0);
        glTranslatef(0, -LEG_HEIGHT / 2, 0);
        
        glPushMatrix();
            glScalef(LEG_WIDTH, LEG_HEIGHT, LEG_LENGTH);
            glColor3f(0.5, 0.3, 0.1);
            glutSolidCube(1);
        glPopMatrix();

        // Pie derecho
        glTranslatef(0, -(LEG_HEIGHT + LEG_WIDTH) / 2, 0);
        glScalef(LEG_WIDTH, LEG_WIDTH, LEG_LENGTH);
        glColor3f(0.6, 0.4, 0.2);
        glutSolidCube(1);
    glPopMatrix();

    // Pierna derecha
    glPushMatrix();
        glTranslatef((BODY_WIDTH) / 4, -(BODY_HEIGHT + LEG_HEIGHT) / 2, 0);
        glTranslatef(0, LEG_HEIGHT / 2, 0);
        glRotatef(walkingAngle, 1, 0, 0);
        glTranslatef(0, -LEG_HEIGHT / 2, 0);
        
        glPushMatrix();
            glScalef(LEG_WIDTH, LEG_HEIGHT, LEG_LENGTH);
            glColor3f(0.5, 0.3, 0.1);
            glutSolidCube(1);
        glPopMatrix();

        // Pie izquierdo
        glTranslatef(0, -(LEG_HEIGHT + LEG_WIDTH) / 2, 0);
        glScalef(LEG_WIDTH, LEG_WIDTH, LEG_LENGTH);
        glColor3f(0.6, 0.4, 0.2);
        glutSolidCube(1);
    glPopMatrix();

    // Cabeza
    glPushMatrix();
        glTranslatef(0, BODY_HEIGHT - HEAD_HEIGHT / 2, 0);
        glPushMatrix();
            glScalef(HEAD_WIDTH, HEAD_HEIGHT, HEAD_LENGTH);
            glColor3f(1.0, 1.0, 0.0);
            glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch(key) { // Cambié los if por switch
        case 'r':
        case 'R':
            armRotationAngleR += 5;
            printf("Roración brazo derecho: %d\n", armRotationAngleR);
            break;
        case 'l':
        case 'L':
            armRotationAngleR -= 5;
            printf("Rotación brazo derecho: %d\n", armRotationAngleR);
            break;
        case 'u':
        case 'U':
            armRotationAngleL += 5;
            printf("Rotación brazo izquierdo: %d\n", armRotationAngleL);
            break;
        case 'd':
        case 'D':
            armRotationAngleL -= 5;
            printf("Rotación brazo izquierdo: %d\n", armRotationAngleL);
            break;
        case 'e':
        case 'E':
            isWalking = !isWalking;
            printf("Animación caminado: %s\n", isWalking ? "SI" : "NO");
            break;
        case 'w':
        case 'W':
            walkingAngle += 3.0 * walkingDirection;
            printf("Angulo de caminar: %d\n", walkingAngle);
            if (walkingAngle >= 30.0 || walkingAngle <= -30.0) {
                walkingDirection *= -1;
            }
            glutPostRedisplay();
            break;
        case 's':
        case 'S':
            walkingAngle -= 3.0 * walkingDirection;
            printf("Angulo de caminar: %d\n", walkingAngle);
            if (walkingAngle >= 30.0 || walkingAngle <= -30.0) {
                walkingDirection *= -1;
            }
            glutPostRedisplay();
            break;
        case 'q':
        case 'Q':
            exit(0);
            break;
        case 'a':
        case 'A':
            isRotating = !isRotating;
            printf("Rotación cámara: %s\n", isRotating ? "SI" : "NO");
            break;
    }
    glutPostRedisplay();
}

void resize(int width, int height) { // Magia negra
  float ar = (float)width / (float)height;
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}