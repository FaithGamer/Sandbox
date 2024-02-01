#pragma once
#include "std_macros.h"
#include "Singleton.h"
#include "TypeId.h"
#include "Sandbox/Log.h"
#include "Sandbox/Delegate.h"
#include "Sandbox/Render/Texture.h"
#include <unordered_map>

namespace Sandbox
{
	class Assets;

	class OpaqueAsset
	{
	public:
		virtual ~OpaqueAsset() {};
		virtual int32_t GetType() = 0;
	protected:
	};

	template<class T>
	class Asset : public OpaqueAsset
	{
	public:
		Asset() = default;

		Asset(const Asset<T>& asset) : m_ptr(asset.m_ptr)
		{

		}
		sptr<T> Ptr()
		{
			return m_ptr;
		}
		int32_t GetType()
		{
			return TypeId::GetId<T>();
		}
	private:

		friend Assets;
		sptr<T> m_ptr;

	};



	class Assets : public Singleton<Assets>
	{
	public:

		Assets();


		void HotReload();

		template <class T>
		static Asset<T> Get(String name)
		{
			return Instance()->GetPrivate<T>(name);
		}


	private:
		friend sptr<Assets> Singleton<Assets>::Instance();
		friend void Singleton<Assets>::Kill();

		template<class T, class... Args>
		static sptr<Asset<T>> MakeAsset(Args&&... args)
		{
			auto asset = makesptr<Asset<T>>();
			asset->m_ptr = makesptr<T>(args...);
			return asset;
		}

		void LoadAssets();
		void InitAddAssetFunctions();
		void AddAsset(String path);
		void CompileShaders();
		template<class T>
		void InsertAsset(String filename, sptr<Asset<T>> asset)
		{
			if (m_assets.find(filename) == m_assets.end())
			{
				m_assets.insert(MakePair(filename, asset));
			}
			else
			{
				LOG_ERROR("More than one asset with the same filename: " + filename + ", only one could be loaded.");
			}
		}

		void GenerateSprites(String filename, Serialized& spritesheet, sptr<Texture> texture);
		Serialized CreateDefaultTextureImportSettings();
		Serialized CreateDefaultSpritesheet(sptr<Texture> texture);
		void AddTexture(String filename, String path);
		void AddConfig(String filename, String path);
		void AddFragmentShader(String filename, String path);
		void AddVertexShader(String filename, String path);
		void AddMaterial(String filename, String path);

		template <class T>
		Asset<T> GetPrivate(String name)
		{
			auto find_it = m_assets.find(name);

			ASSERT_LOG_ERROR((bool)(find_it != m_assets.end()), "Cannot find asset, " + name);
			ASSERT_LOG_ERROR((bool)(find_it->second->GetType() == TypeId::GetId<T>()), "Getting wrong asset type, " + name);

			return *static_pointer_cast<Asset<T>>(find_it->second);

		}

		std::unordered_map<String, sptr<OpaqueAsset>> m_assets;
		std::unordered_map<String, Delegate<void, String, String>> m_addAssetFunctions;
		std::unordered_map<String, std::pair<String, String>> m_shadersPath;
		TextureImportSettings m_defaultImportSettings;
	};
}
