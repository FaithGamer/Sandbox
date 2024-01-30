#include "pch.h"
#include "Sandbox/Assets.h"
#include "Sandbox/Render/Texture.h"
#include "Sandbox/Json.h"

namespace Sandbox
{
	sptr<OpaqueAsset> AddTexture(String path)
	{
		return makesptr<Asset<Texture>>(path);
	}
	sptr<OpaqueAsset> AddConfig(String path)
	{
		return makesptr<Asset<Config>>(path);
	}

	Assets::Assets()
	{
		InitAddAssetFunctions();
		LoadAssets();
	}

	void Assets::InitAddAssetFunctions()
	{
		m_addAssetFunctions.insert(std::make_pair(".png", &AddTexture));
		m_addAssetFunctions.insert(std::make_pair(".config", &AddConfig));
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

	void Assets::AddAsset(String path)
	{
		size_t i = path.find_last_of(".");
		String extension = path.substr(i, path.size() - i);
		auto find_it = m_addAssetFunctions.find(extension);

		if (find_it == m_addAssetFunctions.end())
		{
			LOG_WARN("Asset file extension not supported, " + path);
			return;
		}

		String pathtmp = path;
		auto opaque = find_it->second.Call(pathtmp);
		i = path.find_last_of("/") + 1;
		String filename = path.substr(i, path.size() - i);

		LOG_INFO("Asset loaded: " + filename);
		m_assets.insert(std::make_pair(filename, opaque));
	}
}