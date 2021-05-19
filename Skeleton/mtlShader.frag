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
uniform float specularExponent; //shininess

void main(){
	//Light Colour
    vec4 lightColor = vec4(1, 1, 1, 1);

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

    vec3 diffuseComponent = (textureVal * lightColor * cosTheta).rgb;
    vec3 ambientComponent = (ambientColor * textureVal).rgb;
    //dont know why dividing by 4???? 19th may
    vec3 specularComponent = (specularColor * lightColor).rgb * pow(cosAlpha, specularExponent/4.0);

    color.rgb = ambientComponent + diffuseComponent + specularComponent;
	// Material properties
	//TODO: compute light model here
    //calculate alpha separately
    color.a = 1.0;
    
}
