#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D screenTexture;
const float offset = 1.0 / 400; 

void main()
{
    /*
    color = texture(screenTexture, TexCoords);     //原图
    */

    /*
    color = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);   //反色
    */

    /*
    color = texture(screenTexture, TexCoords);
    float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;    //灰度化
    color = vec4(average, average, average, 1.0);
    */

     vec2 offsets[9] = vec2[](
        vec2(-offset, offset),  // top-left
        vec2(0.0f,    offset),  // top-center
        vec2(offset,  offset),  // top-right
        vec2(-offset, 0.0f),    // center-left
        vec2(0.0f,    0.0f),    // center-center
        vec2(offset,  0.0f),    // center-right
        vec2(-offset, -offset), // bottom-left
        vec2(0.0f,    -offset), // bottom-center
        vec2(offset,  -offset)  // bottom-right
    );
    /*
    float kernel[9] = float[](          //锐化
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
    */
    /*
    float kernel[9] = float[](          //模糊
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
    );
    */
    
    float kernel[9] = float[](          //边缘检测
        -1, -1, -1,
        -1,  8, -1,
        -1, -1, -1
    );
    

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col;
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    color = vec4(col, 1.0);
}