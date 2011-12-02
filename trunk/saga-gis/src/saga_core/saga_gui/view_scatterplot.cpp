/**********************************************************
 * Version $Id$
 *********************************************************/

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
//                 VIEW_ScatterPlot.cpp                  //
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
#include <wx/window.h>

#include <saga_api/saga_api.h>

#include "res_commands.h"
#include "res_controls.h"
#include "res_images.h"
#include "res_dialogs.h"

#include "helper.h"
#include "dc_helper.h"

#include "wksp.h"
#include "wksp_data_manager.h"

#include "wksp_grid.h"
#include "wksp_shapes.h"

#include "view_scatterplot.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define CHECK_DATA(pData)	if( pData != NULL && !g_pData->Exists(pData) )	{	pData	= NULL;	}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void		Add_ScatterPlot(CSG_Grid *pGrid)
{
	CSG_Parameters	P(NULL, wxString::Format(wxT("%s: %s"), _TL("[CAP] Scatterplot"), pGrid->Get_Name()), _TL(""), NULL);

	P.Add_Choice(
		NULL	, "TYPE"	, _TL("Compare with..."),
		_TL(""),
		CSG_String::Format(SG_T("%s|%s|"),
			_TL("another grid"),
			_TL("points")
		)
	);

	//-----------------------------------------------------
	if( DLG_Parameters(&P) )
	{
		if( P("TYPE")->asInt() == 0 )
		{
			P.Del_Parameters();

			P.Add_Grid(
				NULL	, "GRID"	, _TL("[CAP] Grid"),
				_TL(""),
				PARAMETER_INPUT
			);

			if( DLG_Parameters(&P) )
			{
				new CVIEW_ScatterPlot(pGrid, P("GRID")->asGrid());
			}
		}
		else
		{
			P.Del_Parameters();

			CSG_Parameter	*pNode	= P.Add_Shapes(
				NULL	, "POINTS"	, _TL("[CAP] Points"),
				_TL(""),
				PARAMETER_INPUT, SHAPE_TYPE_Point
			);

			P.Add_Table_Field(
				pNode	, "FIELD"	, _TL("[CAP] Attribute"),
				_TL("")
			);

			if( DLG_Parameters(&P) )
			{
				new CVIEW_ScatterPlot(pGrid, P("POINTS")->asShapes(), P("FIELD")->asInt());
			}
		}
	}
}

