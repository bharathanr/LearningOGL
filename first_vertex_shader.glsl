//in_position is bound to the incoming vertex positions.
in vec2 in_position;

void main(void)
{
	//The incoming z coordinate is zero. Also set W to 1.
	gl_position = vec4(in_position, 1.0);
	//TODO: Add stuff for coloring
}
