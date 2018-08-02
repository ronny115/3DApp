#include "deferredBuffersClass.h"

DeferredBuffersClass::DeferredBuffersClass()
{
	int i;
	for (i = 0; i < BUFFER_COUNT; i++)
	{
		mRenderTargetTextureArray[i] = 0;
		mRenderTargetViewArray[i] = 0;
		mShaderResourceViewArray[i] = 0;
	}

	mDepthStencilBuffer = 0;
	mDepthStencilView = 0;
}

DeferredBuffersClass::DeferredBuffersClass(const DeferredBuffersClass& other) {}
DeferredBuffersClass::~DeferredBuffersClass() {}

bool DeferredBuffersClass::Initialize(ID3D11Device * device, int textureWidth, int textureHeight, float screenDepth, float screenNear)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	int i;

	mTextureWidth = textureWidth;
	mTextureHeight = textureHeight;

	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	for (i = 0; i < BUFFER_COUNT; i++)
	{
		result = device->CreateTexture2D(&textureDesc, NULL, &mRenderTargetTextureArray[i]);
		if (FAILED(result))
		{
			return false;
		}
	}

	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	for (i = 0; i < BUFFER_COUNT; i++)
	{
		result = device->CreateRenderTargetView(mRenderTargetTextureArray[i], &renderTargetViewDesc, &mRenderTargetViewArray[i]);
		if (FAILED(result))
		{
			return false;
		}
	}

	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	for (i = 0; i < BUFFER_COUNT; i++)
	{
		result = device->CreateShaderResourceView(mRenderTargetTextureArray[i], &shaderResourceViewDesc, &mShaderResourceViewArray[i]);
		if (FAILED(result))
		{
			return false;
		}
	}

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = textureWidth;
	depthBufferDesc.Height = textureHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	for (i = 0; i < BUFFER_COUNT; i++)
	{
		result = device->CreateTexture2D(&depthBufferDesc, NULL, &mDepthStencilBuffer);
		if (FAILED(result))
		{
			return false;
		}
	}

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	
	result = device->CreateDepthStencilView(mDepthStencilBuffer, &depthStencilViewDesc, &mDepthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	mViewport.Width = (float)textureWidth;
	mViewport.Height = (float)textureHeight;
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;
	mViewport.TopLeftX = 0.0f;
	mViewport.TopLeftY = 0.0f;

	return true;
}

void DeferredBuffersClass::Shutdown()
{
	int i;
	if (mDepthStencilView)
	{
		mDepthStencilView->Release();
		mDepthStencilView = 0;
	}
	if (mDepthStencilBuffer)
	{
		mDepthStencilBuffer->Release();
		mDepthStencilBuffer = 0;
	}
	for (i = 0; i < BUFFER_COUNT; i++)
	{
		if (mShaderResourceViewArray[i])
		{
			mShaderResourceViewArray[i]->Release();
			mShaderResourceViewArray[i] = 0;
		}
		if (mRenderTargetViewArray[i])
		{
			mRenderTargetViewArray[i]->Release();
			mRenderTargetViewArray[i] = 0;
		}
		if (mRenderTargetTextureArray[i])
		{
			mRenderTargetTextureArray[i]->Release();
			mRenderTargetTextureArray[i] = 0;
		}
	}
	return;
}

void DeferredBuffersClass::SetRenderTargets(ID3D11DeviceContext* deviceContext)
{
	deviceContext->OMSetRenderTargets(BUFFER_COUNT, mRenderTargetViewArray, mDepthStencilView);
	deviceContext->RSSetViewports(1, &mViewport);
	return;
}

void DeferredBuffersClass::ClearRenderTargets(ID3D11DeviceContext*deviceContext, float red, float green, float blue, float alpha)
{
	float color[4];
	int i;

	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	for (i = 0; i < BUFFER_COUNT; i++)
	{
		deviceContext->ClearRenderTargetView(mRenderTargetViewArray[i], color);
	}
	deviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

ID3D11ShaderResourceView * DeferredBuffersClass::GetShaderResourceView(int view)
{
	return mShaderResourceViewArray[view];
}