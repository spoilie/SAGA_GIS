
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library                     //
//                                                       //
//                       io_gdal                         //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                    ogr_import.cpp                     //
//                                                       //
//            Copyright (C) 2008 O. Conrad               //
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
//    e-mail:     oconrad@saga-gis.de                    //
//                                                       //
//    contact:    Olaf Conrad                            //
//                Bundesstr. 55                          //
//                D-20146 Hamburg                        //
//                Germany                                //
//                                                       //
///////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "ogr_import.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
COGR_Import::COGR_Import(void)
{
	Set_Name	(_TL("OGR: Import Vector Data"));

	Set_Author	(SG_T("(c) 2008 by O.Conrad"));

	CSG_String	Description;

	Description	= _TW(
		"The \"GDAL Vector Data Import\" module imports vector data from various file/database formats using the "
		"\"Geospatial Data Abstraction Library\" (GDAL) by Frank Warmerdam. "
		"For more information have a look at the GDAL homepage:\n"
		"  <a target=\"_blank\" href=\"http://www.gdal.org/\">"
		"  http://www.gdal.org</a>\n"
		"\n"
		"Following vector data formats are currently supported:\n"
		"<table border=\"1\"><tr><th>Name</th><th>Description</th></tr>\n"
	);

	for(int i=0; i<g_OGR_Driver.Get_Count(); i++)
    {
		if( g_OGR_Driver.Can_Read(i) )
		{
			Description	+= CSG_String::Format(SG_T("<tr><td>%s</td><td>%s</td></tr>\n"),
				g_OGR_Driver.Get_Name(i).b_str(),
				g_OGR_Driver.Get_Description(i).b_str()
			);
		}
    }

	Description	+= SG_T("</table>");

	Set_Description(Description);

	//-----------------------------------------------------
	Parameters.Add_Shapes_List(
		NULL, "SHAPES"	, _TL("Shapes"),
		_TL(""),
		PARAMETER_OUTPUT
	);

	Parameters.Add_FilePath(
		NULL, "FILE"	, _TL("File"),
		_TL("")
	);
}

//---------------------------------------------------------
COGR_Import::~COGR_Import(void)
{}



///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool COGR_Import::On_Execute(void)
{
	COGR_DataSource	ds;

	//-----------------------------------------------------
	if( !ds.Create(Parameters("FILE")->asString()) )
	{
		Message_Add(_TL("could not open data source"));

		return( false );
	}

	if( ds.Get_Count() <= 0 )
	{
		Message_Add(_TL("no layers in data source"));

		return( false );
	}

	//-----------------------------------------------------
	Parameters("SHAPES")->asShapesList()->Del_Items();

	for(int iLayer=0; iLayer<ds.Get_Count(); iLayer++)
	{
		CSG_Shapes	*pShapes	= ds.Read_Shapes(iLayer);

		if( pShapes )
		{
			Parameters("SHAPES")->asShapesList()->Add_Item(pShapes);
		}
	}

	return( Parameters("SHAPES")->asShapesList()->Get_Count() > 0 );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
