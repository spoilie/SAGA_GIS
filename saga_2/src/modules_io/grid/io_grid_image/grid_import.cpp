
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                       image_io                        //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                    Grid_Import.cpp                    //
//                                                       //
//                 Copyright (C) 2005 by                 //
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
//    contact:    SAGA User Group Association            //
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
#include <wx/filename.h>
#include <wx/image.h>

#include "grid_import.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CGrid_Import::CGrid_Import(void)
{
	//-----------------------------------------------------
	Set_Name	(_TL("Import Image (bmp, jpg, png, tif, gif, pnm, xpm)"));

	Set_Author	(_TL("Copyrights (c) 2005 by Olaf Conrad"));

	Set_Description(
		_TL("Loads an image.")
	);

	//-----------------------------------------------------
	Parameters.Add_Grid_Output(
		NULL	, "OUT_GRID"	, _TL("Image"),
		""
	);

	Parameters.Add_Grid_Output(
		NULL	, "OUT_RED"		, _TL("Image (Red Channel)"),
		""
	);

	Parameters.Add_Grid_Output(
		NULL	, "OUT_GREEN"	, _TL("Image (Green Channel)"),
		""
	);

	Parameters.Add_Grid_Output(
		NULL	, "OUT_BLUE"	, _TL("Image (Blue Channel)"),
		""
	);

	//-----------------------------------------------------
	Parameters.Add_FilePath(
		NULL	, "FILE"		, _TL("Image File"),
		"",
		_TL(
		"All Recognized File Types"						"|*.bmp;*.ico;*.gif;*.jpg;*.jif;*.jpeg;*.pcx;*.png;*.pnm;*.tif;*.tiff;*.xpm|"
		"CompuServe Graphics Interchange (*.gif)"		"|*.gif|"
		"JPEG - JFIF Compliant (*.jpg, *.jif, *.jpeg)"	"|*.jpg;*.jif;*.jpeg|"
		"Portable Network Graphics (*.png)"				"|*.png|"
		"Tagged Image File Format (*.tif, *.tiff)"		"|*.tif;*.tiff|"
		"Windows or OS/2 Bitmap (*.bmp)"				"|*.bmp|"
		"Zsoft Paintbrush (*.pcx)"						"|*.pcx|"
		"All Files"										"|*.*")
	);

	Parameters.Add_Choice(
		NULL	, "METHOD"		, _TL("Options"),
		"",

		_TL("Standard|"
		"Split Channels|"
		"Enforce True Color|")
	);

	//-----------------------------------------------------
//	wxInitAllImageHandlers();
	wxImage::AddHandler(new wxBMPHandler);	// For loading and saving.
	wxImage::AddHandler(new wxICOHandler);	// For loading and saving.
	wxImage::AddHandler(new wxJPEGHandler);	// For loading and saving.
	wxImage::AddHandler(new wxPCXHandler);	// For loading and saving.
	wxImage::AddHandler(new wxPNGHandler);	// For loading (including alpha support) and saving.
	wxImage::AddHandler(new wxPNMHandler);	// For loading and saving.
	wxImage::AddHandler(new wxTIFFHandler);	// For loading and saving.
	wxImage::AddHandler(new wxXPMHandler);	// For loading and saving.
	wxImage::AddHandler(new wxGIFHandler);	// Only for loading, due to legal issues.
}

//---------------------------------------------------------
CGrid_Import::~CGrid_Import(void)
{}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define ADD_GRID(g, s, t)	g	= SG_Create_Grid(t, img.GetWidth(), img.GetHeight(), Cellsize, xMin, yMin); g->Set_Name(s);

