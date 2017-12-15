#version 330

in vec2 TexCoord;

in vec4 color;
in vec3 normal_vector;
in mat4 rotationMatrix;
in float _time;


uniform sampler2D Diffuse;
uniform sampler2D AO;

void main()
{
	float redValue = dot( vec4(normal_vector,1), vec4(0,0,1,1) );

	vec4  light_toon =  vec4( 0, (1 - redValue)* abs(sin(_time)), redValue + (1 - redValue), 1);


	gl_FragColor = light_toon;
}