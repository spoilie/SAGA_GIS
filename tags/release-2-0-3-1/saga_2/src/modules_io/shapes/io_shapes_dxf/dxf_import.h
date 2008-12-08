
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                     io_shapes_dxf                     //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                     dxf_import.cpp                    //
//                                                       //
//                 Copyright (C) 2007 by                 //
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
#ifndef HEADER_INCLUDED__dxf_import_H
#define HEADER_INCLUDED__dxf_import_H


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "MLB_Interface.h"

#include "./dxflib/dl_creationadapter.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CDXF_Import : public CSG_Module, DL_CreationAdapter
{
public:
	CDXF_Import(void);
	virtual ~CDXF_Import(void);

	virtual const SG_Char *	Get_MenuPath	(void)		{	return( _TL("R:Import") );	}


protected:

	virtual bool			On_Execute		(void);


private:

	TSG_Point_3D			m_Offset;

	CSG_Shape				*m_pShape;

	CSG_Shapes				*m_pPoints, *m_pLines, *m_pPolygons;


	virtual void			addLayer		(const DL_LayerData    &data);

	virtual void			addBlock		(const DL_BlockData    &data);
	virtual void			endBlock		(void);

	virtual void			addPolyline		(const DL_PolylineData &data);
	virtual void			endSequence		(void);

	virtual void			addPoint		(const DL_PointData    &data);
	virtual void			addLine			(const DL_LineData     &data);
	virtual void			addArc			(const DL_ArcData      &data);
	virtual void			addCircle		(const DL_CircleData   &data);
	virtual void			addVertex		(const DL_VertexData   &data);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__dxf_import_H
