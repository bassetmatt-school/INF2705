#include "scenes.hpp"

#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui/imgui.h"

#include "utils.hpp"

TesselationScene::TesselationScene(Resources& resources)
	: Scene(resources)
	, m_viewWireframe(0),
	m_tessData(nullptr, 4 * sizeof(GLfloat)) {
	m_tessParams ={
		.minTess = 1.0f,
		.maxTess = 256.0f,
		.minDist= 10.0f,
		.maxDist= 100.0f
	};
	m_res.tessellation.setUniformBlockBinding("TessellationData", 1);
	m_tessData.setBindingIndex(1);
	m_tessData.updateData(&m_tessParams, 0, sizeof(m_tessParams));
	glPatchParameteri(GL_PATCH_VERTICES, 4);
}

TesselationScene::~TesselationScene() {

}

void TesselationScene::render(glm::mat4& view, glm::mat4& projPersp) {
	drawMenu();
	m_tessData.updateData(&m_tessParams, 0, sizeof(m_tessParams));

	glm::mat4 mvp;
	glm::mat4 projView = projPersp * view;
	glm::mat4 modelView;

	m_res.tessellation.use();
	m_res.heightmapTexture.use(0);
	m_res.groundTexture.use(1);
	m_res.sandTexture.use(2);
	m_res.snowTexture.use(3);

	mvp = projView;
	glUniformMatrix4fv(m_res.mvpLocationTessellation, 1, GL_FALSE, &mvp[0][0]);
	modelView = view;
	glUniformMatrix4fv(m_res.modelViewLocationTessellation, 1, GL_FALSE, &modelView[0][0]);

	glUniform1i(m_res.viewWireframeLocationTessellation, m_viewWireframe);

	m_res.tesselationPlane.draw(GL_PATCHES, m_res.tesselationPlaneCount);

	mvp = projPersp * glm::mat4(glm::mat3(view));
	drawSky(mvp);
}

void TesselationScene::drawMenu() {
	ImGui::Begin("Tessellation Parameters");

	ImGui::Checkbox("View wireframe?", (bool*) &m_viewWireframe);

	ImGui::DragFloat("Min tess", &m_tessParams.minTess, 0.5f, 1.0f, 32.0f);
	ImGui::DragFloat("Max tess", &m_tessParams.maxTess, 1.0f, 16.0f, 1024.0f);

	ImGui::End();
}


static const unsigned int MAX_N_PARTICULES = 100000;
static Particle particles[MAX_N_PARTICULES] ={ {{0,0,0},{0,0,0},{0,0,0,0}, {0,0}, 0} };

// Automatically enable the attribs of the shader to avoid code redundancy
void enableAttribs(GLuint vao, GLuint vbo, ShaderProgram& shader, bool enable = true) {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	bool verbose = false;
	GLint locs[5] ={
		shader.getAttribLoc("position", verbose),
		shader.getAttribLoc("velocity", verbose),
		shader.getAttribLoc("color", verbose),
		shader.getAttribLoc("size", verbose),
		shader.getAttribLoc("timeToLive", verbose),
	};
	if (enable) glEnableVertexAttribArray(locs[0]);
	glVertexAttribPointer(locs[0], 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*) 0);
	if (enable) glEnableVertexAttribArray(locs[1]);
	glVertexAttribPointer(locs[1], 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*) 12);
	if (enable) glEnableVertexAttribArray(locs[2]);
	glVertexAttribPointer(locs[2], 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*) 24);
	if (enable) glEnableVertexAttribArray(locs[3]);
	glVertexAttribPointer(locs[3], 2, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*) 40);
	if (enable) glEnableVertexAttribArray(locs[4]);
	glVertexAttribPointer(locs[4], 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*) 48);
}

ParticleScene::ParticleScene(Resources& resources, Window& w)
	: Scene(resources)
	, m_w(w)
	, m_oldTime(m_w.getTick() / 1000.0f)
	, m_cumulativeTime(0.0f)
	, m_nParticles(1)
	, m_nMaxParticles(MAX_N_PARTICULES) {
	glEnable(GL_PROGRAM_POINT_SIZE);

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(2, m_vbo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles, GL_STATIC_DRAW);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_tfo);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles, GL_STREAM_READ);

	m_res.particule.use();
	enableAttribs(m_vao, m_vbo[0], m_res.particule, true);

	m_res.transformFeedback.use();
	enableAttribs(m_vao, m_vbo[1], m_res.transformFeedback, true);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

ParticleScene::~ParticleScene() {
	glUseProgram(0);
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(2, m_vbo);
	glDeleteBuffers(1, &m_tfo);
}

void ParticleScene::render(glm::mat4& view, glm::mat4& projPersp) {
	glm::mat4 mvp;
	glm::mat4 projView = projPersp * view;
	glm::mat4 modelView;

	float time = m_w.getTick() / 1000.0;
	float dt = time - m_oldTime;
	m_oldTime = time;
	m_cumulativeTime += dt;
	if (dt > 1.0f)
		m_nParticles = 1;

	//////////////////////////
	// Retroaction
	//////////////////////////
	m_res.transformFeedback.use();

	glUniform1f(m_res.timeLocationTransformFeedback, time);
	glUniform1f(m_res.dtLocationTransformFeedback, dt);

	glBindVertexArray(m_vao);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_vbo[1]);

	enableAttribs(m_vao, m_vbo[0], m_res.transformFeedback, false);
	glEnable(GL_RASTERIZER_DISCARD);
	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, m_nParticles);
	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);


	glBindVertexArray(0);
	std::swap(m_vbo[0], m_vbo[1]);


	// Draw skybox first, without the function to change some parameter on the depth test.
	glDepthFunc(GL_LEQUAL);
	glDepthMask(0x00);
	m_res.skyboxShader.use();
	m_res.skyboxTexture.use();
	mvp = projPersp * glm::mat4(glm::mat3(view));
	glUniformMatrix4fv(m_res.mvpLocationSky, 1, GL_FALSE, &mvp[0][0]);
	m_res.skybox.draw(GL_TRIANGLES, 6 * 6);
	glDepthMask(0xff);
	glDepthFunc(GL_LESS);


	m_res.particule.use();
	m_res.flameTexture.use();

	modelView = view;
	glUniformMatrix4fv(m_res.modelViewLocationParticle, 1, GL_FALSE, &modelView[0][0]);
	glUniformMatrix4fv(m_res.projectionLocationParticle, 1, GL_FALSE, &projPersp[0][0]);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	enableAttribs(m_vao, m_vbo[0], m_res.particule, false);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// `GL_POINTS` because we specifiy the input of the shaders
	glDrawArrays(GL_POINTS, 0, m_nParticles);
	glBindVertexArray(0);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	if (m_cumulativeTime > 1.0f / 60.0f) {
		m_cumulativeTime = 0.0f;
		// Make the fire grow faster
		if ((m_nParticles += 5) > m_nMaxParticles)
			m_nParticles = m_nMaxParticles;
	}
}
