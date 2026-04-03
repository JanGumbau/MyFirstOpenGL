#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>


#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 960

struct ShaderProgram {
	GLuint vertexShader = 0;
	GLuint geometryShader = 0;
	GLuint fragmentShader = 0;
};

void Resize_Window(GLFWwindow* window, int iFrameBufferWidth, int iFrameBufferHeight) {

	//Definir nuevo tamaño del viewport
	glViewport(0, 0, iFrameBufferWidth, iFrameBufferHeight);
}

//Funcion que devuelve una string para devolver el shader a cargar en GPU
std::string Load_File(const std::string& filePath) {

	std::ifstream file(filePath);

	std::string fileContent;
	std::string line;

	//Control errores
	if (!file.is_open()) {
		std::cerr << "No se ha podido abrir el archivo, pta vida tt" << filePath << std::endl;
		std::exit(EXIT_FAILURE);
	}

	//Leemos contenido del shader y lo almacenamos
	while (std::getline(file, line)) {
		fileContent += line + "\n";
	}

	//Cerramos stream de datos y return contenido
	file.close();
	return fileContent;

}

GLuint LoadVertexShader(const std::string& filePath) {

	//Crear vertex shader a la GPU
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//Leo el archivo con el shader y lo almaceno
	std::string sShaderCode = Load_File(filePath);
	const char* cShaderSource = sShaderCode.c_str();

	//Vinculo el vertex shader a la GPU
	glShaderSource(vertexShader, 1, &cShaderSource, nullptr);

	//Compilar vertex shader
	glCompileShader(vertexShader);

	//Verificación de la compilación del shader
	GLint success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (success) {
		return vertexShader;
	}
	else {

		//Obtener longitud del log
		GLint logLenght;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLenght);

		//Obtenemos el log
		std::vector<GLchar> errorLog(logLenght);
		glGetShaderInfoLog(vertexShader, logLenght, nullptr, errorLog.data());

		//Mostramos el log
		std::cerr << "Se ha producido el siguiente error: " << errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}
GLuint LoadGeometryShader(const std::string& filePath) {

	//Crear vertex shader a la GPU
	GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);

	//Leo el archivo con el shader y lo almaceno
	std::string sShaderCode = Load_File(filePath);
	const char* cShaderSource = sShaderCode.c_str();

	//Vinculo el vertex shader a la GPU
	glShaderSource(geometryShader, 1, &cShaderSource, nullptr);

	//Compilar vertex shader
	glCompileShader(geometryShader);

	//Verificación de la compilación del shader
	GLint success;
	glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);

	if (success) {
		return geometryShader;
	}
	else {

		//Obtener longitud del log
		GLint logLenght;
		glGetShaderiv(geometryShader, GL_INFO_LOG_LENGTH, &logLenght);

		//Obtenemos el log
		std::vector<GLchar> errorLog(logLenght);
		glGetShaderInfoLog(geometryShader, logLenght, nullptr, errorLog.data());

		//Mostramos el log
		std::cerr << "Se ha producido el siguiente error: " << errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}
GLuint LoadFragmentShader(const std::string& filePath) {

	// Crear fragment shader en la GPU (Corrected to GL_FRAGMENT_SHADER)
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Leo el archivo con el shader y lo almaceno
	std::string sShaderCode = Load_File(filePath);
	const char* cShaderSource = sShaderCode.c_str();

	// Vinculo el fragment shader a la GPU
	glShaderSource(fragmentShader, 1, &cShaderSource, nullptr);

	// Compilar fragment shader
	glCompileShader(fragmentShader);

	// Verificación de la compilación del shader
	GLint success;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (success) {
		return fragmentShader;
	}
	else {
		// Obtener longitud del log
		GLint logLenght;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLenght);

		// Obtenemos el log
		std::vector<GLchar> errorLog(logLenght);
		glGetShaderInfoLog(fragmentShader, logLenght, nullptr, errorLog.data());

		// Mostramos el log
		std::cerr << "Se ha producido el siguiente error: " << errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

//Usando el struct que encapsula los shaders creare el programa en la GPU que los usara
GLuint CreateProgram(const ShaderProgram& shaders) {

	//Creamos programa
	GLuint program = glCreateProgram();

	//Verificar si existe un vertexShader a cargar
	if (shaders.vertexShader != 0) {

		glAttachShader(program, shaders.vertexShader);
	}
	//verificar si hi ha geometry shader
	if (shaders.geometryShader != 0) {

		glAttachShader(program, shaders.geometryShader);
	}
	if (shaders.fragmentShader != 0) {

		glAttachShader(program, shaders.fragmentShader);
	}

	//Linkear el programa
	glLinkProgram(program);

	//Obtener estado del programa
	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (success) {

		//Liberar recursos
		if (shaders.vertexShader != 0) {

			glDetachShader(program, shaders.vertexShader);
		}
		if (shaders.geometryShader != 0) {

			glDetachShader(program, shaders.geometryShader);
		}
		if (shaders.fragmentShader != 0) {

			glDetachShader(program, shaders.fragmentShader);
		}

		return program;
	}
	else {

		//Obtenemos longitud del log
		GLint logLenght;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLenght);

		//Almacenamos log
		std::vector <GLchar> errorLog(logLenght);
		glGetProgramInfoLog(program, logLenght, nullptr, errorLog.data());

		//Printeamos log
		std::cerr << "Error al linkar el programa: " << errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);

	}
}

