#include "pch.h"
#include "FactorySingleton.h"

ComPtr<ID2D1Factory> FactorySingleton::m_GraphicsFactory;
ComPtr<IWICImagingFactory> FactorySingleton::m_ImageFactory;

template <typename T>
void CreateInstance(REFCLSID clsid, Microsoft::WRL::ComPtr<T>& ptr)
{
	ASSERT(!ptr);
	HR(CoCreateInstance(clsid, nullptr, CLSCTX_INPROC_SERVER,
		__uuidof(T), reinterpret_cast<void**>(ptr.GetAddressOf())));
}


ComPtr<ID2D1Factory> FactorySingleton::GetGraphicsFactory()
{
	if (!m_GraphicsFactory)
	{
		D2D1_FACTORY_OPTIONS fo = {};

#ifdef DEBUG
		fo.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

		HR(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
			fo,
			m_GraphicsFactory.GetAddressOf()));

	}
	return m_GraphicsFactory;
}

ComPtr<IWICImagingFactory> FactorySingleton::GetImageFactory()
{
	if (!m_ImageFactory)
	{
		CreateInstance(CLSID_WICImagingFactory, m_ImageFactory);
	}
	return m_ImageFactory;
}

void FactorySingleton::DestroyImageFactory()
{
	if (m_ImageFactory)
	{
		m_ImageFactory.Reset();
	}
}
