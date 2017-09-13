//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: Defines the interface that tools implement to allow views to call
//			through them.
//
//===========================================================================//

#ifndef TOOLINTERFACE_H
#define TOOLINTERFACE_H
#ifdef _WIN32
#pragma once
#endif

#include "IBaseTool.h"

enum
{
	constrainNone = 0x00,	// transformation with no constrains
	constrainOnlyHorz = 0x01,	// only horizontal translations
	constrainOnlyVert = 0x02, // only vertical translations
	constrainSnap = 0x04,	// rounds to document snap grid
	constrainIntSnap = 0x08,	// rounds value to one unit (integer)
	constrainHalfSnap = 0x10, // rounds to half of snap grid
	constrainCenter = 0x20,
	constrainMoveAll = 0x40, // translate all handles
};


class CBaseTool : public IBaseTool
{
public:

	inline CBaseTool();
	virtual ~CBaseTool() override {}

	//
	// Called by the tool manager to activate/deactivate tools.
	//

	virtual void Init(CMapDoc *pDocument) override;

	void Activate() override;
	void Deactivate() override;
	virtual bool CanDeactivate(void) override { return true; }

	virtual bool IsTranslating(void) override { return false; }	// return true if tool is currently changing objects
	inline bool IsActiveTool(void) override { return m_bActiveTool; }

	// true if tool has objects to work on
	virtual bool IsEmpty() override { return m_bEmpty; }

	// detach tool from any object working on
	virtual void SetEmpty() override { m_bEmpty = true; }

	// attach a certain object to that tool
	virtual void Attach(CMapClass *pObject) override {};

	//
	// Methods for hammer to find information about this tool
	//
	virtual const char *GetToolName() override { return ""; }

	//
	// Notifications for tool activation/deactivation.
	//
	virtual void OnActivate() override {}
	virtual void OnDeactivate() override {}

	virtual const char* GetVMFChunkName() { return nullptr; }
	virtual ChunkFileResult_t LoadVMF(CChunkFile *pFile) { return (ChunkFileResult_t)0; /*ChunkFile_Ok*/ }
	virtual ChunkFileResult_t SaveVMF(CChunkFile *pFile, CSaveInfo *pSaveInfo) { return (ChunkFileResult_t)0; /*ChunkFile_Ok*/ }
	//
	// Messages sent by the 3D view:
	//
	virtual bool OnContextMenu3D(CMapView3D *pView, UINT nFlags, const Vector2D &vPoint) { return false; }
	virtual bool OnLMouseDown3D(CMapView3D *pView, UINT nFlags, const Vector2D &vPoint) { return false; }
	virtual bool OnLMouseUp3D(CMapView3D *pView, UINT nFlags, const Vector2D &vPoint) { return false; }
	virtual bool OnLMouseDblClk3D(CMapView3D *pView, UINT nFlags, const Vector2D &vPoint) { return false; }
	virtual bool OnRMouseDown3D(CMapView3D *pView, UINT nFlags, const Vector2D &vPoint) { return false; }
	virtual bool OnRMouseUp3D(CMapView3D *pView, UINT nFlags, const Vector2D &vPoint) { return false; }
	virtual bool OnMouseMove3D(CMapView3D *pView, UINT nFlags, const Vector2D &vPoint) { return false; }
	virtual bool OnMouseWheel3D(CMapView3D *pView, UINT nFlags, short zDelta, const Vector2D &vPoint) { return false; }

	virtual bool OnKeyDown3D(CMapView3D *pView, UINT nChar, UINT nRepCnt, UINT nFlags) { return false; }
	virtual bool OnKeyUp3D(CMapView3D *pView, UINT nChar, UINT nRepCnt, UINT nFlags) { return false; }
	virtual bool OnChar3D(CMapView3D *pView, UINT nChar, UINT nRepCnt, UINT nFlags) { return false; }

