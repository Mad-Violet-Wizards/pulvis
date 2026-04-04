#include "GLTileRenderer.hpp"

#include <glad/glad.h>

#include "assets/AssetRegistry.hpp"
#include "assets/AssetEntry.hpp"
#include "FilePath.hpp"
#include "FileTypes.hpp"
#include "Logger.hpp"

namespace pulvis::rendering::gl
{
	CGLTileRenderer::CGLTileRenderer(CGLRenderDevice& _render_device, pulvis::fs::assets::CAssetRegistry& _assets_registry)
		: m_Device(_render_device)
		, m_AssetsRegistry(_assets_registry)
		, m_Shader()
		, m_Initialized(false)
	{
	}

	CGLTileRenderer::~CGLTileRenderer()
	{
		Shutdown();
	}

	void CGLTileRenderer::Initialize()
	{
		if (m_Initialized)
		{
			return;
		}

		if (!LoadAndCompileShader("engine/shaders/tilemap.vert", "engine/shaders/tilemap.frag"))
		{
			PULVIS_ERROR_LOG("GLTileRenderer: Shader invalid, initialization failed.");
			return;
		}

		if (!m_Shader.IsValid())
		{
			PULVIS_ERROR_LOG("GLTileRenderer: Shader invalid, initialization failed.");
		}

		glGenVertexArrays(1, &m_Vao);
		glBindVertexArray(m_Vao);

		m_Initialized = true;
	}

	void CGLTileRenderer::Shutdown()
	{
		if (!m_Initialized || m_Vao == 0)
		{
			return;
		}

		glDeleteVertexArrays(1, &m_Vao);
		m_Vao = 0;
		m_Initialized = false;
	}

	void CGLTileRenderer::Draw(const std::vector<STileDrawCmd>& _batches,
		const CCamera2D& _camera,
		const SFrameRenderState& _frame_state)
	{
		if (!m_Initialized)
		{
			Initialize();
		}

		const glm::mat4 vp = _camera.GetViewProjMatrix();

		m_Shader.Use();
		m_Shader.SetMat4("uMVP", vp);

		glBindVertexArray(m_Vao);

		for (const STileDrawCmd& batch : _batches)
		{
			if (batch.VboID == 0 || batch.VertexCount == 0)
			{
				continue;
			}

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, batch.TextureID);
			m_Shader.SetInt("uTexture", 0);
			m_Shader.SetVec2("uAtlasSize", batch.TextureSize);

			glm::vec4 batch_ambient = batch.AmbientColor * _frame_state.AmbientLight;
			m_Shader.SetVec4("uAmbientColor", batch_ambient);

			float anim_time = (batch.AnimationTime > 0.f) ? batch.AnimationTime : _frame_state.AnimationTime;
			m_Shader.SetFloat("uTime", anim_time);

			glBindBuffer(GL_ARRAY_BUFFER, batch.VboID);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(STileVertex), (void*)0);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(STileVertex),
				(void*)offsetof(STileVertex, TextureCoordinates));

			glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(batch.VertexCount));
		}

		glBindVertexArray(0);
	}

	bool CGLTileRenderer::LoadAndCompileShader(const std::string& _vert_path, const std::string& _frag_path)
	{
		using namespace pulvis::fs;
		using namespace pulvis::fs::assets;

		auto load_source = [&](const std::string& _virtual_path) -> std::string
			{
				SAssetHandle handle = m_AssetsRegistry.Find(_virtual_path);

				if (handle == INVALID_ASSET_HANDLE)
				{
					handle = m_AssetsRegistry.RegisterAsset(EDomain::Engine, _virtual_path, EAssetType::Shader);

					if (!m_AssetsRegistry.LoadAndProcess(handle))
					{
						PULVIS_ERROR_LOG("GLTileRenderer: Failed to load shader '{}'.", _virtual_path);
						return {};
					}
				}

				const SAssetEntry* entry = m_AssetsRegistry.Get(handle);
				if (!entry || entry->RawData.Empty())
				{
					PULVIS_ERROR_LOG("GLTileRenderer: Shader '{}' has no source data.", _virtual_path);
					return {};
				}

				return entry->RawData.ToString();
			};

		const std::string vert_source = load_source(_vert_path);
		const std::string frag_source = load_source(_frag_path);

		if (vert_source.empty() || frag_source.empty())
		{
			return false;
		}

		return m_Shader.CompileFromSource(vert_source.c_str(), frag_source.c_str());
	}
}