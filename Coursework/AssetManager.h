#pragma once

#include <string>
#include <map>

#include "Singleton.h"
#include "AudioClip.h"
#include "Font.h"
#include "Model.h"
#include "Texture.h"
#include "OBJLoader.h"

class AssetManager : public Singleton<AssetManager>
{
public:
	AssetManager();
	AssetManager(const AssetManager&);
	~AssetManager();

	void Initialise();
	void Shutdown();

	void LoadAudio(AudioClip** clip, std::string filename, bool is3D = false);
	void LoadFont(Font** font, std::string filename);
	void LoadModel(Model** model, std::string filename);
	void LoadTexture(Texture** texture, std::string filename);

private:
	std::map<std::string, AudioClip*> AudioDatabase_;
	std::map<std::string, Font*> FontDatabase_;
	std::map<std::string, Model*> ModelDatabase_;
	std::map<std::string, Texture*> TextureDatabase_;

	bool Result_;
};