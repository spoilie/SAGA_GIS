
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    User Interface                     //
//                                                       //
//                    Program: SAGA                      //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                SAGA_Frame_Layout.cpp                  //
//                                                       //
//          Copyright (C) 2005 by Olaf Conrad            //
//                                                       //
//-------------------------------------------------------//
//                                                       //
// This file is part of 'SAGA - System for Automated     //
// Geoscientific Analyses'. SAGA is free software; you   //
// can redistribute it and/or modify it under the terms  //
// of the GNU General Public License as published by the //
// Free Software Foundation; version 2 of the License.   //
//                                                       //
// SAGA is distributed in the hope that it will be       //
// useful, but WITHOUT ANY WARRANTY; without even the    //
// implied warranty of MERCHANTABILITY or FITNESS FOR A  //
// PARTICULAR PURPOSE. See the GNU General Public        //
// License for more details.                             //
//                                                       //
// You should have received a copy of the GNU General    //
// Public License along with this program; if not,       //
// write to the Free Software Foundation, Inc.,          //
// 59 Temple Place - Suite 330, Boston, MA 02111-1307,   //
// USA.                                                  //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//    contact:    Olaf Conrad                            //
//                Institute of Geography                 //
//                University of Goettingen               //
//                Goldschmidtstr. 5                      //
//                37077 Goettingen                       //
//                Germany                                //
//                                                       //
//    e-mail:     oconrad@saga-gis.org                   //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "res_commands.h"
#include "res_controls.h"
#include "res_images.h"

#include "helper.h"

#include "saga_frame.h"
#include "saga_frame_layout.h"

//---------------------------------------------------------
#include <wx/toolbar.h>

//---------------------------------------------------------
#if defined(__SAGA_GUI_USE_wxFL__)

	#include <wx/dcscreen.h>
	#include <wx/dcmemory.h>
	#include <wx/fl/controlbar.h>	// core API
	#include <wx/fl/barhintspl.h>	// bevel for bars with "X"s and grooves
	#include <wx/fl/rowdragpl.h>	// NC-look with draggable rows
	#include <wx/fl/cbcustom.h>		// customization plugin
	#include <wx/fl/antiflickpl.h>	// double-buffered repaint of decorations
	#include <wx/fl/hintanimpl.h>
	#include <wx/fl/panedrawpl.h>
	#include <wx/fl/gcupdatesmgr.h>
	#include <wx/fl/dyntbarhnd.h>
	#include <wx/fl/dyntbar.h>

#elif defined(__SAGA_GUI_USE_wxDOCKIT__)

	#include "wx/layoutmanager.h"
	#include "wx/dockwindow.h"
	#include "wx/dockhost.h"
	#include "wx/pane.h"
	#include "wx/util.h"
	#include "wx/slidebar.h"
	#include "wx/toolbutton.h"

#endif


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define SAGA_BAR_STATES			4

//---------------------------------------------------------
#if defined(__SAGA_GUI_USE_wxFL__)

	#define SAGA_BAR_DOCKED_H	wxCBAR_DOCKED_HORIZONTALLY
	#define SAGA_BAR_DOCKED_V	wxCBAR_DOCKED_VERTICALLY
	#define SAGA_BAR_FLOATING	wxCBAR_FLOATING
	#define SAGA_BAR_HIDDEN		wxCBAR_HIDDEN

	#define SAGA_BAR_ALIGN_L	FL_ALIGN_LEFT
	#define SAGA_BAR_ALIGN_R	FL_ALIGN_RIGHT
	#define SAGA_BAR_ALIGN_T	FL_ALIGN_TOP
	#define SAGA_BAR_ALIGN_B	FL_ALIGN_BOTTOM

#elif defined(__SAGA_GUI_USE_wxDOCKIT__)

	#define SAGA_BAR_DOCKED_H	0
	#define SAGA_BAR_DOCKED_V	1
	#define SAGA_BAR_FLOATING	2
	#define SAGA_BAR_HIDDEN		3

	#define SAGA_BAR_ALIGN_T	0
	#define SAGA_BAR_ALIGN_B	1
	#define SAGA_BAR_ALIGN_L	2
	#define SAGA_BAR_ALIGN_R	3

#endif

