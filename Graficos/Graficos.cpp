// Graficos.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#define GLEW_STATIC
#include "GL\glew.h"
#include "GLFW\glfw3.h"

#include <iostream>

#include "Vertice.h"
#include "Shader.h"
#include "Modelo.h"

#include <math.h>

using namespace std;

GLfloat red, green, blue;
GLuint posicionID;
GLuint colorID;

vector<Vertice> triangulo;
GLuint vertexArrayID;
GLuint bufferID;

vector<Vertice> cuadrado;
GLuint vertexArrayCuadradoID;
GLuint bufferCuadradoID;

vector<Vertice> circulo;
GLuint vertexArrayCirculoID;
GLuint bufferCirculoID;

Modelo *figura;
Shader *shader;

void actualizar()
{

}

void dibujar()
{
	//Enlazar el shader
	shader->enlazarShader();
	
	//Especificar el vertex array
	glBindVertexArray(vertexArrayID);
	glDrawArrays(GL_TRIANGLES, 0, triangulo.size());
	glBindVertexArray(vertexArrayCuadradoID); //agregado
	glDrawArrays(GL_POLYGON, 0, cuadrado.size()); //agregado
	glBindVertexArray(vertexArrayCirculoID); //agregado
	glDrawArrays(GL_POLYGON, 0, circulo.size()); //agregado	

	//soltar el vertex array
	glBindVertexArray(0);
	
	//soltar el shader
	shader->desenlazarShader();
}

void inicializarFigura()
{
	figura = new Modelo();
	Vertice v1 =
	{ vec3(-1.0f, 0.5f, 0.0f), vec4(1.0f, 0.0f, 1.f, 1.0f) };
	Vertice v2 =
	{ vec3(0.0f, -0.5f, 0.0f), vec4(1.0f, 0.0f, 1.f, 1.0f) };
	Vertice v3 =
	{ vec3(1.0f, 0.5f, 0.0f), vec4(1.0f, 0.0f, 1.f, 1.0f) };

	figura->vertices.push_back(v1);
	figura->vertices.push_back(v2);
	figura->vertices.push_back(v3);

}

void inicializarTriangulo()
{
	//Inicializar triangulo
	Vertice v1 =
	{ vec3(-1.0f, -0.5f, 0.0f), vec4(1.0f,1.0f,1.f,1.0f) };
	Vertice v2 =
	{ vec3(0.0f, 0.5f, 0.0f), vec4(1.0f,1.0f,1.f,1.0f) };
	Vertice v3 =
	{ vec3(1.0f, -0.5f, 0.0f), vec4(1.0f,1.0f,1.f,1.0f) };

	triangulo.push_back(v1);
	triangulo.push_back(v2);
	triangulo.push_back(v3);
}

void inicializarCuadrado()
{
	//Inicializar cuadrado
	Vertice v1 =
	{ vec3(-0.5f, -0.5f, 0.0f), vec4(1.0f,1.0f,1.f,1.0f) };
	Vertice v2 =
	{ vec3(-0.5f, 0.5f, 0.0f), vec4(1.0f,1.0f,1.f,1.0f) };
	Vertice v3 =
	{ vec3(0.5f, 0.5f, 0.0f), vec4(1.0f,1.0f,1.f,1.0f) };
	Vertice v4 =
	{ vec3(0.5f, -0.5f, 0.0f), vec4(1.0f,1.0f,1.f,1.0f) };

	cuadrado.push_back(v1);
	cuadrado.push_back(v2);
	cuadrado.push_back(v3);
	cuadrado.push_back(v4);
}

void inicializarCirculo()
{
	//Inicializar circulo
	for (int i = 0; i < 360; i++) {
		Vertice v1 =
		{ vec3(0.4 * cos(i) *1, 0.5 * sin(i) *1, 0.0f), 
			vec4(0.0f, 0.0f, 1.0f, 1.0f) };
		circulo.push_back(v1);
	}
}

