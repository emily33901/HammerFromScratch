#include "..\hammer_tool\stdafx.h"

#define HAMMER_TOOL

#include "CBaseTool.h"

#include "..\hammer\mapdoc.h"
#include "..\hammer\mapview.h"
#include "..\hammer\mapview2d.h"
#include "..\hammer\mapview3d.h"
#include "..\hammer\mapface.h"
#include "..\hammer\mapsolid.h"
#include "..\hammer\toolmanager.h"

#include "..\hammer\mainfrm.h"
#include "..\hammer\statusbarids.h"


// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

class CTestTool : public CBaseTool
{
	const char *GetName() { return "Test Tool"; }

	bool OnLMouseDown2D(CMapView2D *pView, UINT nFlags, const Vector2D &vPoint) override;
	bool OnLMouseDown3D(CMapView3D *pView, UINT nFlags, const Vector2D &vPoint) override;

	void UpdateStatusBar() override;
};

EXPORT_TOOL(CTestTool);


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : pView - 
//			nFlags - 
//			point - 
// Output : Returns true if the message was handled, false if not.
//-----------------------------------------------------------------------------
bool CTestTool::OnLMouseDown2D(CMapView2D *pView, UINT nFlags, const Vector2D &vPoint)
{
	if (nFlags & MK_CONTROL)
	{
		//
		// CONTROL is down, perform selection only.
		//
		pView->SelectAt(vPoint, false, true);
	}
	else
	{
		pView->SelectAt(vPoint, true, true);
	}

	return (true);
}


//-----------------------------------------------------------------------------
// Purpose: Handles left mouse button down events in the 3D view.
// Input  : Per CWnd::OnLButtonDown.
// Output : Returns true if the message was handled, false if not.
//-----------------------------------------------------------------------------
bool CTestTool::OnLMouseDown3D(CMapView3D *pView, UINT nFlags, const Vector2D &vPoint)
{
	CMapDoc *pDoc = pView->GetMapDoc();
	if (pDoc == NULL)
	{
		return false;
	}

	bool bShift = ((GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0);

	ULONG ulFace;
	CMapClass *pObject = pView->NearestObjectAt(vPoint, ulFace);

	if ((pObject != NULL) && (pObject->IsMapClass(MAPCLASS_TYPE(CMapSolid))))
	{
		CMapSolid *pSolid = (CMapSolid *)pObject;

		int cmd = scToggle | scClear;

		// No clear if CTRL pressed.
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		{
			cmd &= ~scClear;
		}

		// If they are holding down SHIFT, select the entire solid.
		if (bShift)
		{
			pDoc->SelectFace(pSolid, -1, cmd);
		}
		// Otherwise, select a single face.
		else
		{
			pDoc->SelectFace(pSolid, ulFace, cmd);
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTestTool::UpdateStatusBar()
{
	CString str;
	str.Format("%d faces selected", m_pDocument->GetSelection()->GetCount());
	SetStatusText(SBI_SELECTION, str);
	SetStatusText(SBI_SIZE, "");
}