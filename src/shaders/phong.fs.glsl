#version 450 core

in ATTRIB_VS_OUT {
	vec2 texCoords;
	vec3 normal;
	vec3 lightDir[3];
	vec3 spotDir[3];
	vec3 obsPos;
} attribIn;

struct Material {
	vec3 emission;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct UniversalLight {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
	vec3 spotDirection;
};

layout (std140) uniform LightingBlock {
	Material mat;
	UniversalLight lights[3];
	vec3 lightModelAmbient;
	bool useBlinn;
	bool useSpotlight;
	bool useDirect3D;
	bool useTexture;
	float spotExponent;
	float spotOpeningAngle;
};

uniform sampler2D diffuseSampler;
uniform sampler2D specularSampler;

out vec4 FragColor;

float computeSpot(in vec3 spotDir, in vec3 lightDir, in vec3 normal) {
	float spotFactor = 0.0;
	if (dot(spotDir, normal) >= 0.0) {
		float spotDot = dot(lightDir, spotDir);
      float opening = cos(radians(spotOpeningAngle));

		if (useDirect3D)
			spotFactor = smoothstep(pow(opening, 1.01 + spotExponent/2.0), opening, spotDot);
		else
			spotFactor = spotDot > opening ? pow(spotDot, spotExponent) : 0.0;
	}
	return spotFactor;
}

vec3 computeLight(in int lightIndex, in vec3 normal, in vec3 lightDir, in vec3 obsPos) {
	vec3 color = vec3(0.0);

	// Diffuse texture loading (or not)
	vec3 texDiffuse = useTexture ? texture(diffuseSampler, attribIn.texCoords).rgb : vec3(1.0);

	// Ambiant component
	color += mat.ambient * lights[lightIndex].ambient * texDiffuse;

	// Diffuse component (spotlight or not)
	float LdotN = dot(lightDir, normal);
	if (LdotN > 0.0) {
		float	spot = useSpotlight ?
			computeSpot(attribIn.spotDir[lightIndex], lightDir, normal) :
			1.0f;
		color += spot * mat.diffuse * lights[lightIndex].diffuse * LdotN * texDiffuse;

		// Specular component
		float spec = 0.0;
		if (useBlinn) { // Blinn
			vec3 halfVec = normalize(lightDir + obsPos);
			spec = dot(halfVec, normal);
		} else { // Phong
			vec3 reflectDir = reflect(-lightDir, normal);
			spec = dot(reflectDir, obsPos);
		}

		// No need to take the max between spec and 0.0 since we ignore the negative case
		if (spec > 0) {
			// Specular texture
			float texSpec = useTexture ? texture(specularSampler, attribIn.texCoords).r : 1.0;
			// Apply shininess to lighthen formula below
			spec = pow(spec, mat.shininess);
			// Also multiply by spotlight factor to avoid reflexion outside of spotlight cone
			color += spot * mat.specular * lights[lightIndex].specular * spec * texSpec;
		}
	}

	return color;
}

void main() {
	// Emission
	vec3 color = mat.emission;
	// Ambient
	vec3 texDiffuse = vec3(1.0);
	if (useTexture) {
		vec4 tex = texture(diffuseSampler, attribIn.texCoords);
		// Useless here since the diffuse texture only has 3 channels
		if (tex.a < 0.3) discard;
		texDiffuse = tex.rgb;
	}
	color += mat.ambient * lightModelAmbient * texDiffuse;

	// Normalized after rasterization
	vec3 normal = normalize(attribIn.normal);
	vec3 obsPos = normalize(attribIn.obsPos);

	for (int i = 0; i < 3; ++i) {
		vec3 lightDir = normalize(attribIn.lightDir[i]);
		color += computeLight(i, normal, lightDir, obsPos);
	}

	FragColor = clamp(vec4(color, 1.0), 0.0, 1.0);
}
