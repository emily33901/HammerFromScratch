//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: Launcher for hammer, which is sitting in its own DLL
//
//===========================================================================//

#include "appframework/AppFramework.h"
#include "datacache/idatacache.h"
#include "datacache/imdlcache.h"
#include "filesystem.h"
#include "filesystem_init.h"
#include "ihammer.h"
#include "inputsystem/iinputsystem.h"
#include "istudiorender.h"
#include "materialsystem/imaterialsystem.h"
#include "tier0/dbg.h"
#include "tier0/icommandline.h"
#include "vgui/isurface.h"
#include "vgui/ivgui.h"
#include "vphysics_interface.h"
#include "vstdlib/cvar.h"
#include <direct.h>
#include <eh.h>
#include <stdlib.h>
#include <windows.h>
//#include "SteamWriteMinidump.h"

//-----------------------------------------------------------------------------
// Global systems
//-----------------------------------------------------------------------------
IHammer *g_pHammer;
IMaterialSystem *g_pMaterialSystem;
IFileSystem *g_pFileSystem;
IDataCache *g_pDataCache;
IInputSystem *g_pInputSystem;

extern "C" void WriteMiniDumpUsingExceptionInfo
( 
 unsigned int			uStructuredExceptionCode, 
struct _EXCEPTION_POINTERS * pExceptionInfo
	)
{
#ifndef NO_STEAM
	// TODO: dynamically set the minidump comment from contextual info about the crash (i.e current VPROF node)?
	//SteamWriteMiniDumpUsingExceptionInfoWithBuildId( uStructuredExceptionCode, pExceptionInfo, 0 );
#endif
}

//-----------------------------------------------------------------------------
// The application object
//-----------------------------------------------------------------------------
class CHammerApp : public CAppSystemGroup
{
public:
	// Methods of IApplication
	virtual bool Create( );
	virtual bool PreInit( );
	virtual int Main( );
	virtual void PostShutdown();
	virtual void Destroy();

private:
	int	MainLoop();
};


//-----------------------------------------------------------------------------
// Define the application object
//-----------------------------------------------------------------------------
CHammerApp	g_ApplicationObject;
DEFINE_WINDOWED_APPLICATION_OBJECT_GLOBALVAR( g_ApplicationObject );


//-----------------------------------------------------------------------------
// Create all singleton systems
//-----------------------------------------------------------------------------
bool CHammerApp::Create( )
{
	_chdir("..");

	// Save some memory so engine/hammer isn't so painful
	CommandLine()->AppendParm("-disallowhwmorph", NULL);
	//CommandLine_AppendParm("-disallowhwmorph");

	IAppSystem *pSystem;

	// Add in the cvar factory
	AppModule_t cvarModule = LoadModule( VStdLib_GetICVarFactory() );
	pSystem = AddSystem( cvarModule, CVAR_INTERFACE_VERSION );
	if ( !pSystem )
		return false;

	//const char *pFileSystemDLL = "forgery\\filesystem_steam.dll";

	bool bSteam = false;
	char pFileSystemDLL[MAX_PATH];
	if (FileSystem_GetFileSystemDLLName(pFileSystemDLL, MAX_PATH, bSteam) != FS_OK)
		return false;

	AppModule_t fileSystemModule = LoadModule("filesystem_stdio.dll");
	g_pFileSystem = (IFileSystem*)AddSystem( fileSystemModule, "VFileSystem022" );

	AppSystemInfo_t appSystems[] = 
	{
		{ "forgery\\materialsystem.dll",	MATERIAL_SYSTEM_INTERFACE_VERSION },
		//{ "materialsystem.dll",			"VMaterialSystem081" },
		{ "forgery\\inputsystem.dll",		INPUTSYSTEM_INTERFACE_VERSION },
		{ "forgery\\studiorender.dll",		STUDIO_RENDER_INTERFACE_VERSION },
		{ "vphysics.dll",					VPHYSICS_INTERFACE_VERSION },
		{ "forgery\\datacache.dll",			DATACACHE_INTERFACE_VERSION },
		{ "forgery\\datacache.dll",			MDLCACHE_INTERFACE_VERSION },
		{ "forgery\\datacache.dll",			STUDIO_DATA_CACHE_INTERFACE_VERSION },
		{ "forgery\\vguimatsurface.dll",	VGUI_SURFACE_INTERFACE_VERSION },
		{ "forgery\\vgui2.dll",				VGUI_IVGUI_INTERFACE_VERSION },
		{ "forgery\\hammer_dll.dll",		INTERFACEVERSION_HAMMER },
		{ "", "" }	// Required to terminate the list
	};

	if ( !AddSystems( appSystems ) ) 
		return false;

	// Connect to interfaces loaded in AddSystems that we need locally
	g_pMaterialSystem = (IMaterialSystem*)FindSystem( MATERIAL_SYSTEM_INTERFACE_VERSION );
	g_pHammer = (IHammer*)FindSystem( INTERFACEVERSION_HAMMER );
	g_pDataCache = (IDataCache*)FindSystem( DATACACHE_INTERFACE_VERSION );
	g_pInputSystem = (IInputSystem*)FindSystem( INPUTSYSTEM_INTERFACE_VERSION );

	// This has to be done before connection.
	g_pMaterialSystem->SetShaderAPI( "forgery\\shaderapidx9.dll" );

	return true;
}

