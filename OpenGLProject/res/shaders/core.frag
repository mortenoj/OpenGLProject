#version 330 core

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform vec3 lightPos0;
uniform vec3 cameraPos;

void main() {
    //fs_color = vec4(vs_color, 1.0f);
    //fs_color = texture(texture0, vs_texcoord) + texture(texture1, vs_texcoord);

    // Ambient light
    vec3 ambientLight = vec3(0.1f, 0.1f, 0.1f);

    // Diffuse light
    vec3 posToLightDirVec = normalize(vs_position - lightPos0); // Vec from any pos to the light source
    vec3 diffuseColor = vec3(1.0f, 1.0f, 1.0f);
    // Dot product between the normal and pos to light vec will give the angle or how far away the pos is from the light which decides how much the light is dimmed. A point directly under the light source will have little dim, but a point far away will be more dimmed Clamp to not get negative numbers of the dot product.
    float diffuse = clamp(dot(posToLightDirVec, vs_normal), 0, 1); 
    vec3 diffuseFinal = diffuseColor * diffuse; // Add color to the diffuse light

    // Specular light
    vec3 lightToPosDirVec = normalize(lightPos0 - vs_position); // dirVec from pos to light
    // Reflect is the equal angle on the opposite. Dir vec from normal to the reflective angle
    vec3 reflectiveDirVec = normalize(reflect(lightToPosDirVec, normalize(vs_normal)));
    vec3 posToViewDirVec = normalize(vs_position - cameraPos);
    float specularConstant = pow(max(dot(posToViewDirVec, reflectiveDirVec), 0), 30); // The specular value
    vec3 specularFinal = vec3(1.0f, 1.0f, 1.0f) * specularConstant; // Add color

    // Attenuation

    // Final light
    fs_color = texture(texture0, vs_texcoord) *
        vec4(vs_color, 1.0f) *
        (vec4(ambientLight, 1.0f) + vec4(diffuseFinal, 1.0f) + vec4(specularFinal, 1.0f));
}


