#version 330 core

out vec4 color;

in VS_OUT
{
	vec3 position;
	vec2 uv;
	vec3 cameraPos;
	mat3 TBN;
} fs_in;

struct Material
{
	sampler2D texture_diffuse;
	sampler2D texture_specular;
	sampler2D texture_reflection;
	sampler2D texture_normal;
	float shininess;
};

struct DirLight
{
	//light direction
	vec3 direction;
	
	//light color
	vec3 color;
	//intensities
	float ambient;
	float diffuse;
	float specular;
};


struct PointLight
{
	//position of the light
	vec3 position;
	//light color
	vec3 color;
	//intensities
	float ambient;
	float diffuse;
	float specular;
	
	//attenuation
	float constant;
    float linear;
    float quadratic;
};

struct	SpotLight
{
	//transform
	vec3 position;
	vec3 direction;
	
	//cone values
	float cutOff;
	float outerCutOff;
	
	//light color
	vec3 color;
	
	//intensities
	float ambient;
	float diffuse;
	float specular;
	
	//attenuation
	float constant;
	float linear;
	float quadratic;
};

const int POINT_LIGHT_NUMBER = 1;
const int DIR_LIGHT_NUMBER 	 = 1;
const int SPOT_LIGHT_NUMBER  = 1;
const float PI = 3.14159265f;

uniform Material   material;
uniform PointLight pointLights[POINT_LIGHT_NUMBER];
uniform DirLight   dirLights[DIR_LIGHT_NUMBER];
uniform SpotLight  spotLights[SPOT_LIGHT_NUMBER];

vec3 CalcPointLight(PointLight _light, vec3 _normal, vec3 _fragPos, vec3 _viewDir, vec3 _textureColor, vec3 _specularColor);
vec3 CalcDirLight(DirLight _light, vec3 _normal, vec3 _viewDir, vec3 _textureColor, vec3 _specularColor);
vec3 CalcSpotLight(SpotLight _light, vec3 _normal, vec3 _fragPos, vec3 _viewDir, vec3 _textureColor, vec3 _specularColor);

void main() 
{	
	// Obtain normal from normal map in range [0;1]
	vec3 normal = texture(material.texture_normal, fs_in.uv).rgb;
	//Transform the normal vector to range [-1;1].
	normal = normalize(normal * 2.0 - 1.0); //< the normal in tangent space
	//Use the TBN matrix to transform the tangent space normal to world space
	normal = normalize(fs_in.TBN * normal);
	
	vec3 viewDir = normalize(fs_in.cameraPos - fs_in.position);
	
	vec3 textureColor = texture(material.texture_diffuse, fs_in.uv).rgb;
	vec3 specularColor = texture(material.texture_specular, fs_in.uv).rgb;
	
	vec3 result = vec3(0.0f, 0.0f, 0.0f);
	
	//Calc all the directional lights
	for(int i = 0; i < DIR_LIGHT_NUMBER; ++i)
	{
		result += CalcDirLight(dirLights[i], normal, viewDir, textureColor, specularColor);
	}
	
	//Calc all the point lights
	for(int i = 0; i < POINT_LIGHT_NUMBER; ++i)
	{
		result += CalcPointLight(pointLights[i], normal, fs_in.position, viewDir, textureColor, specularColor);
	}
	
	//Calc all the spot lights
	for(int i = 0; i < SPOT_LIGHT_NUMBER; ++i)
	{
		result += CalcSpotLight(spotLights[i], normal, fs_in.position, viewDir, textureColor, specularColor);
	}
	
	color = vec4(result, 1.0f);
}

vec3 CalcPointLight(PointLight _light, vec3 _normal, vec3 _fragPos, vec3 _viewDir, vec3 _textureColor, vec3 _specularColor)
{
	vec3 lightDir = normalize(_light.position - _fragPos);

	//diffuse
	float diff = max(dot(_normal, lightDir), 0.0f);
	
	float spec = 0.0f;
	
	float energyConservation = (8.0f + material.shininess) / (8.0f * PI); 
	vec3 halfwayDir = normalize(lightDir + _viewDir);
	spec = energyConservation * pow(max(dot(_normal, halfwayDir), 0.0f), material.shininess); 
	
	//attenuation
	float rdistance = length(_light.position - _fragPos);
	float attenuation = 1.0f / (_light.constant + _light.linear * rdistance + _light.quadratic * (rdistance * rdistance));
	
	//Combine result
	vec3 ambient  = (_light.ambient  * _light.color) * _textureColor;
	vec3 diffuse  = (_light.diffuse  * diff * _light.color) * _textureColor;
	vec3 specular = (_light.specular * spec * _light.color) * _specularColor;
	
	//ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	
	return (ambient + diffuse + specular);
}

vec3 CalcDirLight(DirLight _light, vec3 _normal, vec3 _viewDir, vec3 _textureColor, vec3 _specularColor)
{
	vec3 lightDir = normalize(-_light.direction);
	
	//diffuse shading
	float diff = max(dot(_normal, lightDir), 0.0f);
	
	//specular shading
	float spec = 0.0f;
	
	float energyConservation = (8.0f + material.shininess) / (8.0f * PI); 
	vec3 halfwayDir = normalize(lightDir + _viewDir);
	spec = energyConservation * pow(max(dot(_normal, halfwayDir), 0.0f), material.shininess); 
	
	//Combine result
	vec3 ambient  = (_light.ambient  * _light.color) * _textureColor;
	vec3 diffuse  = (_light.diffuse  * diff * _light.color) * _textureColor;
	vec3 specular = (_light.specular * spec * _light.color) * _specularColor;
	
	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight _light, vec3 _normal, vec3 _fragPos, vec3 _viewDir, vec3 _textureColor, vec3 _specularColor)
{
	vec3 lightDir = normalize(_light.position - _fragPos);
	
	//diffuse shading
	float diff = max(dot(_normal, lightDir), 0.0f);
	
	//specular shading
	float spec = 0.0f;
	
	float energyConservation = (8.0f + material.shininess) / (8.0f * PI); 
	vec3 halfwayDir = normalize(lightDir + _viewDir);
	spec = energyConservation * pow(max(dot(_normal, halfwayDir), 0.0f), material.shininess); 
	
	// Attenuation
	float rdistance = length(_light.position - _fragPos);
	float attenuation = 1.0f / (_light.constant + _light.linear * rdistance + _light.quadratic * (rdistance * rdistance));
	
	// SpotLight intensity
	float theta = dot(lightDir, normalize(-_light.direction));
	float epsilon = _light.cutOff - _light.outerCutOff;
	float intensity = clamp((theta - _light.outerCutOff) / epsilon, 0.0f, 1.0f);
	
	//Combine result
	vec3 ambient  = (_light.ambient  * _light.color) * _textureColor;
	vec3 diffuse  = (_light.diffuse  * diff * _light.color) * _textureColor;
	vec3 specular = (_light.specular * spec * _light.color) * _specularColor;
	
	//Apply attenuation and intensity to the shading
	// ambient  *= attenuation * intensity; 
	diffuse  *= attenuation * intensity;
	specular *= attenuation * intensity;
	
	//Combine results
	return (diffuse + ambient + specular);
}