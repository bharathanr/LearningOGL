#include <iostream>
#include "GL/glut.h"

//Freeglut pulls these in anyway
//#include <GL/gl.h>
//#include <GL/glu.h>

void init(void)
{
	/* select clearing (background) color       */
	glClearColor(0.0, 0.0, 0.0, 0.0);
	/*  initialize viewing values */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

void display(void)
{
	/* clear all pixels */
	glClear(GL_COLOR_BUFFER_BIT);

	/* draw white polygon (rectangle) with corners at
	 * (0.25, 0.25, 0.0) and (0.75, 0.75, 0.0)
	 */
	glColor3f(1.0, 1.0, 1.0);
	
	GLfloat square[] = {0.25, 0.25, 0.0, 0.75, 0.25, 0.0, 0.75, 0.75, 0.0,\
				 0.25, 0.75, 0.0};
	//Indices assume GL_TRIANGLES  TODO-- check!
	GLint square_indices = {1, 2, 3, 1, 4, 3};
	//FFP
	/*	
	glBegin(GL_POLYGON);
		glVertex3f(0.25, 0.25, 0.0);
		glVertex3f(0.75, 0.25, 0.0);
		glVertex3f(0.75, 0.75, 0.0);
		glVertex3f(0.25, 0.75, 0.0);
	glEnd();
	*/

	//Using vertex arrays
	
	/*
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertex_buffer);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
	*/

	//Using VBO's and VAO's
	
	//Generate the name for a Vertex Array
	GLuint vao_name;
	glGenVertexArrays(1, &vao_name);
	
	//Bind this vao as the currently used object
	glBindVertexArray(vao_name);
	
	//Create the names two vertex buffer object
	GLuint vbo_names[2];
	glGenBuffers(2, vbo_names);

	//Bind the first vbo and indicate that it contains vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	/*TODO: See if there is a better spec for vbo access 
	 *than GL_STATIC_DRAW
	 */
	
	//Copy the vertex data from the square array to this buffer 
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 12, square,\
			 GL_STATIC_DRAW);
	
	/*Specify that the vertex data is going into vertex attribute zero.
	 *in the vertex attribute array. Enable the corresponding attribute 
	 *in the array.
	 */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//Bind the second vbo as being an active buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
	
	//Copy the index data to the second vbo
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * 6, \
			 square_indices, GL_STATIC_DRAW);
	
	/*Specify that the index data is going into vertex attribute one
	 *n the vertex attribute array. Enable the corresponding attribute 
	 *in the array.
	 */
	glVertexAttribPointer(1, 1, GLint, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	//Shaders!	
	GLchar* vertex_source, fragment_source;
	GLuint vertex_shader, fragment_shader;
	GLuint shader_program;

	//Obtain the source for the shader programs.
	
	//Obtain the name for the shader program.
	shader_program = glCreateProgram();

	//Obtain the names of the shaders
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	/* don’t wait!
	 * start processing buffered OpenGL routines
	 */
	glFlush();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(250, 250);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("hello");
	init();
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}
