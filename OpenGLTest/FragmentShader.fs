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

out vec4 FragColor;

in vec3 FragPos;
in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float blend;

uniform vec3 lightPos;  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;
  
uniform Material material;
uniform Light light;

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

    // ambient
    vec3 ambient = light.ambient  * tex;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * tex);
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * texSpecular); 
    
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}