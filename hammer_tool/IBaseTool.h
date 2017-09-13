#pragma once


// interface that defines the cross dll border between hammer and tools

class CMapView2D;
class CMapView3D;
class CMapViewLogical;
class CRender2D;
class CRender3D;
class CMapDoc;
class CMapClass;
class Vector2D;
class CChunkFile;
class CSaveInfo;

typedef unsigned int        UINT;

enum ChunkFileResult_t;

class IBaseTool
{
public:

	//inline CBaseTool();
	virtual ~IBaseTool() = 0;

	//
	// Called by the tool manager to activate/deactivate tools.
	//

	virtual void Init(CMapDoc *pDocument) = 0;;

	virtual void Activate() = 0;
	virtual void Deactivate() = 0;
	virtual bool CanDeactivate(void) = 0;

	virtual bool IsTranslating(void) = 0;	// return true if tool is currently changing objects
	virtual bool IsActiveTool(void) = 0;

	// true if tool has objects to work on
	virtual bool IsEmpty() = 0;

	// detach tool from any object working on
	virtual void SetEmpty() = 0;

	// attach a certain object to that tool
	virtual void Attach(CMapClass *pObject) = 0;

	//
	// Methods for hammer to find information about this tool
	//
	virtual const char *GetToolName() = 0;

	//
	// Notifications for tool activation/deactivation.
	//
	virtual void OnActivate() = 0;
	virtual void OnDeactivate() = 0;

	virtual const char* GetVMFChunkName() = 0;
	virtual ChunkFileResult_t LoadVMF(CChunkFile *pFile) = 0;
	virtual ChunkFileResult_t SaveVMF(CChunkFile *pFile, CSaveInfo *pSaveInfo) = 0;
	//
	// Messages sent by the 3D view:
	//
	virtual bool OnContextMenu3D(CMapView3D *pView, UINT nFlags, const Vector2D &vPoint) = 0;
	virtual bool OnLMouseDown3D(CMapView3D *pView, UINT nFlags, const Vector2D &vPoint) = 0;
	virtual bool OnLMouseUp3D(CMapView3D *pView, UINT nFlags, const Vector2D &vPoint) = 0;
	virtual bool OnLMouseDblClk3D(CMapView3D *pView, UINT nFlags, const Vector2D &vPoint) = 0;
	virtual bool OnRMouseDown3D(CMapView3D *pView, UINT nFlags, const Vector2D &vPoint) = 0;
	virtual bool OnRMouseUp3D(CMapView3D *pView, UINT nFlags, const Vector2D &vPoint) = 0;
	virtual bool OnMouseMove3D(CMapView3D *pView, UINT nFlags, const Vector2D &vPoint) = 0;
	virtual bool OnMouseWheel3D(CMapView3D *pView, UINT nFlags, short zDelta, const Vector2D &vPoint) = 0;

	virtual bool OnKeyDown3D(CMapView3D *pView, UINT nChar, UINT nRepCnt, UINT nFlags) = 0;
	virtual bool OnKeyUp3D(CMapView3D *pView, UINT nChar, UINT nRepCnt, UINT nFlags) = 0;
	virtual bool OnChar3D(CMapView3D *pView, UINT nChar, UINT nRepCnt, UINT nFlags) = 0;

	//
	// Messages sent by the 2D view:
	//
	virtual bool OnContextMenu2D(CMapView2D *pView, UINT nFlags, const Vector2D &vPoint) = 0;
	virtual bool OnLMouseDown2D(CMapView2D *pView, UINT nFlags, const Vector2D &vPoint) = 0;
	virtual bool OnLMouseUp2D(CMapView2D *pView, UINT nFlags, const Vector2D &vPoint) = 0;
	virtual bool OnLMouseDblClk2D(CMapView2D *pView, UINT nFlags, const Vector2D &vPoint) = 0;
	virtual bool OnRMouseDown2D(CMapView2D *pView, UINT nFlags, const Vector2D &vPoint) = 0;
	virtual bool OnRMouseUp2D(CMapView2D *pView, UINT nFlags, const Vector2D &vPoint) = 0;
	virtual bool OnMouseMove2D(CMapView2D *pView, UINT nFlags, const Vector2D &vPoint) = 0;
	virtual bool OnMouseWheel2D(CMapView2D *pView, UINT nFlags, short zDelta, const Vector2D &vPoint) = 0;

	virtual bool OnKeyDown2D(CMapView2D *pView, UINT nChar, UINT nRepCnt, UINT nFlags) = 0;
	virtual bool OnKeyUp2D(CMapView2D *pView, UINT nChar, UINT nRepCnt, UINT nFlags) = 0;
	virtual bool OnChar2D(CMapView2D *pView, UINT nChar, UINT nRepCnt, UINT nFlags) = 0;

	//
	// Messages sent by the logical view:
	//
	virtual bool OnContextMenuLogical(CMapViewLogical *pView, UINT nFlags, const Vector2D &vPoint) = 0;
	virtual bool OnLMouseDownLogical(CMapViewLogical *pView, UINT nFlags, const Vector2D &vPoint) = 0;
	virtual bool OnLMouseUpLogical(CMapViewLogical *pView, UINT nFlags, const Vector2D &vPoint) = 0;
	virtual bool OnLMouseDblClkLogical(CMapViewLogical *pView, UINT nFlags, const Vector2D &vPoint) = 0;
	virtual bool OnRMouseDownLogical(CMapViewLogical *pView, UINT nFlags, const Vector2D &vPoint) = 0;
	virtual bool OnRMouseUpLogical(CMapViewLogical *pView, UINT nFlags, const Vector2D &vPoint) = 0;
	virtual bool OnMouseMoveLogical(CMapViewLogical *pView, UINT nFlags, const Vector2D &vPoint) = 0;
	virtual bool OnMouseWheelLogical(CMapViewLogical *pView, UINT nFlags, short zDelta, const Vector2D &vPoint) = 0;

	virtual bool OnKeyDownLogical(CMapViewLogical *pView, UINT nChar, UINT nRepCnt, UINT nFlags) = 0;
	virtual bool OnKeyUpLogical(CMapViewLogical *pView, UINT nChar, UINT nRepCnt, UINT nFlags) = 0;
	virtual bool OnCharLogical(CMapViewLogical *pView, UINT nChar, UINT nRepCnt, UINT nFlags) = 0;

	//
	// Rendering.
	//
	virtual void RenderTool2D(CRender2D *pRender) = 0;
	virtual void RenderToolLogical(CRender2D *pRender) = 0;
	virtual void RenderTool3D(CRender3D *pRender) = 0;
	virtual void UpdateStatusBar(void) = 0;
};

#ifdef HAMMER_TOOL
IBaseTool *GetTool();
#define EXPORT_TOOL(type) \
static type __g_tool; \
IBaseTool *GetTool() {return &__g_tool;}
#endif