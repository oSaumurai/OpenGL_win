#pragma once
#include "Test.h"
#include "Camera/Camera.h"
#include "controller/KeyboardController.h"
#include "controller/commands/Command_Include.h"
#include "gameOjbects/Skybox/Skybox.h"
#include "gameOjbects/BlockObject/Sphere.h"
#include "Texture.h"

#include "levelLoader/Model.h"
#include "window/Window.h"

namespace test {
	class PBR_Test: public Test {
	public:
		PBR_Test();
		~PBR_Test();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		//Camera camera;
		KeyboardController* keyboardController;
		Camera* camera;

		unsigned int captureFBO;
		unsigned int captureRBO;
		unsigned int envCubemap;
		unsigned int irradianceMap;
		unsigned int prefilterMap;
		unsigned int brdfLUTTexture;
		unsigned int quadVAO = 0;
		unsigned int quadVBO;

		Shader skybox_shader{ "res/shader/skybox.shader" };
		Shader PBR_shader{ "res/shader/PBR.shader" };
		Shader pbr1_shader{ "res/shader/pbr1.shader" };
		Shader model_shader{ "res/shader/Assimp.shader" };
		Shader irradianceShader{ "res/shader/irradiance.shader" };
		Shader prefilterShader{ "res/shader/prefilter.shader" };
		Shader BRDF_shader{ "res/shader/BRDF.shader" };

		// objects //
		Skybox skybox{ "res/object/skybox/hdri", true };
		Model potion{ "res/object/potion/scene.gltf" };
		Sphere sphere{};

		// Textures //
		//Texture albedo   { "res/object/PBR/albedo.png" };
		//Texture normal   { "res/object/PBR/normal.png" };
		//Texture metallic { "res/object/PBR/metal.png" };
		//Texture roughness{ "res/object/PBR/roughness.png" };
		//Texture ao       { "res/object/PBR/ao.png" };		

		
		Texture albedo   { "res/object/PBR/glazed_terran/Color.png" };
		Texture normal   { "res/object/PBR/glazed_terran/NormalGL.png" };
		Texture metallic { "res/object/PBR/glazed_terran/Displacement.png" };
		Texture roughness{ "res/object/PBR/glazed_terran/Roughness.png" };

		Texture copper_albedo   { "res/object/PBR/alu/basecolor.png" };
		Texture copper_normal   { "res/object/PBR/alu/normal.png" };
		Texture copper_metallic { "res/object/PBR/alu/metallic.png" };
		Texture copper_roughness{ "res/object/PBR/alu/roughness.png" };
		//Texture ao       { "res/object/PBR/glazed_terran/ao.png" };


		float viewAngle = 60.0f;

		glm::mat4 m_View{ glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -6.0f)) }, m_Proj{ glm::perspective(glm::radians(60.f), 16.0f/9.0f, 0.1f, 100.0f) };
		glm::vec3 m_Translation;
		glm::vec3 m_Rotation;
		glm::vec3 light_Pos;

	private:
		void InitController();
		void UnRegisterController();
		void initMap();
		void renderQuad();
	};
}