//---------------------------------------------------------
bool CGrid_Import::On_Execute(void)
{
	int					x, y, yy, Method;
	double				d, Cellsize, xMin, yMin;
	CSG_Colors				Colors;
	CGrid				*pR, *pG, *pB;
	FILE				*Stream;
	wxFileName			fName;
	wxImage				img;
	wxImageHistogram	hst;

	//-----------------------------------------------------
	fName	= Parameters("FILE")	->asString();
	Method	= Parameters("METHOD")	->asInt();

	if( img.LoadFile(fName.GetFullPath()) )
	{
		if(      !fName.GetExt().CmpNoCase("bmp") )
		{
			fName.SetExt("bpw");
		}
		else if( !fName.GetExt().CmpNoCase("jpg") )
		{
			fName.SetExt("jgw");
		}
		else if( !fName.GetExt().CmpNoCase("png") )
		{
			fName.SetExt("pgw");
		}
		else if( !fName.GetExt().CmpNoCase("tif") )
		{
			fName.SetExt("tfw");
		}
		else
		{
			fName.SetExt("world");
		}

		//-------------------------------------------------
		if( (Stream = fopen(fName.GetFullPath(), "r")) != NULL )
		{
			fscanf(Stream, "%lf %lf %lf %lf %lf %lf ", &Cellsize, &d, &d, &d, &xMin, &yMin);
			yMin		= yMin - (img.GetHeight() - 1) * Cellsize;
			fclose(Stream);
		}
		else
		{
			Cellsize	= 1.0;
			xMin		= 0.0;
			yMin		= 0.0;
		}

		//-------------------------------------------------
		if( Method == 1 )
		{
			ADD_GRID(pR, wxString::Format("%s [R]", fName.GetName().c_str()), GRID_TYPE_Byte);
			ADD_GRID(pG, wxString::Format("%s [G]", fName.GetName().c_str()), GRID_TYPE_Byte);
			ADD_GRID(pB, wxString::Format("%s [B]", fName.GetName().c_str()), GRID_TYPE_Byte);

			for(y=0, yy=pR->Get_NY()-1; y<pR->Get_NY() && Set_Progress(y, pR->Get_NY()); y++, yy--)
			{
				for(x=0; x<pR->Get_NX(); x++)
				{
					pR->Set_Value(x, y, img.GetRed  (x, yy));
					pG->Set_Value(x, y, img.GetGreen(x, yy));
					pB->Set_Value(x, y, img.GetBlue (x, yy));
				}
			}

			Parameters("OUT_RED")	->Set_Value(pR);
			Colors.Set_Palette(COLORS_PALETTE_BLACK_RED);
			DataObject_Set_Colors(pR, Colors);

			Parameters("OUT_GREEN")	->Set_Value(pG);
			Colors.Set_Palette(COLORS_PALETTE_BLACK_GREEN);
			DataObject_Set_Colors(pG, Colors);

			Parameters("OUT_BLUE")	->Set_Value(pB);
			Colors.Set_Palette(COLORS_PALETTE_BLACK_BLUE);
			DataObject_Set_Colors(pB, Colors);
		}

		//-------------------------------------------------
		else if( Method == 0 && (yy = img.ComputeHistogram(hst)) <= 256 )
		{
			Colors.Set_Count(yy);

			for(wxImageHistogram::iterator i=hst.begin(); i!=hst.end(); ++i)
			{
				Colors.Set_Color(i->second.index, COLOR_GET_R(i->first), COLOR_GET_G(i->first), COLOR_GET_B(i->first));
			}

			ADD_GRID(pR, fName.GetName(), yy <= 2 ? GRID_TYPE_Bit : GRID_TYPE_Byte);

			for(y=0, yy=pR->Get_NY()-1; y<pR->Get_NY() && Set_Progress(y, pR->Get_NY()); y++, yy--)
			{
				for(x=0; x<pR->Get_NX(); x++)
				{
					pR->Set_Value(x, y, hst[COLOR_GET_RGB(img.GetRed(x, yy), img.GetGreen(x, yy), img.GetBlue(x, yy))].index);
				}
			}

			Parameters("OUT_GRID")	->Set_Value(pR);
			DataObject_Set_Colors(pR, Colors);
		}

		//-------------------------------------------------
		else
		{
			hst.clear();

			ADD_GRID(pR, fName.GetName(), GRID_TYPE_Int);

			for(y=0, yy=pR->Get_NY()-1; y<pR->Get_NY() && Set_Progress(y, pR->Get_NY()); y++, yy--)
			{
				for(x=0; x<pR->Get_NX(); x++)
				{
					pR->Set_Value(x, y, COLOR_GET_RGB(img.GetRed(x, yy), img.GetGreen(x, yy), img.GetBlue(x, yy)));
				}
			}

			Parameters("OUT_GRID")	->Set_Value(pR);
			Colors.Set_Palette(COLORS_PALETTE_BLACK_WHITE);
			DataObject_Set_Colors(pR, Colors);
		}

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
