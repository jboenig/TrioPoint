//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: ShapeShifter.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __SHAPESHIFTER_H__
#define __SHAPESHIFTER_H__

#include "ShapeShifterVer.h"

namespace ShapeShifter {
#include "SSCL.h"
};

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// Interface GUIDs

// {D802517B-7EC8-4020-B84B-D0C97D6938FA}
DEFINE_GUID(IID_IDevicePresentationDX8, 
0xd802517b, 0x7ec8, 0x4020, 0xb8, 0x4b, 0xd0, 0xc9, 0x7d, 0x69, 0x38, 0xfa);
interface DECLSPEC_UUID("D802517B-7EC8-4020-B84B-D0C97D6938FA") IDevicePresentationDX8;

// {0FE9379E-2236-4ca4-9D41-04864DEA515D}
DEFINE_GUID(IID_IPresentationManager, 
0xfe9379e, 0x2236, 0x4ca4, 0x9d, 0x41, 0x4, 0x86, 0x4d, 0xea, 0x51, 0x5d);
interface DECLSPEC_UUID("0FE9379E-2236-4ca4-9D41-04864DEA515D") IPresentationManager;

// {1C28FCE6-91F7-4974-8BE6-86883DDB17C9}
DEFINE_GUID(IID_IRenderDX8, 
0x1c28fce6, 0x91f7, 0x4974, 0x8b, 0xe6, 0x86, 0x88, 0x3d, 0xdb, 0x17, 0xc9);
interface DECLSPEC_UUID("1C28FCE6-91F7-4974-8BE6-86883DDB17C9") IRenderDX8;

// {548C2E46-0579-4a75-8324-216B59AC081D}
DEFINE_GUID(IID_IDrawMeshDX8, 
0x548c2e46, 0x579, 0x4a75, 0x83, 0x24, 0x21, 0x6b, 0x59, 0xac, 0x8, 0x1d);
interface DECLSPEC_UUID("548C2E46-0579-4a75-8324-216B59AC081D") IDrawMeshDX8;

// {BC01687A-D6B4-4cf9-BAFA-C10F9C3C8C0E}
DEFINE_GUID(IID_IMeshEx, 
0xbc01687a, 0xd6b4, 0x4cf9, 0xba, 0xfa, 0xc1, 0xf, 0x9c, 0x3c, 0x8c, 0xe);
interface DECLSPEC_UUID("BC01687A-D6B4-4cf9-BAFA-C10F9C3C8C0E") IMeshEx;

// {447337C8-027D-4b16-A016-994CBDD20570}
DEFINE_GUID(IID_ICameraEx, 
0x447337c8, 0x27d, 0x4b16, 0xa0, 0x16, 0x99, 0x4c, 0xbd, 0xd2, 0x5, 0x70);
interface DECLSPEC_UUID("447337C8-027D-4b16-A016-994CBDD20570") ICameraEx;

// {21C98AF8-1178-4fdf-9A3E-EB69046A9B42}
DEFINE_GUID(IID_IContextObject, 
0x21c98af8, 0x1178, 0x4fdf, 0x9a, 0x3e, 0xeb, 0x69, 0x4, 0x6a, 0x9b, 0x42);
interface DECLSPEC_UUID("21C98AF8-1178-4fdf-9A3E-EB69046A9B42") IContextObject;

// {84365AC9-EF77-4e32-B311-21D01015886D}
DEFINE_GUID(IID_IContextProvider, 
0x84365ac9, 0xef77, 0x4e32, 0xb3, 0x11, 0x21, 0xd0, 0x10, 0x15, 0x88, 0x6d);
interface DECLSPEC_UUID("84365AC9-EF77-4e32-B311-21D01015886D") IContextProvider;

