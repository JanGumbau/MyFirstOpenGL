#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#define WINDIW_WIDTH 1280
#define WINDIW_HEIGHT 720

void ResizeWindow(GLFWwindow* window, int iNewFrameBufferWidth, int iNewFrameBufferHeight) {

	glViewport(0, 0, iNewFrameBufferWidth, iNewFrameBufferHeight);

}


void main() {

	// Initialize GLFW per gestionar ventanas e inputs
	glfwInit();

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //agafa la versió més nova d'OpenGL segons els drivers de la targeta gràfica
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //versió major d'OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); //no es pot redimensionar la finestra

	GLFWwindow* window = glfwCreateWindow(WINDIW_WIDTH, WINDIW_HEIGHT, "My First OpenGL", NULL, NULL); 
	//crea la finestra null serveix per a pantalles múltiples o compartir contextos entre finestres

	glfwSetFramebufferSizeCallback(window, ResizeWindow); //funció que es crida quan es redimensiona la finestra


	glfwMakeContextCurrent(window); //definir finestra en la que es treballarà

	glewExperimental = GL_TRUE; //activo funcions experimentals per totes les gràfiques
	if (glewInit() == GLEW_OK) { //inicialitzar GLEW
		glClearColor(1.f, 0.f, 0.f, 1.f); //definir color base (vermell)

		GLuint vaoPoints, vboPoints; //identificadors del vertex array object i vertex buffer object

		glGenVertexArrays(1, &vaoPoints); //generar un vertex array object
		glBindVertexArray(vaoPoints); //activar el vertex array object

		glGenBuffers(1, &vboPoints); //generar un vertex buffer object

		glBindBuffer(GL_ARRAY_BUFFER, vboPoints); //activar el vertex buffer object

		GLfloat points[] = { 0.f, 0.f}; //coordenades del punt a dibuixar

		glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW); //enviar les dades al buffer

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0); //definir com s'han d'interpretar les dades del buffer
		//activo el atribut 0, que és el que utilitzaré al shader per a les coordenades del punt, cada vertex té 2 components (x i y), son floats, no estan normalitzats, cada vertex ocupa 2*sizeof(GLfloat) bytes, y empiezan en el byte 0 del buffer
		glEnableVertexAttribArray(0);

		//desvinculo vbo
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//desvinculo VAO
		glBindVertexArray(0);






	
		while(!glfwWindowShouldClose(window)) { //mentre la finestra no s'hagi tancat

			glfwPollEvents(); //pullejem events

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //neteja el buffer de color

			glBindVertexArray(vaoPoints); //activar el vertex array object
			glDrawArrays(GL_POINTS, 0, 1); //dibujar un punto a partir del vertex array object activado, empezando en el vertex 0 y dibujando 1 vertex

			glBindVertexArray(vaoPoints); //desactivar el vertex array object


			glFlush(); //fins que la tarjeta gráfica hagi acabat de dibuixar, no segueix amb el següent codi
			glfwSwapBuffers(window); //intercanvia el buffer de dibuix amb el buffer de visualització
			

		}
		
	}
	else {

		std::cout << "peta" << std::endl;
		glfwTerminate(); //tancar GLFW
	}

	glfwTerminate();
}


