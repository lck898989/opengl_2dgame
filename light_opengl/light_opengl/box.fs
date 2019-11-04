#version 330 core
in vec3 color;
out vec4 fragColor;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
in vec3 fragPos;
in vec3 normal_frag;
in vec2 atextcoord;
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 position;
	vec3 specular;
};
uniform Material material;
uniform Light light;
vec3 diffLight(vec3 normalVector) {
	float diff;
	vec3 lightDir = normalize(lightPos - fragPos);
	diff = max(dot(normalVector,lightDir),0.0);
	vec3 res;
	res = diff * vec3(texture(material.diffuse,atextcoord)) * lightColor * light.diffuse;
	return res;
} 
vec3 mirrorLight(vec3 normalVector) {
	vec3 lightDir = normalize(lightPos - fragPos);
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir,normalVector);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
	return spec * (vec3(texture(material.specular,atextcoord))) * lightColor * light.specular;
}
vec3 ambientLight() {
    vec3 res;
	res = lightColor * light.ambient * vec3(texture(material.diffuse,atextcoord));
	return res;
}
void main() {
	vec3 normalVector = normalize(normal_frag);
	vec3 diff = diffLight(normalVector);
	vec3 mirror = mirrorLight(normalVector);
	vec3 phoneLight = ambientLight() + diff + mirror;
	fragColor = vec4(phoneLight,1.0);
}