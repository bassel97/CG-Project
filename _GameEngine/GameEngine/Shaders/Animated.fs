#version 330

in vec4 myColor;
in vec2 TexCoord;

uniform sampler2D Diffuse;
uniform sampler2D AO;

void main()
{

	// Output color = red 
	//gl_FragColor = vec4(1.0 , 0.0, 0.0, 1.0);

	//gl_FragColor = myColor;
	
	gl_FragColor = texture(Diffuse, TexCoord) /* texture(AO, TexCoord)*/;
}