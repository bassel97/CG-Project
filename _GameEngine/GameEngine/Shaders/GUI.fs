#version 330

in vec2 TexCoord;

uniform sampler2D Image;

void main()
{	
    vec4 color = texture(Image, TexCoord);
	
    if(color == vec4(0.0,0.0,0.0,1.0))
        discard;

    gl_FragColor = color; 
    //gl_FragColor.a = 0.8f;
    
    //gl_FragColor = vec4(1.0f, 0.0f, 1.0f , 1.0f);
}