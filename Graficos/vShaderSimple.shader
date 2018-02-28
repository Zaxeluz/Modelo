#version 330 core
//La nprimera linea es la version

//Atributos de entrada
layout(location = 0) in vec3 posicion;
in vec4 color;

//Atributos de salida
out vec4 fragmentColor;


//Funcion main
void main()
{
	//La posicion de salida del vertice se guarda en el gl_Position (vec4)
	gl_Position.xyz = posicion;
	gl_Position.w = 1.0;

	//Valor outColoro
	fragmentColor = color;
}