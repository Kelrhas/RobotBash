#pragma once


class AssetManager
{
public:
	~AssetManager();

	bool LoadAsset();
	bool UnloadAsset();

private:
	AssetManager();

};

extern AssetManager* g_pAssetManager;
