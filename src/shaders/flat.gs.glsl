#version 450 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in ATTRIB_OUT {
	vec3 position;
	vec2 texCoords;
} attribIn[];

out ATTRIB_VS_OUT {
	vec2 texCoords;
	vec3 emission;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
} attribOut;

uniform mat4 view;
uniform mat4 modelView;
uniform mat3 normalMatrix;

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
	vec3 vertex0 = attribIn[0].position;
	vec3 vertex1 = attribIn[1].position;
	vec3 vertex2 = attribIn[2].position;
	vec3 side1 = vertex1 - vertex0;
	vec3 side2 = vertex2 - vertex0;
	vec3 normal = cross(side1, side2);

	vec3 faceCenterPos = (vertex0 + vertex1 + vertex2) / 3.0f;

	vec3 n = normalize(normalMatrix * normal);

	vec4 viewPos = modelView * vec4(faceCenterPos, 1.0f);
	vec3 obsPos = normalize(-viewPos.xyz);

	vec3 emission = mat.emission;
	vec3 ambient = lightModelAmbient * mat.ambient;
	vec3 diffuse = vec3(0.0f);
	vec3 specular = vec3(0.0f);

	for (int i = 0; i < 3; ++i) {
		vec3 lightDir = normalize(( view * vec4(lights[i].position, 1.0f)).xyz - viewPos.xyz);
		vec3 spotDir = normalize(mat3(view) * -lights[i].spotDirection);
		float spotFactor = max(int(!useSpotlight), computeSpot(spotDir, lightDir, n));

		vec3 lightDiffuse;
		vec3 lightSpecular;
		computeLight(i, n, lightDir, obsPos, lightDiffuse, lightSpecular);
		ambient += lights[i].ambient * mat.ambient;
		diffuse += lightDiffuse * spotFactor;
		specular += lightSpecular * spotFactor;
	}

	for (int i = 0; i < gl_in.length(); ++i) {
		gl_Position = gl_in[i].gl_Position;
		attribOut.texCoords = attribIn[i].texCoords;
		attribOut.emission = emission;
		attribOut.ambient = ambient;
		attribOut.diffuse = diffuse;
		attribOut.specular = specular;
		EmitVertex();
	}
	EndPrimitive();
}
