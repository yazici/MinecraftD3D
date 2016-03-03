#pragma once

#include <d3d11.h>
#include <d3dx10math.h>
#include <vector>

#include "Primitive.h"
#include "Material.h"
#include "Texture.h"

using namespace std;

class Water : public Primitive
{
public:
	Water();
	Water(const Water&);
	~Water();

	// Initialising
	bool Initialise(WCHAR* textureFilename, ObjSize WaterResolution);

	// Shutdown
	void Shutdown();

	// Frame
	bool Frame();

	// Setter
	void ToggleTime(bool);

	// Getters
	float GetTessellation();
	float GetWaterHeight();
	D3DXVECTOR2 GetNormalMapTiling();
	float GetWaterTimer();
	float GetWaveHeight();
	float GetReflectRefractScale();
	D3DXVECTOR4 GetRefractionTint();
	float GetSpecularShininess();

private:
	// Properties
	D3DXVECTOR2 NormalMapTiling_;
	D3DXVECTOR4 RefractionTint_;

	float Tessellation_;
	float WaterHeight_;
	float ReflectRefractScale_;
	float WaterShininess_;
	float WaterTimer_;
	float WaveSpeed_;
	float WaveHeight_;

	// Buffers
	ID3D11Buffer* VertexBuffer_;
	ID3D11Buffer* IndexBuffer_;
};


