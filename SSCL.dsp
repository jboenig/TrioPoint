# Microsoft Developer Studio Project File - Name="SSCL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SSCL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SSCL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SSCL.mak" CFG="SSCL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SSCL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SSCL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SSCL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I ".\Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SSCL_INTERNET" /FR /Yu"stdafx.h" /FD /GZ /Zm200 /c
# ADD MTL /I ".\Include"
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Licensing\Debug\LicenseUtil.obj d3d8.lib d3dx8.lib dxguid.lib dxerr8.lib dinput8.lib kernel32.lib user32.lib gdi32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib winmm.lib wininet.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# Begin Custom Build - Performing registration
OutDir=.\Debug
WkspDir=.
TargetPath=.\Debug\SSCL.dll
TargetName=SSCL
InputPath=.\Debug\SSCL.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "$(TargetPath)" "$(WkspDir)\Bin" 
	regsvr32 /s /c "$(WkspDir)\Bin\$(TargetName)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "SSCL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SSCL___Win32_Release"
# PROP BASE Intermediate_Dir "SSCL___Win32_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O1 /I ".\Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /Yu"stdafx.h" /FD /Zm200 /c
# ADD MTL /I ".\Include"
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ddraw.lib dxguid.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib sxlmt.lib d3dx.lib winmm.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 Licensing\Release\LicenseUtil.obj d3d8.lib d3dx8.lib dxguid.lib dxerr8.lib dinput8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib d3dx.lib winmm.lib /nologo /subsystem:windows /dll /machine:I386
# Begin Custom Build - Performing registration
OutDir=.\Release
WkspDir=.
TargetPath=.\Release\SSCL.dll
TargetName=SSCL
InputPath=.\Release\SSCL.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "$(TargetPath)" "$(WkspDir)\Bin" 
	regsvr32 /s /c "$(WkspDir)\Bin\$(TargetName)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "SSCL - Win32 Debug"
# Name "SSCL - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Src\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\AnimationLoop.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Behavior.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\BSPTree.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Camera.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Command.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Controller.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Device.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Engine.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\EventSink.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\FaceArray.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Group.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\InputDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Internet.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\LicenseMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Light.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Material.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\MatrixStack.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Measurement.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\MeasurementScale.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Mesh.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\MeshEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Model.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\ModelReader3DS.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\ModelReaderDXF.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\ModelReaderXML.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\ModelWriterXML.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Namespace.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\PropPages.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\RenderingContext.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Shape.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\ShapeShifterCtl.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\ShapeShifterUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\SSCL.cpp
# End Source File
# Begin Source File

SOURCE=.\SSCL.def
# End Source File
# Begin Source File

SOURCE=.\SSCL.idl
# ADD MTL /tlb ".\SSCL.tlb" /h "Include/SSCL.h" /iid "SSCL_i.c" /Oicf
# End Source File
# Begin Source File

SOURCE=.\SSCL.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Src\Texture.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\TextureStage.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Tracker.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Viewport.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\ViewportDecorator.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Visual.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\WndMessageSource.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\XMLArchive.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Include\AboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\Include\AnimationLoop.h
# End Source File
# Begin Source File

SOURCE=.\Include\Behavior.h
# End Source File
# Begin Source File

SOURCE=.\Include\BSPTree.h
# End Source File
# Begin Source File

SOURCE=.\Include\Camera.h
# End Source File
# Begin Source File

SOURCE=.\Include\Command.h
# End Source File
# Begin Source File

SOURCE=.\Include\CommandDispatcher.h
# End Source File
# Begin Source File

SOURCE=.\Include\Controller.h
# End Source File
# Begin Source File

SOURCE=.\Include\ControllerStates.h
# End Source File
# Begin Source File

SOURCE=.\Include\ControllerUpdates.h
# End Source File
# Begin Source File

SOURCE=.\Include\Device.h
# End Source File
# Begin Source File

SOURCE=.\Include\DevicePresentation.h
# End Source File
# Begin Source File

SOURCE=.\Include\DynamicArray.h
# End Source File
# Begin Source File

SOURCE=.\Include\Engine.h
# End Source File
# Begin Source File

SOURCE=.\Include\EventSink.h
# End Source File
# Begin Source File

SOURCE=.\Include\FaceArray.h
# End Source File
# Begin Source File

SOURCE=.\Include\Geometry3d.h
# End Source File
# Begin Source File

SOURCE=.\Include\Group.h
# End Source File
# Begin Source File

SOURCE=.\Include\InputDevice.h
# End Source File
# Begin Source File

SOURCE=.\Include\Internet.h
# End Source File
# Begin Source File

SOURCE=.\Include\LicenseMgr.h
# End Source File
# Begin Source File

