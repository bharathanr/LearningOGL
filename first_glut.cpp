#include <iostream>
#include <string>
#include <fstream>

//For OGL 3.2
#define GL3_PROTOTYPES
#include "GL3/gl3.h"
#define __gl_h_

#include "GL/freeglut.h"


//Freeglut pulls these in anyway
//#include <GL/gl.h>
//#include <GL/glu.h>

#define DEBUG 1

void init()
{
	/* select clearing (background) color       */
	glClearColor(0.0, 0.0, 0.0, 0.0);

	/*  initialize viewing values */
	/*
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
	*/
}

/*
 * Reads the contents of a file and stores them in a c_string. 
 * The c_string is allocated dynamically. If 
 * there is any error at any stage, a null c_string is returned.
 * Input:	
 *	The full path to a file (including the filename) as 
 *	a std::string. Note that it is assumed that the file
 *	is assumed to be plaintext. Also, if the filename alone
 * 	is specified, it assumes that the file is in the current 
 *	directory.
 * Returns: 
 *         A c style string with the contents of the shader's file.
 *	   A null string.
 */
GLchar* file_to_char_pointer(std::string path_to_file)
{
	//TODO: Add exception handling.
	//TODO: Make this function nicer, so as to use glShaderSource fully.

	GLchar* shader_source_code = NULL;
	unsigned int file_size;

	//Open the file.
	std::ifstream shader_file(path_to_file.c_str());
	//Read in all the lines if the file is open.
	if(shader_file.is_open())
	{
		//Calculate the file's size in bytes
		
		//Move the get pointer to the end
		shader_file.seekg(0, std::ios::end);
		//The position of the get pointer gives the number of bytes
		file_size = shader_file.tellg();
		if(file_size != -1)
		{
			/* Allocate enough space in the char array to hold
			 * the file's contents.
			 */
			//TODO: Check this code.
			shader_source_code = new GLchar[file_size];
			
			//Send the get pointer back to the beginning.
			shader_file.seekg(0, std::ios::beg);
			
			//Read the file into the source code buffer.
			int i = 0;	//Gives the position in the buffer to write the 
				//next character to.
			while(!shader_file.eof())
			{
				/* TODO: See if the other ways to read data are
				 * better.
				 */
				/* Get the next character and write it to 
				 * the buffer. 
				 */				
				shader_file.get(shader_source_code[i]);
				//Step to te next position in the buffer.
				i++;
			}
		}
			
	}
	
	//Close the file.
	shader_file.close();
	#ifdef DEBUG
	std::cout << shader_source_code;
	#endif
	//Return the said pointer.
	return shader_source_code;
}

void display(void)
{
	/* clear all pixels */
	glClear(GL_COLOR_BUFFER_BIT);
	
	//OLD OGL CODE
	/* draw white polygon (rectangle) with corners at
	 * (0.25, 0.25, 0.0) and (0.75, 0.75, 0.0)
	 */
	//glColor3f(1.0, 1.0, 1.0);
	
	GLfloat square[] = {0.25, 0.25, 0.0, 0.75, 0.25, 0.0, 0.75, 0.75, 0.0,\
				 0.25, 0.75, 0.0};
	//Indices assume GL_TRIANGLES  TODO-- check!
	GLuint square_indices[] = {1, 2, 3, 1, 4, 3};
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
	glBindBuffer(GL_ARRAY_BUFFER, vbo_names[0]);

	/* TODO: See if there is a better spec for vbo access 
	 * than GL_STATIC_DRAW
	 */
	
	//Copy the vertex data from the square array to this buffer 
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 12, square,\
			 GL_STATIC_DRAW);
	
	/* Specify that the vertex data is going into vertex attribute zero.
	 * in the vertex attribute array. Enable the corresponding attribute 
	 * in the array.
	 */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//Bind the second vbo as being an active buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_names[1]);
	
	//Copy the index data to the second vbo
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 6, \
			 square_indices, GL_STATIC_DRAW);
	
	/* Specify that the index data is going into vertex attribute one
	 * n the vertex attribute array. Enable the corresponding attribute 
	 * in the array.
	 */
	glVertexAttribPointer(1, 1, GL_INT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	//Shaders!	
	GLchar *vertex_source, *fragment_source;
	GLuint vertex_shader, fragment_shader;
	GLuint shader_program;

	//Obtain the source for the shader programs.
	vertex_source = file_to_char_pointer("first_vertex_shader.glsl");
	fragment_source = file_to_char_pointer("first_fragment_shader.glsl");

	if((vertex_source != NULL) && (fragment_source != NULL))
	{
	
		//Obtain the name for the shader program.
		shader_program = glCreateProgram();
	
		//Obtain the names of the shaders
		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

		//Set the source code of the shaders.
		//TODO: Fix this unsafe casting.
		glShaderSource(vertex_shader, 1, (const GLchar**)&vertex_source, NULL); 
							//Strings \0 terminated
		glShaderSource(fragment_shader, 1, (const GLchar**) &fragment_source, NULL);
	
		//Compile the shaders
		glCompileShader(vertex_shader);
		glCompileShader(fragment_shader);
	
		//Attach the shaders to the program
		/* TODO: Figure out these lines carefully. Is the order of attaching
		 * shaders important?
		 */
		glAttachShader(shader_program, vertex_shader);
		glAttachShader(shader_program, fragment_shader);
	
		//Link the program
		glLinkProgram(shader_program);

		//Bind parameters to the shader.
		glBindAttribLocation(shader_program, 0, "in_position");	

		//Set this program as the active shader program
		glUseProgram(shader_program);

		//Draw code.
//		glDrawElements(GL_TRIANGLES, 6,\
				 GL_UNSIGNED_INT, );

		//Do cleanup. :)
	}
}

int main(int argc, char** argv)
{
	//The order of initialisation may be wrong. TODO check.

	//For OGL3.2
	glutInitContextVersion (3, 2);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	//Usual glut code
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