// {7C4C7C2A-5271-4794-8947-A2D8A1DD8332}
DEFINE_GUID(IID_IResourceManagerDX8, 
0x7c4c7c2a, 0x5271, 0x4794, 0x89, 0x47, 0xa2, 0xd8, 0xa1, 0xdd, 0x83, 0x32);
interface DECLSPEC_UUID("7C4C7C2A-5271-4794-8947-A2D8A1DD8332") IResourceManagerDX8;

// {36B6B883-B0B5-41db-B5FC-E5A09095928D}
DEFINE_GUID(IID_IInitInputDeviceDX8, 
0x36b6b883, 0xb0b5, 0x41db, 0xb5, 0xfc, 0xe5, 0xa0, 0x90, 0x95, 0x92, 0x8d);
interface DECLSPEC_UUID("36B6B883-B0B5-41db-B5FC-E5A09095928D") IInitInputDeviceDX8;

// {8A6B7627-659C-4455-ABBB-C0ACD17EB462}
DEFINE_GUID(IID_IInputDeviceProviderDX8, 
0x8a6b7627, 0x659c, 0x4455, 0xab, 0xbb, 0xc0, 0xac, 0xd1, 0x7e, 0xb4, 0x62);
interface DECLSPEC_UUID("8A6B7627-659C-4455-ABBB-C0ACD17EB462") IInputDeviceProviderDX8;

// {72F99C01-657C-4e29-9A6B-33375A7FDC21}
DEFINE_GUID(IID_IInputDeviceUserDX8, 
0x72f99c01, 0x657c, 0x4e29, 0x9a, 0x6b, 0x33, 0x37, 0x5a, 0x7f, 0xdc, 0x21);
interface DECLSPEC_UUID("72F99C01-657C-4e29-9A6B-33375A7FDC21") IInputDeviceUserDX8;

/////////////////////////////////////////////////////////////////////////
// Structure GUIDs

struct DECLSPEC_UUID("003BB316-70B4-44eb-9A76-51F3FB6AF72D") VECTOR3D;
struct DECLSPEC_UUID("505500BF-BB2C-463c-B1A5-1D9EB92C4866") LINE3D;
struct DECLSPEC_UUID("1FBA3290-C357-441c-A371-893CEFCDC582") TRIANGLE3D;
struct DECLSPEC_UUID("C4B804E3-ABB4-401b-8FA1-46452B76B2F4") BOX3D;
struct DECLSPEC_UUID("652A0023-07A2-4aa8-B703-B18FE4C7F224") SPHERE3D;
struct DECLSPEC_UUID("F868136B-133A-42ea-A170-951603DAE9B0") PLANE3D;
struct DECLSPEC_UUID("3C1920CC-89FC-49b2-B4AB-EF2D0E1E0976") QUAD3D;
struct DECLSPEC_UUID("2D2F3B0F-9195-4bf1-9D52-B8D2F559D6AB") TEXTURECOORDINATE3D;
struct DECLSPEC_UUID("136C49D7-1322-4a29-A0E9-51BB1B4F0DDD") DISPLAYMODEINFO;

};

#include "VtRecord.h"
#include "ShapeShifterUtil.h"
#include "Geometry3d.h"
#include "Vertex3d.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// Constants

const int NUM_INPUT_DEVICE_CATEGORIES = (sizeof(enum InputDeviceCategory) / 2);

/////////////////////////////////////////////////////////////////////////
// IDevicePresentationDX8

