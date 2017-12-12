#version 330

in vec4 myColor;
in vec2 TexCoord;
in vec3 normal_vector;

uniform sampler2D Diffuse;
uniform sampler2D AO;

in mat4 rotationMatrix;

void main()
{
	vec3 norm = normalize(rotationMatrix * vec4(normal_vector,1)).xyz;

	vec3 light_Dir = normalize( vec3(1,1,0) );
	float as = 0.3;

	vec4 ambient = as * vec4(1);

	float diff = max( dot(norm,light_Dir), 0.0);

	vec4 diffuse =  diff * vec4(1);
	// Output color = red 
	//gl_FragColor = vec4(1.0 , 0.0, 0.0, 1.0);

	//gl_FragColor = myColor;
	
	vec4  light_toon =  (ambient+diffuse) *  texture(Diffuse, TexCoord)  * texture(AO, TexCoord);

	gl_FragColor = light_toon;
	//gl_FragColor.a = 0.2f;
}