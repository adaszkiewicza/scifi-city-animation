#version 330 core

#define POINT_NO 2
#define SPOT_NO 3

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;

    bool hasTexture;
    sampler2D textureID;
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
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Fog {
    bool fogOn;
    float fogIntensity;
    vec3 color;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;
uniform SpotLight spotLight[SPOT_NO];
uniform PointLight pointLight[POINT_NO];
uniform DirLight dirLight;
uniform Material material;
uniform vec3 constColor;
uniform Fog fog;
uniform bool reflection;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
float CalcFogFactor(vec3 fragPos, vec3 cameraPos, float fogIntensity);
 
void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0f);

    for (int i = 0; i < POINT_NO; i++)
        result += CalcPointLight(pointLight[i], norm, FragPos, viewDir);
    for (int i = 0; i < SPOT_NO; i++)
        result += CalcSpotLight(spotLight[i], norm, FragPos, viewDir);

    result += CalcDirLight(dirLight, norm, viewDir);

    if (fog.fogOn) {
        float fogFactor = CalcFogFactor(FragPos, viewPos, fog.fogIntensity);
        result = mix(fog.color, result, fogFactor);
    }

    if (reflection)
        result = mix(vec3(0.0, 0.0, 0.0), result, 0.3);

    FragColor = vec4(result, 1.0);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));    

    vec3 ambient, diffuse, specular, textureVal;

    if (material.hasTexture)
        textureVal = vec3(texture(material.textureID, TexCoords));
    else 
        textureVal = vec3(1.0);

    ambient = light.ambient * material.ambient * textureVal;
    diffuse = light.diffuse * diff * material.diffuse * textureVal;
    specular = light.specular * spec * material.specular;


    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));    

    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient, diffuse, specular, textureVal;

    if (material.hasTexture)
        textureVal = vec3(texture(material.textureID, TexCoords));
    else 
        textureVal = vec3(1.0);

    ambient = light.ambient * material.ambient * textureVal;
    diffuse = light.diffuse * diff * material.diffuse * textureVal;
    specular = light.specular * spec * material.specular;
    

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient, diffuse, specular, textureVal;
    if (material.hasTexture)
        textureVal = vec3(texture(material.textureID, TexCoords));
    else 
        textureVal = vec3(1.0);
        
    ambient = light.ambient * material.ambient * textureVal;
    diffuse = light.diffuse * diff * material.diffuse * textureVal;
    specular = light.specular * spec * material.specular;

    return (ambient + diffuse + specular);
} 

float CalcFogFactor(vec3 fragPos, vec3 cameraPos, float fogIntensity)
{
    if (fogIntensity == 0.0) 
        return 1.0;

    float gradient = (fogIntensity * fogIntensity - 50 * fogIntensity + 60);
    float distance = length(cameraPos - fragPos);
    float fog = exp(-pow((distance / gradient), 4));
    fog = clamp(fog, 0.0, 1.0);
    return fog;
}