DECLARE_INTERFACE_(IDevicePresentationDX8, IUnknown)
{
	STDMETHOD(SetPresentationParameters)(D3DPRESENT_PARAMETERS* pPresentParams) = 0;
	STDMETHOD(GetPresentationParameters)(D3DPRESENT_PARAMETERS* pPresentParams) = 0;
	STDMETHOD(SetPresentationBuffers)(LPDIRECT3DDEVICE8 pDevice,
	                                  LPDIRECT3DSWAPCHAIN8 pSwapChain,
	                                  LPDIRECT3DSURFACE8 pDepthStencil) = 0;
	STDMETHOD(SharePresentationBuffers)(IDevicePresentationDX8* pShareWith) = 0;
	STDMETHOD(FreePresentationBuffers)() = 0;
	STDMETHOD(GetSwapChainType)(DWORD* pFlag) = 0;
	STDMETHOD(GetDevice)(LPDIRECT3DDEVICE8* ppDevice) = 0;
	STDMETHOD(GetSwapChain)(LPDIRECT3DSWAPCHAIN8* ppSwapChain) = 0;
	STDMETHOD(GetBackBuffer)(LPDIRECT3DSURFACE8* ppBackBuffer) = 0;
	STDMETHOD(GetDepthStencil)(LPDIRECT3DSURFACE8* ppDepthStencil) = 0;
	STDMETHOD(SelectBackBuffer)(UINT nBackBuffer, D3DBACKBUFFER_TYPE bbType) = 0;
	STDMETHOD(SetRenderTarget)() = 0;
	STDMETHOD(Present)(CONST RECT* pSourceRect,
	                   CONST RECT* pDestRect,
	                   HWND hDestWindowOverride,
	                   CONST RGNDATA* pDirtyRegion) = 0;
};

_COM_SMARTPTR_TYPEDEF(IDevicePresentationDX8, IID_IDevicePresentationDX8);

const DWORD SWAP_CHAIN_NONE            = 0;
const DWORD SWAP_CHAIN_IMPLICIT        = 1;
const DWORD SWAP_CHAIN_ADDITIONAL      = 2;

/////////////////////////////////////////////////////////////////////////
// IPresentationManager

DECLARE_INTERFACE_(IPresentationManager, IUnknown)
{
	STDMETHOD(AddPresentation)(IDevicePresentationDX8* pPresentation) = 0;
	STDMETHOD(RemovePresentation)(IDevicePresentationDX8* pPresentation) = 0;
	STDMETHOD(InitPresentation)(IDevicePresentationDX8* pPresentation) = 0;
	STDMETHOD(GetDeviceParameters)(D3DPRESENT_PARAMETERS* pPresentParams) = 0;
};

_COM_SMARTPTR_TYPEDEF(IPresentationManager, IID_IPresentationManager);

/////////////////////////////////////////////////////////////////////////
// IResourceManagerDX8

DECLARE_INTERFACE_(IResourceManagerDX8, IUnknown)
{
	STDMETHOD(GetD3DDevice8)(LPDIRECT3DDEVICE8* ppD3DDevice8) = 0;
	STDMETHOD(GenerateResourceId)(D3DRESOURCETYPE resType, DeviceResourceId* pResId) = 0;
	STDMETHOD(ReserveResourceId)(D3DRESOURCETYPE resType, DeviceResourceId nResId) = 0;
	STDMETHOD(SetSurface)(DeviceResourceId nResId, LPDIRECT3DSURFACE8 pSurface) = 0;
	STDMETHOD(GetSurface)(DeviceResourceId nResId, LPDIRECT3DSURFACE8* ppSurface) = 0;
	STDMETHOD(SetVolume)(DeviceResourceId nResId, LPDIRECT3DVOLUME8 pVolume) = 0;
	STDMETHOD(GetVolume)(DeviceResourceId nResId, LPDIRECT3DVOLUME8* ppVolume) = 0;
	STDMETHOD(SetTexture)(DeviceResourceId nResId, LPDIRECT3DTEXTURE8 pTexture) = 0;
	STDMETHOD(GetTexture)(DeviceResourceId nResId, LPDIRECT3DTEXTURE8* ppTexture) = 0;
	STDMETHOD(SetVolumeTexture)(DeviceResourceId nResId, LPDIRECT3DVOLUMETEXTURE8 pVolTexture) = 0;
	STDMETHOD(GetVolumeTexture)(DeviceResourceId nResId, LPDIRECT3DVOLUMETEXTURE8* ppVolTexture) = 0;
	STDMETHOD(SetCubeTexture)(DeviceResourceId nResId, LPDIRECT3DCUBETEXTURE8 pCubeTexture) = 0;
	STDMETHOD(GetCubeTexture)(DeviceResourceId nResId, LPDIRECT3DCUBETEXTURE8* ppCubeTexture) = 0;
	STDMETHOD(SetVertexBuffer)(DeviceResourceId nResId, LPDIRECT3DVERTEXBUFFER8 pVB) = 0;
	STDMETHOD(GetVertexBuffer)(DeviceResourceId nResId, LPDIRECT3DVERTEXBUFFER8* ppVB) = 0;
	STDMETHOD(SetIndexBuffer)(DeviceResourceId nResId, LPDIRECT3DINDEXBUFFER8 pIB) = 0;
	STDMETHOD(GetIndexBuffer)(DeviceResourceId nResId, LPDIRECT3DINDEXBUFFER8* ppIB) = 0;
	STDMETHOD(FreeResource)(D3DRESOURCETYPE resType, DeviceResourceId nResId) = 0;
	STDMETHOD(FreeAllResources)(D3DRESOURCETYPE resType) = 0;
};

