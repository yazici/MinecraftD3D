#include "Terrain.h"
#include "DirectXManager.h"

Terrain::Terrain() : GameObject()
{
}

Terrain::Terrain(const Terrain& other)
{
}

Terrain::~Terrain()
{
}

bool Terrain::Initialise(Rect3D terrainSize, TerrainType type, string textureFilename, string normalTextureFilename, Vector2 textureRepeat, float terrainScale, float terrainSmoothing)
{
	TerrainFactory TerrainFactory;
	int terrainSeed;

	//==============
	// Create Model
	//==============

	Model_ = new Model;
	if (!Model_)
	{
		return false;
	}

	// Create Terrain
	if (type == TerrainType::FLAT)
	{
		TerrainFactory.CreateTerrain(terrainSize, textureRepeat, terrainScale, *Model_);
	}
	else if (type == TerrainType::PERLIN)
	{
		terrainSeed = rand() % 100000;

		TerrainFactory.CreateTerrainWithPerlinNoise(terrainSize, textureRepeat, terrainScale, terrainSmoothing * 15, *Model_, terrainSeed);
	}

	//=================
	// Create Material
	//=================

	Material* newMaterial = new Material;
	Result_ = newMaterial->SetBaseTexture(textureFilename);
	if (!Result_)
	{
		return false;
	}

	Result_ = newMaterial->SetNormalTexture(normalTextureFilename);
	if (!Result_)
	{
		return false;
	}

	Model_->AddMaterial(newMaterial);

	//==================
	// Create Transform
	//==================

	Transform_ = new Transform;
	if (!Transform_)
	{
		return false;
	}

	//=================
	// Initialise Vars
	//=================

	Frame_ = 0;
	IsReflectable_ = true;
	IsActive_ = true;

	return true;
}