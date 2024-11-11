// SISTEMA SOLAR en C++
// usando clases
// para complicarme la vida

// Controles:
// W: Acercar cámara
// S: Alejar cámara
// R(ecio): Aumentar velocidad rotación
// F(astn't): Disminuir velocidad rotación
// Q(uit): Cerrar programa

// la textura de las estrellas es 8K así q tarda en cargar ;p

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include "SOIL/SOIL.h"
#else
#include <GL/gl.h>
#include <GL/glut.h>
#include <SOIL/SOIL.h> // Simple OpenGL Image Library
#endif

// Librerias de C++
#include <cmath>
#include <ctime>
#include <string>
#include <iostream>

// Para no tener que poner std::
using namespace std;

// Definición de una clase para los planetas
class Planeta {
// Definición de variables públicas
public:
    float radio;
    float distancia;
    float velocidad;
    int texturaID;

	// Función constructora
    Planeta(float r, float d, float v):
		radio(r),
		distancia(d),
		velocidad(v) {/*No se ejecuta nada*/}
};

// Definición de una clase para el sistema solar
class SistemaSolar {
public:
	Planeta sol,
			mercurio,
			venus,
			tierra,
			marte,
			jupiter,
			saturno,
			urano,
			neptuno,
			pluton;

	SistemaSolar():
		sol(0.5, 0, 0),
		mercurio(0.1, 1.4, 4.1),
		venus(0.15, 2, 1.6),
		tierra(0.15, 2.6, 1),
		marte(0.12, 3.2, 0.5),
		jupiter(0.3, 4.4, 0.08),
		saturno(0.25, 5.4, 0.03),
		urano(0.2, 6.2, 0.01),
		neptuno(0.2, 6.8, 0.006),
		pluton(0.06, 7.4, 0.004) {}

};

// Esta clase encapsula todas las funciones para que el programa funcione
// Inspirado en Godot, donde los nodos son hijos de un nodo raíz
class Manager {
private:
// Variables privadas, las cuales unicamente Manager tiene acceso a
// Manager es la clase raíz que incializa GLUT, lo configura,
// inicializa la clase sistemaSolar que a su vez incializa los planetas
// Se encarga de manejar la posición de la cámara y de manejar las
// variables de la skybox.
	float velocidad;
	float rotacion;

	float posCamaraX;
	float posCamaraY;
	float posCamaraZ;

	int skyboxID;
	int radioSkybox;

	float light_ambient[4];
	float light_diffuse[4];
	float light_specular[4];
	float light_position[4];

	float mat_ambient[4];
	float mat_diffuse[4];
	float mat_specular[4];

	float high_shininess[1];

	SistemaSolar sistemaSolar;

	enum movCamara {
		X,
		Y,
		Z
	};

	// Función para cargar solo una textura del planeta
	// Devuelve el indice que SOIL le asigne al cargarla
	int cargarTextura(string filename) {
    	return SOIL_load_OGL_texture(filename.c_str(), SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS|SOIL_FLAG_NTSC_SAFE_RGB);
	}

	// Función para cargar todas las texturas
	void cargarTexturas() {
		skyboxID						= cargarTextura("../texturas/estrellas.jpg");
		sistemaSolar.sol.texturaID		= cargarTextura("../texturas/sol.jpg");
		sistemaSolar.mercurio.texturaID = cargarTextura("../texturas/mercurio.jpg");
		sistemaSolar.venus.texturaID 	= cargarTextura("../texturas/venus.jpg");
		sistemaSolar.tierra.texturaID 	= cargarTextura("../texturas/tierra.jpg");
		sistemaSolar.marte.texturaID 	= cargarTextura("../texturas/marte.jpg");
		sistemaSolar.jupiter.texturaID 	= cargarTextura("../texturas/jupiter.jpg");
		sistemaSolar.saturno.texturaID 	= cargarTextura("../texturas/saturno.jpg");
		sistemaSolar.urano.texturaID 	= cargarTextura("../texturas/urano.jpg");
		sistemaSolar.neptuno.texturaID 	= cargarTextura("../texturas/neptuno.jpg");
		sistemaSolar.pluton.texturaID 	= cargarTextura("../texturas/pluton.jpg");
	}

	// Avanza un tick en la simulación
	void tick() {
		rotacion += velocidad;
	}

	// Todavía está en construcción
	void moverCamara(enum movCamara movCamara, float cantMov){
		switch (movCamara) {
			case X:
				// if (cantMov > 0 && posCamaraX < radioSkybox - 1)
				// 	posCamaraX += cantMov;
				// else if (cantMov < 0 && posCamaraX > -radioSkybox + 1)
				// 	posCamaraX += cantMov;
				break;
			case Y:
				// if (cantMov > 0 && posCamaraY < radioSkybox - 1)
				// 	posCamaraY += cantMov;
				// else if (cantMov < 0 && posCamaraY > -radioSkybox + 1)
				// 	posCamaraY += cantMov;
				break;
			case Z:
				if (cantMov > 0 && posCamaraZ < radioSkybox - 1)
					posCamaraZ += cantMov;
				else if (cantMov < 0 && posCamaraZ > -radioSkybox + 1)
					posCamaraZ += cantMov;
				break;

		}
	}