_COM_SMARTPTR_TYPEDEF(IResourceManagerDX8, IID_IResourceManagerDX8);

#define SSCL_RESOURCEID_NULL    -1

/////////////////////////////////////////////////////////////////////////
// IRenderDX8

DECLARE_INTERFACE_(IRenderDX8, IUnknown)
{
	STDMETHOD(Render)(LPDIRECT3DDEVICE8 pDevice,
	                  IResourceManagerDX8* pDevResources) = 0;
};

_COM_SMARTPTR_TYPEDEF(IRenderDX8, IID_IRenderDX8);

/////////////////////////////////////////////////////////////////////////
// IContextObject

DECLARE_INTERFACE_(IContextObject, IUnknown)
{
	STDMETHOD(SelectObject)(LPDIRECT3DDEVICE8 pDevice,
	                        IResourceManagerDX8* pDevResources,
	                        DWORD dwTextureStage) = 0;
};

_COM_SMARTPTR_TYPEDEF(IContextObject, IID_IContextObject);

/////////////////////////////////////////////////////////////////////////
// IContextProvider

DECLARE_INTERFACE_(IContextProvider, IUnknown)
{
	STDMETHOD(SelectContext)(LPDIRECT3DDEVICE8 pDevice,
	                         IResourceManagerDX8* pDevResources,
	                         long nContextId) = 0;
};

_COM_SMARTPTR_TYPEDEF(IContextProvider, IID_IContextProvider);

/////////////////////////////////////////////////////////////////////////
// IMeshEx

DECLARE_INTERFACE_(IMeshEx, IMesh)
{
	STDMETHOD(_GetVertexFormat)(DWORD* pVertexTypeDesc) = 0;

	STDMETHOD(_GetVertexSize)(UINT* pVertexSize) = 0;

	STDMETHOD(_GetVertexCount)(long* pNumVertices) = 0;

	STDMETHOD(_GetVertexBuffer)(LPVOID* ppVertexBuf) = 0;

	STDMETHOD(_SetVertexBuffer)(LPVOID pVertexBuf, VertexCount nVertices) = 0;

	STDMETHOD(_PutVertex)(LPVOID pVertex,
	                      DWORD dwVertexTypeDesc,
	                      VertexHandle* pVertexOut) = 0;

	STDMETHOD(_GetVertices)(VertexHandle StartVertex,
	                        long nNumVertices,
	                        LPVOID pVertexOutBuffer,
	                        DWORD dwVertexTypeDesc) = 0;

	STDMETHOD(_SetVertices)(LPVOID pVertexInBuffer,
	                        DWORD dwVertexTypeDesc,
	                        VertexHandle StartVertex,
	                        VertexCount nNumVertices) = 0;

	STDMETHOD(_GetPoints)(D3DVECTOR* pPoints,
	                      VertexCount nBufferSize,
	                      VertexCount* pActualSize) = 0;

	STDMETHOD(_GetTriangleCount)(FaceCount* pNumTriangles) = 0;

	STDMETHOD(_GetTriangleList)(D3DVECTOR* pTriangleList,
	                            FaceCount* pTrianglesCopied) = 0;

	STDMETHOD(_AddVerticesToPrimitive)(PrimitiveIndex nPrimIdx,
	                                   LPVOID pVertices,
	                                   DWORD dwVertexTypeDesc,
	                                   VertexCount nNumVertices) = 0;

	STDMETHOD(_AttachVerticesToPrimitive)(PrimitiveIndex nPrimIdx,
	                                      WORD* pArray,
	                                      const VertexCount nNumVertices) = 0;
};