SOURCE=.\Licensing\LicenseUtil.h
# End Source File
# Begin Source File

SOURCE=.\Include\Light.h
# End Source File
# Begin Source File

SOURCE=.\Include\Material.h
# End Source File
# Begin Source File

SOURCE=.\Include\MatrixStack.h
# End Source File
# Begin Source File

SOURCE=.\Include\Measurement.h
# End Source File
# Begin Source File

SOURCE=.\Include\MeasurementScale.h
# End Source File
# Begin Source File

SOURCE=.\Include\Mesh.h
# End Source File
# Begin Source File

SOURCE=.\Include\MeshEditor.h
# End Source File
# Begin Source File

SOURCE=.\Include\Model.h
# End Source File
# Begin Source File

SOURCE=.\Include\ModelReader3DS.h
# End Source File
# Begin Source File

SOURCE=.\Include\ModelReaderDXF.h
# End Source File
# Begin Source File

SOURCE=.\Include\ModelReaderXML.h
# End Source File
# Begin Source File

SOURCE=.\Include\ModelWriterXML.h
# End Source File
# Begin Source File

SOURCE=.\Include\Namespace.h
# End Source File
# Begin Source File

SOURCE=.\Include\Node.h
# End Source File
# Begin Source File

SOURCE=.\Include\NodeCollection.h
# End Source File
# Begin Source File

SOURCE=.\Include\PersistXML.h
# End Source File
# Begin Source File

SOURCE=.\Include\Primitive.h
# End Source File
# Begin Source File

SOURCE=.\Include\Property.h
# End Source File
# Begin Source File

SOURCE=.\Include\PropPages.h
# End Source File
# Begin Source File

SOURCE=.\Include\RenderingContext.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Include\Shape.h
# End Source File
# Begin Source File

SOURCE=.\Include\ShapeShifter.h
# End Source File
# Begin Source File

SOURCE=.\Include\ShapeShifterCtl.h
# End Source File
# Begin Source File

SOURCE=.\Include\ShapeShifterDispId.h
# End Source File
# Begin Source File

SOURCE=.\Include\ShapeShifterUtil.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Include\StyleFlags.h
# End Source File
# Begin Source File

SOURCE=.\Include\Texture.h
# End Source File
# Begin Source File

SOURCE=.\Include\TextureStage.h
# End Source File
# Begin Source File

SOURCE=.\Include\Tracker.h
# End Source File
# Begin Source File

SOURCE=.\Include\Vertex3d.h
# End Source File
# Begin Source File

SOURCE=.\Include\Viewport.h
# End Source File
# Begin Source File

SOURCE=.\Include\ViewportDecorator.h
# End Source File
# Begin Source File

SOURCE=.\Include\Visual.h
# End Source File
# Begin Source File

SOURCE=.\Include\Visual2D.h
# End Source File
# Begin Source File

SOURCE=.\Include\VtRecord.h
# End Source File
# Begin Source File

SOURCE=.\Include\WndMessageSource.h
# End Source File
# Begin Source File

SOURCE=.\Include\XMLArchive.h
# End Source File
# Begin Source File

SOURCE=.\Include\XMLTags.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Res\AnimationLoop.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\AxisGuide.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\BehaviorPropPage.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\BrowseCtlr.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\BrowseEditCtlr.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\BSPTree.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\Camera.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\CameraFly.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\CameraStep.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\CompositeShape.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\CreateShapeCommand.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\DeleteCommand.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\Device.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\DirectionalLight.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\DragDrop.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\Engine.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\EnumNodeList.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\EnumVariantNodeList.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\FaceArray.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\FirstPersonController.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\FlightCtlr.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\GeneralPropPage.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\Group.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\GroupCommand.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\InputDevice.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\InsertCommand.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\InsertDeleteNodes.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\MacroCommand.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\Material.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\MeasurementScale.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\Mesh.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\MeshEditor.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\Model.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\ModelReader3DS.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\ModelReaderDXF.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\ModelReaderVRML.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\ModelReaderXML.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\ModelWriterXML.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\MouseSelect.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\MoveCommand.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\Namespace.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\NodeList.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\PointLight.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\PrelitMesh.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\RenderingContext.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\RotateCommand.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\ScaleCommand.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\Shape.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\ShapeShifter.ico
# End Source File
# Begin Source File

SOURCE=.\Res\ShapeShifterCtl.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\SpotLight.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\Texture.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\TextureStage.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\Tracker.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\TypesPropPage.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\UngroupCommand.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\Viewport.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\ViewPropPage.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\WndMessageSource.rgs
# End Source File
# Begin Source File

SOURCE=.\Res\XMLArchive.rgs
# End Source File
# End Group
# End Target
# End Project
