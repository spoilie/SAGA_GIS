
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                       io_shapes                       //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                    Surfer_BLN.cpp                     //
//                                                       //
//                 Copyright (C) 2006 by                 //
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
#include "surfer_bln.h"


///////////////////////////////////////////////////////////
//														 //
//						Import							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSurfer_BLN_Import::CSurfer_BLN_Import(void)
{
	CParameter	*pNode;

	//-----------------------------------------------------
	// 1. Info...

	Set_Name(_TL("Import Surfer Blanking Files"));

	Set_Author(_TL("Copyrights (c) 2006 by Olaf Conrad"));

	Set_Description(_TL(
		"Import polygons/polylines from Golden Software's Surfer Blanking File format.\n")
	);


	//-----------------------------------------------------
	// 2. Parameters...

	Parameters.Add_Shapes(
		NULL	, "SHAPES"	, _TL("Shapes"),
		"",
		PARAMETER_OUTPUT
	);

	Parameters.Add_Table(
		NULL	, "TABLE"	, _TL("Look up table (Points)"),
		"",
		PARAMETER_OUTPUT_OPTIONAL
	);

	Parameters.Add_FilePath(
		NULL	, "FILE"	, _TL("File"),
		"",
		_TL("Surfer Blanking Files (*.bln)|*.bln|All Files|*.*")
	);

	pNode	= Parameters.Add_Choice(
		NULL	, "TYPE"	, _TL("Shape Type"),
		"",
		_TL(
		"points|"
		"lines|"
		"polygons|"),	1
	);
}

//---------------------------------------------------------
CSurfer_BLN_Import::~CSurfer_BLN_Import(void)
{}

