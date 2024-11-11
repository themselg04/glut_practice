// SISTEMA SOLAR

// Controles: 
// +: Acercar cámara
// -: Alejar cámara
// F(aster): Aumentar velocidad rotación
// S(lower): Disminuir velocidad rotación
// Q(uit): Cerrar programa

// la textura de las estrellas es 8K así q tarda en cargar ;p

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include <SOIL/SOIL.h> // Simple OpenGL Image Library
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
// #include <string>

// Ángulo de rotación global
float rotacion = 0;
float velocidad = 0.3;

// Posición en z de la cámara
float camPos = 10;

// Para guardar el ID de la textura del skybox
int skybox = 0;

// Estructura para los Planetas
typedef struct _planeta_{
	float radio;      // Radio del Planeta
	float distancia;  // Distancia al sol
	float velocidad;  // Velocidad de traslación
	int idTextura; 	  // ID de la Textura del planeta
} Planeta;

// Definición de los Planetas (,el sol y Plutón) (radio, distancia al sol, velocidad relativa a la tierra)
Planeta sol      =  {0.50, 0.0, 0.000, 0};
Planeta mercurio =  {0.10, 1.4, 4.100, 0};
Planeta venus    =  {0.15, 2.0, 1.600, 0};
Planeta tierra   =  {0.15, 2.6, 1.000, 0};
Planeta marte    =  {0.12, 3.2, 0.500, 0};
Planeta jupiter  =  {0.30, 4.4, 0.080, 0};
Planeta saturno  =  {0.25, 5.4, 0.030, 0};
Planeta urano    =  {0.20, 6.2, 0.010, 0};
Planeta neptuno  =  {0.20, 6.8, 0.006, 0};
Planeta pluton   =  {0.06, 7.4, 0.004, 0};

// Cosas de luces
float light_ambient[]  = {0.0, 0.0, 0.0, 1.0};
float light_diffuse[]  = {1.0, 1.0, 1.0, 1.0};
float light_specular[] = {1.0, 1.0, 1.0, 1.0};
float light_position[] = {0.0, 0.0, 0.0, 1.0}; // Luz desde el sol

float mat_ambient[]    = {0.7, 0.7, 0.7, 1.0};
float mat_diffuse[]    = {0.8, 0.8, 0.8, 1.0};
float mat_specular[]   = {1.0, 1.0, 1.0, 1.0};

float high_shininess[] = {100};

int cargarTextura(char* filename);
void cargarTexturas();

void dibujarSkybox();
void dibujarPlaneta(Planeta planeta);
void dibujarAnillos();

void display();
void keyboard(unsigned char key, int x, int y);
void resize(int width, int height);

int main(int argc, char **argv) {
	srand(time(NULL));
	rotacion = rand()%100000; // Para que cada vez que el programa se abra los planetas esten en posiciones aleatorias

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Sistema Solar");

	printf("Renderizador: %s\n", glGetString(GL_RENDERER));
	printf("Versión de OpenGL soportada: %s\n", glGetString(GL_VERSION));

	cargarTexturas();

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

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

// Función para cargar solo una textura del planeta
// Devuelve el indice que SOIL le asigne al cargarla
int cargarTextura(const char* filename) {
    return SOIL_load_OGL_texture(filename, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB);
}

// Función para cargar todas las texturas
void cargarTexturas() {
	skybox 				= cargarTextura("../texturas/estrellas.jpg");
    sol.idTextura 		= cargarTextura("../texturas/sol.jpg");
    mercurio.idTextura 	= cargarTextura("../texturas/mercurio.jpg");
    venus.idTextura 	= cargarTextura("../texturas/venus.jpg");
    tierra.idTextura 	= cargarTextura("../texturas/tierra.jpg");
    marte.idTextura 	= cargarTextura("../texturas/marte.jpg");
    jupiter.idTextura 	= cargarTextura("../texturas/jupiter.jpg");
    saturno.idTextura 	= cargarTextura("../texturas/saturno.jpg");
    urano.idTextura 	= cargarTextura("../texturas/urano.jpg");
    neptuno.idTextura 	= cargarTextura("../texturas/neptuno.jpg");
    pluton.idTextura 	= cargarTextura("../texturas/pluton.jpg");
}

// Función para dibujar el skybox usando una esfera
void dibujarSkybox() {
    glPushMatrix();
        // Desactivamos la iluminación para el fondo
        glDisable(GL_LIGHTING);
        
        // Activamos y configuramos la textura
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, skybox); 
        
		// Rotar para dar efeccto dinamico
		// Rota al 1% de velocidad
        glRotatef(rotacion * 0.01, 0, 0, 1);

		GLUquadric* quad = gluNewQuadric(); // Se crea una superfice cuadrica
    	gluQuadricTexture(quad, GL_TRUE); // Se le aplica la textura
        gluSphere(quad, 50, 30, 30); // Se crea con un radio de 50
		gluDeleteQuadric(quad);

        glDisable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
    glPopMatrix();
}