	//
	// Messages sent by the 2D view:
	//
	virtual bool OnContextMenu2D(CMapView2D *pView, UINT nFlags, const Vector2D &vPoint) { return false; }
	virtual bool OnLMouseDown2D(CMapView2D *pView, UINT nFlags, const Vector2D &vPoint) { return false; }
	virtual bool OnLMouseUp2D(CMapView2D *pView, UINT nFlags, const Vector2D &vPoint) { return false; }
	virtual bool OnLMouseDblClk2D(CMapView2D *pView, UINT nFlags, const Vector2D &vPoint) { return false; }
	virtual bool OnRMouseDown2D(CMapView2D *pView, UINT nFlags, const Vector2D &vPoint) { return false; }
	virtual bool OnRMouseUp2D(CMapView2D *pView, UINT nFlags, const Vector2D &vPoint) { return false; }
	virtual bool OnMouseMove2D(CMapView2D *pView, UINT nFlags, const Vector2D &vPoint) { return true; }
	virtual bool OnMouseWheel2D(CMapView2D *pView, UINT nFlags, short zDelta, const Vector2D &vPoint) { return false; }

	virtual bool OnKeyDown2D(CMapView2D *pView, UINT nChar, UINT nRepCnt, UINT nFlags) { return false; }
	virtual bool OnKeyUp2D(CMapView2D *pView, UINT nChar, UINT nRepCnt, UINT nFlags) { return false; }
	virtual bool OnChar2D(CMapView2D *pView, UINT nChar, UINT nRepCnt, UINT nFlags) { return false; }

	//
	// Messages sent by the logical view:
	//
	virtual bool OnContextMenuLogical(CMapViewLogical *pView, UINT nFlags, const Vector2D &vPoint) { return false; }
	virtual bool OnLMouseDownLogical(CMapViewLogical *pView, UINT nFlags, const Vector2D &vPoint) { return false; }
	virtual bool OnLMouseUpLogical(CMapViewLogical *pView, UINT nFlags, const Vector2D &vPoint) { return false; }
	virtual bool OnLMouseDblClkLogical(CMapViewLogical *pView, UINT nFlags, const Vector2D &vPoint) { return false; }
	virtual bool OnRMouseDownLogical(CMapViewLogical *pView, UINT nFlags, const Vector2D &vPoint) { return false; }
	virtual bool OnRMouseUpLogical(CMapViewLogical *pView, UINT nFlags, const Vector2D &vPoint) { return false; }
	virtual bool OnMouseMoveLogical(CMapViewLogical *pView, UINT nFlags, const Vector2D &vPoint) { return true; }
	virtual bool OnMouseWheelLogical(CMapViewLogical *pView, UINT nFlags, short zDelta, const Vector2D &vPoint) { return false; }

	virtual bool OnKeyDownLogical(CMapViewLogical *pView, UINT nChar, UINT nRepCnt, UINT nFlags) { return false; }
	virtual bool OnKeyUpLogical(CMapViewLogical *pView, UINT nChar, UINT nRepCnt, UINT nFlags) { return false; }
	virtual bool OnCharLogical(CMapViewLogical *pView, UINT nChar, UINT nRepCnt, UINT nFlags) { return false; }

	//
	// Rendering.
	//
	virtual void RenderTool2D(CRender2D *pRender) {}
	virtual void RenderToolLogical(CRender2D *pRender) {}
	virtual void RenderTool3D(CRender3D *pRender) {}
	virtual void UpdateStatusBar(void) {}

protected:

	bool m_bActiveTool;		// Set to true when this is the active tool.
	bool m_bEmpty;		// true if the tool has objects to work on
	CMapDoc *m_pDocument;
};


//-----------------------------------------------------------------------------
// Purpose: Constructor.
//-----------------------------------------------------------------------------
CBaseTool::CBaseTool()
{
	m_bEmpty = true;
	m_bActiveTool = false;
	m_pDocument = nullptr;
}

#endif // TOOLINTERFACE_H
