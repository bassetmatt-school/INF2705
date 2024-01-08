#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 velocity;
layout (location = 2) in vec4 color;
layout (location = 3) in vec2 size;
layout (location = 4) in float timeToLive;

out vec3 positionMod;
out vec3 velocityMod;
out vec4 colorMod;
out vec2 sizeMod;
out float timeToLiveMod;

uniform float time;
uniform float dt;

uint seed = uint(time * 1000.0) + uint(gl_VertexID);
uint randhash() {// entre  0 et UINT_MAX
	uint i=((seed++)^12345391u)*2654435769u;
	i ^= (i<<6u)^(i>>26u); i *= 2654435769u; i += (i<<5u)^(i>>12u);
	return i;
}

float random() { // entre  0 et 1
	const float UINT_MAX = 4294967295.0;
	return float(randhash()) / UINT_MAX;
}

const float PI = 3.14159265359f;
vec3 randomInCircle(in float radius, in float height) {
	float r = radius * sqrt(random());
	float theta = random() * 2 * PI;
	return vec3(r * cos(theta), height, r * sin(theta));
}

#define U_BUFFER
#ifdef U_BUFFER
layout (std140, binding = 2) uniform ParticleData {
	float MAX_TIME_TO_LIVE;
	float INITIAL_TIME_TO_LIVE_RATIO ;

	float INITIAL_RADIUS;
	float INITIAL_HEIGHT;
	float FINAL_RADIUS;
	float FINAL_HEIGHT;

	float INITIAL_SPEED_MIN;
	float INITIAL_SPEED_MAX;


	vec2 PARTICLE_SIZE;

	float INITIAL_ALPHA;
	float ALPHA;
	vec3 YELLOW_COLOR;
	vec3 ORANGE_COLOR;
	vec3 DARK_RED_COLOR;

	vec3 ACCELERATION;
};

#else

const float MAX_TIME_TO_LIVE = 2.0f;
const float INITIAL_TIME_TO_LIVE_RATIO = 0.85f;

const float INITIAL_RADIUS = 0.2f;
const float INITIAL_HEIGHT = 0.0f;
const float FINAL_RADIUS = 0.5f;
const float FINAL_HEIGHT = 5.0f;

const float INITIAL_SPEED_MIN = 0.5f;
const float INITIAL_SPEED_MAX = 0.6f;


const vec2 PARTICLE_SIZE = vec2(0.04f);

const float INITIAL_ALPHA = 0.0f;
const float ALPHA = 0.1f;
const vec3 YELLOW_COLOR = vec3(1.0f, 0.9f, 0.0f);
const vec3 ORANGE_COLOR = vec3(1.0f, 0.4f, 0.2f);
const vec3 DARK_RED_COLOR = vec3(0.1, 0.0, 0.0);

const vec3 ACCELERATION = vec3(0.0f, 0.1f, 0.0f);
#endif

void init_particle() {
	positionMod = randomInCircle(INITIAL_RADIUS, INITIAL_HEIGHT);

	vec3 vel = normalize(randomInCircle(FINAL_RADIUS, FINAL_HEIGHT));
	float f = INITIAL_SPEED_MIN + (INITIAL_SPEED_MAX - INITIAL_SPEED_MIN) * random();
	velocityMod = vel * f;
	colorMod = vec4(YELLOW_COLOR, INITIAL_ALPHA);
	sizeMod = PARTICLE_SIZE;

	// Gets value in [ratio, 1]
	timeToLiveMod = mix(random(), 1.0, INITIAL_TIME_TO_LIVE_RATIO);
	timeToLiveMod *= MAX_TIME_TO_LIVE;
}

void update_particle() {
	positionMod = position + velocity * dt;
	velocityMod = velocity + ACCELERATION * dt;
	float timeNormalised = 1 - timeToLiveMod / MAX_TIME_TO_LIVE;
	if (timeNormalised < 0.5) {
		// If less than 0.25, full yellow, if more than 0.3, full orange
		float f = smoothstep(0.25f, 0.3f, timeNormalised);
		colorMod.rgb = mix(YELLOW_COLOR, ORANGE_COLOR, f);
	} else {
		float f = smoothstep(0.5, 1.0f, timeNormalised);
		colorMod.rgb = mix(ORANGE_COLOR, DARK_RED_COLOR, f);
	}
	colorMod.a = ALPHA * smoothstep(0.0f, 0.2f, timeNormalised) * (1 - smoothstep(0.8f, 1.0f, timeNormalised));
	sizeMod = PARTICLE_SIZE;
	sizeMod *= 1 + 0.5 * smoothstep(0.5f, 1.0f, timeNormalised);
}

void main() {
	timeToLiveMod = timeToLive - dt;
	if ( timeToLiveMod < 0.0f) {
		init_particle();
	} else {
		update_particle();
	}
}
