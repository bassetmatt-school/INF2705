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

void computeLight(in int lightIndex, in vec3 normal, in vec3 lightDir, in vec3 obsPos, out vec3 diffuseColor, out vec3 specularColor) {
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lights[lightIndex].diffuse * mat.diffuse;

	float spec = useBlinn ?
					dot(normalize( lightDir + obsPos ), normal) :
					dot(reflect(-lightDir, normal), obsPos);
	spec = max(0.0, spec);
	spec = pow(spec, mat.shininess);
	vec3 specular = spec * lights[lightIndex].specular * mat.specular;

	diffuseColor = diffuse;
	specularColor = specular;
}

void main() {
	vec4 diffuseTexel = vec4(1.0);
	vec4 specularTexel = vec4(1.0);
	if (useTexture) {
		diffuseTexel =	texture(diffuseSampler, attribIn.texCoords);
		if (diffuseTexel.a < 0.3) discard;
		specularTexel = texture(specularSampler, attribIn.texCoords);
	}

	vec3 n = normalize(attribIn.normal);
	vec3 op = normalize(attribIn.obsPos);

	vec3 emission = mat.emission;
	vec3 ambient = mat.ambient * lightModelAmbient;
	vec3 diffuse = vec3(0.0f);
	vec3 specular = vec3(0.0f);

	for (int i = 0; i < 3; ++i) {
		vec3 ldir = normalize(attribIn.lightDir[i]);
		vec3 spotDir = normalize(attribIn.spotDir[i]);
		float spotFactor = max(int(!useSpotlight), computeSpot(spotDir, ldir, n));
		ambient += lights[i].ambient * mat.ambient;
		vec3 diffuseColor, specularColor;
		computeLight(i, n, ldir, op, diffuseColor, specularColor);
		diffuse += diffuseColor * spotFactor;
		specular += specularColor * spotFactor;
	}

	vec3 color = emission + min(ambient + diffuse, vec3(1.0f)) * vec3(diffuseTexel) + specular * vec3(specularTexel.x);

	color = clamp(color, vec3(0.0f), vec3(1.0f));
	FragColor = vec4(color, 1.0f);
}