int main() {

	//Definir semilla random
	srand(static_cast <unsigned int>(time(NULL)));

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

		//Compilar shaders
		ShaderProgram myFirstProgram;
		myFirstProgram.vertexShader = LoadVertexShader("MyFirstVertexShader.glsl");
		myFirstProgram.geometryShader = LoadGeometryShader("MyFirstGeometryShader.glsl");
		myFirstProgram.fragmentShader = LoadFragmentShader("MyFirstFragmentShader.glsl");

		//Compilar el programa
		GLuint myfirstCompiledProgram;
		myfirstCompiledProgram = CreateProgram(myFirstProgram);

		GLint offsetReference = glGetUniformLocation(myfirstCompiledProgram, "offset");
		//Definimos color para limpiar el buffer de color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		GLuint vaoPuntos, vboPuntos, vboAleatorios;

		//Definimos cantidad de vao a crear y donde almacenarlos 
		glGenVertexArrays(1, &vaoPuntos);

		//Indico que el VAO activo de la GPU es el que acabo de crear
		glBindVertexArray(vaoPuntos);

		//Definimos cantidad de vbo a crear y donde almacenarlos
		glGenBuffers(1, &vboPuntos);
		glGenBuffers(1, &vboAleatorios);

		//Indico que el VBO activo es el que acabo de crear y que almacenará un array. Todos los VBO que genere se asignaran al último VAO que he hecho glBindVertexArray
		glBindBuffer(GL_ARRAY_BUFFER, vboPuntos);

		//Posición X e Y del punto
		GLfloat punto[] = {
				 -0.5f, -0.25f,
				0.5f, -0.25f,
			    0.0f,  0.6f,   
		  
		 

		};

		glPolygonMode(GL_FRONT, GL_FILL);

		//Ponemos los valores en el VBO creado
		glBufferData(GL_ARRAY_BUFFER, sizeof(punto), punto, GL_STATIC_DRAW);

		//Indicamos donde almacenar y como esta distribuida la información
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

		//Indicamos que la tarjeta gráfica puede usar el atributo 0
		glEnableVertexAttribArray(0);

		//Desvinculamos VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Crear offset aleatorio
		GLfloat randomOffset[12];

		//Creamos aleatorios
		for (int i = 0; i < 12; i++) {
			randomOffset[i] = static_cast <GLfloat>(rand()) / RAND_MAX * 0.5f;
		}

		//Indico vbo activo a configurar
		glBindBuffer(GL_ARRAY_BUFFER, vboAleatorios);

		//Ponemos los valores en el VBO creado
		glBufferData(GL_ARRAY_BUFFER, sizeof(randomOffset), randomOffset, GL_STATIC_DRAW);

		//Indicamos donde almacenar y como esta distribuida la información
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

		//Indicamos que la tarjeta gráfica puede usar el atributo 1
		glEnableVertexAttribArray(1);

		//Desvinculamos VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Desvinculamos VAO
		glBindVertexArray(0);

		//Indicar a la GPU que use el programa
		glUseProgram(myfirstCompiledProgram);

		glm::vec2 direction = glm::vec2(0.4f, 0.5f);
		float speed = 0.1f;
		//modificar variable
		glUniform2fv(offsetReference, 1, &direction[0]);
		


		//Generamos el game loop
		while (!glfwWindowShouldClose(window)) {

			//Pulleamos los eventos (botones, teclas, mouse...)
			glfwPollEvents();

			//verificar inputs
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
				direction.y += speed;
			
			}
			else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
				direction.y -= speed;
				
			}
			else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
				direction.x -= speed;
			}
			
			else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
				direction.x += speed;
				
			}
			glUniform2fv(offsetReference, 1, &direction[0]);
			//Limpiamos los buffers
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


			
			//Definimos que queremos usar el VAO con los puntos
			glBindVertexArray(vaoPuntos);

			//Definimos que queremos dibujar
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			//Dejamos de usar el VAO indicado anteriormente
			glBindVertexArray(0);

			//Cambiamos buffers
			glFlush();
			glfwSwapBuffers(window);
		}

		//Desativar y liberar programa
		glUseProgram(0);
		glDeleteProgram(myfirstCompiledProgram);

	}
	else {
		std::cout << "Ha petao." << std::endl;
		glfwTerminate();
	}
	//Finalizamos GLFW
	glfwTerminate();
}