//---------------------------------------------------------
void		Add_ScatterPlot(CSG_Table *pTable)
{
	CSG_String	sChoices;

	for(int i=0; i<pTable->Get_Field_Count(); i++)
	{
		sChoices.Append(CSG_String::Format(SG_T("%s|"), pTable->Get_Field_Name(i)));
	}

	CSG_Parameters	P(NULL, CSG_String::Format(SG_T("%s: %s"), _TL("[CAP] Scatterplot"), pTable->Get_Name()), _TL(""));

	P.Add_Choice(NULL, "FIELD_A", wxT("X"), wxT(""), sChoices);
	P.Add_Choice(NULL, "FIELD_B", wxT("Y"), wxT(""), sChoices);

	if( DLG_Parameters(&P) )
	{
		new CVIEW_ScatterPlot(pTable, P("FIELD_A")->asInt(), P("FIELD_B")->asInt());
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
IMPLEMENT_CLASS(CVIEW_ScatterPlot, CVIEW_Base);

//---------------------------------------------------------
BEGIN_EVENT_TABLE(CVIEW_ScatterPlot, CVIEW_Base)
	EVT_PAINT			(CVIEW_ScatterPlot::On_Paint)
	EVT_SIZE			(CVIEW_ScatterPlot::On_Size)

	EVT_MENU			(ID_CMD_SCATTERPLOT_PARAMETERS	, CVIEW_ScatterPlot::On_Parameters)
	EVT_MENU			(ID_CMD_SCATTERPLOT_UPDATE		, CVIEW_ScatterPlot::On_Update)
	EVT_MENU			(ID_CMD_HISTOGRAM_AS_TABLE		, CVIEW_ScatterPlot::On_AsTable)
END_EVENT_TABLE()


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CVIEW_ScatterPlot::CVIEW_ScatterPlot(CSG_Grid *pGrid_X, CSG_Grid *pGrid_Y)
	: CVIEW_Base(ID_VIEW_SCATTERPLOT, _TL("[CAP] Scatterplot"), ID_IMG_WND_SCATTERPLOT, CVIEW_ScatterPlot::_Create_Menu(), _TL("[CAP] Scatterplot"))
{
	_On_Construction();

	m_Method		= 0;
	m_pGrid_X		= pGrid_X;
	m_pGrid_Y		= pGrid_Y;

	_Initialize_Grids();
}

//---------------------------------------------------------
CVIEW_ScatterPlot::CVIEW_ScatterPlot(CSG_Grid *pGrid_X, CSG_Shapes *pShapes_Y, int Field)
	: CVIEW_Base(ID_VIEW_SCATTERPLOT, _TL("[CAP] Scatterplot"), ID_IMG_WND_SCATTERPLOT, CVIEW_ScatterPlot::_Create_Menu(), _TL("[CAP] Scatterplot"))
{
	_On_Construction();

	m_Method		= 1;
	m_pGrid_X		= pGrid_X;
	m_pShapes		= pShapes_Y;
	m_xField		= Field;

	_Initialize_Shapes();
}

//---------------------------------------------------------
CVIEW_ScatterPlot::CVIEW_ScatterPlot(CSG_Table *pTable, int Field_X, int Field_Y)
	: CVIEW_Base(ID_VIEW_SCATTERPLOT, _TL("[CAP] Scatterplot"), ID_IMG_WND_SCATTERPLOT, CVIEW_ScatterPlot::_Create_Menu(), _TL("[CAP] Scatterplot"))
{
	_On_Construction();

	m_Method		= 2;
	m_pTable		= pTable;
	m_xField		= Field_X;
	m_yField		= Field_Y;

	_Initialize_Table();
}

//---------------------------------------------------------
CVIEW_ScatterPlot::~CVIEW_ScatterPlot(void)
{}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
wxMenu * CVIEW_ScatterPlot::_Create_Menu(void)
{
	wxMenu	*pMenu	= new wxMenu();

	CMD_Menu_Add_Item(pMenu, false, ID_CMD_SCATTERPLOT_PARAMETERS);
	CMD_Menu_Add_Item(pMenu, false, ID_CMD_SCATTERPLOT_UPDATE);
	CMD_Menu_Add_Item(pMenu, false, ID_CMD_HISTOGRAM_AS_TABLE);

	return( pMenu );
}

//---------------------------------------------------------
wxToolBarBase * CVIEW_ScatterPlot::_Create_ToolBar(void)
{
	wxToolBarBase	*pToolBar	= CMD_ToolBar_Create(ID_TB_VIEW_SCATTERPLOT);

	CMD_ToolBar_Add_Item(pToolBar, false, ID_CMD_SCATTERPLOT_PARAMETERS);
	CMD_ToolBar_Add_Item(pToolBar, false, ID_CMD_SCATTERPLOT_UPDATE);
	CMD_ToolBar_Add_Item(pToolBar, false, ID_CMD_HISTOGRAM_AS_TABLE);

	CMD_ToolBar_Add(pToolBar, _TL("[CAP] Scatterplot"));

	return( pToolBar );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_ScatterPlot::Update_ScatterPlot(void)
{
	switch( m_Method )
	{
	case 0:	_Initialize_Grids	();	break;
	case 1:	_Initialize_Shapes	();	break;
	case 2:	_Initialize_Table	();	break;
	}

	Refresh();
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_ScatterPlot::_On_Construction(void)
{
	m_maxSamples	= 100000;
	m_Method		= -1;
	m_pGrid_X		= NULL;
	m_pGrid_Y		= NULL;
	m_pShapes		= NULL;
	m_pTable		= NULL;
	m_xField		= 0;
	m_yField		= 0;

	SYS_Set_Color_BG_Window(this);

	m_Parameters.Add_Choice(
		NULL, "TYPE"		, _TL("[CAP] Regression Formula"),
		_TL(""),
		wxT("Y = a + b * X|")
		wxT("Y = a + b / X|")
		wxT("Y = a / (b - X)|")
		wxT("Y = a * X^b|")
		wxT("Y = a e^(b * X)|")
		wxT("Y = a + b * ln(X)|")
	);

	m_Parameters.Add_String(
		NULL, "INFO"		, _TL("[CAP] Regression Details"),
		_TL(""),
		_TL(""), true
	);

	m_Parameters.Add_Font(
		NULL, "FONT"		, _TL("[CAP] Font"),
		_TL("")
	);

	m_Parameters.Add_Value(
		NULL, "REGRESSION"	, _TL("[CAP] Show Regression Curve"),
		_TL(""),
		PARAMETER_TYPE_Bool, true
	);

	m_Parameters.Add_Choice(
		NULL, "METHOD"		, _TL("[CAP] Display Type"),
		_TL(""),
		CSG_String::Format(SG_T("%s|%s|"),
			_TL("[CAP] Circles"),
			_TL("[CAP] Points")
		)
	);

	m_Parameters.Add_Value(
		NULL, "RESOLUTION"	, _TL("[CAP] Display Resolution"),
		_TL(""),
		PARAMETER_TYPE_Int, 1, 1, true
	);

	m_Parameters.Add_Colors(
		NULL, "COLORS"		, _TL("[CAP] Colors"),
		_TL("")
	);

	m_Parameters.Add_Value(
		NULL	, "MAX"		, _TL("Maximimum Number of Samples"),
		_TL(""),
		PARAMETER_TYPE_Int	, m_maxSamples, 0, true
	);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_ScatterPlot::On_Parameters(wxCommandEvent &event)
{
	if( DLG_Parameters(&m_Parameters) )
	{
		if( m_Parameters("MAX")->asInt() != m_maxSamples )
		{
			m_maxSamples	= m_Parameters("MAX")->asInt();

			Update_ScatterPlot();
		}

		if( m_Parameters("TYPE")->asInt() != m_Regression.Get_Type() )
		{
			m_Regression.Calculate((TSG_Regression_Type)m_Parameters("TYPE")->asInt());
		}

		Refresh();
	}
}

//---------------------------------------------------------
void CVIEW_ScatterPlot::On_Update(wxCommandEvent &event)
{
	Update_ScatterPlot();
}

//---------------------------------------------------------
void CVIEW_ScatterPlot::On_AsTable(wxCommandEvent &event)
{
	if( m_Regression.Get_Count() > 1 )
	{
		CSG_Table	*pTable	= new CSG_Table;

		pTable->Set_Name(CSG_String::Format(SG_T("%s: [%s]-[%s]"), _TL("[CAP] Scatterplot"), m_sX.c_str(), m_sY.c_str()));

		pTable->Add_Field(SG_T("ID"), SG_DATATYPE_Int);
		pTable->Add_Field(m_sX      , SG_DATATYPE_Double);
		pTable->Add_Field(m_sY      , SG_DATATYPE_Double);

		for(int i=0; i<m_Regression.Get_Count() && PROGRESSBAR_Set_Position(i, m_Regression.Get_Count()); i++)
		{
			CSG_Table_Record	*pRecord	= pTable->Add_Record();

			pRecord->Set_Value(0, i + 1);
			pRecord->Set_Value(1, m_Regression.Get_xValue(i));
			pRecord->Set_Value(2, m_Regression.Get_yValue(i));
		}

		PROGRESSBAR_Set_Position(0);

		g_pData->Add(pTable);
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_ScatterPlot::On_Size(wxSizeEvent &event)
{
	Refresh();

	event.Skip();
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define GET_DC_X(x)	(r.GetLeft()   + (int)(dx * ((x) - m_Regression.Get_xMin())))
#define GET_DC_Y(y)	(r.GetBottom() - (int)(dy * ((y) - m_Regression.Get_yMin())))

//---------------------------------------------------------
void CVIEW_ScatterPlot::On_Paint(wxPaintEvent &event)
{
	wxPaintDC	dc(this);
	wxRect		r(wxPoint(0, 0), GetClientSize());

	Draw_Edge(dc, EDGE_STYLE_SUNKEN, r);

	Draw(dc, r);
}

//---------------------------------------------------------
void CVIEW_ScatterPlot::Draw(wxDC &dc, wxRect r)
{
	double	dx, dy;

	r	= _Draw_Get_rDiagram(r);

	dc.SetFont				(Get_Font(m_Parameters("FONT")));
	dc.SetTextForeground	(m_Parameters("FONT")->asColor());

	if( m_Regression.Get_Count() > 1 )
	{
		dx	= (r.GetWidth()  - 1.0) / (m_Regression.Get_xMax() - m_Regression.Get_xMin());
		dy	= (r.GetHeight() - 1.0) / (m_Regression.Get_yMax() - m_Regression.Get_yMin());

		//-------------------------------------------------
		if( m_Parameters("METHOD")->asInt() == 1 )
		{
			_Draw_Image (dc, r, dx, dy);
		}
		else
		{
			_Draw_Points(dc, r, dx, dy);
		}

		//-------------------------------------------------
		if( m_Parameters("REGRESSION")->asBool() )
		{
			_Draw_Regression(dc, r, dx, dy);
		}

		//-------------------------------------------------
		_Draw_Frame(dc, r);
	}
	else
	{
		Draw_Text(dc, TEXTALIGN_CENTER, r.GetLeft() + r.GetWidth() / 2, r.GetTop() + r.GetHeight() / 2, _TL("[ERR] Invalid data!"));
	}
}

//---------------------------------------------------------
wxRect CVIEW_ScatterPlot::_Draw_Get_rDiagram(wxRect r)
{
	return(	wxRect(
		wxPoint(r.GetLeft()  + 70, r.GetTop()    + 20),
		wxPoint(r.GetRight() - 20, r.GetBottom() - 50)
	));
}

//---------------------------------------------------------
void CVIEW_ScatterPlot::_Draw_Regression(wxDC &dc, wxRect r, double dx, double dy)
{
	int			ix, ay, by;
	double		a, b, x, y, ex;
	wxString	s;
	wxPen		Pen, oldPen	= dc.GetPen();
	wxColour	Col, oldCol	= dc.GetTextForeground();

	Col	= wxColour(255, 0, 0);
	Pen.SetColour(Col);
	dc.SetPen(Pen);
	dc.SetTextForeground(Col);

	//-----------------------------------------------------
	a	= m_Regression.Get_Constant();
	b	= m_Regression.Get_Coefficient();

	//-----------------------------------------------------
	dc.DrawCircle(
		GET_DC_X(m_Regression.Get_xMean()),
		GET_DC_Y(m_Regression.Get_yMean()), 2
	);

	ex	= (m_Regression.Get_xMax() - m_Regression.Get_xMin()) / (double)r.GetWidth();
	x	= m_Regression.Get_xMin();
	by	= 0;

	for(ix=0; ix<r.GetWidth(); ix++, x+=ex)
	{
		switch( m_Regression.Get_Type() )
		{
		default:				y	= 0.0;				break;
		case REGRESSION_Linear:	y	= a + b * x;		break;
		case REGRESSION_Rez_X:	y	= a + b / x;		break;
		case REGRESSION_Rez_Y:	y	= a / (b - x);		break;
		case REGRESSION_Pow:	y	= a * pow(x, b);	break;
		case REGRESSION_Exp:	y	= a * exp(b * x);	break;
		case REGRESSION_Log:	y	= a + b * log(x);	break;
		}

		ay	= by;
		by	= r.GetBottom() - (int)(dy * (y - m_Regression.Get_yMin()));

		if( ix > 0 && r.GetTop() < ay && ay < r.GetBottom() && r.GetTop() < by && by < r.GetBottom() )
		{
			dc.DrawLine(r.GetLeft() + ix - 1, ay, r.GetLeft() + ix, by);
		}
	}

	//-----------------------------------------------------
	switch( m_Regression.Get_Type() )
	{
	case REGRESSION_Linear:	s	= wxT("Y = %f%+f*X");		break;
	case REGRESSION_Rez_X:	s	= wxT("Y = %f%+f/X");		break;
	case REGRESSION_Rez_Y:	s	= wxT("Y = %f/(%f-X)");		break;
	case REGRESSION_Pow:	s	= wxT("Y = %f*X^%f");		break;
	case REGRESSION_Exp:	s	= wxT("Y = %f e^(%f*X)");	break;
	case REGRESSION_Log:	s	= wxT("Y = %f%+f*ln(X)");	break;
	}

	Draw_Text(dc, TEXTALIGN_TOPLEFT, r.GetLeft(), r.GetTop(),
		wxString::Format(s, m_Regression.Get_Constant(), m_Regression.Get_Coefficient())
	);

	Draw_Text(dc, TEXTALIGN_BOTTOMRIGHT, r.GetRight(), r.GetBottom(),
		wxString::Format(wxT("R2: %f%%"), 100.0 * m_Regression.Get_R2())	// Coefficient of Determination...
	);

	dc.SetPen(oldPen);
	dc.SetTextForeground(oldCol);
}

//---------------------------------------------------------
void CVIEW_ScatterPlot::_Draw_Image(wxDC &dc, wxRect r, double dx, double dy)
{
	int			x, y, i, ax, ay, bx, by, Resolution;
	double		zMax;
	CSG_Grid	Count;
	CSG_Colors	*pColors	= m_Parameters("COLORS")->asColors();
	wxPen		Pen, oldPen(dc.GetPen());

	//-----------------------------------------------------
	Resolution	= m_Parameters("RESOLUTION")->asInt();

	Count.Create(SG_DATATYPE_Word, 1 + (r.GetWidth() / Resolution), 1 + (r.GetHeight() / Resolution));

	dx	/= Resolution;
	dy	/= Resolution;

	for(i=0, zMax=0; i<m_Regression.Get_Count(); i++)
	{
		x	= (int)(dx * (m_Regression.Get_xValue(i) - m_Regression.Get_xMin()));
		y	= (int)(dy * (m_Regression.Get_yValue(i) - m_Regression.Get_yMin()));
		Count.Add_Value(x, y, 1);
		if( Count(x, y) > zMax )
			zMax	= Count(x, y);
	}

	//-----------------------------------------------------
	if( Resolution <= 1 )
	{
		for(y=0, ay=r.GetBottom(); y<Count.Get_NY(); y++, ay--)
		{
			for(x=0, ax=r.GetLeft(); x<Count.Get_NX(); x++, ax++)
			{
				if( (i = Count.asInt(x, y)) > 0 )
				{
					i	= (int)((pColors->Get_Count() - 1) * i / zMax);
					Pen.SetColour(Get_Color_asWX(pColors->Get_Color(i)));
					dc.SetPen(Pen);
					dc.DrawPoint(ax, ay);
				}
			}
		}
	}
	else
	{
		dx	= r.GetWidth()  / (double)(Count.Get_NX() - 1.0);
		dy	= r.GetHeight() / (double)(Count.Get_NY() - 1.0);

		for(y=0, by=r.GetBottom(); y<Count.Get_NY(); y++)
		{
			ay	= by;
			by	= r.GetBottom() - (int)(y * dy);

			for(x=0, bx=r.GetLeft(); x<Count.Get_NX(); x++)
			{
				ax	= bx;
				bx	= r.GetLeft() + (int)(x * dx);

				if( (i = Count.asInt(x, y)) > 0 )
				{
					i	= (int)((pColors->Get_Count() - 1) * i / zMax);
					Draw_FillRect(dc, Get_Color_asWX(pColors->Get_Color(i)), ax, ay, bx, by);
				}
			}
		}
	}

	//-----------------------------------------------------
	dc.SetPen(oldPen);
}

//---------------------------------------------------------
void CVIEW_ScatterPlot::_Draw_Points(wxDC &dc, wxRect r, double dx, double dy)
{
	for(int i=0; i<m_Regression.Get_Count(); i++)
	{
		dc.DrawCircle(
			GET_DC_X(m_Regression.Get_xValue(i)),
			GET_DC_Y(m_Regression.Get_yValue(i)), 2
		);
	}
}

//---------------------------------------------------------
void CVIEW_ScatterPlot::_Draw_Frame(wxDC &dc, wxRect r)
{
	const int	dyFont		= 12,
				Precision	= 3;

	int		iPixel, iStep, nSteps;
	double	dPixel, dz;
	wxFont	Font;

	//-----------------------------------------------------
	Draw_Edge(dc, EDGE_STYLE_SIMPLE, r);

	Draw_Text(dc, TEXTALIGN_BOTTOMCENTER, r.GetRight(), r.GetTop() + r.GetHeight() / 2, -90.0, m_sY.c_str());
	Draw_Text(dc, TEXTALIGN_BOTTOMCENTER, r.GetLeft() + r.GetWidth() / 2, r.GetTop(), m_sX.c_str());

	Font	= dc.GetFont();
	Font.SetPointSize((int)(0.7 * dyFont));
	dc.SetFont(Font);

	//-------------------------------------------------
	dPixel	= dyFont;
	nSteps	= (int)(r.GetHeight() / dPixel);
	dz		= (m_Regression.Get_yMax() - m_Regression.Get_yMin()) * dPixel / (double)r.GetHeight();

	for(iStep=0; iStep<=nSteps; iStep++)
	{
		iPixel	= r.GetBottom()	- (int)(dPixel * iStep);
		dc.DrawLine(r.GetLeft(), iPixel, r.GetLeft() - 5, iPixel);
		Draw_Text(dc, TEXTALIGN_CENTERRIGHT, r.GetLeft() - 7, iPixel,
			wxString::Format(wxT("%.*f"), Precision, m_Regression.Get_yMin() + iStep * dz)
		);
	}

	//-------------------------------------------------
	dPixel	= dyFont + 5;
	nSteps	= (int)(r.GetWidth() / dPixel);
	dz		= (m_Regression.Get_xMax() - m_Regression.Get_xMin()) * dPixel / (double)r.GetWidth();

	for(iStep=0; iStep<=nSteps; iStep++)
	{
		iPixel	= r.GetLeft() + (int)(dPixel * iStep);
		dc.DrawLine(iPixel, r.GetBottom(), iPixel, r.GetBottom() + 5);
		Draw_Text(dc, TEXTALIGN_CENTERRIGHT, iPixel, r.GetBottom() + 7, 45.0,
			wxString::Format(wxT("%.*f"), Precision, m_Regression.Get_xMin() + iStep * dz)
		);
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CVIEW_ScatterPlot::_Initialize(void)
{
	bool	bResult	= m_Regression.Calculate();

	PROCESS_Set_Okay(true);

	m_Parameters("METHOD")	->Set_Value(m_Regression.Get_Count() > 1000 ? 1 : 0);
	m_Parameters("INFO")	->Set_Value((void *)m_Regression.asString());

	return( bResult );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CVIEW_ScatterPlot::_Initialize_Grids()
{
	CHECK_DATA(m_pGrid_X);
	CHECK_DATA(m_pGrid_Y);

	if( !m_pGrid_X || !m_pGrid_Y )
	{
		return( false );
	}

	//-----------------------------------------------------
	m_Regression.Destroy();

	m_sTitle.Printf(wxT("%s: [%s/%s]"), _TL("[CAP] Scatterplot"), m_pGrid_X->Get_Name(), m_pGrid_Y->Get_Name());

	m_sX.Printf(wxT("%s"), m_pGrid_X->Get_Name());
	m_sY.Printf(wxT("%s"), m_pGrid_Y->Get_Name());

	bool	bEqual			= m_pGrid_X->Get_System() == m_pGrid_Y->Get_System();
	int		Interpolation	= GRID_INTERPOLATION_BSpline;
	double	Step			= m_maxSamples > 0 && m_pGrid_X->Get_NCells() > m_maxSamples ? m_pGrid_X->Get_NCells() / m_maxSamples : 1.0;

	for(double i=0; i<m_pGrid_X->Get_NCells() && PROGRESSBAR_Set_Position(i, m_pGrid_X->Get_NCells()); i+=Step)
	{
		if( !m_pGrid_X->is_NoData((long)i) )
		{
			if( bEqual )
			{
				if( !m_pGrid_Y->is_NoData((long)i) )
				{
					m_Regression.Add_Values(
						m_pGrid_X->asDouble((long)i, true),
						m_pGrid_Y->asDouble((long)i, true)
					);
				}
			}
			else
			{
				int		x	= ((long)i) % m_pGrid_X->Get_NX();
				int		y	= ((long)i) / m_pGrid_X->Get_NX();
				double	z;

				if(	m_pGrid_Y->Get_Value(m_pGrid_X->Get_System().Get_Grid_to_World(x, y), z, Interpolation, true) )
				{
					m_Regression.Add_Values(m_pGrid_X->asDouble((long)i, true), z);
				}
			}
		}
	}

	return( _Initialize() );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CVIEW_ScatterPlot::_Initialize_Shapes(void)
{
	CHECK_DATA(m_pGrid_X);
	CHECK_DATA(m_pShapes);

	if( !m_pGrid_X || !m_pShapes || m_xField < 0 || m_xField >= m_pShapes->Get_Field_Count() )
	{
		return( false );
	}

	m_Regression.Destroy();

	m_sTitle.Printf(wxT("%s: [%s/%s]"), _TL("[CAP] Scatterplot"), m_pGrid_X->Get_Name(), m_pShapes->Get_Name());

	m_sX.Printf(wxT("%s"), m_pGrid_X->Get_Name());
	m_sY.Printf(wxT("%s"), m_pShapes->Get_Field_Name(m_xField));

	double	z, Step	= m_maxSamples > 0 && m_pShapes->Get_Count() > m_maxSamples ? m_pShapes->Get_Count() / m_maxSamples : 1.0;

	for(double i=0; i<m_pShapes->Get_Count() && PROGRESSBAR_Set_Position(i, m_pShapes->Get_Count()); i+=Step)
	{
		CSG_Shape	*pShape	= m_pShapes->Get_Shape((int)i);

		if( !pShape->is_NoData(m_xField) && m_pGrid_X->Get_Value(pShape->Get_Point(0), z, GRID_INTERPOLATION_BSpline, true) )
		{
			m_Regression.Add_Values(z, pShape->asDouble(m_xField));
		}
	}

	return( _Initialize() );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CVIEW_ScatterPlot::_Initialize_Table(void)
{
	CHECK_DATA(m_pTable);

	if( !m_pTable || m_xField < 0 || m_xField >= m_pTable->Get_Field_Count() || m_yField < 0 || m_yField >= m_pTable->Get_Field_Count() )
	{
		return( false );
	}

	double	Step	= m_maxSamples > 0 && m_pTable->Get_Count() > m_maxSamples ? m_pTable->Get_Count() / m_maxSamples : 1.0;

	m_Regression.Destroy();

	m_sTitle.Printf(wxT("%s: [%s]"), _TL("[CAP] Scatterplot"), m_pTable->Get_Name());

	m_sX.Printf(wxT("%s"), m_pTable->Get_Field_Name(m_xField));
	m_sY.Printf(wxT("%s"), m_pTable->Get_Field_Name(m_yField));

	for(double i=0; i<m_pTable->Get_Record_Count() && PROGRESSBAR_Set_Position(i, m_pTable->Get_Record_Count()); i+=Step)
	{
		CSG_Table_Record	*pRecord	= m_pTable->Get_Record((int)i);

		if( !pRecord->is_NoData(m_xField) && !pRecord->is_NoData(m_yField) )
		{
			m_Regression.Add_Values(
				pRecord->asDouble(m_xField),
				pRecord->asDouble(m_yField)
			);
		}
	}

	return( _Initialize() );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
