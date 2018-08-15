#include "RenderDevice.h"
#include <string.h>

void mfgDefaultRenderDeviceDesc(mfgRenderDeviceDesc * desc)
{
	desc->vsyncEnabled = MFM_FALSE;
}

void mfgDefaultVertexElement(mfgVertexElement * element)
{
	memset(element->name, '\0', sizeof(element->name));
	element->stride = 0;
	element->offset = 0;
	element->bufferIndex = 0;
	element->type = 0;
	element->size = 0;
}

void mfgDefaultSamplerDesc(mfgSamplerDesc * desc)
{
	desc->borderColor[0] = 0.0f;
	desc->borderColor[1] = 0.0f;
	desc->borderColor[2] = 0.0f;
	desc->borderColor[3] = 0.0f;
	desc->maxAnisotropy = 1;
	desc->minFilter = MFG_NEAREST;
	desc->magFilter = MFG_NEAREST;
	desc->mipmapFilter = MFG_NONE;
	desc->adressU = MFG_REPEAT;
	desc->adressV = MFG_REPEAT;
	desc->adressW = MFG_REPEAT;
}

void mfgDefaultRasterStateDesc(mfgRasterStateDesc * desc)
{
	desc->cullEnabled = MFM_TRUE;
	desc->frontFace = MFG_CCW;
	desc->cullFace = MFG_BACK;
	desc->rasterMode = MFG_FILL;
}

void mfgDefaultDepthStencilStateDesc(mfgDepthStencilStateDesc * desc)
{
	desc->depthEnabled = MFM_TRUE;
	desc->depthWriteEnabled = MFM_TRUE;
	desc->depthNear = 0.0f;
	desc->depthFar = 1.0f;
	desc->depthCompare = MFG_LESS;
	desc->stencilRef = 0;
	desc->stencilEnabled = MFM_FALSE;
	desc->stencilReadMask = 0xFF;
	desc->stencilWriteMask = 0xFF;
	desc->frontFaceStencilCompare = MFG_ALWAYS;
	desc->frontFaceStencilFail = MFG_KEEP;
	desc->frontFaceStencilPass = MFG_KEEP;
	desc->frontFaceDepthFail = MFG_KEEP;
	desc->backFaceStencilCompare = MFG_ALWAYS;
	desc->backFaceStencilFail = MFG_KEEP;
	desc->backFaceStencilPass = MFG_KEEP;
	desc->backFaceDepthFail = MFG_KEEP;
}

void mfgDefaultBlendStateDesc(mfgBlendStateDesc * desc)
{
	desc->blendEnabled = MFM_FALSE;
	desc->sourceFactor = MFG_ONE;
	desc->destinationFactor = MFG_ZERO;
	desc->blendOperation = MFG_ADD;
	desc->sourceAlphaFactor = MFG_ONE;
	desc->destinationAlphaFactor = MFG_ZERO;
	desc->blendAlphaOperation = MFG_ADD;
}
