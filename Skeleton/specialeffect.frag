#version 330 core

in vec2 UV;

out vec4 color;

uniform sampler2D myTextureSampler;
uniform float renderMode;
uniform float time;

const float PI = 3.1415926535; //Digits of pi for the fisheye effect.

vec4 wobbly(vec2 UV) {
    //Black and white motion wobble.
    float R = texture(myTextureSampler, UV+0.02*vec2(sin(time+UV.x),cos(time+UV.y))).r/(3);
    float G = texture(myTextureSampler, UV+0.01*vec2(sin(time+UV.x),cos(time+UV.y))).g/(3);
    float B = texture(myTextureSampler, UV+0.01*vec2(sin(time+UV.x),cos(time+UV.y))).b/(3);
    return vec4(R+G+B, R+G+B, R+G+B, 1); 
}

vec4 fisheye(vec2 UV) {
    // Fisheye effect that zooms in and out.
    float aperture = 128.0+(40*sin(time));
    float apertureHalf = 0.5 * aperture * (PI / 180.0);
    float maxFactor = sin(apertureHalf);
    
    vec2 coords;
    vec2 xy = 2.0 * UV.xy - 1.0;
    float d = length(xy);
    if (d < (2.0-maxFactor)) {
        d = length(xy * maxFactor);
        float z = sqrt(1.0 - d * d);
        float r = atan(d, z) / PI;
        float phi = atan(xy.y, xy.x);
        coords.x = r * cos(phi) + 0.5;
        coords.y = r * sin(phi) + 0.5;
    } else {
        coords = UV.xy;
    }
    return texture(myTextureSampler, coords);
}

void main(){
    if (renderMode == 2.0) {
        // Zooming Fisheye effect.
        color.rgb = fisheye(UV).rgb;
    } else if (renderMode == 3.0) {
        // Black and white + wobbly effect.
        color.rgb = wobbly(UV).rgb;
    } else {
        // Render mode is equal to 1.0, no effect.
        color.rgb = texture(myTextureSampler, UV).rgb;
    }
    // Alpha is always 1.
    color.a = 1.0;
}