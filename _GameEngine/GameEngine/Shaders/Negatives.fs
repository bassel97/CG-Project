#version 330

in vec2 TexCoord;

in vec4 color;
in vec3 normal_vector;
in mat4 rotationMatrix;

uniform sampler2D Diffuse;
uniform sampler2D AO;

void main()
{
	vec3 norm = normalize(rotationMatrix * vec4(normal_vector,1)).xyz;

	vec3 light_Dir = normalize( vec3(1,1,0) );

	float diff = max( dot(norm,light_Dir), 0.0);

	vec4 diffuse =  diff * vec4(1);


	float as = 0.3;

	vec4 ambient = as * vec4(1);


	// Output color = red 
	//gl_FragColor = vec4(1.0 , 0.0, 0.0, 1.0);

	//gl_FragColor = myColor;
	
	vec4  light_toon =  (ambient+diffuse) *  texture(Diffuse, TexCoord)  * texture(AO, TexCoord);

	vec4 negative = vec4( light_toon.b , light_toon.b , light_toon.r , 1 );

	gl_FragColor = negative;
	//gl_FragColor = color;
}