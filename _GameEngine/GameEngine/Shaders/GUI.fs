#version 330

in vec2 TexCoord;

uniform sampler2D Image;

const float offset = 1.0 / 300.0; 

void main()
{	
    vec4 color = texture(Image, TexCoord);
	
    if(color == vec4(0.0,0.0,0.0,1.0))
        discard;

    gl_FragColor = color;
         
}