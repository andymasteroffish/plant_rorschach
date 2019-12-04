#version 120
#extension GL_ARB_texture_rectangle : enable

//THIS ASSUMES WE ARE GETTING A GREYSCALE IMAGE


//uniform sampler2DRect myTex;

//this is the src image drawn into the texture in testApp
uniform sampler2DRect tex0;

//this value will be adjusted by testApp
uniform float increaseVal = 0.0;

void main (void){
    
    vec2 pos = gl_TexCoord[0].st;
    
    //get the current color of this pixel
    vec4 src = texture2DRect(tex0, pos).rgba;
    //increase to push it toward white
    src.r += increaseVal;
    src.g += increaseVal;
    src.b += increaseVal;
    
    //set the pixel to be the new color
    gl_FragColor = src;
    
}