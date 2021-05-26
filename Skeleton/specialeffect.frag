#version 330 core

in vec2 UV;

out vec4 color;

uniform sampler2D myTextureSampler;
uniform float renderMode;
uniform float time;

vec4 effect1(vec2 UV) {
    return texture(myTextureSampler, UV);
}

vec4 effect2(vec2 UV) {
    return texture(myTextureSampler, UV);
}

vec4 effect3(vec2 UV) {
    return texture(myTextureSampler, UV);
}

void main(){
    if (renderMode == 2.0) {
        color.rgb = texture( myTextureSampler, UV + 0.005*vec2( sin(time+1024.0*UV.x),cos(time+768.0*UV.y)) ).rgb;
        // color.rgb = effect1(UV).rgb;
        ;
    } else if (renderMode == 3.0) {
        // color.rgb = effect2(UV).rgb;
        ;
    } else if (renderMode == 4.0) {
        // color.rgb = effect2(UV).rgb;
        ;
    } else {
        // Render mode is equal to 1.0
        // No effect.
        color.rgb = texture(myTextureSampler, UV).rgb;
    }
    // Alpha is always 1.
    color.a = 1.0;
    //write some cool effects here.
}