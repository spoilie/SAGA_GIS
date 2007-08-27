
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                      Grid_Shapes                      //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//               Grid_To_Points_Random.cpp               //
//                                                       //
//                 Copyright (C) 2003 by                 //
//                      Olaf Conrad                      //
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
//    e-mail:     oconrad@saga-gis.org                   //
//                                                       //
//    contact:    Olaf Conrad                            //
//                Institute of Geography                 //
//                University of Goettingen               //
//                Goldschmidtstr. 5                      //
//                37077 Goettingen                       //
//                Germany                                //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include <time.h>

#include "Grid_To_Points_Random.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CGrid_To_Points_Random::CGrid_To_Points_Random()
{
	Set_Name	(_TL("Grid Values to Points (randomly)"));

	Set_Author	(_TL("Copyrights (c) 2001 by Olaf Conrad"));

	Set_Description	(_TW(
		"Extract randomly points from gridded data.\n"
		"(c) 2001 by Olaf Conrad, Goettingen\nemail: oconrad@gwdg.de")
	);

	Parameters.Add_Grid(	NULL, "GRID"	, _TL("Grid")		, _TL(""), PARAMETER_INPUT);
	Parameters.Add_Value(	NULL, "FREQ"	, _TL("Frequency")	, _TL("One per x"), PARAMETER_TYPE_Int, 100, 1, true);

	Parameters.Add_Shapes(	NULL, "POINTS"	, _TL("Points")		, _TL(""), PARAMETER_OUTPUT, SHAPE_TYPE_Point);
}

//---------------------------------------------------------
CGrid_To_Points_Random::~CGrid_To_Points_Random()
{}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CGrid_To_Points_Random::On_Execute(void)
{
	int		x, y, n;
	double	frequency;
	CSG_Grid	*pGrid;
	CSG_Shape	*pShape;
	CSG_Shapes	*pShapes;

	pGrid		= Parameters("GRID")->asGrid();
	frequency	= 1.0 / Parameters("FREQ")->asDouble();
	pShapes		= Parameters("POINTS")->asShapes();

	pShapes->Create(SHAPE_TYPE_Point, pGrid->Get_Name());
	pShapes->Get_Table().Add_Field("ID"		, TABLE_FIELDTYPE_Int);
	pShapes->Get_Table().Add_Field("VALUE"	, TABLE_FIELDTYPE_Double);

	srand((unsigned)time(NULL));

	for(n=0, y=0; y<Get_NY() && Set_Progress(y); y++)
	{
		for(x=0; x<Get_NX(); x++)
		{
			if( (double)rand() / (double)RAND_MAX <= frequency )
			{
				pShape	= pShapes->Add_Shape();

				pShape->Add_Point(
					pGrid->Get_XMin() + x * Get_Cellsize(),
					pGrid->Get_YMin() + y * Get_Cellsize()
				);

				pShape->Get_Record()->Set_Value(0, ++n);
				pShape->Get_Record()->Set_Value(1, pGrid->asDouble(x, y));
			}
		}
	}

	return( true );
}
