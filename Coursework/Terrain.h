#pragma once

#include <d3d11.h>
#include <d3dx10math.h>
#include <stdio.h>

#include "GameObject.h"
#include "Texture.h"
#include "TerrainFactory.h"

enum TerrainType { FlatTerrain, PerlinTerrain };

class Terrain : public GameObject
{
public:
	Terrain();
	Terrain(const Terrain&);
	~Terrain();

	// initialise
	bool initialise(Rect3D terrainSize, TerrainType type, string textureFilename, string normalTextureFilename, Vector2 textureRepeat, float terrainScale = 1.0f, float terrainSmoothing = 1.0f);
};


