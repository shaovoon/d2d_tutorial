#pragma once
#include <wrl.h>
#include <d2d1.h>

using namespace D2D1;
using namespace Microsoft::WRL;

class FactorySingleton
{
public:
	static ComPtr<ID2D1Factory> GetGraphicsFactory();
private:
	static ComPtr<ID2D1Factory> m_GraphicsFactory;
};

