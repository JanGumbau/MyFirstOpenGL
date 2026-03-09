#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

void Resize_Window(GLFWwindow* window, int iFrameBufferWidth, int iFrameBufferHeight) {

	//Definir nuevo tamaño del viewport
	glViewport(0, 0, iFrameBufferWidth, iFrameBufferHeight);
}

// función que devuelve string con todo el archivo leído
std::string Load_File(const std::string& filePath) {
	std::ifstream file(filePath);

	std::string fileContent;
	std::string line;

	int wordCount = 0;


	// try catch típico
	if (!file.is_open()) {
		std::cerr << "No se ha podido abrir el archivo" << filePath << std::endl;
		std::exit(EXIT_FAILURE);
	}


	while (std::getline(file, line)) {

		std::stringstream ss(line);
		std::string word;

		while (ss >> word) {
			wordCount++;
		}
	}

	file.close();

	std::cout << "Numero de palabras: " << wordCount << std::endl;
	return "";


	/*

	//
	while (std::getline(file, line)) {
		fileContent += line + "\n";
	}

	// cerramos stream datos y devolvemos la variable/contenido
	file.close();
	return fileContent;*/
}

GLuint LoadVertexShader(const std::string& filePath) {

	// crear un vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	std::string sShaderCode = Load_File(filePath);
	const char* cShaderSource = sShaderCode.c_str();

	// vinculamos el vertex shader con su código fuente
	glShaderSource(vertexShader, 1, &cShaderSource, nullptr);

	// compilamos
	glCompileShader(vertexShader);

	// Verificar errores de compilación
	GLint success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	//Si la compilacion ha sido exitosa devolvemos el vertex shader
	if (success) {

		return vertexShader;
	}
	else {

		//Obtenemos longitud del log
		GLint logLength;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);

		//Obtenemos el log
		std::vector<GLchar> errorLog(logLength);
		glGetShaderInfoLog(vertexShader, logLength, nullptr, errorLog.data());

		//Mostramos el log y finalizamos programa
		std::cerr << "Se ha producido un error al cargar el vertex shader: "
			<< errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

void main()
{
	std::cout << Load_File("DELETEME.txt") << std::endl;

	int iFrameBufferWidth = 0;
	int iFrameBufferHeight = 0;

	//Inicializamos GLFW para gestionar ventanas e inputs
	glfwInit();

	//Configuramos la ventana
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//Inicializamos la ventana
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "My Engine", NULL, NULL);

	//Asignamos función de callback para cuando el frame buffer es modificado
	glfwSetFramebufferSizeCallback(window, Resize_Window);

	//Definimos espacio de trabajo
	glfwMakeContextCurrent(window);

	//Permitimos a GLEW usar funcionalidades experimentales
	glewExperimental = GL_TRUE;

	//Activamos cull face
	glEnable(GL_CULL_FACE);

	//Indicamos lado del culling
	glCullFace(GL_BACK);

	//Inicializamos GLEW y controlamos errores
	if (glewInit() == GLEW_OK) {

		//Definimos color para limpiar el buffer de color
		glClearColor(1.f, 0.f, 0.f, 1.f);

		GLuint vaoPuntos, vboPuntos;

		//Definimos cantidad de vao a crear y donde almacenarlos 
		glGenVertexArrays(1, &vaoPuntos);

		//Indico que el VAO activo de la GPU es el que acabo de crear
		glBindVertexArray(vaoPuntos);

		//Definimos cantidad de vbo a crear y donde almacenarlos
		glGenBuffers(1, &vboPuntos);

		//Indico que el VBO activo es el que acabo de crear y que almacenará un array. Todos los VBO que genere se asignaran al último VAO que he hecho glBindVertexArray
		glBindBuffer(GL_ARRAY_BUFFER, vboPuntos);

		//Posición X e Y del punto
		GLfloat punto[] = {
			-0.5f,  0.5f, // Vértice superior izquierdo
			-0.5f, -0.5f, // Vértice superior derecho
			 0.0f,  0.5f, // Vértice inferior derecho
			 0.0f, -0.5f, // Vértice inferior derecho
			 0.5f,  0.5f, // Vértice inferior izquierdo
			 0.5f, -0.5f  // Vértice superior izquierdo
		};

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//Ponemos los valores en el VBO creado
		glBufferData(GL_ARRAY_BUFFER, sizeof(punto), punto, GL_STATIC_DRAW);

		//Indicamos donde almacenar y como esta distribuida la información
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

		//Indicamos que la tarjeta gráfica puede usar el atributo 0
		glEnableVertexAttribArray(0);

		//Desvinculamos VAO
		glBindVertexArray(0);

		//Desvinculamos VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Generamos el game loop
		while (!glfwWindowShouldClose(window)) {

			//Pulleamos los eventos (botones, teclas, mouse...)
			glfwPollEvents();

			//Limpiamos los buffers
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			//Definimos que queremos usar el VAO con los puntos
			glBindVertexArray(vaoPuntos);

			//Definimos que queremos dibujar
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);

			//Dejamos de usar el VAO indicado anteriormente
			glBindVertexArray(0);

			//Cambiamos buffers
			glFlush();
			glfwSwapBuffers(window);
		}

	}
	else {
		std::cout << "Ha petao." << std::endl;
		glfwTerminate();
	}
	//Finalizamos GLFW
	glfwTerminate();
}