_COM_SMARTPTR_TYPEDEF(IMeshEx, IID_IMeshEx);

/////////////////////////////////////////////////////////////////////////
// ICameraEx

DECLARE_INTERFACE_(ICameraEx, ICamera)
{
	STDMETHOD(GetCameraState)(D3DVECTOR* pCameraPos,
	                          D3DVECTOR* pCameraAim,
	                          D3DVECTOR* pCameraUp) = 0;
	STDMETHOD(SetCameraState)(D3DVECTOR cameraPos,
	                          D3DVECTOR cameraAim,
	                          D3DVECTOR cameraUp) = 0;
	STDMETHOD(GetViewMatrix)(D3DXMATRIX* pViewMatrix) = 0;
};

_COM_SMARTPTR_TYPEDEF(ICameraEx, IID_ICameraEx);

/////////////////////////////////////////////////////////////////////////
// IDrawMeshDX8

DECLARE_INTERFACE_(IDrawMeshDX8, IUnknown)
{
	STDMETHOD(BeginDraw)(LPDIRECT3DDEVICE8 pDevice,
	                     IResourceManagerDX8* pDevResources,
	                     BOOL* pBuffered) = 0;

	STDMETHOD(DrawPrimitive)(LPDIRECT3DDEVICE8 pDevice,
	                         IResourceManagerDX8* pDevResources,
	                         PrimitiveIndex nPrimIdx) = 0;

	STDMETHOD(DrawPrimitiveBuffered)(LPDIRECT3DDEVICE8 pDevice,
	                                 IResourceManagerDX8* pDevResources,
	                                 PrimitiveIndex nPrimIdx) = 0;

	STDMETHOD(EndDraw)(LPDIRECT3DDEVICE8 pDevice, IResourceManagerDX8* pDevResources) = 0;

	STDMETHOD(Draw)(LPDIRECT3DDEVICE8 pDevice,
	                IResourceManagerDX8* pDevResources,
	                IContextProvider* pContextProvider) = 0;
};

_COM_SMARTPTR_TYPEDEF(IDrawMeshDX8, IID_IDrawMeshDX8);

/////////////////////////////////////////////////////////////////////////
// IInitInputDeviceDX8

DECLARE_INTERFACE_(IInitInputDeviceDX8, IUnknown)
{
	STDMETHOD(_InitObject)(LPDIRECTINPUT8 pDI8,
	                       LPCDIDEVICEINSTANCE pDeviceInfo) = 0;
};

_COM_SMARTPTR_TYPEDEF(IInitInputDeviceDX8, IID_IInitInputDeviceDX8);

/////////////////////////////////////////////////////////////////////////
// IInputDeviceProviderDX8

DECLARE_INTERFACE_(IInputDeviceProviderDX8, IUnknown)
{
	STDMETHOD(GetInputDevice)(enum InputDeviceCategory devCat,
	                          LPDIRECTINPUTDEVICE8* ppDIDevice) = 0;
};

