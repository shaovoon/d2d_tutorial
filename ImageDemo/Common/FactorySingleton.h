#pragma once
#include <wrl.h>
#include <d2d1.h>

using namespace D2D1;
using namespace Microsoft::WRL;

class FactorySingleton
{
public:
	static ComPtr<ID2D1Factory> GetGraphicsFactory();
	static ComPtr<IDWriteFactory> GetDWriteFactory();
	static ComPtr<IWICImagingFactory> GetImageFactory();
	static void DestroyImageFactory();
private:
	static ComPtr<ID2D1Factory> m_GraphicsFactory;
	static ComPtr<IDWriteFactory> m_DWriteFactory;
	static ComPtr<IWICImagingFactory> m_ImageFactory;
};

