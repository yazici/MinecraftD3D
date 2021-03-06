#include "InstancedGameObject.h"
#include "AssetManager.h"

InstancedGameObject::InstancedGameObject() : GameObject()
{
	m_instanceBuffer = 0;
	m_instanceCount = 0;
}

InstancedGameObject::~InstancedGameObject()
{
}

bool InstancedGameObject::render()
{
	if (!m_isActive || !m_shader || !m_model)
		return true;

	// Define how we want the model to be rendered
	setRenderModes();

	// render Reflection
	if (m_reflective == renderMode::On)
	{
		m_shader->setRenderMode(ProjectionMode::Perspective, ViewMode::Reflection);

		Texture* reflectionTexture;
		AssetManager::getInstance()->loadTexture(&reflectionTexture, "ReflectionTexture");
		reflectionTexture->setRenderTarget();

		renderMeshes();

		DirectXManager::getInstance()->setBackBufferRenderTarget();
		DirectXManager::getInstance()->ResetViewport();
	}

	// Define how we want to see the model
	m_shader->setRenderMode(ProjectionMode::Perspective, ViewMode::View);

	// render to render Texture
	if (m_postprocessing == renderMode::On)
	{
		// get and set render texture
		Texture* renderTexture;
		AssetManager::getInstance()->loadTexture(&renderTexture, "renderTexture");
		renderTexture->setRenderTarget();

		// render the model
		renderMeshes();

		// Reset render Target
		DirectXManager::getInstance()->setBackBufferRenderTarget();
		DirectXManager::getInstance()->ResetViewport();

		// Clean Up
		renderTexture = 0;
	}

	// render Mesh
	renderMeshes();

	// Reset Pipeline settings
	resetRenderModes();

	return true;
}

bool InstancedGameObject::renderMeshes()
{
	// render the model
	for (int i = 0; i < m_model->getMeshCount(); i++)
	{
		// Make sure the mesh is active for it to be rendered
		if (m_model->getMesh(i)->isActive())
		{
			// Send model to pipeline
			sendModelToPipeline(m_model->getMesh(i));

			// Send material to shader
			m_shader->prepare(m_model->getMaterial(i), m_transform);

			// render Object
			m_shader->render(m_model->getMesh(i)->getVertexCount(), m_instanceCount);
		}
	}

	return true;
}

bool InstancedGameObject::sendModelToPipeline(Mesh3D* objMesh)
{
	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];

	// set vertex buffer stride and offset
	strides[0] = sizeof(VertexData);
	strides[1] = sizeof(InstanceData);
	offsets[0] = 0;
	offsets[1] = 0;

	// get data buffers
	bufferPointers[0] = objMesh->getVertexBuffer();
	bufferPointers[1] = m_instanceBuffer;

	// set the vertex buffer to active in the input assembler so it can be rendered
	DirectXManager::getInstance()->getDeviceContext()->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

	// set the type of primitive that should be rendered from this vertex buffer
	DirectXManager::getInstance()->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Clean Up
	bufferPointers[0] = 0;
	bufferPointers[1] = 0;

	return true;
}

void InstancedGameObject::addInstance(InstanceData instanceData)
{
	// Add to list
	m_instances.push_back(instanceData);

	// Store number of instances
	m_instanceCount = m_instances.size();
}

void InstancedGameObject::addInstance(D3DXVECTOR3 position, D3DXVECTOR2 textureOffset, D3DXVECTOR2 textureTotal)
{
	// create getInstance
	InstanceData instanceData;
	instanceData.position = position;
	instanceData.textureOffset = textureOffset;
	instanceData.textureTotal = textureTotal;

	// Add to list
	m_instances.push_back(instanceData);

	// Store number of instances
	m_instanceCount = m_instances.size();
}

void InstancedGameObject::clearInstances()
{
	m_instances.clear();
}

void InstancedGameObject::rebuildInstanceBuffer()
{
	InstanceData* instances = 0;
	D3D11_BUFFER_DESC instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA instanceData;
	HRESULT result;

	// Define how many instances we have
	m_instanceCount = m_instances.size();

	// Make sure we have instances to buffer
	if (m_instanceCount == 0)
	{
		return;
	}

	// Check if we have already created instances
	if (m_instanceBuffer)
	{
		m_instanceBuffer->Release();
		m_instanceBuffer = 0;
	}

	// create the instance array.
	instances = new InstanceData[m_instanceCount];
	if (!instances)
	{
		return;
	}

	for (int i = 0; i < m_instanceCount; i++)
	{
		instances[i] = m_instances[i];
	}

	// set up the description of the instance buffer.
	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(InstanceData) * m_instanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the instance data.
	instanceData.pSysMem = instances;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	// create the instance buffer.
	result = DirectXManager::getInstance()->getDevice()->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer);
	if (FAILED(result))
	{
		return;
	}

	//==========
	// Clean Up
	//==========

	delete[] instances;
	instances = 0;

	m_instances.clear();
}

int InstancedGameObject::getInstanceCount()
{
	return m_instanceCount;
}

ID3D11Buffer* InstancedGameObject::getInstanceBuffer()
{
	return m_instanceBuffer;
}