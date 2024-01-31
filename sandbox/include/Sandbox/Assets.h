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
	class OpaqueAsset
	{
	public:
		virtual ~OpaqueAsset() {};
		int32_t GetType()
		{
			return m_type;
		}
	protected:
		int32_t m_type;
	};

	template<class T>
	class Asset : public OpaqueAsset
	{
	public:
		template<class... Args>
		Asset(Args&&... args)
		{
			m_ptr = makesptr<T>(args...);
			m_type = TypeId::GetId<T>();
		}
		template <class T>
		Asset(const Asset<T>& asset) : m_ptr(asset.m_ptr)
		{

		}
		void Copy(const Asset<T>& asset)
		{
			m_ptr = asset.m_ptr;
		}
		Asset()
		{

		}
		sptr<T> Ptr()
		{
			return m_ptr;
		}
	private:
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

		void LoadAssets();
		void InitAddAssetFunctions();
		void AddAsset(String path);
		void CompileShaders();

		void CreateDefaultTextureSettingsFile(String path);
		void AddTexture(String filename, String path);
		void AddConfig(String filename, String path);
		void AddSprites(String filename, String path);
		void AddFragmentShader(String filename, String path);
		void AddVertexShader(String filename, String path);
		void AddMaterial(String filename, String path);

		template <class T>
		Asset<T> GetPrivate(String name)
		{
			auto find_it = m_assets.find(name);

			ASSERT_LOG_ERROR((bool)(find_it != m_assets.end()), "Cannot find asset, " + name)
				ASSERT_LOG_ERROR((bool)(find_it->second->GetType() == TypeId::GetId<T>()), "Getting wrong asset type, " + name);

			auto ptr = static_pointer_cast<Asset<T>>(find_it->second);
			Asset<T> asset;
			asset.Copy(*ptr);
			return asset;
		}

		std::unordered_map<String, sptr<OpaqueAsset>> m_assets;
		std::unordered_map<String, Delegate<void, String, String>> m_addAssetFunctions;
		std::unordered_map<String, std::pair<String, String>> m_shadersPath;
		TextureImportSettings m_defaultImportSettings;
	};
}
