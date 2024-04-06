#version 330 core

struct Material {    
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirectionLight 
{
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight 
{
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

#define NR_POINT_LIGHTS 4  

in vec3 FragPos;
in vec3 Normal;
in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

//Two Texture Blend
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float blend;

//Light
uniform vec3 lightPos;  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;
  
uniform Material material;
uniform Light light;

uniform DirectionLight dirlight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

vec3 CalculateDirectionLights(DirectionLight light, vec3 normal, vec3 viewDir);
vec3 CalculatePointLights(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalculateSpotLights(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    //vec4 tex1 = texture(texture1, TexCoord);
    //vec4 tex2 = texture(texture2, TexCoord);
    //vec4 mat1 = texture(material.diffuse, TexCoord);
    //vec4 texMix = mix(tex1, tex2, blend);
    //texMix = mix(texMix, mat1, 0.5);    
    //vec3 tex = texMix.rgb;
    //------------------------------------------------------

    //vec4 texMix = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), blend);
    vec3 tex = vec3(texture(material.diffuse, TexCoord)); //tex ambient and tex diffuse have same values
    
    vec3 texSpecular = vec3(texture(material.specular, TexCoord));

    //Get Normal
    vec3 norm = normalize(Normal);
    //Get View Direction
    vec3 viewDir =  normalize(viewPos - FragPos);

    //Direction Light
    vec3 result = CalculateDirectionLights(dirlight, norm, viewDir);

    //Point Lights 
    for(int i = 0; i < NR_POINT_LIGHTS; ++i)
    {
        result += CalculatePointLights(pointLights[i],norm, FragPos, viewDir);
    }

    //Spot Light
    result += CalculateSpotLights(spotLight,norm, FragPos, viewDir);

    //// ambient
    //vec3 ambient = light.ambient  * tex;
  	//
    //// diffuse 
    ////vec3 norm = normalize(Normal);
    //vec3 lightDir = normalize(lightPos - FragPos);
    //float diff = max(dot(norm, lightDir), 0.0);
    //vec3 diffuse = light.diffuse * (diff * tex);
    //
    //// specular
    //float specularStrength = 0.5;
    ////vec3 viewDir = normalize(viewPos - FragPos);
    //vec3 reflectDir = reflect(-lightDir, norm);  
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //vec3 specular = light.specular * (spec * texSpecular); 
    //
    ////vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}

vec3 CalculateDirectionLights(DirectionLight light, vec3 normal, vec3 viewDir)
{
    vec3 tex = vec3(texture(material.diffuse, TexCoord)); //tex ambient and tex diffuse have same values
    
    vec3 texSpecular = vec3(texture(material.specular, TexCoord));

    //Set light direction. 
    //Remember to reverse the direction. Users set the direction from the light source. OpenGl reads it the other way
  	vec3 lightDir = normalize(-light.direction); 
    
    //Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);    
    
    //Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        
    //Ambient, diffuse, specular
    vec3 ambient = light.ambient  * tex;
    vec3 diffuse = light.diffuse * (diff * tex);
    vec3 specular = light.specular * (spec * texSpecular); 

    vec3 result = ambient + diffuse + specular;
    return result;
}

vec3 CalculatePointLights(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 tex = vec3(texture(material.diffuse, TexCoord)); //tex ambient and tex diffuse have same values
    
    vec3 texSpecular = vec3(texture(material.specular, TexCoord));
    
    //Get light direction
    vec3 lightDir = normalize(light.position - fragPos);
    
    //Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
        
    //Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    //Attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance)); 
    
    //Ambient, diffuse, specular
    vec3 ambient = light.ambient  * tex;
    vec3 diffuse = light.diffuse * (diff * tex);
    vec3 specular = light.specular * (spec * texSpecular); 

    //Combine attenuation
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;

    return result;
}

vec3 CalculateSpotLights(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 tex = vec3(texture(material.diffuse, TexCoord)); //tex ambient and tex diffuse have same values
    
    vec3 texSpecular = vec3(texture(material.specular, TexCoord));

    //Get light direction
    vec3 lightDir = normalize(light.position - fragPos);

    //Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    //Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    //Attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance)); 

    //Spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    //Ambient, diffuse, specular
    vec3 ambient = light.ambient  * tex;
    vec3 diffuse = light.diffuse * (diff * tex);
    vec3 specular = light.specular * (spec * texSpecular); 

    //Combine attenuation and intensity
    ambient  *= attenuation * intensity;
    diffuse  *= attenuation * intensity;
    specular *= attenuation * intensity;

    vec3 result = ambient + diffuse + specular;
    return result;
}