//---------------------------------------------------------
#define SAGA_BAR_DEF_X			10
#define SAGA_BAR_DEF_Y			10
#define SAGA_BAR_DEF_NX			300
#define SAGA_BAR_DEF_NY			300

//---------------------------------------------------------
#define TOOLBAR_SIZE_IMG		16
#define TOOLBAR_SIZE_GAP		4

//---------------------------------------------------------
#define CONFIG_READ(ENTRY, KEY, TMP, VALUE, DEFAULT)	VALUE	= CONFIG_Read(ENTRY, KEY, TMP) ? TMP : (DEFAULT);


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
const char *	SAGA_GUI_Get_Version_FrameLayout	(void)
{
	static wxString	s;

#if defined(__SAGA_GUI_USE_wxFL__)

	s.Printf("wxFrameLayout %f", WXCONTROLBAR_VERSION);

#elif defined(__SAGA_GUI_USE_wxDOCKIT__)

	s.Printf("wxDockit 2.1");

#endif

	return( s );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSAGA_Frame_Layout::CSAGA_Frame_Layout(wxMDIParentFrame *pFrame)
{
	long	l;
	wxRect	r;

	//-----------------------------------------------------
	m_pFrame	= pFrame;

	m_nBars		= 0;
	m_Bars		= NULL;

	_Bar_Initialise();

	//-----------------------------------------------------
#if defined(__SAGA_GUI_USE_wxFL__)

	m_pLayout	= new wxFrameLayout(m_pFrame, m_pFrame->GetClientWindow());
	m_pLayout	->SetUpdatesManager(new cbGCUpdatesMgr());
	m_pLayout	->PushDefaultPlugins();
	m_pLayout	->AddPlugin(CLASSINFO(cbPaneDrawPlugin));
	m_pLayout	->AddPlugin(CLASSINFO(cbBarHintsPlugin), FL_ALIGN_BOTTOM_PANE|FL_ALIGN_LEFT_PANE|FL_ALIGN_RIGHT_PANE);
	m_pLayout	->AddPlugin(CLASSINFO(cbHintAnimationPlugin));
	m_pLayout	->AddPlugin(CLASSINFO(cbRowDragPlugin));
	m_pLayout	->AddPlugin(CLASSINFO(cbAntiflickerPlugin));
	m_pLayout	->AddPlugin(CLASSINFO(cbSimpleCustomizationPlugin));

#elif defined(__SAGA_GUI_USE_wxDOCKIT__)

	m_pSlideBar	= new wxSlideBar(m_pFrame, 0);
	m_pSlideBar	->SetMode(wxSLIDE_MODE_COMPACT);	// wxSLIDE_MODE_SIMPLE);

	m_pLayout	= new wxLayoutManager(m_pFrame);
	m_pLayout	->AddDefaultHosts();

	m_pLayout	->GetDockHost(wxDEFAULT_TOP_HOST   )->SetAreaSize(CONFIG_Read("/FL/MAIN", "BAR_SIZE_TOP"   , l) ? l : 127);
	m_pLayout	->GetDockHost(wxDEFAULT_BOTTOM_HOST)->SetAreaSize(CONFIG_Read("/FL/MAIN", "BAR_SIZE_BOTTOM", l) ? l : 127);
	m_pLayout	->GetDockHost(wxDEFAULT_LEFT_HOST  )->SetAreaSize(CONFIG_Read("/FL/MAIN", "BAR_SIZE_LEFT"  , l) ? l : 127);
	m_pLayout	->GetDockHost(wxDEFAULT_RIGHT_HOST )->SetAreaSize(CONFIG_Read("/FL/MAIN", "BAR_SIZE_RIGHT" , l) ? l : 127);

#endif

	//-----------------------------------------------------
	r.SetX		(CONFIG_Read("/FL/MAIN", "POS_X" , l) ? l : m_pFrame->GetPosition().x);
	r.SetY		(CONFIG_Read("/FL/MAIN", "POS_Y" , l) ? l : m_pFrame->GetPosition().y);
	r.SetWidth	(CONFIG_Read("/FL/MAIN", "SIZE_X", l) ? l : m_pFrame->GetSize().x);
	r.SetHeight	(CONFIG_Read("/FL/MAIN", "SIZE_Y", l) ? l : m_pFrame->GetSize().y);

	m_pFrame->SetSize(r);

	if( !(CONFIG_Read("/FL/MAIN", "STATE", l) && l == 0) )
	{
		m_pFrame->Maximize();
	}
}

//---------------------------------------------------------
CSAGA_Frame_Layout::~CSAGA_Frame_Layout(void)
{
	//-----------------------------------------------------
	if( m_pFrame->IsIconized() )
	{
		m_pFrame->Iconize(false);
	}

	if( m_pFrame->IsMaximized() )
	{
		CONFIG_Write("/FL/MAIN", "STATE" , (long)1);
	}
	else
	{
		CONFIG_Write("/FL/MAIN", "STATE" , (long)0);
		CONFIG_Write("/FL/MAIN", "POS_X" , (long)m_pFrame->GetPosition().x);
		CONFIG_Write("/FL/MAIN", "POS_Y" , (long)m_pFrame->GetPosition().y);
		CONFIG_Write("/FL/MAIN", "SIZE_X", (long)m_pFrame->GetSize().x);
		CONFIG_Write("/FL/MAIN", "SIZE_Y", (long)m_pFrame->GetSize().y);
	}

#if defined(__SAGA_GUI_USE_wxDOCKIT__)

	CONFIG_Write("/FL/MAIN", "BAR_SIZE_TOP"   , (long)m_pLayout->GetDockHost(wxDEFAULT_TOP_HOST   )->GetAreaSize());
	CONFIG_Write("/FL/MAIN", "BAR_SIZE_BOTTOM", (long)m_pLayout->GetDockHost(wxDEFAULT_BOTTOM_HOST)->GetAreaSize());
	CONFIG_Write("/FL/MAIN", "BAR_SIZE_LEFT"  , (long)m_pLayout->GetDockHost(wxDEFAULT_LEFT_HOST  )->GetAreaSize());
	CONFIG_Write("/FL/MAIN", "BAR_SIZE_RIGHT" , (long)m_pLayout->GetDockHost(wxDEFAULT_RIGHT_HOST )->GetAreaSize());

#endif

	//-----------------------------------------------------
	if( m_nBars > 0 )
	{
		for(int i=0; i<m_nBars; i++)
		{
			_Bar_Position_Write(m_Bars[i]);
		}

		free(m_Bars);
	}

	delete(m_pLayout);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CSAGA_Frame_Layout::Show_Initially(void)
{
	m_pFrame->Show(true);

#if defined(__SAGA_GUI_USE_wxFL__)

	m_pFrame->SendSizeEvent();

	m_pLayout->RefreshNow();

#endif
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSAGA_Frame_Layout::TBar * CSAGA_Frame_Layout::_Bar_Find(wxWindow *pWindow)
{
	for(int i=0; i<m_nBars; i++)
	{
		if( pWindow == m_Bars[i].pWnd )
		{
			return( m_Bars + i );
		}
	}

	return( NULL );
}

//---------------------------------------------------------
void CSAGA_Frame_Layout::_Bar_Initialise(void)
{
	long		l;
	wxString	Entries;

	if( !CONFIG_Read("/FL/MAIN", "STATE", l) )
	{
		Entries.Printf("/FL/BAR%d", ID_WND_ACTIVE);
		CONFIG_Write(Entries, "STATE", (long)SAGA_BAR_DOCKED_V);
		CONFIG_Write(Entries, "ALIGN", (long)SAGA_BAR_ALIGN_R);

		Entries.Printf("/FL/BAR%d", ID_WND_INFO);
		CONFIG_Write(Entries, "STATE", (long)SAGA_BAR_DOCKED_H);
		CONFIG_Write(Entries, "ALIGN", (long)SAGA_BAR_ALIGN_B);

		Entries.Printf("/FL/BAR%d", ID_WND_WKSP);
		CONFIG_Write(Entries, "STATE", (long)SAGA_BAR_DOCKED_V);
		CONFIG_Write(Entries, "ALIGN", (long)SAGA_BAR_ALIGN_L);
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CSAGA_Frame_Layout::Bar_Add(wxWindow *pWindow, wxString Name)
{
	_Bar_Position_Read(pWindow, Name, false);
}

//---------------------------------------------------------
void CSAGA_Frame_Layout::_Bar_Position_Read(wxWindow *pWindow, wxString Name, bool bToolbar)
{
	int			State, Alignment, nx, ny;
	long		l;
	wxString	Entries	= wxString::Format("/FL/BAR%d", pWindow->GetId());
	TBar		*pBar;

	m_nBars++;
	m_Bars			= (TBar *)realloc(m_Bars, m_nBars * sizeof(TBar));
	pBar			= m_Bars + m_nBars - 1;
	pBar->pWnd		= pWindow;

#if defined(__SAGA_GUI_USE_wxFL__)

	int			RowNo;

	nx			= !bToolbar ? 100 : 10;
	ny			= !bToolbar ? 100 : TOOLBAR_SIZE_IMG + 2 * (6 + TOOLBAR_SIZE_GAP);

	cbDimInfo	DimInfo(nx, ny, ny, nx, nx, ny, false, 6, 6, !bToolbar ? NULL : new cbDynToolBarDimHandler());

	CONFIG_READ(Entries, "STATE" , l, State		, SAGA_BAR_DOCKED_H);
	CONFIG_READ(Entries, "ALIGN" , l, Alignment	, bToolbar ? SAGA_BAR_ALIGN_T : SAGA_BAR_ALIGN_B);
	CONFIG_READ(Entries, "ROWNO" , l, RowNo		, 0);

	for(int i=0; i<SAGA_BAR_STATES; i++)
	{
		CONFIG_READ(Entries, wxString::Format("%d_POS_X" , i), l, DimInfo.mBounds[i].x, SAGA_BAR_DEF_X);
		CONFIG_READ(Entries, wxString::Format("%d_POS_Y" , i), l, DimInfo.mBounds[i].y, SAGA_BAR_DEF_Y);
		CONFIG_READ(Entries, wxString::Format("%d_SIZE_X", i), l, DimInfo.mSizes [i].x, SAGA_BAR_DEF_NX);
		CONFIG_READ(Entries, wxString::Format("%d_SIZE_Y", i), l, DimInfo.mSizes [i].y, SAGA_BAR_DEF_NY);
	}

	m_pLayout->AddBar(pWindow, DimInfo, Alignment, RowNo, 0, Name, false, State);
	m_pLayout->RecalcLayout();

#elif defined(__SAGA_GUI_USE_wxDOCKIT__)

	pBar->pWnd_Dock	= NULL;
	pBar->pWnd_Bar	= NULL;

	if( bToolbar )
	{
		pBar->pWnd_Bar	= m_pSlideBar->AddWindow(pWindow, Name);
	}
	else
	{
		int			x, y;
		wxHostInfo	hi;

		CONFIG_READ(Entries, "STATE" , l, State		, SAGA_BAR_DOCKED_H);
		CONFIG_READ(Entries, "ALIGN" , l, Alignment	, bToolbar ? SAGA_BAR_ALIGN_T : SAGA_BAR_ALIGN_B);

		CONFIG_READ(Entries, "POS_X" , l,  x		, SAGA_BAR_DEF_X);
		CONFIG_READ(Entries, "POS_Y" , l,  y		, SAGA_BAR_DEF_Y);
		CONFIG_READ(Entries, "SIZE_X", l, nx		, SAGA_BAR_DEF_NX);
		CONFIG_READ(Entries, "SIZE_Y", l, ny		, SAGA_BAR_DEF_NY);

		pBar->pWnd_Dock	= new wxDockWindow(m_pFrame, 0, Name, wxPoint(x, y), wxSize(nx, ny));
		pBar->pWnd_Dock->SetName(Name);
		pBar->pWnd_Dock->SetClient(pWindow);

		m_pLayout->AddDockWindow(pBar->pWnd_Dock);

		switch( State )
		{
		case SAGA_BAR_DOCKED_H:
			hi	= m_pLayout->GetDockHost(Alignment == SAGA_BAR_ALIGN_T ? wxDEFAULT_TOP_HOST : wxDEFAULT_BOTTOM_HOST);
			m_pLayout->DockWindow(pBar->pWnd_Dock, hi);
			break;

		case SAGA_BAR_DOCKED_V:
			hi	= m_pLayout->GetDockHost(Alignment == SAGA_BAR_ALIGN_L ? wxDEFAULT_LEFT_HOST : wxDEFAULT_RIGHT_HOST);
			m_pLayout->DockWindow(pBar->pWnd_Dock, hi);
			break;

		case SAGA_BAR_FLOATING:
			pBar->pWnd_Dock->Show(true);
			Bar_Show(pWindow, true);
			break;

		case SAGA_BAR_HIDDEN:
			pBar->pWnd_Dock->Show(true);
			Bar_Show(pWindow, false);
			break;
		}
	}

#endif
}

//---------------------------------------------------------
void CSAGA_Frame_Layout::_Bar_Position_Write(TBar &Bar)
{
	wxString	Entries;

	Entries.Printf("/FL/BAR%d", Bar.pWnd->GetId());

#if defined(__SAGA_GUI_USE_wxFL__)

	cbBarInfo	*pBar;

	if( (pBar = m_pLayout->FindBarByWindow(Bar.pWnd)) != NULL )
	{
		CONFIG_Write(Entries, "STATE"	, (long)pBar->mState);
		CONFIG_Write(Entries, "ALIGN"	, (long)pBar->mAlignment);
		CONFIG_Write(Entries, "ROWNO"	, (long)pBar->mRowNo);
		CONFIG_Write(Entries, "FPOSX"	, (long)pBar->mPosIfFloated.x);
		CONFIG_Write(Entries, "FPOSY"	, (long)pBar->mPosIfFloated.y);
		CONFIG_Write(Entries, "LRUPANE"	, (long)pBar->mDimInfo.mLRUPane);

		for(int i=0; i<SAGA_BAR_STATES; i++)
		{
			CONFIG_Write(Entries, wxString::Format("%d_POS_X" , i), (long)pBar->mDimInfo.mBounds[i].x);
			CONFIG_Write(Entries, wxString::Format("%d_POS_Y" , i), (long)pBar->mDimInfo.mBounds[i].y);
			CONFIG_Write(Entries, wxString::Format("%d_SIZE_X", i), (long)pBar->mDimInfo.mSizes [i].x);
			CONFIG_Write(Entries, wxString::Format("%d_SIZE_Y", i), (long)pBar->mDimInfo.mSizes [i].y);
		}
	}

#elif defined(__SAGA_GUI_USE_wxDOCKIT__)

	if( Bar.pWnd_Dock != NULL )
	{
		int		State, Alignment;

		if( Bar.pWnd->IsShown() )
		{
			if( Bar.pWnd_Dock->IsDocked() )
			{
				wxHostInfo	hi	= Bar.pWnd_Dock->GetDockingInfo();

				if(      hi.GetHost() == m_pLayout->GetDockHost(wxDEFAULT_LEFT_HOST) )
					Alignment	= SAGA_BAR_ALIGN_L;
				else if( hi.GetHost() == m_pLayout->GetDockHost(wxDEFAULT_RIGHT_HOST) )
					Alignment	= SAGA_BAR_ALIGN_R;
				else if( hi.GetHost() == m_pLayout->GetDockHost(wxDEFAULT_TOP_HOST) )
					Alignment	= SAGA_BAR_ALIGN_T;
				else // if( hi.GetHost() == m_pLayout->GetDockHost(wxDEFAULT_BOTTOM_HOST) )
					Alignment	= SAGA_BAR_ALIGN_B;

				CONFIG_Write(Entries, "ALIGN" , (long)Alignment);

				State	= Alignment == SAGA_BAR_ALIGN_L || Alignment == SAGA_BAR_ALIGN_R ? SAGA_BAR_DOCKED_V : SAGA_BAR_DOCKED_H;
			}
			else
			{
				CONFIG_Write(Entries, "POS_X" , (long)Bar.pWnd_Dock->GetPosition().x);
				CONFIG_Write(Entries, "POS_Y" , (long)Bar.pWnd_Dock->GetPosition().y);
				CONFIG_Write(Entries, "SIZE_X", (long)Bar.pWnd_Dock->GetSize().x);
				CONFIG_Write(Entries, "SIZE_Y", (long)Bar.pWnd_Dock->GetSize().y);

				State	= SAGA_BAR_FLOATING;
			}
		}
		else
		{
			State	= SAGA_BAR_HIDDEN;
		}

		CONFIG_Write(Entries, "STATE" , (long)State);
	}

#endif
}

//---------------------------------------------------------
void CSAGA_Frame_Layout::Bar_Toggle(wxWindow *pWindow)
{
#if defined(__SAGA_GUI_USE_wxFL__)

	m_pLayout->InverseVisibility(m_pLayout->FindBarByWindow(pWindow));

#elif defined(__SAGA_GUI_USE_wxDOCKIT__)

	TBar	*pBar;

	if( (pBar = _Bar_Find(pWindow)) != NULL )
	{
		bool	bShow	= !pBar->pWnd->IsShown();

		if( pBar->pWnd_Dock )
		{
			pBar->pWnd		->Show(bShow);
			pBar->pWnd_Dock	->Show(bShow);
		}
		else
		{
			pBar->pWnd		->Show(bShow);
			pBar->pWnd_Bar	->Show(bShow);

			m_pSlideBar->UpdateLayout();
		}
	}

#endif
}

//---------------------------------------------------------
void CSAGA_Frame_Layout::Bar_Show(wxWindow *pWindow, bool bShow)
{
	if( pWindow && pWindow->IsShown() != bShow )
	{
		Bar_Toggle(pWindow);
	}
}


///////////////////////////////////////////////////////////
//														 //
//						ToolBar							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
wxToolBarBase * CSAGA_Frame_Layout::TB_Create(int ID)
{
#if defined(__SAGA_GUI_USE_wxFL__)

	return( new wxDynamicToolBar(m_pFrame, ID) );

#elif defined(__SAGA_GUI_USE_wxDOCKIT__)

	return( new wxToolBar(m_pFrame, ID, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL|wxTB_FLAT) );

#endif
}

//---------------------------------------------------------
void CSAGA_Frame_Layout::TB_Add(wxToolBarBase *pToolBar, const char *Name)
{
	pToolBar->Realize();

	_Bar_Position_Read(pToolBar, Name, true);

	Bar_Show(pToolBar, false);
}

//---------------------------------------------------------
void CSAGA_Frame_Layout::TB_Add_Item(wxToolBarBase *pToolBar, bool bCheck, int Cmd_ID)
{
#if defined(__SAGA_GUI_USE_wxFL__)

	if( bCheck )
		((wxDynamicToolBar *)pToolBar)->AddTool(Cmd_ID, IMG_Get_Bitmap(CMD_Get_ImageID(Cmd_ID), TOOLBAR_SIZE_IMG), wxNullBitmap, true);
	//	((wxDynamicToolBar *)pToolBar)->AddCheckTool(Cmd_ID, CMD_Get_Name(Cmd_ID), IMG_Get_Bitmap(CMD_Get_ImageID(Cmd_ID), TOOLBAR_SIZE_IMG), wxNullBitmap, CMD_Get_Help(Cmd_ID));
	else
		((wxDynamicToolBar *)pToolBar)->AddTool(Cmd_ID, IMG_Get_Bitmap(CMD_Get_ImageID(Cmd_ID), TOOLBAR_SIZE_IMG), wxNullBitmap, false);

#elif defined(__SAGA_GUI_USE_wxDOCKIT__)

	if( bCheck )
		((wxToolBar        *)pToolBar)->AddTool(Cmd_ID, CMD_Get_Name(Cmd_ID), IMG_Get_Bitmap(CMD_Get_ImageID(Cmd_ID), TOOLBAR_SIZE_IMG), CMD_Get_Help(Cmd_ID), wxITEM_CHECK);
	else
		((wxToolBar        *)pToolBar)->AddTool(Cmd_ID, CMD_Get_Name(Cmd_ID), IMG_Get_Bitmap(CMD_Get_ImageID(Cmd_ID), TOOLBAR_SIZE_IMG), CMD_Get_Help(Cmd_ID));

#endif
}

//---------------------------------------------------------
void CSAGA_Frame_Layout::TB_Add_Separator(wxToolBarBase *pToolBar)
{
#if defined(__SAGA_GUI_USE_wxFL__)

	((wxDynamicToolBar *)pToolBar)->AddSeparator();

#elif defined(__SAGA_GUI_USE_wxDOCKIT__)

	((wxToolBar        *)pToolBar)->AddSeparator();

#endif
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------