_COM_SMARTPTR_TYPEDEF(IInputDeviceProviderDX8, IID_IInputDeviceProviderDX8);

/////////////////////////////////////////////////////////////////////////
// IInputDeviceUserDX8

DECLARE_INTERFACE_(IInputDeviceUserDX8, IUnknown)
{
	STDMETHOD(AcquireInputDevices)(IInputDeviceProviderDX8* pDevProvider) = 0;
	STDMETHOD(FreeInputDevices)() = 0;
};

_COM_SMARTPTR_TYPEDEF(IInputDeviceUserDX8, IID_IInputDeviceUserDX8);

/////////////////////////////////////////////////////////////////////////
// Smart pointer declarations

_COM_SMARTPTR_TYPEDEF(IPropertyContainer, __uuidof(IPropertyContainer));
_COM_SMARTPTR_TYPEDEF(IDeviceResource, __uuidof(IDeviceResource));
_COM_SMARTPTR_TYPEDEF(INode, __uuidof(INode));
_COM_SMARTPTR_TYPEDEF(ICompositeNode, __uuidof(ICompositeNode));
_COM_SMARTPTR_TYPEDEF(IXMLArchive, __uuidof(IXMLArchive));
_COM_SMARTPTR_TYPEDEF(IPersistXML, __uuidof(IPersistXML));
_COM_SMARTPTR_TYPEDEF(IWndMessageReceiver, __uuidof(IWndMessageReceiver));
_COM_SMARTPTR_TYPEDEF(IWndMessageSource, __uuidof(IWndMessageSource));
_COM_SMARTPTR_TYPEDEF(IDevice, __uuidof(IDevice));
_COM_SMARTPTR_TYPEDEF(IInputDevice, __uuidof(IInputDevice));
_COM_SMARTPTR_TYPEDEF(IMeasurementScale, __uuidof(IMeasurementScale));
_COM_SMARTPTR_TYPEDEF(IEnumNodes, __uuidof(IEnumNodes));
_COM_SMARTPTR_TYPEDEF(INodes, __uuidof(INodes));
_COM_SMARTPTR_TYPEDEF(IVisual, __uuidof(IVisual));
_COM_SMARTPTR_TYPEDEF(IFaceArray, __uuidof(IFaceArray));
_COM_SMARTPTR_TYPEDEF(IMesh, __uuidof(IMesh));
_COM_SMARTPTR_TYPEDEF(IMeshEditor, __uuidof(IMeshEditor));
_COM_SMARTPTR_TYPEDEF(ILight, __uuidof(ILight));
_COM_SMARTPTR_TYPEDEF(ISpotLight, __uuidof(ISpotLight));
_COM_SMARTPTR_TYPEDEF(IMaterial, __uuidof(IMaterial));
_COM_SMARTPTR_TYPEDEF(ITexture, __uuidof(ITexture));
_COM_SMARTPTR_TYPEDEF(ITextureStage, __uuidof(ITextureStage));
_COM_SMARTPTR_TYPEDEF(IRenderingContext, __uuidof(IRenderingContext));
_COM_SMARTPTR_TYPEDEF(IShape, __uuidof(IShape));
_COM_SMARTPTR_TYPEDEF(IModel, __uuidof(IModel));
_COM_SMARTPTR_TYPEDEF(IModelReader, __uuidof(IModelReader));
_COM_SMARTPTR_TYPEDEF(IModelWriter, __uuidof(IModelWriter));
_COM_SMARTPTR_TYPEDEF(IVisual2D, __uuidof(IVisual2D));
_COM_SMARTPTR_TYPEDEF(ICamera, __uuidof(ICamera));
_COM_SMARTPTR_TYPEDEF(IViewport, __uuidof(IViewport));
_COM_SMARTPTR_TYPEDEF(IAxisGuide, __uuidof(IAxisGuide));
_COM_SMARTPTR_TYPEDEF(IAnimate, __uuidof(IAnimate));
_COM_SMARTPTR_TYPEDEF(IAnimationLoop, __uuidof(IAnimationLoop));
_COM_SMARTPTR_TYPEDEF(ICommand, __uuidof(ICommand));
_COM_SMARTPTR_TYPEDEF(INodeCommand, __uuidof(INodeCommand));
_COM_SMARTPTR_TYPEDEF(IMoveCommand, __uuidof(IMoveCommand));
_COM_SMARTPTR_TYPEDEF(IRotateCommand, __uuidof(IRotateCommand));
_COM_SMARTPTR_TYPEDEF(IScaleCommand, __uuidof(IScaleCommand));
_COM_SMARTPTR_TYPEDEF(IMacroCommand, __uuidof(IMacroCommand));
_COM_SMARTPTR_TYPEDEF(ICommandDispatcher, __uuidof(ICommandDispatcher));
_COM_SMARTPTR_TYPEDEF(IBehavior, __uuidof(IBehavior));
_COM_SMARTPTR_TYPEDEF(ICameraFly, __uuidof(ICameraFly));
_COM_SMARTPTR_TYPEDEF(ICameraStep, __uuidof(ICameraStep));
_COM_SMARTPTR_TYPEDEF(ISelectNodes, __uuidof(ISelectNodes));
_COM_SMARTPTR_TYPEDEF(IMoveNodes, __uuidof(IMoveNodes));
_COM_SMARTPTR_TYPEDEF(IInsertDeleteNodes, __uuidof(IInsertDeleteNodes));
_COM_SMARTPTR_TYPEDEF(IController, __uuidof(IController));
_COM_SMARTPTR_TYPEDEF(IFirstPersonController, __uuidof(IFirstPersonController));
_COM_SMARTPTR_TYPEDEF(IEngine, __uuidof(IEngine));