void dibujarPlaneta(Planeta planeta) {
	glPushMatrix();
		// Rotación alrededor del sol
		// Velocidad y distancia definida por su struct
		glRotatef(rotacion * planeta.velocidad, 0.0, 1.0, 0.0);
		glTranslatef(planeta.distancia, 0.0, 0.0);

		// Dibujar y rotar unicamente la esfera sobre su propio eje
		glPushMatrix();
			// Activamos y configuramos la textura
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, planeta.idTextura);

			// Rotar para poner la textura bien
			glRotatef(-90, 1, 0, 0);
            glRotatef(180, 0, 1, 0);
            glRotatef(rotacion * 2, 0, 0, 1); // Rotación sobre su propio eje

			GLUquadric* quad = gluNewQuadric(); // Se crea una superficie cuadrica
			gluQuadricTexture(quad, GL_TRUE); // Se le aplica la textura
			gluSphere(quad, planeta.radio, 30 , 30); // Se crea con el radio definido por su struct
			gluDeleteQuadric(quad);
			
			// Desactivamos la textura
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();		

		// Si es Saturno, dibujamos sus anillos
		if(planeta.radio == saturno.radio)
			dibujarAnillos();
	glPopMatrix();
}

void dibujarAnillos() {
	glPushMatrix();
		// Desactivamos la iluminación para los anillos
		glDisable(GL_LIGHTING);
		
		// Color de los anillos
		glColor4f(0.8, 0.8, 0.7, 0.8);
		
		// Dibujamos varios anillos concéntricos
		for(float radius = 0.3; radius < 0.5; radius += 0.015) {
				glBegin(GL_LINE_LOOP);
				for(int i = 0; i < 360; i += 5) {
						float angle = i * M_PI / 180.0;
						glVertex3f(cos(angle) * radius, 0.0, sin(angle) * radius);
				}
				glEnd();
		}
		
		// Reactivamos la iluminación
		glEnable(GL_LIGHTING);
	glPopMatrix();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0, 3, camPos, // Posición de la cámara
			  0, 0, 0,  	// Punto al que mira
			  0, 1, 0);     // Rotación de la cámara

    // Dibujamos el skybox
    dibujarSkybox();

	// Dibujamos cada Planeta (incluidos el Sol y Plutón)
	dibujarPlaneta(sol);
	dibujarPlaneta(mercurio);
	dibujarPlaneta(venus);
	dibujarPlaneta(tierra);
	dibujarPlaneta(marte);
	dibujarPlaneta(jupiter);
	dibujarPlaneta(saturno);
	dibujarPlaneta(urano);
	dibujarPlaneta(neptuno);
	dibujarPlaneta(pluton);

	// Incrementamos el ángulo para el siguiente frame
	rotacion += velocidad;

	glutSwapBuffers();
	glutPostRedisplay(); // Se redibuja el display constantemente
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'q':
		case 'Q':
			exit(0);
			break;
		case '+':
			camPos -= 0.25;
			break;
		case '-':
			camPos += 0.25;
			break;
		case 'f':
		case 'F':
			velocidad += 0.03;
			break;
		case 's':
		case 'S':
			if (velocidad > 0) {
				velocidad -= 0.03;
			}
			break;
	}
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

