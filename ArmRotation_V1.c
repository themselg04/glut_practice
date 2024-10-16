#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif
//#include <math.h>
#include <stdio.h>

#define BODY_HEIGHT 5.0
#define BODY_WIDTH 3.0
#define BODY_LENGTH 3.0

#define ARM_HEIGHT 3.5
#define ARM_WIDTH 1.0
#define ARM_LENGTH 1.0

int armRotationAngle = 0;

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glLoadIdentity(); // Limpiar las matrices antes de dibujar?

	glColor3f(1.0,0.0,0.0);
	glTranslatef(0.0, 0.0, -12.0);

	glTranslatef(0, BODY_HEIGHT/2,0);
	glPushMatrix();
		glScalef(BODY_WIDTH, BODY_HEIGHT, BODY_LENGTH);
		glColor3f(0.0,0.6,0.3);
		glutSolidCube(1);
	glPopMatrix();

	// Dibujamos el brazo derecho
  glPushMatrix();
     glTranslatef(-(BODY_WIDTH)/2,(BODY_HEIGHT-ARM_HEIGHT)/2,0);
     glTranslatef(0,ARM_HEIGHT/2,0);
     glRotatef(armRotationAngle - 30, 0, 0, 1);
     glTranslatef(0,-ARM_HEIGHT/2,0);
     
     glPushMatrix();
       glScalef(ARM_WIDTH,ARM_HEIGHT,ARM_LENGTH);
       glutSolidCube(1);
     glPopMatrix();

     // ya tenemos el brazo... la mano
     glTranslatef(0,-(ARM_HEIGHT+ARM_WIDTH)/2,0);
     glColor3f(1,0.6,0.6);
     glScalef(ARM_WIDTH,ARM_WIDTH,ARM_LENGTH);
     glutSolidCube(1);
  glPopMatrix();

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 'r' || key == 'R') {
        armRotationAngle += 5; // Aumentar ángulo de rotación
    }
    if (key == 'l' || key == 'L') {
        armRotationAngle -= 5; // Disminuir ángulo de rotación
    }
    printf("Rotación del brazo: %d\n", armRotationAngle);
    glutPostRedisplay(); // Redibujar la escena
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(600,600);
	glutInitWindowPosition(50,50);
	glutCreateWindow("Jugando con GLUT");

	glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST); 

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80.0, 1.0, 1.0, 100.00);

	glShadeModel(GL_SMOOTH); 

    glMatrixMode(GL_MODELVIEW); // Necesario para que no se vea la pantalla negra al usar el glLoadIdentity dentro de display()
    glLoadIdentity();

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST); 
	glClearDepth(1.0);

    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

	glutDisplayFunc(display);
    glutKeyboardFunc(keyboard); // Registrar el manejador de teclado


	glutMainLoop();

	return 0;
}