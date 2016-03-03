#pragma once

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>

#include "Constants.h"
#include "Singleton.h"
#include "DataStructs.h"

class DirectXManager : public Singleton<DirectXManager>
{
public:
	DirectXManager();
	DirectXManager(const DirectXManager&);
	~DirectXManager();

	// Initialising
	bool Initialise(ScreenResolution WindowDimension, HWND hwnd);

	// Shutdown
	void Shutdown();
	
	// Rendering
	void BeginScene(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
	void EndScene();

	// Rendering Toggles
	void ToggleZBuffer(bool flag);
	void ToggleAlphaBlending(bool flag);
	void ToggleCulling(bool flag);
	void ToggleWireframe(bool flag);

	// Blend States
	void EnableSecondBlendState();
	void EnableFireBlendState();

	// Misc
	void SetBackBufferRenderTarget();
	void ResetViewport();

	// DirectXManager Getters
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	// Matrix Getters
	void GetProjectionMatrix(D3DXMATRIX& ProjectionMatrix);
	void GetWorldMatrix(D3DXMATRIX& WorldMatrix);
	void GetOrthoMatrix(D3DXMATRIX& OrthoMatrix);

	// Wireframe
	bool GetWireframeStatus();

private:
	// Video Card Information
	int VideoCardMemory;
	char VideoCardDescription[128];

	// Viewport
	D3D11_VIEWPORT Viewport_;

	// Matrices
	D3DXMATRIX OrthoMatrix_;
	D3DXMATRIX ProjectionMatrix_;
	D3DXMATRIX WorldMatrix_;
	
	// Blend States
	ID3D11BlendState* AlphaCloudBlendState_;
	ID3D11BlendState* AlphaEnableBlendingState_;
	ID3D11BlendState* AlphaFireBlendState_;
	ID3D11BlendState* NormalBlendingState_;

	// Stencil States
	ID3D11DepthStencilState* DepthDisabledStencilState_;
	ID3D11DepthStencilState* DepthStencilState_;
	ID3D11DepthStencilView* DepthStencilView;

	// Adapters
	ID3D11Device* Device_;
	ID3D11DeviceContext* DeviceContext_;

	// Rasterizer States
	ID3D11RasterizerState* RasterStateNoCulling_;
	ID3D11RasterizerState* RasterState_;

	// Rendering
	ID3D11RenderTargetView* RenderTargetView_;
	ID3D11Texture2D* DepthStencilBuffer_;
	IDXGISwapChain* SwapChain_;

	// Flags
	bool Wireframe_;
	bool VSyncEnabled_;
};


