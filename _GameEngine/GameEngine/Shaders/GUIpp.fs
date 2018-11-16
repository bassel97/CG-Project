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

    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
    float kernel2[9] = float[](
    1.0 / 8, 2.0 / 8, 1.0 / 8,
    2.0 / 8, 4.0 / 8, 2.0 / 8,
    1.0 / 8, 2.0 / 8, 1.0 / 8  
    );
    float kernel3[9] = float[](
    1.0, 1.0, 1.0,
    1.0, -8.0, 1.0,
    1.0, 1.0, 1.0
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(Image, TexCoord.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel3[i];
    
    float edgeMag = (length(  col ) / 2f);

    //gl_FragColor = vec4(edgeMag,edgeMag,edgeMag,1.0) ;
    gl_FragColor = (1 - vec4(edgeMag,edgeMag,edgeMag,1.0)) * color ;

}