/////////////////////////////////////////////////////////////////////////
// Error handling

namespace Error {

inline HRESULT WINAPI SetInfo(const CLSID& clsid,
                              const IID& iid,
                              HRESULT hRes,
                              UINT nDescID,
                              HINSTANCE hInst = _Module.m_hInst)
{
	return AtlSetErrorInfo(clsid, (LPCOLESTR)MAKEINTRESOURCE(nDescID), 0, NULL, iid, hRes, hInst);
}

inline HRESULT WINAPI SetInfo(const CLSID& clsid,
                              const IID& iid,
                              HRESULT hRes,
                              HINSTANCE hInst = _Module.m_hInst)
{
	UINT nDescID = HRESULT_CODE(hRes);
	return AtlSetErrorInfo(clsid, (LPCOLESTR)MAKEINTRESOURCE(nDescID), 0, NULL, iid, hRes, hInst);
}

// Error codes

// const HRESULT ERRORNAME = MAKE_HRESULT(1, FACILITY_ITF, 0x0200);

const HRESULT E_ENGINE_ALREADY_INIT = MAKE_HRESULT(1, FACILITY_ITF, IDS_E_ENGINE_ALREADY_INIT);
const HRESULT E_DIRECTX_INIT        = MAKE_HRESULT(1, FACILITY_ITF, IDS_E_DIRECTX_INIT);
const HRESULT E_WININET_INIT        = MAKE_HRESULT(1, FACILITY_ITF, IDS_E_WININET_INIT);
const HRESULT E_INIT_VIEWPORT       = MAKE_HRESULT(1, FACILITY_ITF, IDS_E_INIT_VIEWPORT);

};  // namespace Error

/////////////////////////////////////////////////////////////////////////
// Child notification codes

namespace ChildNotification {

const long ADD           = 0x00000001;
const long REMOVE        = 0x00000002;
const long REMOVE_ALL    = 0x00000004;
const long BOUNDS        = 0x00000008;

};  // namespace ChildNotification

};  // namespace ShapeShifter

#endif  // #ifndef __SHAPESHIFTER_H__
