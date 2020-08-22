#include <wrl.h>
#include <d2d1.h>
#include <dwrite.h>

template <typename T>
void CreateInstance(REFCLSID clsid, Microsoft::WRL::ComPtr<T>& ptr)
{
	ASSERT(!ptr);
	HR(CoCreateInstance(clsid, nullptr, CLSCTX_INPROC_SERVER,
		__uuidof(T), reinterpret_cast<void**>(ptr.GetAddressOf())));
}
