#version 330 core

 //    IN
in vec3 crntPos;
in vec3 Normal;
in vec3 color;
in vec2 texCoord;


 //    UNIFORM
uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;


 //    OUT
out vec4 FragColor;


 // FUNCTIONS
vec4 pointLight()
{
	vec3 lightVec = lightPos - crntPos;

	//                  Air Density Implementation               
	//                (1 / (a * x * x + b * x + 1))              
	float dist = length(lightVec);
	float a = 1.0;
	float b = 0.07;
	float intensity = 1.0f / (a * dist * dist + b * dist + 1);

	//   AMBIENT LIGHTING
	float ambient = 0.2f;

	//   DIFFUSE LIGHTING
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	//   SPECULAR LIGHTING
	float specularLight = 0.5f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
	float specular = specAmount * specularLight;

	return (texture(diffuse0, texCoord) * (diffuse * intensity + ambient) + texture(specular0, texCoord).r * specular * intensity) * lightColor;
}

vec4 directLight()
{
	float ambient = 0.2f;

	//   DIFFUSE LIGHTING
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	//   SPECULAR LIGHTING
	float specularLight = 0.5f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
	float specular = specAmount * specularLight;

	return (texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular) * lightColor;
}

vec4 spotLight()
{
	// Controls the size of Spot Light
	float outerCone = 0.9f;
	float innerCone = 0.95f;

	// AMBIENT LIGHTING
	float ambient = 0.2f;

	//   DIFFUSE LIGHTING
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	//   SPECULAR LIGHTING
	float specularLight = 0.5f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
	float specular = specAmount * specularLight;

	//   SPOT LIGHTING
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float intensity = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	return (texture(diffuse0, texCoord) * (diffuse * intensity + ambient) + texture(specular0, texCoord).r * specular * intensity) * lightColor;
}

float near = 0.1f;
float far = 100.0f;

float linearizeDepth(float depth)
{
	return (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));
}

float logisticDepth(float depth, float steepness = 0.5f, float offset = 10.0f)
{
	float zVal = linearizeDepth(depth);
	return (1 / (1 + exp(-steepness * (zVal - offset))));
}

void main()
{
	//float depth = logisticDepth(gl_FragCoord.z);
	//FragColor = directLight() * (1.0f - depth) + vec4(depth * vec3(0.85f, 0.85f, 0.9f), 1.0f);
	FragColor = directLight();
}