//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: SSCL.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f ShapeShifterps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>

namespace ShapeShifter {
#include "SSCL.h"
#include "SSCL_i.c"
};

#include "Engine.h"
#include "Model.h"
#include "Camera.h"
#include "Viewport.h"
#include "FaceArray.h"
#include "Mesh.h"
#include "RenderingContext.h"
#include "Shape.h"
#include "Group.h"
#include "Light.h"
#include "Controller.h"
#include "Behavior.h"
#include "Tracker.h"
#include "Material.h"
#include "Texture.h"
#include "TextureStage.h"
#include "NodeCollection.h"
#include "MeshEditor.h"
#include "WndMessageSource.h"
#include "Device.h"
#include "InputDevice.h"
#include "MeasurementScale.h"
#include "AnimationLoop.h"
#include "Command.h"
#include "XMLArchive.h"
#include "Namespace.h"
#include "ModelReader3DS.h"
#include "ModelReaderDXF.h"
#include "ModelReaderXML.h"
#include "ModelWriterXML.h"
#include "ShapeShifterCtl.h"
#include "ViewportDecorator.h"
#include "PropPages.h"
#include "BSPTree.h"

#define OBJECT_ENTRY_NON_CREATEABLE_EX(clsid, class) \
	{&clsid, class::UpdateRegistry, \
	NULL, NULL, NULL, 0, NULL, class::GetCategoryMap, class::ObjectMain },

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(ShapeShifter::CLSID_Engine, ShapeShifter::CEngine)
OBJECT_ENTRY(ShapeShifter::CLSID_Model, ShapeShifter::CModel)
OBJECT_ENTRY(ShapeShifter::CLSID_Camera, ShapeShifter::CCamera)
OBJECT_ENTRY(ShapeShifter::CLSID_Viewport, ShapeShifter::CViewport)
OBJECT_ENTRY(ShapeShifter::CLSID_FaceArray, ShapeShifter::CFaceArray)
OBJECT_ENTRY(ShapeShifter::CLSID_Mesh, ShapeShifter::CMesh)
OBJECT_ENTRY(ShapeShifter::CLSID_PrelitMesh, ShapeShifter::CPrelitMesh)
OBJECT_ENTRY(ShapeShifter::CLSID_RenderingContext, ShapeShifter::CRenderingContext)
OBJECT_ENTRY(ShapeShifter::CLSID_Shape, ShapeShifter::CShape)
OBJECT_ENTRY(ShapeShifter::CLSID_CompositeShape, ShapeShifter::CCompositeShape)
OBJECT_ENTRY(ShapeShifter::CLSID_Group, ShapeShifter::CGroup)
OBJECT_ENTRY(ShapeShifter::CLSID_DirectionalLight, ShapeShifter::CDirectionalLight)
OBJECT_ENTRY(ShapeShifter::CLSID_PointLight, ShapeShifter::CPointLight)
OBJECT_ENTRY(ShapeShifter::CLSID_SpotLight, ShapeShifter::CSpotLight)
OBJECT_ENTRY(ShapeShifter::CLSID_BrowseController, ShapeShifter::CBrowseController)
OBJECT_ENTRY(ShapeShifter::CLSID_BrowseEditController, ShapeShifter::CBrowseEditController)
OBJECT_ENTRY(ShapeShifter::CLSID_FlightController, ShapeShifter::CFlightController)
OBJECT_ENTRY(ShapeShifter::CLSID_Material, ShapeShifter::CMaterial)
OBJECT_ENTRY(ShapeShifter::CLSID_Texture, ShapeShifter::CTexture)
OBJECT_ENTRY(ShapeShifter::CLSID_TextureStage, ShapeShifter::CTextureStage)
OBJECT_ENTRY(ShapeShifter::CLSID_EnumNodeList, ShapeShifter::CEnumNodeList)
OBJECT_ENTRY(ShapeShifter::CLSID_EnumVariantNodeList, ShapeShifter::CEnumVariantNodeList)
OBJECT_ENTRY(ShapeShifter::CLSID_NodeList, ShapeShifter::CNodeList)
OBJECT_ENTRY(ShapeShifter::CLSID_WndMessageSource, ShapeShifter::CWndMessageSource)
OBJECT_ENTRY(ShapeShifter::CLSID_MeshEditor, ShapeShifter::CMeshEditor)
OBJECT_ENTRY_NON_CREATEABLE_EX(ShapeShifter::CLSID_Device, ShapeShifter::CDevice)
OBJECT_ENTRY(ShapeShifter::CLSID_MeasurementScale, ShapeShifter::CMeasurementScale)
OBJECT_ENTRY(ShapeShifter::CLSID_AnimationLoop, ShapeShifter::CAnimationLoop)
OBJECT_ENTRY_NON_CREATEABLE_EX(ShapeShifter::CLSID_CameraFly, ShapeShifter::CCameraFly)
OBJECT_ENTRY_NON_CREATEABLE_EX(ShapeShifter::CLSID_CameraStep, ShapeShifter::CCameraStep)
OBJECT_ENTRY_NON_CREATEABLE_EX(ShapeShifter::CLSID_MouseSelect, ShapeShifter::CMouseSelect)
OBJECT_ENTRY_NON_CREATEABLE_EX(ShapeShifter::CLSID_DragDrop, ShapeShifter::CDragDrop)
OBJECT_ENTRY_NON_CREATEABLE_EX(ShapeShifter::CLSID_InsertDeleteNodes, ShapeShifter::CInsertDeleteNodes)
OBJECT_ENTRY_NON_CREATEABLE_EX(ShapeShifter::CLSID_Tracker, ShapeShifter::CTracker)
OBJECT_ENTRY(ShapeShifter::CLSID_CreateShapeCommand, ShapeShifter::CCreateShapeCommand)
OBJECT_ENTRY(ShapeShifter::CLSID_InsertCommand, ShapeShifter::CInsertCommand)
OBJECT_ENTRY(ShapeShifter::CLSID_MoveCommand, ShapeShifter::CMoveCommand)
OBJECT_ENTRY(ShapeShifter::CLSID_RotateCommand, ShapeShifter::CRotateCommand)
OBJECT_ENTRY(ShapeShifter::CLSID_ScaleCommand, ShapeShifter::CScaleCommand)
OBJECT_ENTRY(ShapeShifter::CLSID_GroupCommand, ShapeShifter::CGroupCommand)
OBJECT_ENTRY(ShapeShifter::CLSID_UngroupCommand, ShapeShifter::CUngroupCommand)
OBJECT_ENTRY(ShapeShifter::CLSID_MacroCommand, ShapeShifter::CMacroCommand)
OBJECT_ENTRY(ShapeShifter::CLSID_XMLArchive, ShapeShifter::CXMLArchive)
OBJECT_ENTRY(ShapeShifter::CLSID_Namespace, ShapeShifter::CNamespace)
OBJECT_ENTRY(ShapeShifter::CLSID_ModelReader3DS, ShapeShifter::CModelReader3DS)
OBJECT_ENTRY(ShapeShifter::CLSID_ModelReaderDXF, ShapeShifter::CModelReaderDXF)
OBJECT_ENTRY(ShapeShifter::CLSID_ModelReaderXML, ShapeShifter::CModelReaderXML)
OBJECT_ENTRY(ShapeShifter::CLSID_ModelWriterXML, ShapeShifter::CModelWriterXML)
OBJECT_ENTRY(ShapeShifter::CLSID_ShapeShifterCtl, ShapeShifter::CShapeShifterCtl)
OBJECT_ENTRY_NON_CREATEABLE_EX(ShapeShifter::CLSID_AxisGuide, ShapeShifter::CAxisGuide)
OBJECT_ENTRY(ShapeShifter::CLSID_GeneralPropPage, ShapeShifter::CGeneralPropPage)
OBJECT_ENTRY(ShapeShifter::CLSID_ViewPropPage, ShapeShifter::CViewPropPage)
OBJECT_ENTRY(ShapeShifter::CLSID_TypesPropPage, ShapeShifter::CTypesPropPage)
OBJECT_ENTRY(ShapeShifter::CLSID_BehaviorPropPage, ShapeShifter::CBehaviorPropPage)
OBJECT_ENTRY_NON_CREATEABLE_EX(ShapeShifter::CLSID_BSPTree, ShapeShifter::CBSPTree)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        _Module.Init(ObjectMap, hInstance, &ShapeShifter::LIBID_ShapeShifter);
        DisableThreadLibraryCalls(hInstance);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
	{
        _Module.Term();
	}
    return TRUE;    // ok
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
    return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
    return _Module.UnregisterServer(TRUE);
}