int main()
{
	//Declaración de Ventana
	GLFWwindow *window;
	
	//Propiedades de la ventana
	GLfloat ancho = 1024;
	GLfloat alto = 768;

	//Inicialización de GLFW
	if (!glfwInit())
	{
		//Si no se inicia bien termina la ejecución
		exit(EXIT_FAILURE);
	}

	//Inicialización de la ventana
	window = glfwCreateWindow(ancho, alto, "Gráficos", NULL, NULL);

	//Verificación de creación de ventana
	if (!window)
	{
		//Cerrar todos los procesos de GLFW
		glfwTerminate();
		//Termina ejecución
		exit(EXIT_FAILURE);
	}


	//Establecer "windows" como contexto
	glfwMakeContextCurrent(window);

	//Se trae las funciones de OpenGL Moderno
	glewExperimental = true;

	//Inicializar GLEW
	GLenum glewError = glewInit();

	//Verificar si GLEW se inicializo bien
	if (glewError != GLEW_OK)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	const GLubyte *version = glGetString(GL_VERSION);
	cout << "Version de OpenGL:" << version << endl;

	red = green = blue = 0.75f;

	inicializarTriangulo();
	inicializarCuadrado();
	inicializarCirculo();
	inicializarFigura();

	//Crear instancia del shader
	const char * rutaVertex = "vShaderSimple.shader";
	const char * rutaFragment = "fShaderSimple.shader";

	shader = new Shader(rutaVertex, rutaFragment);

	//Mapeo de atributos
	posicionID = glGetAttribLocation(shader->getID(), "posicion");
	colorID = glGetAttribLocation(shader->getID(), "color");

	//Desenlazar shader
	shader->desenlazarShader();

	//Crear un vertex array
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	//Crear vertex buffer
	glGenBuffers(1, &bufferID);
	//De aqui en adelante se trabaja con este buffer
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);

	//Llenar el buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertice) * triangulo.size(), 
		triangulo.data(), GL_STATIC_DRAW);

	//Habilitar el atributo solo en el vertex array en uso
	glEnableVertexAttribArray(posicionID);

	//Especificar a OpenGL como usar la memoria con ese atributo
	glVertexAttribPointer(posicionID, 3, GL_FLOAT,
		GL_FALSE, sizeof(Vertice), 0);


	//Soltarlos
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	//Instrucciones par el cuadrado
	glGenVertexArrays(1, &vertexArrayCuadradoID);
	glBindVertexArray(vertexArrayCuadradoID);
	glGenBuffers(1, &bufferCuadradoID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferCuadradoID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertice) * cuadrado.size(), 
		cuadrado.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(posicionID);
	glEnableVertexAttribArray(colorID);
	glVertexAttribPointer(posicionID, 3, GL_FLOAT,
		GL_FALSE, sizeof(Vertice), 0);
	glVertexAttribPointer(colorID, 4, GL_FLOAT,
		GL_FALSE, sizeof(Vertice), (void*) sizeof(vec3));
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Instrucciones par el circulo
	glGenVertexArrays(1, &vertexArrayCirculoID);
	glBindVertexArray(vertexArrayCirculoID);
	glGenBuffers(1, &bufferCirculoID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferCirculoID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertice) * circulo.size(),
		circulo.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(posicionID);
	glEnableVertexAttribArray(colorID);
	glVertexAttribPointer(posicionID, 3, GL_FLOAT,
		GL_FALSE, sizeof(Vertice), 0);
	glVertexAttribPointer(colorID, 4, GL_FLOAT,
		GL_FALSE, sizeof(Vertice), (void*) sizeof(vec3));
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	

	//Ciclo de Dibujo
	while (!glfwWindowShouldClose(window))
	{
		//Establecer el area de render (viewport)
		glViewport(0, 0, ancho, alto);
		//Establecer el color con el que se limpia
		glClearColor(red, green, blue, 1);

		//Limpiar pantalla
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Rutina de Dibujo
		actualizar();
		dibujar();

		//Intercambio de Buffers
		glfwSwapBuffers(window);
		//Buscar nueva señal de entrada
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
    return 0;
}

