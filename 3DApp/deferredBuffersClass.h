#ifndef _DEFERREDBUFFERSCLASS_H_
#define _DEFERREDBUFFERSCLASS_H_

const int BUFFER_COUNT = 2;
#include <d3d11.h>
#include <directxmath.h>

class DeferredBuffersClass
{
public:
	DeferredBuffersClass();
	DeferredBuffersClass(const DeferredBuffersClass&);
	~DeferredBuffersClass();

	bool Initialize(ID3D11Device*, int, int, float, float);
	void Shutdown();
	
	void SetRenderTargets(ID3D11DeviceContext*);
	void ClearRenderTargets(ID3D11DeviceContext*, float, float, float, float);

	ID3D11ShaderResourceView* GetShaderResourceView(int);

private:
	int mTextureWidth, mTextureHeight;
	ID3D11Texture2D * mRenderTargetTextureArray[BUFFER_COUNT];
	ID3D11RenderTargetView * mRenderTargetViewArray[BUFFER_COUNT];
	ID3D11ShaderResourceView * mShaderResourceViewArray[BUFFER_COUNT];
	ID3D11Texture2D * mDepthStencilBuffer;
	ID3D11DepthStencilView * mDepthStencilView;
	D3D11_VIEWPORT mViewport;
};



#endif