void CHammerApp::Destroy()
{
	g_pFileSystem = NULL;
	g_pMaterialSystem = NULL;
	g_pDataCache = NULL;
	g_pHammer = NULL;
	g_pInputSystem = NULL;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
SpewRetval_t HammerSpewFunc( SpewType_t type, tchar const *pMsg )
{
	if ( type == SPEW_ASSERT )
	{
		return SPEW_DEBUGGER;
	}
	else if( type == SPEW_ERROR )
	{
		MessageBox( NULL, pMsg, "Hammer Error", MB_OK | MB_ICONSTOP );
		return SPEW_ABORT;
	}
	else
	{
		return SPEW_CONTINUE;
	}
}

//-----------------------------------------------------------------------------
// Init, shutdown
//-----------------------------------------------------------------------------
bool CHammerApp::PreInit( )
{
	SpewOutputFunc( HammerSpewFunc );
	if ( !g_pHammer->InitSessionGameConfig( GetVProjectCmdLineValue() ) )
		return false;

	bool bDone = false;
	do
	{
		CFSSteamSetupInfo steamInfo;
		steamInfo.m_pDirectoryName = g_pHammer->GetDefaultModFullPath();
		steamInfo.m_bOnlyUseDirectoryName = true;
		steamInfo.m_bToolsMode = true;
		steamInfo.m_bSetSteamDLLPath = true;
		steamInfo.m_bSteam = /*g_pFileSystem->IsSteam()*/ true;
		if ( FileSystem_SetupSteamEnvironment( steamInfo ) != FS_OK )
		{
			MessageBox( NULL, "Failed to setup steam environment.", "Error", MB_OK );
			return false;
		}

		// F1ssi0N:
		// becuase filesystem expects a sane file structure
		// and what we dont have is a sane file structure
		// we cant rely on it to mount content properly

		char vproject_path[MAX_PATH];
		Q_snprintf(vproject_path, MAX_PATH, "\"%s\"", steamInfo.m_pDirectoryName);

		CommandLine()->AppendParm("-vproject", vproject_path);
		FileSystem_UseVProjectBinDir(true);

		CFSSearchPathsInit searchPaths;

		searchPaths.m_bLowViolence = false;
		searchPaths.m_bMountHDContent = true;
		searchPaths.m_pDirectoryName = steamInfo.m_pDirectoryName;
		searchPaths.m_pFileSystem = g_pFileSystem;

		FileSystem_LoadSearchPaths(searchPaths);

		bDone = true;

	} while (!bDone);

	// Required to run through the editor
	g_pMaterialSystem->EnableEditorMaterials();

	// needed for VGUI model rendering
	g_pMaterialSystem->SetAdapter( 0, MATERIAL_INIT_ALLOCATE_FULLSCREEN_TEXTURE );

	return true; 
}

void CHammerApp::PostShutdown()
{
}


//-----------------------------------------------------------------------------
// main application
//-----------------------------------------------------------------------------
int CHammerApp::Main( )
{
	return g_pHammer->MainLoop();
}






