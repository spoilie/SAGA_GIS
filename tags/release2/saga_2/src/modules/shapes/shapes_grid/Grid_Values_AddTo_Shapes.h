
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
//               Grid_Values_AddTo_Shapes.h              //
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
//                                                       //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#ifndef HEADER_INCLUDED__Grid_Values_AddTo_Shapes_H
#define HEADER_INCLUDED__Grid_Values_AddTo_Shapes_H


//---------------------------------------------------------
#include "MLB_Interface.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CGrid_Values_AddTo_Shapes : public CModule  
{
public:
	CGrid_Values_AddTo_Shapes(void);
	virtual ~CGrid_Values_AddTo_Shapes(void);


protected:

	virtual bool				On_Execute			(void);


private:

	int							Interpol;

	CShapes						*pResult;

	CParameter_Grid_List		*pGrids;


	void						Get_Data_Point		(CShape *pShape );
	void						Get_Data_Line		(CShape *pShape );
	void						Get_Data_Polygon	(CShape *pShape );

};

#endif // #ifndef HEADER_INCLUDED__Grid_Values_AddTo_Shapes_H