	// Función para dibujar el skybox usando una esfera
	void dibujarSkybox() {
		glPushMatrix();
			// Desactivamos la iluminación para el fondo
			glDisable(GL_LIGHTING);

			// Activamos y configuramos la textura
			
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, skyboxID);

			// Rotar para dar efeccto dinamico
			// Rota al 1% de velocidad
			glRotatef(rotacion * 0.01, 0, 0, 1);

			GLUquadric* quad = gluNewQuadric(); // Se crea una superfice cuadrica
			gluQuadricTexture(quad, GL_TRUE); // Se le aplica la textura
			gluSphere(quad, radioSkybox, 30, 30); // Se crea con un radio de 50
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
				glBindTexture(GL_TEXTURE_2D, planeta.texturaID);

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
			if(planeta.radio == sistemaSolar.saturno.radio)
				dibujarAnillos();
		glPopMatrix();
	}

	// Función para dibujar los anillos de saturno
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

public:
	Manager():
		velocidad(0.3),
		posCamaraX(0), posCamaraY(3), posCamaraZ(10),
		radioSkybox(50),
		light_ambient{0, 0, 0, 1},
		light_diffuse{1, 1, 1, 1},
		light_specular{1, 1, 1, 1},
		light_position{0, 0, 0, 1}, // Luz desde el sol
		mat_ambient{0.7, 0.7, 0.7, 1.0},
		mat_diffuse{0.8, 0.8, 0.8, 1.0},
		mat_specular{1, 1, 1, 1},
		high_shininess{100} {
			// Usar el tiempo como semilla para la función rand
			// Se usa rand para dar una posición de rotación aleatoria al iniciar el programa
			srand(time(0));
			rotacion = rand() % 100000;
		}

	// Función para iniciar GLUT e imprimir la GPU y versión maxima de OpenGL soportada
	void init(int argc, char **argv) {
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
		glutInitWindowSize(1000, 1000);
		glutInitWindowPosition(0, 0);
		glutCreateWindow("Sistema Solar");
		cout << "Renderizador: " << glGetString(GL_RENDERER) << endl;
		cout << "Versión de OpenGL soportada: " << glGetString(GL_VERSION) << endl;
		cargarTexturas();
	}

	// Función para configurar las opciones de GLUT y configurar la luz
	void config() {
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

	}

	void display() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		gluLookAt  (posCamaraX, posCamaraY, posCamaraZ, 	// Posición de la cámara
					0, 0, 0,  								// Punto al que mira
					0, 1, 0);     							// Rotación de la cámara (es un vector que apunta hacia arriba)

		// Dibujamos el skybox
		dibujarSkybox();

		// Dibujamos cada Planeta (incluidos el Sol y Plutón)
		dibujarPlaneta(sistemaSolar.sol);
		dibujarPlaneta(sistemaSolar.mercurio);
		dibujarPlaneta(sistemaSolar.venus);
		dibujarPlaneta(sistemaSolar.tierra);
		dibujarPlaneta(sistemaSolar.marte);
		dibujarPlaneta(sistemaSolar.jupiter);
		dibujarPlaneta(sistemaSolar.saturno);
		dibujarPlaneta(sistemaSolar.urano);
		dibujarPlaneta(sistemaSolar.neptuno);
		dibujarPlaneta(sistemaSolar.pluton);

		// Incrementamos el ángulo para el siguiente frame
		tick();

		glutSwapBuffers();
		glutPostRedisplay(); // Se redibuja el display constantemente
	}

	// Cambiado por un switch case para mayor readability
	void keyboard(unsigned char key) {
		switch (key) {
			case 'q': case 'Q':
				exit(0);
				break;
			case 'w': case 'W':
				moverCamara(Z, -0.25);
				break;
			case 's': case 'S':
				moverCamara(Z, 0.25);
				break;
			case 'r': case 'R':
				velocidad += 0.03;
				break;
			case 'f': case 'F':
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
} manager; // Se inicializa una instancia global de la clase Manager llamada manager

// Como GLUT es una libreria de C, hay que crear funciones estaticas que llamen a las funciones la instancia manager
static void display() { manager.display(); }
static void keyboard(unsigned char k, int x, int y) { manager.keyboard(k); }
static void resize(int w, int h) { manager.resize(w, h); }

int main(int argc, char **argv) {
	manager.init(argc, argv);

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	manager.config();
	glutMainLoop();
	return 0;
}

// Si, no usé prototipos, aún no sé completamente bien como funcionan las clases en C++