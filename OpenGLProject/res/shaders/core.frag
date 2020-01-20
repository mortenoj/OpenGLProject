#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    sampler2D diffuseTex;
    sampler2D specularTex;
};

// Input
in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

// Output
out vec4 fs_color;

// Uniforms
uniform Material material;
uniform vec3 lightPos0;
uniform vec3 cameraPos;

// Functions
vec3 calculateAmbient(Material material) {
    return material.ambient;
}

vec3 calculateDiffuse(Material material, vec3 position, vec3 normal, vec3 lightPosition) {
    vec3 posToLightDirVec = normalize(lightPosition - position); // Vec from any pos to the light source

    // Dot product between the normal and pos to light vec will give the angle or how far away the pos is from the light which decides how much the light is dimmed. A point directly under the light source will have little dim, but a point far away will be more dimmed Clamp to not get negative numbers of the dot product.
    float diffuse = clamp(dot(posToLightDirVec, normal), 0, 1); 
    return material.diffuse * diffuse; // Add color to the diffuse light
}


vec3 calculateSpecular(Material material, vec3 position, vec3 normal, vec3 cameraPosition, vec3 lightPosition) {
    vec3 lightToPosDirVec = normalize(position - lightPosition); // dirVec from pos to light
    // Reflect is the equal angle on the opposite. Dir vec from normal to the reflective angle
    vec3 reflectiveDirVec = normalize(reflect(lightToPosDirVec, normalize(normal)));
    vec3 posToViewDirVec = normalize(cameraPosition - position);
    float specularConstant = pow(max(dot(posToViewDirVec, reflectiveDirVec), 0), 30); // The specular value
    return material.specular * specularConstant; // Add color
}

void main() {
    vec3 ambientFinal = calculateAmbient(material);
    vec3 diffuseFinal = calculateDiffuse(material, vs_position, vs_normal, lightPos0);
    vec3 specularFinal = calculateSpecular(material, vs_position, vs_normal, cameraPos, lightPos0);

    // Attenuation

    // Final light
    fs_color = texture(material.diffuseTex, vs_texcoord) * vec4(vs_color, 1.0f) * (
            vec4(ambientFinal,  1.0f) +
            vec4(diffuseFinal,  1.0f) +
            vec4(specularFinal, 1.0f)
        );
}


