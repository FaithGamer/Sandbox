#include "pch.h"
#include <stb/stb_image.h>
#include "Sandbox/Assets.h"
#include "Sandbox/Render/Texture.h"
#include "Sandbox/Json.h"
#include "Sandbox/Utils/Files.h"
#include "Sandbox/Render/Shader.h"

namespace Sandbox
{
	void Assets::CreateDefaultTextureSettingsFile(String path)
	{
		Json m_defaultImportSettings.ToJson();
		LOG_INFO("texture config path: " + path);
	}
	void Assets::AddTexture(String filename, String path)
	{
		String configPath = path.substr(0, path.find_last_of("/")+1);
		String nameNoExtension = filename.substr(0, filename.find_last_of("."));
		configPath += nameNoExtension + ".texture";
	

		if (!std::filesystem::exists(configPath))
		{
			CreateDefaultTextureSettingsFile(configPath);
		}

		m_assets.insert(std::make_pair(filename, makesptr<Asset<Texture>>(path)));
	}
	void Assets::AddConfig(String filename, String path)
	{
		m_assets.insert(std::make_pair(filename, makesptr<Asset<Config>>(path)));
	}
	void Assets::AddFragmentShader(String filename, String path)
	{
		auto length = filename.size() - (filename.size() - filename.find_last_of("."));
		String shadername = filename.substr(0, length);
		LOG_INFO("shadername: " + shadername);
		m_shadersPath[shadername].second = path;
	}
	void Assets::AddVertexShader(String filename, String path)
	{
		auto length = filename.size() - (filename.size() - filename.find_last_of("."));
		String shadername = filename.substr(0, length);
		m_shadersPath[shadername].first = path;
	}

	Assets::Assets()
	{
		stbi_set_flip_vertically_on_load(true);
		InitAddAssetFunctions();
		LoadAssets();
		CompileShaders();
	}

	void Assets::InitAddAssetFunctions()
	{
		m_addAssetFunctions.insert(std::make_pair(".png", Delegate(&Assets::AddTexture, this)));
		m_addAssetFunctions.insert(std::make_pair(".config", Delegate(&Assets::AddConfig, this)));
		m_addAssetFunctions.insert(std::make_pair(".vert", Delegate(&Assets::AddVertexShader, this)));
		m_addAssetFunctions.insert(std::make_pair(".frag", Delegate(&Assets::AddFragmentShader, this)));
		//m_addAssetFunctions.insert(std::make_pair(".spritesheet", &AddSprites));
	}

	void Assets::LoadAssets()
	{
		//Iterate every subfolders in assets and load every file into the asset map
		std::filesystem::path root = "assets/";

		std::list<std::filesystem::path> folders;
		folders.push_front(root);

		while (!folders.empty())
		{
			std::filesystem::directory_iterator folder_it(folders.front());
			while (folder_it != std::filesystem::directory_iterator())
			{
				if (std::filesystem::is_regular_file(*folder_it))
				{
					String path = folder_it->path().generic_string();
					if (path.size() < 1)
					{
						folder_it++;
						continue;
					}
					AddAsset(path);
				}
				else
				{
					folders.push_back(folder_it->path());
				}
				folder_it++;
			}
			folders.pop_front();
		}
	}

	std::string LoadShaderSourceFromFile(std::string path)
	{
		std::ifstream shaderFile(path);
		if (!shaderFile.is_open())
		{
			LOG_ERROR("Unable to open the shader: " + path);
		}
		return IO::IfstreamToString(shaderFile);
	}


	void Assets::CompileShaders()
	{
		LOG_INFO("Compiling shaders...");
		for (auto& shader : m_shadersPath)
		{
			String& vertPath = shader.second.first;
			String& fragPath = shader.second.second;
			if (vertPath == "" || fragPath == "")
				continue;

			String vertSrc = LoadShaderSourceFromFile(vertPath);
			String fragSrc = LoadShaderSourceFromFile(fragPath);
			auto shaderAsset = makesptr<Asset<Shader>>(vertSrc, fragSrc);
			String path = vertPath;
			size_t i = path.find_last_of("/");
			size_t j = path.find_last_of(".");
			size_t s = path.size();
			String assetName = path.substr(i + 1, (s - i) - (s - j)) + "shader";
			m_assets.insert(MakePair(assetName, shaderAsset));
		}
	}

	void Assets::AddAsset(String path)
	{
		size_t i = path.find_last_of(".");
		String extension = path.substr(i, path.size() - i);

		if (extension == ".texture")
			return;

		auto find_it = m_addAssetFunctions.find(extension);

		if (find_it == m_addAssetFunctions.end())
		{
			LOG_WARN("Asset file extension not supported, " + path);
			return;
		}

		String pathtmp = path;
		i = path.find_last_of("/") + 1;
		String filename = path.substr(i, path.size() - i);
		find_it->second.Call(filename, pathtmp);
		LOG_INFO("Asset loaded " + path);

		//m_assets.insert(std::make_pair(filename, opaque));
	}
}