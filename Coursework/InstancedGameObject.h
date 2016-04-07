#pragma once

#include "GameObject.h"

struct InstanceData
{
	D3DXVECTOR3 position;
	D3DXVECTOR2 textureOffset;
};

class InstancedGameObject : public GameObject
{
public:
	InstancedGameObject();
	~InstancedGameObject();

	void AddInstance(D3DXVECTOR3 position, D3DXVECTOR2 textureOffset = D3DXVECTOR2(1, 1));
	void ClearInstances();

	void RebuildInstanceBuffer();

	int GetInstanceCount();
	ID3D11Buffer* GetInstanceBuffer();

protected:
	bool SendModelToPipeline(Mesh3D* objMesh);

	ID3D11Buffer* InstanceBuffer_;
	int InstanceCount_;
	std::vector<InstanceData> Instances_;
};

