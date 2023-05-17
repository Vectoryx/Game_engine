/*#version 450 core

layout(location = 0) out vec4 color;

layout(location = 0) in vec2 v_TexCoord;
layout(location = 1) in vec3 f_position;
layout(location = 2) in vec3 v_normal;
layout(location = 3) in vec4 v_color;

uniform vec3 u_LightColor;

uniform sampler2D u_Texture;
uniform vec3 u_LightPos;
uniform vec3 u_CameraPos;

void main() {

	// lightning

	// ambient
	vec3 ambient = u_LightColor * 0.2;

	// diffuse
	vec3 normalized = normalize(v_normal);
	vec3 lightDir = normalize(u_LightPos - f_position);
	float diffLight = max(dot(normalized, lightDir), 0.0);
	vec3 diffuse = diffLight * u_LightColor;

	// specular
	vec3 viewDir = normalize(u_CameraPos - f_position);
	vec3 reflectDir = reflect(-lightDir, normalized);
	float specLight = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = 0.5 * specLight * u_LightColor;

	vec4 texColor = texture(u_Texture, v_TexCoord);

	// combine vertex color, texture color, and lightning
	color = v_color * texColor * vec4(ambient + diffuse + specular, 1.0);
};*/
#version 450 core

layout(location = 0) in vec2 v_TexCoord;
layout(location = 1) in vec3 f_position;
layout(location = 2) in vec3 v_normal;

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

uniform DirLight dirLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

struct Material {
    float shininess;
}; 
  
uniform Material material;
uniform sampler2D texture0;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
    // properties
    vec3 norm = normalize(v_normal);
    vec3 viewDir = normalize(viewPos - f_position);

    // phase 1: Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // phase 2: Point lights
    
    FragColor = texture(texture0, v_TexCoord) * vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = (max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(texture0, v_TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture0, v_TexCoord));
    vec3 specular = light.specular * spec * vec3(1.0f, 0.0f, 0.0f);
    return (ambient + diffuse + specular);
} 
