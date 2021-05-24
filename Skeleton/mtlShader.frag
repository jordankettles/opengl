#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 posWorldSpace;
in vec3 normalCameraSpace;
in vec3 eyeDirectionCameraSpace;
in vec3 lightDirectionCameraSpace;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
uniform mat4 MV;
uniform vec3 lightPositionWorldSpace;
uniform vec4 diffuseColor;
uniform vec4 ambientColor;
uniform vec4 specularColor;
uniform float shininess;
uniform float opacity;

void main(){

    vec4 textureVal = texture(myTextureSampler, UV);
    //Distance to the light
    float distance = length(lightPositionWorldSpace - posWorldSpace);
    //Normal vector
    vec3 n = normalize(normalCameraSpace);
    //Light direction from fragment to light.
    vec3 l = normalize(lightDirectionCameraSpace);

    //Ensure that the normal is visible.
    float cosTheta = clamp(dot(n,l), 0, 1);

    //Eye vector
    vec3 E = normalize(eyeDirectionCameraSpace);

    //Direction in which light is reflected
    vec3 R = reflect(-l, n);

    //Enusre that the reflection is visible.
    float cosAlpha = clamp(dot(E, R), 0, 1);

    vec3 diffuseComponent = (textureVal * diffuseColor * cosTheta).rgb;
    vec3 ambientComponent = (ambientColor * textureVal).rgb;
    vec3 specularComponent = (textureVal * specularColor).rgb * pow(cosAlpha, shininess);

    /*float diffuseAlpha = (textureVal * diffuseColor * cosTheta).a;
    float ambientAlpha = (ambientColor * textureVal).a;
    float specularAlpha = textureVal * specularColor).a * pow(cosAlpha, shininess);*/

    color.a = opacity;
    color.rgb = ambientComponent + diffuseComponent + specularComponent;
	//TODO:implement blinn-phong
    //     implement special effect shader (Toon, Sepia, Black-and- White (BW))
    
}
