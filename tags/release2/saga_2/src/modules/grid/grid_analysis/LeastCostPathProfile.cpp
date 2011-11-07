///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                   Cost Analysis                       //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//              LeastCostPathProfile.cpp                 //
//                                                       //
//                 Copyright (C) 2004 by                 //
//               Olaf Conrad & Victor Olaya              //
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
#include "LeastCostPathProfile.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define VALUE_OFFSET	5


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CLeastCostPathProfile::CLeastCostPathProfile(void)
{
	Parameters.Set_Name(_TL("Least Cost Path"));
	Parameters.Set_Description(_TL(
		"Creates a least cost past profile using an accumulated cost surface."
		"\n(c) 2004 Victor Olaya, Goettingen.\nemail: oconrad@gwdg.de\n")
	);

	Parameters.Add_Grid(
		NULL, "DEM"		, 
		_TL("Accumulated cost"),
		_TL("Accumulated cost"),
		PARAMETER_INPUT
	);

	Parameters.Add_Grid_List(
		NULL, 
		"VALUES", 
		_TL("Values"),
		"",
		PARAMETER_INPUT_OPTIONAL
	);

	Parameters.Add_Shapes(
		NULL, 
		"POINTS", 
		_TL("Profile (points)"),
		"",
		PARAMETER_OUTPUT, 
		SHAPE_TYPE_Point
	);

	Parameters.Add_Shapes(
		NULL, 
		"LINE", 
		_TL("Profile (lines)"),
		"",
		PARAMETER_OUTPUT, SHAPE_TYPE_Line
	);
}

//---------------------------------------------------------
CLeastCostPathProfile::~CLeastCostPathProfile(void)
{}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CLeastCostPathProfile::On_Execute(void)
{
	m_pDEM		= Parameters("DEM")		->asGrid();
	m_pValues	= Parameters("VALUES")	->asGridList();
	m_pPoints	= Parameters("POINTS")	->asShapes();
	m_pLine		= Parameters("LINE")	->asShapes();

	DataObject_Update(m_pDEM, true);

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CLeastCostPathProfile::On_Execute_Position(CGEO_Point ptWorld, TModule_Interactive_Mode Mode)
{
	switch( Mode )
	{
	case MODULE_INTERACTIVE_LDOWN:
		Set_Profile(Get_System()->Fit_to_Grid_System(ptWorld));
		break;
	}

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CLeastCostPathProfile::Set_Profile(TGEO_Point ptWorld)
{
	int			x, y, i;

	//-----------------------------------------------------
	if( Get_System()->Get_World_to_Grid(x, y, ptWorld) && m_pDEM->is_InGrid(x, y) )
	{
		m_pPoints->Create(SHAPE_TYPE_Point, CAPI_String::Format(_TL("Profile [%s]"), m_pDEM->Get_Name()));

		m_pPoints->Get_Table().Add_Field("ID"	, TABLE_FIELDTYPE_Int);
		m_pPoints->Get_Table().Add_Field("D"	, TABLE_FIELDTYPE_Double);
		m_pPoints->Get_Table().Add_Field("X"	, TABLE_FIELDTYPE_Double);
		m_pPoints->Get_Table().Add_Field("Y"	, TABLE_FIELDTYPE_Double);
		m_pPoints->Get_Table().Add_Field("Z"	, TABLE_FIELDTYPE_Double);

		for(i=0; i<m_pValues->Get_Count(); i++)
		{
			m_pPoints->Get_Table().Add_Field(m_pValues->asGrid(i)->Get_Name(), TABLE_FIELDTYPE_Double);
		}

		//-----------------------------------------------------
		m_pLine->Create(SHAPE_TYPE_Line, CAPI_String::Format(_TL("Profile [%s]"), m_pDEM->Get_Name()));
		m_pLine->Get_Table().Add_Field("ID"	, TABLE_FIELDTYPE_Int);
		m_pLine->Add_Shape()->Get_Record()->Set_Value(0, 1);

		//-----------------------------------------------------
		Set_Profile(x, y);

		//-----------------------------------------------------
		DataObject_Update(m_pLine	, false);
		DataObject_Update(m_pPoints	, false);

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
void CLeastCostPathProfile::Set_Profile(int iX, int iY)
{
	int iNextX, iNextY;

	iNextX = iX;
	iNextY = iY;
	do {
		iX = iNextX;
		iY = iNextY;
		getNextCell(m_pDEM, iX, iY, iNextX, iNextY);
	
	}while (Add_Point(iX, iY) 
			&& (iX != iNextX || iY != iNextY));
	
}


void CLeastCostPathProfile::getNextCell(CGrid *g,
										int iX,
										int iY,
										int &iNextX,
										int &iNextY) {

    float fMaxSlope;
    float fSlope;

    fMaxSlope = 0;
    fSlope = 0;

    if (iX < 1 || iX >= g->Get_NX()-1 || iY < 1 || iY >= g->Get_NY()-1
            || g->is_NoData(iX,iY)) {
        iNextX = iX;
		iNextY = iY;
		return;
    }// if

    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {                	
            if (!g->is_NoData(iX+i,iY+j)){
                fSlope = (g->asFloat(iX+i,iY+j)
                         - g->asFloat(iX,iY));                                				
                if (fSlope <= fMaxSlope) {
                    iNextX = iX+i;
					iNextY = iY+j;                        
                    fMaxSlope = fSlope;
                }// if
            }//if                    
        }// for
    }// for

}// method

//---------------------------------------------------------
bool CLeastCostPathProfile::Add_Point(int x, int y)
{
	int			i;
	double		Distance;
	TGEO_Point	Point;
	CShape		*pPoint, *pLast;

	if( m_pDEM->is_InGrid(x, y) )
	{
		Point	= Get_System()->Get_Grid_to_World(x, y);

		if( m_pPoints->Get_Count() == 0 )
		{
			Distance	= 0.0;
		}
		else
		{
			pLast		= m_pPoints->Get_Shape(m_pPoints->Get_Count() - 1);
			Distance	= sqrt(MAT_Square(Point.x - pLast->Get_Point(0).x) + MAT_Square(Point.y - pLast->Get_Point(0).y));
			Distance	+= pLast->Get_Record()->asDouble(1);
		}

		pPoint	= m_pPoints->Add_Shape();
		pPoint->Add_Point(Point);

		pPoint->Get_Record()->Set_Value(0, m_pPoints->Get_Count());
		pPoint->Get_Record()->Set_Value(1, Distance);
		pPoint->Get_Record()->Set_Value(2, Point.x);
		pPoint->Get_Record()->Set_Value(3, Point.y);
		pPoint->Get_Record()->Set_Value(4, m_pDEM->asDouble(x, y));

		for(i=0; i<m_pValues->Get_Count(); i++)
		{
			pPoint->Get_Record()->Set_Value(VALUE_OFFSET + i, m_pValues->asGrid(i)->asDouble(x, y, true));
		}

		m_pLine->Get_Shape(0)->Add_Point(Point);

		return( true );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------