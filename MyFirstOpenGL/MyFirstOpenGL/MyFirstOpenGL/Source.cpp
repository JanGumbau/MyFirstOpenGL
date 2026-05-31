#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define NUM_CABINES 8
#define SEGMENTS_RODA 48
#define PI 3.14159265358979323846f

std::vector<GLuint> compiledPrograms;

//Struct para controlar el GameObject
struct GameObject {

	glm::vec3 position = glm::vec3(0.f);
	glm::vec3 rotation = glm::vec3(0.f);
	glm::vec3 scale = glm::vec3(1.f);
};
struct Camera
{
	glm::vec3 position = glm::vec3(0.f, 0.f, 3.f);
	glm::vec3 localVectorUp = glm::vec3(0.f, 1.0f, 0.0f);

	float fFov = 45.f;
	float fNear = 0.1f;
	float fFar = 100.f;
};

struct ShaderProgram {

	GLuint vertexShader = 0;
	GLuint geometryShader = 0;
	GLuint fragmentShader = 0;
};

void Resize_Window(GLFWwindow* window, int iFrameBufferWidth, int iFrameBufferHeight) {

	glViewport(0, 0, iFrameBufferWidth, iFrameBufferHeight);
	glUniform2f(glGetUniformLocation(compiledPrograms[0], "windowSize"), iFrameBufferWidth, iFrameBufferHeight);

}

glm::mat4 GenerateScaleMatrix(glm::vec3 scaleAxis) {

	return glm::scale(glm::mat4(1.0f), scaleAxis);
}

glm::mat4 GenerateRotationMatrix(glm::vec3 axis, float fDegrees) {

	return glm::rotate(glm::mat4(1.0f), glm::radians(fDegrees), glm::normalize(axis));
}

glm::mat4 GenerateTranslationMatrix(glm::vec3 translation) {

	return glm::translate(glm::mat4(1.0f), translation);
}

std::string Load_File(const std::string& filePath) {

	std::ifstream file(filePath);

	std::string fileContent;
	std::string line;

	if (!file.is_open()) {
		std::cerr << "No se ha podido abrir el archivo: " << filePath << std::endl;
		std::exit(EXIT_FAILURE);
	}

	while (std::getline(file, line)) {
		fileContent += line + "\n";
	}

	file.close();

	return fileContent;
}

GLuint LoadFragmentShader(const std::string& filePath) {

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string sShaderCode = Load_File(filePath);
	const char* cShaderSource = sShaderCode.c_str();

	glShaderSource(fragmentShader, 1, &cShaderSource, nullptr);
	glCompileShader(fragmentShader);

	GLint success;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (success) {
		return fragmentShader;
	}
	else {
		GLint logLength;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);

		std::vector<GLchar> errorLog(logLength);
		glGetShaderInfoLog(fragmentShader, logLength, nullptr, errorLog.data());

		std::cerr << "Se ha producido un error al cargar el fragment shader:  " << errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}


