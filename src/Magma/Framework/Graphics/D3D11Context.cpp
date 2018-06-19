#include "D3D11Context.hpp"

#include <Config.hpp>

#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")

void Magma::Framework::Graphics::D3D11Context::Init(Input::Window * window)
{

}

void Magma::Framework::Graphics::D3D11Context::Terminate()
{

}

#endif
