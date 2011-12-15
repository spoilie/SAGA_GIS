
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                       TIN_Tools                       //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//             TIN_From_Grid_Specific_Points.h           //
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
#ifndef HEADER_INCLUDED__TIN_From_Grid_Specific_Points_H
#define HEADER_INCLUDED__TIN_From_Grid_Specific_Points_H

//---------------------------------------------------------
#include "MLB_Interface.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CTIN_From_Grid_Specific_Points : public CModule_Grid
{
public:
	CTIN_From_Grid_Specific_Points(void);
	virtual ~CTIN_From_Grid_Specific_Points(void);

	virtual const char *		Get_MenuPath	(void)	{	return( _TL("R:Conversion") );	}


protected:

	virtual bool				On_Execute		(void);


private:

	bool						Get_MarkHighestNB	(CGrid *pResult, CGrid *pGrid);
	bool						Get_OppositeNB		(CGrid *pResult, CGrid *pGrid, int Threshold);
	bool						Get_FlowDirection	(CGrid *pResult, CGrid *pGrid, int Min, int Max);
	bool						Get_FlowDirection2	(CGrid *pResult, CGrid *pGrid, int Threshold);
	bool						Get_Peucker			(CGrid *pResult, CGrid *pGrid, double Threshold);

};

#endif // #ifndef HEADER_INCLUDED__TIN_From_Grid_Specific_Points_H