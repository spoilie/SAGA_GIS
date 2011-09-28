
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                       Shapes_IO                       //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                        Xyz.cpp                        //
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
#include "xyz.h"


///////////////////////////////////////////////////////////
//														 //
//						Export							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CXYZ_Export::CXYZ_Export(void)
{
	CParameter	*pNode_0, *pNode_1;

	//-----------------------------------------------------
	Set_Name(_TL("Export Shapes to XYZ"));

	Set_Author(_TL("Copyrights (c) 2003 by Olaf Conrad"));

	Set_Description(
		_TL("XYZ export filter for shapes. ")
	);

	//-----------------------------------------------------
	pNode_0	= Parameters.Add_Shapes(
		NULL	, "SHAPES"	, _TL("Shapes"),
		"",
		PARAMETER_INPUT
	);

	pNode_1	= Parameters.Add_Table_Field(
		pNode_0	, "FIELD"	, _TL("Attribute"),
		""
	);

	pNode_0	= Parameters.Add_Value(
		NULL	, "ALL"		, _TL("Save All Attributes"),
		_TL("Ignores specified attribute ('Save Attribute') and saves all attributes."),
		PARAMETER_TYPE_Bool	, true
	);

	pNode_0	= Parameters.Add_FilePath(
		NULL	, "FILENAME", _TL("File"),
		"",
		_TL(
		"XYZ Files (*.xyz)"		"|*.xyz|"
		"Text Files (*.txt)"	"|*.txt|"
		"All Files"				"|*.*"),

		NULL, true
	);
}

//---------------------------------------------------------
CXYZ_Export::~CXYZ_Export(void)
{}

//---------------------------------------------------------
bool CXYZ_Export::On_Execute(void)
{
	bool		bAll;
	int			iShape, iPart, iPoint, iField;
	FILE		*aus;
	TGEO_Point	Point;
	CShape		*pShape;
	CShapes		*pShapes;

	//-----------------------------------------------------
	pShapes		= Parameters("SHAPES")->asShapes();
	bAll		= Parameters("ALL")->asBool();
	iField		= Parameters("FIELD")->asInt();

	if( bAll && (iField < 0 || iField >= pShapes->Get_Table().Get_Field_Count()) )
	{
		return( false );
	}

	//-----------------------------------------------------
	if( (aus = fopen(Parameters("FILENAME")->asString(), "w")) != NULL )
	{
		fprintf(aus, "X\tY");

		if( bAll )
		{
			for(iField=0; iField<pShapes->Get_Table().Get_Field_Count(); iField++)
			{
				fprintf(aus, "\t%s", pShapes->Get_Table().Get_Field_Name(iField));
			}
		}
		else
		{
			fprintf(aus, "\tZ");
		}

		fprintf(aus, "\n");

		//-------------------------------------------------
		for(iShape=0; iShape<pShapes->Get_Count() && Set_Progress(iShape, pShapes->Get_Count()); iShape++)
		{
			pShape	= pShapes->Get_Shape(iShape);

			for(iPart=0; iPart<pShape->Get_Part_Count(); iPart++)
			{
				for(iPoint=0; iPoint<pShape->Get_Point_Count(iPart); iPoint++)
				{
					Point	= pShape->Get_Point(iPoint, iPart);
					fprintf(aus, "%f\t%f", Point.x, Point.y);

					if( bAll )
					{
						for(iField=0; iField<pShapes->Get_Table().Get_Field_Count(); iField++)
						{
							switch( pShapes->Get_Table().Get_Field_Type(iField) )
							{
							case TABLE_FIELDTYPE_String:
								fprintf(aus, "\t\"%s\""	,pShape->Get_Record()->asString(iField));
								break;

							default:
								fprintf(aus, "\t%f"		,pShape->Get_Record()->asDouble(iField));
								break;
							}
						}
					}
					else
					{
						switch( pShapes->Get_Table().Get_Field_Type(iField) )
						{
						case TABLE_FIELDTYPE_String:
							fprintf(aus, "\t\"%s\""	,pShape->Get_Record()->asString(iField));
							break;

						default:
							fprintf(aus, "\t%f"		,pShape->Get_Record()->asDouble(iField));
							break;
						}
					}

					fprintf(aus, "\n");
				}
			}
		}

		//-------------------------------------------------
		fclose(aus);

		return( true );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//						Import							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CXYZ_Import::CXYZ_Import(void)
{
	CParameter	*pNode_0, *pNode_1;

	//-----------------------------------------------------
	Set_Name(_TL("Import Shapes from XYZ"));

	Set_Author(_TL("Copyrights (c) 2003 by Olaf Conrad"));

	Set_Description(
		_TL("Point shapes import from text formated XYZ-table.")
	);

	//-----------------------------------------------------
	pNode_0	= Parameters.Add_Shapes(
		NULL	, "SHAPES"		, _TL("Points"),
		"",
		PARAMETER_OUTPUT, SHAPE_TYPE_Point
	);

//	pNode_0	= Parameters.Add_Value(
//		NULL	, "HEADLINE"	, "File contains headline",
//		"",
//		PARAMETER_TYPE_Bool		, true
//	);

	pNode_1	= Parameters.Add_Value(
		pNode_0	, "X_FIELD"		, _TL("X Column"),
		"",
		PARAMETER_TYPE_Int		, 1, 1, true
	);

	pNode_1	= Parameters.Add_Value(
		pNode_0	, "Y_FIELD"		, _TL("Y Column"),
		"",
		PARAMETER_TYPE_Int		, 2, 1, true
	);

	pNode_0	= Parameters.Add_FilePath(
		NULL	, "FILENAME"	, _TL("File"),
		"",
		_TL(
		"XYZ Files (*.xyz)"		"|*.xyz|"
		"Text Files (*.txt)"	"|*.txt|"
		"All Files"				"|*.*"),

		NULL, false
	);
}

//---------------------------------------------------------
CXYZ_Import::~CXYZ_Import(void)
{}

//---------------------------------------------------------
bool CXYZ_Import::On_Execute(void)
{
	int				xField, yField, iRecord;
	CTable			Table;
	CTable_Record	*pRecord;
	CShapes			*pShapes;
	CShape			*pShape;

	//-----------------------------------------------------
	pShapes	= Parameters("SHAPES")	->asShapes();
	xField	= Parameters("X_FIELD")	->asInt() - 1;
	yField	= Parameters("Y_FIELD")	->asInt() - 1;

	//-----------------------------------------------------
	if( !Table.Create(Parameters("FILENAME")->asString()) )	// Parameters("HEADLINE")->asBool()
	{
		Message_Add(_TL("Table could not be opened."));
	}
	else if( Table.Get_Record_Count() <= 0 )
	{
		Message_Add("Table does not contain any data.");
	}
	else if( xField == yField || xField < 0 || xField >= Table.Get_Field_Count() || yField < 0 || yField >= Table.Get_Field_Count() )
	{
		Message_Add("Invalid X/Y fields.");
	}

	//-----------------------------------------------------
	else
	{
		pShapes->Create(SHAPE_TYPE_Point, Table.Get_Name(), &Table);

		for(iRecord=0; iRecord<Table.Get_Record_Count(); iRecord++)
		{
			pRecord	= Table.Get_Record(iRecord);
			pShape	= pShapes->Add_Shape(pRecord);
			pShape->Add_Point(pRecord->asDouble(xField), pRecord->asDouble(yField));
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