//---------------------------------------------------------
bool CSurfer_BLN_Import::On_Execute(void)
{
	bool			bOk;
	int				ID, Flag, iPoint, nPoints;
	double			x, y;
	FILE			*Stream;
	TShape_Type		Type;
	CAPI_String		FileName, sLine, sName, sDesc, sTemp;
	CTable_Record	*pRecord;
	CTable			*pTable;
	CShape			*pShape;
	CShapes			*pShapes;

	//-----------------------------------------------------
	pShapes		= Parameters("SHAPES")	->asShapes();
	pTable		= Parameters("TABLE")	->asTable();
	FileName	= Parameters("FILE")	->asString();

	switch( Parameters("TYPE")->asInt() )
	{
	case 0:				Type	= SHAPE_TYPE_Point;		break;
	case 1:	default:	Type	= SHAPE_TYPE_Line;		break;
	case 2:				Type	= SHAPE_TYPE_Polygon;	break;
	}

	//-----------------------------------------------------
	if( (Stream = fopen(FileName, "r")) != NULL )
	{
		bOk		= true;
		ID		= 0;

		pShapes->Create(Type, API_Extract_File_Name(FileName, true));

		if( Type == SHAPE_TYPE_Point )
		{
			if( pTable == NULL )
			{
				pTable	= API_Create_Table();
				Parameters("TABLE")->Set_Value(pTable);
			}
			else
			{
				pTable->Destroy();
			}

			pTable->			 Add_Field("ID"		, TABLE_FIELDTYPE_Int);
			pTable->			 Add_Field("FLAG"	, TABLE_FIELDTYPE_Int);
			pTable->			 Add_Field("NAME"	, TABLE_FIELDTYPE_String);
			pTable->			 Add_Field("DESC"	, TABLE_FIELDTYPE_String);

			pShapes->Get_Table().Add_Field("ID"		, TABLE_FIELDTYPE_Int);
			pShapes->Get_Table().Add_Field("ID_LUT"	, TABLE_FIELDTYPE_Int);
			pShapes->Get_Table().Add_Field("Z"		, TABLE_FIELDTYPE_Double);
		}
		else
		{
			pShapes->Get_Table().Add_Field("ID"		, TABLE_FIELDTYPE_Int);
			pShapes->Get_Table().Add_Field("FLAG"	, TABLE_FIELDTYPE_Int);
			pShapes->Get_Table().Add_Field("NAME"	, TABLE_FIELDTYPE_String);
			pShapes->Get_Table().Add_Field("DESC"	, TABLE_FIELDTYPE_String);
		}

		//-------------------------------------------------
		while( bOk && API_Read_Line(Stream, sLine) && sLine.BeforeFirst(',').asInt(nPoints) && nPoints > 0 && Process_Get_Okay(true) )
		{
			Process_Set_Text(CAPI_String::Format("%d. %s", ++ID, _TL("shape in process")));

			sTemp	= sLine.AfterFirst (',');	sLine	= sTemp;
			Flag	= sLine.BeforeFirst(',').asInt();

			sTemp	= sLine.AfterFirst (',');	sLine	= sTemp;
			sTemp	= sLine.BeforeFirst(',');
			sName	= sTemp.AfterFirst('\"').BeforeLast('\"');

			sTemp	= sLine.AfterFirst (',');	sLine	= sTemp;
			sTemp	= sLine.BeforeFirst(',');
			sDesc	= sTemp.AfterFirst('\"').BeforeLast('\"');

			if( Type == SHAPE_TYPE_Point )
			{
				pRecord	= pTable->Add_Record();
				pRecord->Set_Value(0, ID);
				pRecord->Set_Value(1, Flag);
				pRecord->Set_Value(2, sName);
				pRecord->Set_Value(3, sDesc);

				for(iPoint=0; iPoint<nPoints && bOk; iPoint++)
				{
					if( (bOk = API_Read_Line(Stream, sLine)) == true )
					{
						pShape	= pShapes->Add_Shape();
						pShape->Get_Record()->Set_Value(0, iPoint + 1);
						pShape->Get_Record()->Set_Value(1, ID);
						pShape->Get_Record()->Set_Value(2, sLine.AfterLast (',').asDouble());

						x	= sLine.BeforeFirst(',').asDouble();
						y	= sLine.AfterFirst (',').asDouble();
						pShape->Add_Point(x, y);
					}
				}
			}
			else
			{
				pShape	= pShapes->Add_Shape();
				pShape->Get_Record()->Set_Value(0, ID);
				pShape->Get_Record()->Set_Value(1, Flag);
				pShape->Get_Record()->Set_Value(2, sName);
				pShape->Get_Record()->Set_Value(3, sDesc);

				for(iPoint=0; iPoint<nPoints && bOk; iPoint++)
				{
					if( (bOk = API_Read_Line(Stream, sLine)) == true )
					{
						x	= sLine.BeforeFirst(',').asDouble();
						y	= sLine.AfterFirst (',').asDouble();
						pShape->Add_Point(x, y);
					}
				}
			}
		}

		fclose(Stream);
	}

	//-----------------------------------------------------
	if( pShapes->is_Valid() && pShapes->Get_Count() > 0 )
	{
		return( true );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//						Export							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSurfer_BLN_Export::CSurfer_BLN_Export(void)
{
	//-----------------------------------------------------
	// 1. Info...

	Set_Name(_TL("Export Surfer Blanking File"));

	Set_Author(_TL("Copyrights (c) 2006 by Olaf Conrad"));

	Set_Description(_TL(
		"Export shapes to Golden Software's Surfer Blanking File format.\n")
	);


	//-----------------------------------------------------
	// 2. Parameters...

	CParameter	*pNode;

	pNode	= Parameters.Add_Shapes(
		NULL	, "SHAPES"	, _TL("Shapes"),
		"",
		PARAMETER_INPUT
	);

	Parameters.Add_Table_Field(
		pNode	, "NAME"	, _TL("Name"),
		""
	);

	Parameters.Add_Table_Field(
		pNode	, "DESC"	, _TL("Description"),
		""
	);

	Parameters.Add_Table_Field(
		pNode	, "ZVAL"	, _TL("z values"),
		""
	);

	Parameters.Add_Value(
		NULL	, "BNAME"	, _TL("Export names"),
		"",
		PARAMETER_TYPE_Bool	, false
	);

	Parameters.Add_Value(
		NULL	, "BDESC"	, _TL("Export descriptions"),
		"",
		PARAMETER_TYPE_Bool	, false
	);

	Parameters.Add_Value(
		NULL	, "BZVAL"	, _TL("Export z values"),
		"",
		PARAMETER_TYPE_Bool	, false
	);

	Parameters.Add_FilePath(
		NULL	, "FILE"	, _TL("File"),
		"",
		_TL(
		"Surfer Blanking Files (*.bln)|*.bln|All Files|*.*"), NULL, true
	);
}

//---------------------------------------------------------
CSurfer_BLN_Export::~CSurfer_BLN_Export(void)
{}

//---------------------------------------------------------
bool CSurfer_BLN_Export::On_Execute(void)
{
	int			iShape, iPart, iPoint, iName, iDesc, iZVal, Flag;
	double		z;
	FILE		*Stream;
	TGEO_Point	p;
	CShape		*pShape;
	CShapes		*pShapes;

	//-----------------------------------------------------
	pShapes	= Parameters("SHAPES")	->asShapes();

	iName	= Parameters("BNAME")->asBool() ? Parameters("NAME")->asInt() : -1;
	iDesc	= Parameters("BDESC")->asBool() ? Parameters("DESC")->asInt() : -1;
	iZVal	= Parameters("BZVAL")->asBool() ? Parameters("ZVAL")->asInt() : -1;

	Flag	= 1;

	//-----------------------------------------------------
	if( (Stream = fopen(Parameters("FILE")->asString(), "w")) != NULL )
	{
		for(iShape=0; iShape<pShapes->Get_Count() && Set_Progress(iShape, pShapes->Get_Count()); iShape++)
		{
			pShape	= pShapes->Get_Shape(iShape);

			if( iZVal >= 0 )
			{
				z		= pShape->Get_Record()->asDouble(iZVal);
			}

			for(iPart=0; iPart<pShape->Get_Part_Count(); iPart++)
			{
				fprintf(Stream, "%d,%d", pShape->Get_Point_Count(iPart), Flag);

				if( iName >= 0 )
				{
					fprintf(Stream, ",\"%s\"", pShape->Get_Record()->asString(iName));
				}

				if( iDesc >= 0 )
				{
					fprintf(Stream, ",\"%s\"", pShape->Get_Record()->asString(iDesc));
				}

				fprintf(Stream, "\n");

				for(iPoint=0; iPoint<pShape->Get_Point_Count(iPart); iPoint++)
				{
					p	= pShape->Get_Point(iPoint, iPart);

					if( iZVal >= 0 )
					{
						fprintf(Stream, "%f,%f,%f\n", p.x, p.y, z);
					}
					else
					{
						fprintf(Stream, "%f,%f\n"   , p.x, p.y);
					}
				}
			}
		}

		fclose(Stream);

		return( true );
	}

	//-----------------------------------------------------
	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------