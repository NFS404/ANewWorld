/*
   MIT License

   Copyright (c) 2019 Berkay Yigit <berkay2578@gmail.com>
       Copyright holder detail: Nickname(s) used by the copyright holder: 'berkay2578', 'berkayylmao'.

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

#pragma once
#include "stdafx.h"
#include <d3d9.h>

typedef HRESULT(WINAPI* QueryInterface_t)(LPDIRECT3DDEVICE9 pDevice, REFIID riid, void** ppvObj);
typedef ULONG(WINAPI* AddRef_t)(LPDIRECT3DDEVICE9 pDevice);
typedef ULONG(WINAPI* Release_t)(LPDIRECT3DDEVICE9 pDevice);
typedef HRESULT(WINAPI* TestCooperativeLevel_t)(LPDIRECT3DDEVICE9 pDevice);
typedef UINT(WINAPI* GetAvailableTextureMem_t)(LPDIRECT3DDEVICE9 pDevice);
typedef HRESULT(WINAPI* EvictManagedResources_t)(LPDIRECT3DDEVICE9 pDevice);
typedef HRESULT(WINAPI* GetDirect3D_t)(LPDIRECT3DDEVICE9 pDevice, IDirect3D9** ppD3D9);
typedef HRESULT(WINAPI* GetDeviceCaps_t)(LPDIRECT3DDEVICE9 pDevice, D3DCAPS9* pCaps);
typedef HRESULT(WINAPI* GetDisplayMode_t)(LPDIRECT3DDEVICE9 pDevice, UINT iSwapChain, D3DDISPLAYMODE* pMode);
typedef HRESULT(WINAPI* GetCreationParameters_t)(LPDIRECT3DDEVICE9 pDevice, D3DDEVICE_CREATION_PARAMETERS *pParameters);
typedef HRESULT(WINAPI* SetCursorProperties_t)(LPDIRECT3DDEVICE9 pDevice, UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap);
typedef void(WINAPI* SetCursorPosition_t)(LPDIRECT3DDEVICE9 pDevice, int X, int Y, DWORD Flags);
typedef BOOL(WINAPI* ShowCursor_t)(LPDIRECT3DDEVICE9 pDevice, BOOL bShow);
typedef HRESULT(WINAPI* CreateAdditionalSwapChain_t)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain);
typedef HRESULT(WINAPI* GetSwapChain_t)(LPDIRECT3DDEVICE9 pDevice, UINT iSwapChain, IDirect3DSwapChain9** pSwapChain);
typedef UINT(WINAPI* GetNumberOfSwapChains_t)(LPDIRECT3DDEVICE9 pDevice);
typedef HRESULT(WINAPI* Reset_t)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
typedef HRESULT(WINAPI* Present_t)(LPDIRECT3DDEVICE9 pDevice, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion);
typedef HRESULT(WINAPI* GetBackBuffer_t)(LPDIRECT3DDEVICE9 pDevice, UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer);
typedef HRESULT(WINAPI* GetRasterStatus_t)(LPDIRECT3DDEVICE9 pDevice, UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus);
typedef HRESULT(WINAPI* SetDialogBoxMode_t)(LPDIRECT3DDEVICE9 pDevice, BOOL bEnableDialogs);
typedef void(WINAPI* SetGammaRamp_t)(LPDIRECT3DDEVICE9 pDevice, UINT iSwapChain, DWORD Flags, const D3DGAMMARAMP* pRamp);
typedef void(WINAPI* GetGammaRamp_t)(LPDIRECT3DDEVICE9 pDevice, UINT iSwapChain, D3DGAMMARAMP* pRamp);
typedef HRESULT(WINAPI* CreateTexture_t)(LPDIRECT3DDEVICE9 pDevice, UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle);
typedef HRESULT(WINAPI* CreateVolumeTexture_t)(LPDIRECT3DDEVICE9 pDevice, UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9** ppVolumeTexture, HANDLE* pSharedHandle);
typedef HRESULT(WINAPI* CreateCubeTexture_t)(LPDIRECT3DDEVICE9 pDevice, UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle);
typedef HRESULT(WINAPI* CreateVertexBuffer_t)(LPDIRECT3DDEVICE9 pDevice, UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle);
typedef HRESULT(WINAPI* CreateIndexBuffer_t)(LPDIRECT3DDEVICE9 pDevice, UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle);
typedef HRESULT(WINAPI* CreateRenderTarget_t)(LPDIRECT3DDEVICE9 pDevice, UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle);
typedef HRESULT(WINAPI* CreateDepthStencilSurface_t)(LPDIRECT3DDEVICE9 pDevice, UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle);
typedef HRESULT(WINAPI* UpdateSurface_t)(LPDIRECT3DDEVICE9 pDevice, IDirect3DSurface9* pSourceSurface, const RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, const POINT* pDestPoint);
typedef HRESULT(WINAPI* UpdateTexture_t)(LPDIRECT3DDEVICE9 pDevice, IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture);
typedef HRESULT(WINAPI* GetRenderTargetData_t)(LPDIRECT3DDEVICE9 pDevice, IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface);
typedef HRESULT(WINAPI* GetFrontBufferData_t)(LPDIRECT3DDEVICE9 pDevice, UINT iSwapChain, IDirect3DSurface9* pDestSurface);
typedef HRESULT(WINAPI* StretchRect_t)(LPDIRECT3DDEVICE9 pDevice, IDirect3DSurface9* pSourceSurface, const RECT* pSourceRect, IDirect3DSurface9* pDestSurface, const RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter);
typedef HRESULT(WINAPI* ColorFill_t)(LPDIRECT3DDEVICE9 pDevice, IDirect3DSurface9* pSurface, const RECT* pRect, D3DCOLOR color);
typedef HRESULT(WINAPI* CreateOffscreenPlainSurface_t)(LPDIRECT3DDEVICE9 pDevice, UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle);
typedef HRESULT(WINAPI* SetRenderTarget_t)(LPDIRECT3DDEVICE9 pDevice, DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget);
typedef HRESULT(WINAPI* GetRenderTarget_t)(LPDIRECT3DDEVICE9 pDevice, DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget);
typedef HRESULT(WINAPI* SetDepthStencilSurface_t)(LPDIRECT3DDEVICE9 pDevice, IDirect3DSurface9* pNewZStencil);
typedef HRESULT(WINAPI* GetDepthStencilSurface_t)(LPDIRECT3DDEVICE9 pDevice, IDirect3DSurface9** ppZStencilSurface);
typedef HRESULT(WINAPI* BeginScene_t)(LPDIRECT3DDEVICE9 pDevice);
typedef HRESULT(WINAPI* EndScene_t)(LPDIRECT3DDEVICE9 pDevice);
typedef HRESULT(WINAPI* Clear_t)(LPDIRECT3DDEVICE9 pDevice, DWORD Count, const D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil);
typedef HRESULT(WINAPI* SetTransform_t)(LPDIRECT3DDEVICE9 pDevice, D3DTRANSFORMSTATETYPE State, const D3DMATRIX* pMatrix);
typedef HRESULT(WINAPI* GetTransform_t)(LPDIRECT3DDEVICE9 pDevice, D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix);
typedef HRESULT(WINAPI* MultiplyTransform_t)(LPDIRECT3DDEVICE9 pDevice, D3DTRANSFORMSTATETYPE State, const D3DMATRIX* pMatrix);
typedef HRESULT(WINAPI* SetViewport_t)(LPDIRECT3DDEVICE9 pDevice, const D3DVIEWPORT9* pViewport);
typedef HRESULT(WINAPI* GetViewport_t)(LPDIRECT3DDEVICE9 pDevice, D3DVIEWPORT9* pViewport);
typedef HRESULT(WINAPI* SetMaterial_t)(LPDIRECT3DDEVICE9 pDevice, const D3DMATERIAL9* pMaterial);
typedef HRESULT(WINAPI* GetMaterial_t)(LPDIRECT3DDEVICE9 pDevice, D3DMATERIAL9* pMaterial);
typedef HRESULT(WINAPI* SetLight_t)(LPDIRECT3DDEVICE9 pDevice, DWORD Index, const D3DLIGHT9* pLight);
typedef HRESULT(WINAPI* GetLight_t)(LPDIRECT3DDEVICE9 pDevice, DWORD Index, D3DLIGHT9* pLight);
typedef HRESULT(WINAPI* LightEnable_t)(LPDIRECT3DDEVICE9 pDevice, DWORD Index, BOOL Enable);
typedef HRESULT(WINAPI* GetLightEnable_t)(LPDIRECT3DDEVICE9 pDevice, DWORD Index, BOOL* pEnable);
typedef HRESULT(WINAPI* SetClipPlane_t)(LPDIRECT3DDEVICE9 pDevice, DWORD Index, const float* pPlane);
typedef HRESULT(WINAPI* GetClipPlane_t)(LPDIRECT3DDEVICE9 pDevice, DWORD Index, float* pPlane);
typedef HRESULT(WINAPI* SetRenderState_t)(LPDIRECT3DDEVICE9 pDevice, D3DRENDERSTATETYPE State, DWORD Value);
typedef HRESULT(WINAPI* GetRenderState_t)(LPDIRECT3DDEVICE9 pDevice, D3DRENDERSTATETYPE State, DWORD* pValue);
typedef HRESULT(WINAPI* CreateStateBlock_t)(LPDIRECT3DDEVICE9 pDevice, D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB);
typedef HRESULT(WINAPI* BeginStateBlock_t)(LPDIRECT3DDEVICE9 pDevice);
typedef HRESULT(WINAPI* EndStateBlock_t)(LPDIRECT3DDEVICE9 pDevice, IDirect3DStateBlock9** ppSB);
typedef HRESULT(WINAPI* SetClipStatus_t)(LPDIRECT3DDEVICE9 pDevice, const D3DCLIPSTATUS9* pClipStatus);
typedef HRESULT(WINAPI* GetClipStatus_t)(LPDIRECT3DDEVICE9 pDevice, D3DCLIPSTATUS9* pClipStatus);
typedef HRESULT(WINAPI* GetTexture_t)(LPDIRECT3DDEVICE9 pDevice, DWORD Stage, IDirect3DBaseTexture9** ppTexture);
typedef HRESULT(WINAPI* SetTexture_t)(LPDIRECT3DDEVICE9 pDevice, DWORD Stage, IDirect3DBaseTexture9* pTexture);
typedef HRESULT(WINAPI* GetTextureStageState_t)(LPDIRECT3DDEVICE9 pDevice, DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue);
typedef HRESULT(WINAPI* SetTextureStageState_t)(LPDIRECT3DDEVICE9 pDevice, DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value);
typedef HRESULT(WINAPI* GetSamplerState_t)(LPDIRECT3DDEVICE9 pDevice, DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue);
typedef HRESULT(WINAPI* SetSamplerState_t)(LPDIRECT3DDEVICE9 pDevice, DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value);
typedef HRESULT(WINAPI* ValidateDevice_t)(LPDIRECT3DDEVICE9 pDevice, DWORD* pNumPasses);
typedef HRESULT(WINAPI* SetPaletteEntries_t)(LPDIRECT3DDEVICE9 pDevice, UINT PaletteNumber, const PALETTEENTRY* pEntries);
typedef HRESULT(WINAPI* GetPaletteEntries_t)(LPDIRECT3DDEVICE9 pDevice, UINT PaletteNumber, PALETTEENTRY* pEntries);
typedef HRESULT(WINAPI* SetCurrentTexturePalette_t)(LPDIRECT3DDEVICE9 pDevice, UINT PaletteNumber);
typedef HRESULT(WINAPI* GetCurrentTexturePalette_t)(LPDIRECT3DDEVICE9 pDevice, UINT *PaletteNumber);
typedef HRESULT(WINAPI* SetScissorRect_t)(LPDIRECT3DDEVICE9 pDevice, const RECT* pRect);
typedef HRESULT(WINAPI* GetScissorRect_t)(LPDIRECT3DDEVICE9 pDevice, RECT* pRect);
typedef HRESULT(WINAPI* SetSoftwareVertexProcessing_t)(LPDIRECT3DDEVICE9 pDevice, BOOL bSoftware);
typedef BOOL(WINAPI* GetSoftwareVertexProcessing_t)(LPDIRECT3DDEVICE9 pDevice);
typedef HRESULT(WINAPI* SetNPatchMode_t)(LPDIRECT3DDEVICE9 pDevice, float nSegments);
typedef float(WINAPI* GetNPatchMode_t)(LPDIRECT3DDEVICE9 pDevice);
typedef HRESULT(WINAPI* DrawPrimitive_t)(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount);
typedef HRESULT(WINAPI* DrawIndexedPrimitive_t)(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);
typedef HRESULT(WINAPI* DrawPrimitiveUP_t)(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, const void* pVertexStreamZeroData, UINT VertexStreamZeroStride);
typedef HRESULT(WINAPI* DrawIndexedPrimitiveUP_t)(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, const void* pIndexData, D3DFORMAT IndexDataFormat, const void* pVertexStreamZeroData, UINT VertexStreamZeroStride);
typedef HRESULT(WINAPI* ProcessVertices_t)(LPDIRECT3DDEVICE9 pDevice, UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags);
typedef HRESULT(WINAPI* CreateVertexDeclaration_t)(LPDIRECT3DDEVICE9 pDevice, const D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl);
typedef HRESULT(WINAPI* SetVertexDeclaration_t)(LPDIRECT3DDEVICE9 pDevice, IDirect3DVertexDeclaration9* pDecl);
typedef HRESULT(WINAPI* GetVertexDeclaration_t)(LPDIRECT3DDEVICE9 pDevice, IDirect3DVertexDeclaration9** ppDecl);
typedef HRESULT(WINAPI* SetFVF_t)(LPDIRECT3DDEVICE9 pDevice, DWORD FVF);
typedef HRESULT(WINAPI* GetFVF_t)(LPDIRECT3DDEVICE9 pDevice, DWORD* pFVF);
typedef HRESULT(WINAPI* CreateVertexShader_t)(LPDIRECT3DDEVICE9 pDevice, const DWORD* pFunction, IDirect3DVertexShader9** ppShader);
typedef HRESULT(WINAPI* SetVertexShader_t)(LPDIRECT3DDEVICE9 pDevice, IDirect3DVertexShader9* pShader);
typedef HRESULT(WINAPI* GetVertexShader_t)(LPDIRECT3DDEVICE9 pDevice, IDirect3DVertexShader9** ppShader);
typedef HRESULT(WINAPI* SetVertexShaderConstantF_t)(LPDIRECT3DDEVICE9 pDevice, UINT StartRegister, const float* pconstantData, UINT Vector4fCount);
typedef HRESULT(WINAPI* GetVertexShaderConstantF_t)(LPDIRECT3DDEVICE9 pDevice, UINT StartRegister, float* pconstantData, UINT Vector4fCount);
typedef HRESULT(WINAPI* SetVertexShaderConstantI_t)(LPDIRECT3DDEVICE9 pDevice, UINT StartRegister, const int* pconstantData, UINT Vector4iCount);
typedef HRESULT(WINAPI* GetVertexShaderConstantI_t)(LPDIRECT3DDEVICE9 pDevice, UINT StartRegister, int* pconstantData, UINT Vector4iCount);
typedef HRESULT(WINAPI* SetVertexShaderConstantB_t)(LPDIRECT3DDEVICE9 pDevice, UINT StartRegister, const BOOL* pconstantData, UINT BoolCount);
typedef HRESULT(WINAPI* GetVertexShaderConstantB_t)(LPDIRECT3DDEVICE9 pDevice, UINT StartRegister, BOOL* pconstantData, UINT BoolCount);
typedef HRESULT(WINAPI* SetStreamSource_t)(LPDIRECT3DDEVICE9 pDevice, UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride);
typedef HRESULT(WINAPI* GetStreamSource_t)(LPDIRECT3DDEVICE9 pDevice, UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* OffsetInBytes, UINT* pStride);
typedef HRESULT(WINAPI* SetStreamSourceFreq_t)(LPDIRECT3DDEVICE9 pDevice, UINT StreamNumber, UINT Divider);
typedef HRESULT(WINAPI* GetStreamSourceFreq_t)(LPDIRECT3DDEVICE9 pDevice, UINT StreamNumber, UINT* Divider);
typedef HRESULT(WINAPI* SetIndices_t)(LPDIRECT3DDEVICE9 pDevice, IDirect3DIndexBuffer9* pIndexData);
typedef HRESULT(WINAPI* GetIndices_t)(LPDIRECT3DDEVICE9 pDevice, IDirect3DIndexBuffer9** ppIndexData);
typedef HRESULT(WINAPI* CreatePixelShader_t)(LPDIRECT3DDEVICE9 pDevice, const DWORD* pFunction, IDirect3DPixelShader9** ppShader);
typedef HRESULT(WINAPI* SetPixelShader_t)(LPDIRECT3DDEVICE9 pDevice, IDirect3DPixelShader9* pShader);
typedef HRESULT(WINAPI* GetPixelShader_t)(LPDIRECT3DDEVICE9 pDevice, IDirect3DPixelShader9** ppShader);
typedef HRESULT(WINAPI* SetPixelShaderConstantF_t)(LPDIRECT3DDEVICE9 pDevice, UINT StartRegister, const float* pconstantData, UINT Vector4fCount);
typedef HRESULT(WINAPI* GetPixelShaderConstantF_t)(LPDIRECT3DDEVICE9 pDevice, UINT StartRegister, float* pconstantData, UINT Vector4fCount);
typedef HRESULT(WINAPI* SetPixelShaderConstantI_t)(LPDIRECT3DDEVICE9 pDevice, UINT StartRegister, const int* pconstantData, UINT Vector4iCount);
typedef HRESULT(WINAPI* GetPixelShaderConstantI_t)(LPDIRECT3DDEVICE9 pDevice, UINT StartRegister, int* pconstantData, UINT Vector4iCount);
typedef HRESULT(WINAPI* SetPixelShaderConstantB_t)(LPDIRECT3DDEVICE9 pDevice, UINT StartRegister, const BOOL* pconstantData, UINT BoolCount);
typedef HRESULT(WINAPI* GetPixelShaderConstantB_t)(LPDIRECT3DDEVICE9 pDevice, UINT StartRegister, BOOL* pconstantData, UINT BoolCount);
typedef HRESULT(WINAPI* DrawRectPatch_t)(LPDIRECT3DDEVICE9 pDevice, UINT Handle, const float* pNumSegs, const D3DRECTPATCH_INFO* pRectPatchInfo);
typedef HRESULT(WINAPI* DrawTriPatch_t)(LPDIRECT3DDEVICE9 pDevice, UINT Handle, const float* pNumSegs, const D3DTRIPATCH_INFO* pTriPatchInfo);
typedef HRESULT(WINAPI* DeletePatch_t)(LPDIRECT3DDEVICE9 pDevice, UINT Handle);
typedef HRESULT(WINAPI* CreateQuery_t)(LPDIRECT3DDEVICE9 pDevice, D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery);