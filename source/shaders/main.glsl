#shader vertex
#version 410 core

layout(location = 0) in vec3 L_coordinate;
layout(location = 1) in vec3 L_normal;
layout(location = 2) in vec2 L_texCoord;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

out vec3 v_Normal;
out vec2 v_TexCoord;
out vec3 v_FragPos;

void main() {
    mat4 mvp = u_Projection * u_View * u_Model;
    gl_Position = mvp * vec4(L_coordinate, 1.0f);

    v_Normal = mat3(transpose(inverse(u_Model))) * L_normal;
    v_TexCoord = L_texCoord;
    v_FragPos = vec3(u_Model * vec4(L_coordinate, 1.0f));
}

#shader fragment
#version 410 core

struct Material {
    vec3 ambient;
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    float innerCutoff;
    float outerCutoff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_FragPos;

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;

uniform vec3 u_ViewPos;

uniform float u_AlphaCutoff;

vec4 diffTexColor;
vec4 specTexColor;

out vec4 FragColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    // Ambient Lighting
    vec3 ambient = light.ambient * diffTexColor.rgb;

    // Diffuse Lighting
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * diffTexColor.rgb;

    // Specular Lighting
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * specTexColor.rgb;

    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir) {
    // Attenuation
    float distance = length(light.position - v_FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Ambient Lighting
    vec3 ambient = light.ambient * diffTexColor.rgb;

    // Diffuse Lighting
    vec3 lightDir = normalize(light.position - v_FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * diffTexColor.rgb * attenuation;

    // Specular Lighting
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * specTexColor.rgb * attenuation;

    return ambient + diffuse + specular;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - v_FragPos);

    // Calculating Attenuation
    float distance = length(light.position - v_FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Calculating Spotlight Intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.innerCutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0); // ensures intensity is between 0.0 and 1.0

    // Ambient Lighting
    vec3 ambient = light.ambient * diffTexColor.rgb; // ambient lighting should be unaffected by spotLight intensity

    // Diffuse Lighting
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * diffTexColor.rgb * attenuation * intensity;

    // Specular Lighting
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * specTexColor.rgb * attenuation * intensity;

    return ambient + diffuse + specular;
}

void calcTexturesColor() {
    diffTexColor = texture(material.texture_diffuse1, v_TexCoord);
    specTexColor = texture(material.texture_specular1, v_TexCoord);
}

void main() {
    calcTexturesColor();

    vec3 norm = normalize(v_Normal);
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);

    vec3 result = vec3(0.0f);

    // result += CalcDirLight(dirLight, norm, viewDir);
    // result += CalcPointLight(pointLight, norm, viewDir);
    // result += CalcSpotLight(spotLight, norm, viewDir);

    FragColor = vec4(diffTexColor);
}
