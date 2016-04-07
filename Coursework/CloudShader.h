#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class CloudShader : public GameShader
{
public:
	CloudShader();
	~CloudShader();

	bool Initialise(HWND hwnd);

	bool Prepare(Mesh3D* objMesh, Material* objMaterial, Transform* objTransform);
};