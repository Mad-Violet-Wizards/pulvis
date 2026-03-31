#include "loaders/TextureLoader.hpp"
#include "assets/payloads/TexturePayload.hpp"
#include "Logger.hpp"

#include <stb/stb_image.h>

namespace pulvis::rendering
{
	CTextureLoader::CTextureLoader(gl::CGLRenderDevice& _device)
		: m_RenderDevice(_device)
	{
	}

	bool CTextureLoader::Process(fs::assets::SAssetEntry& _entry)
	{
		if (_entry.RawData.Empty())
		{
			PULVIS_ERROR_LOG("RawData empty: {}", _entry.VirtualPath);
			return false;
		}

		int w = 0, h = 0, ch = 0;
		unsigned char* pixels = stbi_load_from_memory(
_entry.RawData.Data(),
			static_cast<int>(_entry.RawData.Size()),
			&w, &h, &ch, 0
		);

		if (!pixels)
		{
			PULVIS_ERROR_LOG("stbi decode failed: {}", _entry.VirtualPath);
			return false;
		}

		gl::SGLTextureDesc desc;
		desc.Width = w;
		desc.Height = h;
		desc.Channels = ch;
		desc.GenerateMipmaps = true;

		gl::SGLTextureHandle gpu_handle = m_RenderDevice.CreateTexture2D(desc, pixels);
		stbi_image_free(pixels);

		auto payload = std::make_unique<fs::assets::STexturePayload>();
		payload->GPUHandle = gpu_handle.ID;
		payload->Width = static_cast<uint32_t>(w);
		payload->Height = static_cast<uint32_t>(h);
		payload->Channels = static_cast<uint32_t>(ch);

		_entry.Payload = std::move(payload);
		_entry.RawData.Clear();

		return true;
	}

	bool CTextureLoader::Unload(fs::assets::SAssetEntry& _entry)
	{
		if (!_entry.Payload)
		{
			return false;
		}

		auto* payload = _entry.GetPayload<fs::assets::STexturePayload>();
		if (payload->GPUHandle != 0)
		{
			gl::SGLTextureHandle tex;
			tex.ID = payload->GPUHandle;
			m_RenderDevice.DestroyTexture(tex);
		}

		_entry.Payload = nullptr;
		return true;
	}
}