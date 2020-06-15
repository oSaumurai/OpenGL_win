#pragma once
#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <memory>
#include <fstream>
#include <strstream>
#include "IndexBuffer.h"

#include "Camera/Camera.h"
#include "controller/Mouse_Controller.h"
#include "controller/KeyboardController.h"
#include "controller/commands/Command_Include.h"

struct vec3d
{
    float x, y, z;
};
struct triangle
{
    vec3d p[3];
};
struct mesh
{
    //std::vector<triangle> tris;
    //std::vector<vec3d> faces;
    std::vector<vec3d> verts;
    std::vector<vec3d> unordered_verts;
    bool LoadFromObjectFile(std::string filename)
    {
        std::ifstream f(filename);
        if (!f.is_open())
            return false;

        while (!f.eof())
        {
            char line[128];
            f.getline(line, 128);

            std::strstream s;
            s << line;

            char junk;
            if (line[0] == 'v')
            {
                vec3d v;
                s >> junk >> v.x >> v.y >> v.z;
                unordered_verts.push_back(v);
            }
            if (line[0] == 'f')
            {
                int f[3];
                s >> junk >> f[0] >> f[1] >> f[2];

                verts.push_back(unordered_verts[f[0] - 1]);
                verts.push_back(unordered_verts[f[1] - 1]);
                verts.push_back(unordered_verts[f[2] - 1]);                
                //tris.push_back({ verts[f[0] - 1],verts[f[1] - 1],verts[f[2] - 1] });
            }
        }

        return true;
    }
};
namespace test {

	class TestTexture3D :public Test
	{
	public:
		TestTexture3D();
		~TestTexture3D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
        //Camera camera;
        MouseController* mouseController;
        KeyboardController* keyboardController;
        Camera* camera;
		std::unique_ptr<VertexArray> m_VAO;
		//std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;

		glm::mat4 m_View, m_Proj;
		glm::vec3 m_Translation;

    private:
        void InitController();
	};
}