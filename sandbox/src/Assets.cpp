#include "pch.h"
#include <stb/stb_image.h>
#include "Sandbox/Assets.h"
#include "Sandbox/Render/Texture.h"
#include "Sandbox/Serialization.h"
#include "Sandbox/Utils/Files.h"
#include "Sandbox/Render/Shader.h"
#include "Sandbox/Render/Sprite.h"

#define TEXTURE_IMPORT_SETTING_IS_ERROR
#define SPRITESHEET_IS_ERROR

namespace Sandbox
{
	void Assets::GenerateSprites(String filename, Serialized& spritesheet, sptr<Texture> texture)
	{
		int width = spritesheet.GetInt("Width");
		int height = spritesheet.GetInt("Height");
		Vec2f origin;
		origin.x = spritesheet.GetArray<float>("Origin")[0];
		origin.y = spritesheet.GetArray<float>("Origin")[1];

		int offsetx = 0;
		int offsety = 0;

		auto texSize = texture->GetSize();

		int columns = texSize.x / width + texSize.x % width;
		int rows = texSize.y / height + texSize.y % height;
		if (texSize.x % width > 0)
		{
			LOG_WARN("Sprite crop size doesn't fit perfectly texture width: " + filename);
		}
		if (texSize.y % height > 0)
		{
			LOG_WARN("Sprite crop size doesn't fit perfectly texture height: " + filename);
		}

		for (int y = 0; y < rows; y++)
		{
			for (int x = 0; x < columns; x++)
			{
				Rect texRect;
				texRect.left = x * width;
				texRect.top = y * height;
				texRect.width = width;
				texRect.height = height;

				sptr<Asset<Sprite>> sprite = MakeAsset<Sprite>(texture, texRect, origin);
				String spriteName = filename + "_" + std::to_string(y) + "_" + std::to_string(x);
				InsertAsset(spriteName, sprite);
			}
		}
	}
	//Serialized DefaultSpriteSheet()
	Serialized Assets::CreateDefaultTextureImportSettings()
	{
		//Create the default texture settings 
		Serialized importSettings = m_defaultImportSettings.Serialize();
		return importSettings;
	}
	Serialized Assets::CreateDefaultSpritesheet(sptr<Texture> texture)
	{
		Serialized spritesheet;

		spritesheet["Width"] = texture->GetSize().x;
		spritesheet["Height"] = texture->GetSize().y;
		spritesheet["Origin"] = { 0.f,0.f };

		return spritesheet;

	}
	void Assets::AddTexture(String filename, String path)
	{
		String settingsPath = path.substr(0, path.find_last_of("/") + 1);
		String nameNoExtension = filename.substr(0, filename.find_last_of("."));
		settingsPath += nameNoExtension + ".texture";

		Serialized settings;

		//Check if we have a texture setting file
		if (!std::filesystem::exists(settingsPath))
		{
			LOG_INFO("Creating default texture settings for " + filename);
			//Create it if not
			settings["ImportSettings"] = CreateDefaultTextureImportSettings();
			settings.WriteOnDisk(settingsPath);
		}
		else
		{
			settings.LoadFromDisk(settingsPath);
		}

		//Load the texture import settings from settings
		Serialized importSettingsCfg = settings.GetObj("ImportSettings");
		TextureImportSettings importSettings(importSettingsCfg);

		//Check if the texture setting is valid (no deserializationError)
		if (importSettings.DeserializationError())
		{
#ifdef TEXTURE_IMPORT_SETTING_IS_ERROR
			ASSERT_LOG_ERROR(false, "Ill formed texture import settings: " + path);
#else
			LOG_WARN("Ill formed texture import settings, creating default texture settings for " + filename);
			importSettingsCfg = CreateDefaultTextureSettings(settingsPath);
			settings["ImportSettings"] = importSettingsCfg;
			settings.WriteOnDisk(settingsPath);
			importSettings.Deserialize(importSettingsCfg);
#endif
			
		}

		//Create the texture with the import settings
		auto texture = MakeAsset<Texture>(path, importSettings);
		InsertAsset(filename, texture);

		//Load sprite sheet from settings
		auto spritesheet = settings.GetObj("Spritesheet");
		if (spritesheet.HadGetError())
		{
			//Texture settings was non existant or ill formed
			spritesheet = CreateDefaultSpritesheet(texture->Ptr());
			settings["Spritesheet"] = spritesheet;
			settings.WriteOnDisk(settingsPath);
		}

		//Create all the sprites asset based on texture and sprite sheet
		GenerateSprites(filename, spritesheet, texture->Ptr());

		//Check if the sprites generated are correct
		if (spritesheet.HadGetError())
		{
#ifdef SPRITESHEET_IS_ERROR
			ASSERT_LOG_ERROR(false, "Ill formed spritesheet: " + path);
#else
			LOG_WARN("Ill formed spritesheet, creating default spritesheet for " + filename);
			spritesheet = CreateDefaultSpritesheet(texture->Ptr());
			settings["Spritesheet"] = spritesheet;
			settings.WriteOnDisk();
#endif

		}
	}
	void Assets::AddConfig(String filename, String path)
	{
		InsertAsset(filename, MakeAsset<Serialized>(path));
	}
	void Assets::AddFragmentShader(String filename, String path)
	{
		auto length = filename.size() - (filename.size() - filename.find_last_of("."));
		String shadername = filename.substr(0, length);
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
			auto shaderAsset = MakeAsset<Shader>(vertSrc, fragSrc);
			String path = vertPath;
			size_t i = path.find_last_of("/");
			size_t j = path.find_last_of(".");
			size_t s = path.size();
			String assetName = path.substr(i + 1, (s - i) - (s - j)) + "shader";
			InsertAsset(assetName, shaderAsset);
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
	}
}