GLuint LoadGeometryShader(const std::string& filePath) {

	GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);

	std::string sShaderCode = Load_File(filePath);
	const char* cShaderSource = sShaderCode.c_str();

	glShaderSource(geometryShader, 1, &cShaderSource, nullptr);
	glCompileShader(geometryShader);

	GLint success;
	glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);

	if (success) {
		return geometryShader;
	}
	else {
		GLint logLength;
		glGetShaderiv(geometryShader, GL_INFO_LOG_LENGTH, &logLength);

		std::vector<GLchar> errorLog(logLength);
		glGetShaderInfoLog(geometryShader, logLength, nullptr, errorLog.data());

		std::cerr << "Se ha producido un error al cargar el geometry shader:  " << errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

GLuint LoadVertexShader(const std::string& filePath) {

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	std::string sShaderCode = Load_File(filePath);
	const char* cShaderSource = sShaderCode.c_str();

	glShaderSource(vertexShader, 1, &cShaderSource, nullptr);
	glCompileShader(vertexShader);

	GLint success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (success) {
		return vertexShader;
	}
	else {
		GLint logLength;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);

		std::vector<GLchar> errorLog(logLength);
		glGetShaderInfoLog(vertexShader, logLength, nullptr, errorLog.data());

		std::cerr << "Se ha producido un error al cargar el vertex shader:  " << errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

GLuint CreateProgram(const ShaderProgram& shaders) {

	GLuint program = glCreateProgram();

	if (shaders.vertexShader != 0) {
		glAttachShader(program, shaders.vertexShader);
	}

	if (shaders.geometryShader != 0) {
		glAttachShader(program, shaders.geometryShader);
	}

	if (shaders.fragmentShader != 0) {
		glAttachShader(program, shaders.fragmentShader);
	}

	glLinkProgram(program);

	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (success) {

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

		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

		std::vector<GLchar> errorLog(logLength);
		glGetProgramInfoLog(program, logLength, nullptr, errorLog.data());

		std::cerr << "Error al linkar el programa:  " << errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

// =========================================================================
// GEOMETRIA DE LA NÒRIA
// =========================================================================
// Genera els vèrtexs d'una nòria centrada en (0,0,0):
//   - Roda exterior: cercle dibuixat com a GL_LINES (parelles de punts)
//   - Radis: línies des del centre fins a cada cabina
//   - Cabines: petits quadrats (4 línies cadascun) als extrems dels radis
// Tot es retorna en un únic buffer per renderitzar amb GL_LINES.
std::vector<GLfloat> GenerarNoria(float radi, float midaCabina) {

	std::vector<GLfloat> vertexs;

	// --- 1) RODA EXTERIOR (cercle de SEGMENTS_RODA segments) ---
	for (int i = 0; i < SEGMENTS_RODA; i++) {
		float ang1 = (float)i / SEGMENTS_RODA * 2.f * PI;
		float ang2 = (float)(i + 1) / SEGMENTS_RODA * 2.f * PI;

		vertexs.push_back(cos(ang1) * radi);
		vertexs.push_back(sin(ang1) * radi);
		vertexs.push_back(0.f);

		vertexs.push_back(cos(ang2) * radi);
		vertexs.push_back(sin(ang2) * radi);
		vertexs.push_back(0.f);
	}

	// --- 2) RADIS (línies del centre fins a cada cabina) ---
	for (int i = 0; i < NUM_CABINES; i++) {
		float ang = (float)i / NUM_CABINES * 2.f * PI;

		// Centre de la nòria
		vertexs.push_back(0.f);
		vertexs.push_back(0.f);
		vertexs.push_back(0.f);

		// Punt on s'aguanta la cabina (a sobre la roda)
		vertexs.push_back(cos(ang) * radi);
		vertexs.push_back(sin(ang) * radi);
		vertexs.push_back(0.f);
	}

	// --- 3) CABINES (quadrat de 4 línies a cada extrem de radi) ---
	float m = midaCabina * 0.5f;
	for (int i = 0; i < NUM_CABINES; i++) {
		float ang = (float)i / NUM_CABINES * 2.f * PI;
		float cx = cos(ang) * radi;
		float cy = sin(ang) * radi;

		// 4 cantonades del quadrat
		float x0 = cx - m, y0 = cy - m;
		float x1 = cx + m, y1 = cy - m;
		float x2 = cx + m, y2 = cy + m;
		float x3 = cx - m, y3 = cy + m;

		// Aresta inferior
		vertexs.push_back(x0); vertexs.push_back(y0); vertexs.push_back(0.f);
		vertexs.push_back(x1); vertexs.push_back(y1); vertexs.push_back(0.f);
		// Aresta dreta
		vertexs.push_back(x1); vertexs.push_back(y1); vertexs.push_back(0.f);
		vertexs.push_back(x2); vertexs.push_back(y2); vertexs.push_back(0.f);
		// Aresta superior
		vertexs.push_back(x2); vertexs.push_back(y2); vertexs.push_back(0.f);
		vertexs.push_back(x3); vertexs.push_back(y3); vertexs.push_back(0.f);
		// Aresta esquerra
		vertexs.push_back(x3); vertexs.push_back(y3); vertexs.push_back(0.f);
		vertexs.push_back(x0); vertexs.push_back(y0); vertexs.push_back(0.f);
	}

	return vertexs;
}

void main() {

	srand(static_cast<unsigned int>(time(NULL)));

	glfwInit();

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "My Engine - Noria", NULL, NULL);

	glfwSetFramebufferSizeCallback(window, Resize_Window);

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	if (glewInit() == GLEW_OK) {

		GameObject noria;
		Camera camera;

		ShaderProgram myFirstProgram;
		myFirstProgram.vertexShader = LoadVertexShader("MyFirstVertexShader.glsl");
		myFirstProgram.geometryShader = LoadGeometryShader("MyFirstGeometryShader.glsl");
		myFirstProgram.fragmentShader = LoadFragmentShader("MyFirstFragmentShader.glsl");

		compiledPrograms.push_back(CreateProgram(myFirstProgram));

		glClearColor(0.05f, 0.05f, 0.1f, 1.f);

		// Generem els vèrtexs de la nòria
		std::vector<GLfloat> vertexsNoria = GenerarNoria(1.0f, 0.25f);
		GLsizei numVertexs = (GLsizei)(vertexsNoria.size() / 3);

		GLuint vaoNoria, vboNoria;
		glGenVertexArrays(1, &vaoNoria);
		glBindVertexArray(vaoNoria);

		glGenBuffers(1, &vboNoria);
		glBindBuffer(GL_ARRAY_BUFFER, vboNoria);

		glBufferData(GL_ARRAY_BUFFER, vertexsNoria.size() * sizeof(GLfloat), vertexsNoria.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glUseProgram(compiledPrograms[0]);

		glUniform2f(glGetUniformLocation(compiledPrograms[0], "windowSize"), WINDOW_WIDTH, WINDOW_HEIGHT);

		// Game loop
		while (!glfwWindowShouldClose(window)) {

			glfwPollEvents();

			// Controls de càmera amb WASD
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.position.y += 0.01f;
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.position.y -= 0.01f;
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.position.x -= 0.01f;
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.position.x += 0.01f;
			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) camera.position.z -= 0.02f;
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) camera.position.z += 0.02f;

			// Posicionem la nòria al centre
			noria.position = glm::vec3(0.f, 0.f, 0.f);
			noria.rotation = glm::vec3(0.f, 0.f, 1.f); // eix de rotació (Z)
			noria.scale = glm::vec3(1.f, 1.f, 1.f);

			// Generem matrius de model
			glm::mat4 translationMatrix = GenerateTranslationMatrix(noria.position);
			glm::mat4 rotationMatrix = glm::mat4(1.0f); // la rotació la fa el SHADER amb el temps
			glm::mat4 scaleMatrix = GenerateScaleMatrix(noria.scale);

			// Matrius de càmera
			glm::mat4 viewMatrix = glm::lookAt(camera.position, camera.position + glm::vec3(0.f, 0.f, -1.f), camera.localVectorUp);
			glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.fFov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, camera.fNear, camera.fFar);

			// Temps en segons des de l'inici (CLAU per a la rotació al shader)
			float fTime = (float)glfwGetTime();

			// Pasem uniforms al shader
			glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[0], "translationMatrix"), 1, GL_FALSE, glm::value_ptr(translationMatrix));
			glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[0], "rotationMatrix"), 1, GL_FALSE, glm::value_ptr(rotationMatrix));
			glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[0], "scaleMatrix"), 1, GL_FALSE, glm::value_ptr(scaleMatrix));
			glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[0], "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
			glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[0], "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
			glUniform1f(glGetUniformLocation(compiledPrograms[0], "time"), fTime);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			glBindVertexArray(vaoNoria);
			glDrawArrays(GL_LINES, 0, numVertexs);
			glBindVertexArray(0);

			glFlush();
			glfwSwapBuffers(window);
		}

		glUseProgram(0);
		glDeleteProgram(compiledPrograms[0]);

	}
	else {
		std::cout << "Ha petao." << std::endl;
		glfwTerminate();
	}

	glfwTerminate();

}
