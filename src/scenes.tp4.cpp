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
	, m_viewWireframe(0) {
	glPatchParameteri(GL_PATCH_VERTICES, 4);
}

TesselationScene::~TesselationScene() {

}

void TesselationScene::render(glm::mat4& view, glm::mat4& projPersp) {
	drawMenu();

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

	// TODO: To remove, only for debug
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//GL_FILL

	// TODO
	m_res.tesselationPlane.draw(GL_PATCHES, m_res.tesselationPlaneCount);

	mvp = projPersp * glm::mat4(glm::mat3(view));
	drawSky(mvp);
}

void TesselationScene::drawMenu() {
	ImGui::Begin("Tessellation Parameters");

	ImGui::Checkbox("View wireframe?", (bool*) &m_viewWireframe);

	ImGui::End();
}


static const unsigned int MAX_N_PARTICULES = 10000;
// TODO: Reset
static Particle particles[MAX_N_PARTICULES] ={ {{0,1,0},{0,0,0},{1,1,0,0}, {5,5},0} };

ParticleScene::ParticleScene(Resources& resources, Window& w)
	: Scene(resources)
	, m_w(w)
	, m_oldTime(m_w.getTick() / 1000.0f)
	, m_cumulativeTime(0.0f)
	, m_nParticles(1)
	, m_nMaxParticles(1000) {
	glEnable(GL_PROGRAM_POINT_SIZE);

	// TODO: Check
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_tfo);
	glGenBuffers(2, m_vbo);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_tfo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particles), NULL, GL_STREAM_READ);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles, GL_STREAM_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<void*>(offsetof(Particle, position)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<void*>(offsetof(Particle, velocity)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<void*>(offsetof(Particle, color)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<void*>(offsetof(Particle, size)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<void*>(offsetof(Particle, timeToLive)));
	glEnableVertexAttribArray(4);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles, GL_STREAM_READ);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<void*>(offsetof(Particle, position)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<void*>(offsetof(Particle, velocity)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<void*>(offsetof(Particle, color)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<void*>(offsetof(Particle, size)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<void*>(offsetof(Particle, timeToLive)));
	glEnableVertexAttribArray(4);

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
	// TODO: Try without

	glUniform1f(m_res.timeLocationTransformFeedback, time);
	glUniform1f(m_res.dtLocationTransformFeedback, dt);

	// glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_tfo);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_vbo[1]);
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<void*>(offsetof(Particle, position)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<void*>(offsetof(Particle, velocity)));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<void*>(offsetof(Particle, color)));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<void*>(offsetof(Particle, size)));
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<void*>(offsetof(Particle, timeToLive)));

	glEnable(GL_RASTERIZER_DISCARD);
	glBeginTransformFeedback(GL_TRIANGLES);
	glDrawArrays(GL_QUADS, 0, sizeof(particles)/sizeof(Particle));
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<void*>(offsetof(Particle, position)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<void*>(offsetof(Particle, velocity)));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<void*>(offsetof(Particle, color)));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<void*>(offsetof(Particle, size)));
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<void*>(offsetof(Particle, timeToLive)));

	// TODO: Draw particles without depth write and with blending
	// glDrawTransformFeedback(GL_TRIANGLES, m_tfo);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(particles)/sizeof(Particle));
	glBindVertexArray(0);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	if (m_cumulativeTime > 1.0f / 60.0f) {
		m_cumulativeTime = 0.0f;
		if (++m_nParticles > m_nMaxParticles)
			m_nParticles = m_nMaxParticles;
	}
}
