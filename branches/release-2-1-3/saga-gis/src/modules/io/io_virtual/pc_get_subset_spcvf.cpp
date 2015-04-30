/**********************************************************
 * Version $Id$
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                      io_virtual                       //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                pc_get_subset_spcvf.cpp                //
//                                                       //
//                 Copyright (C) 2014 by                 //
//                    Volker Wichmann                    //
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
// 51 Franklin Street, 5th Floor, Boston, MA 02110-1301, //
// USA.                                                  //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//    e-mail:     wichmann@laserdata.at                  //
//                                                       //
//    contact:    LASERDATA GmbH                         //
//                Management and Analysis of             //
//                Laserscanning Data                     //
//                Technikerstr. 21a                      //
//                6020 Innsbruck                         //
//                Austria                                //
//                www.laserdata.at                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "pc_get_subset_spcvf.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CPointCloud_Get_Subset_SPCVF_Base::CPointCloud_Get_Subset_SPCVF_Base(void)
{
	m_pShapes			= NULL;
	m_pFilePath			= NULL;
	m_pPointCloudList	= NULL;
}


//---------------------------------------------------------
CPointCloud_Get_Subset_SPCVF_Base::~CPointCloud_Get_Subset_SPCVF_Base(void)
{
	Finalise();
}


//---------------------------------------------------------
void CPointCloud_Get_Subset_SPCVF_Base::Initialise(int iOutputs, CSG_Rect AOI, CSG_Shapes *pShapes, int iFieldName, bool bMultiple, bool bAddOverlap, double dOverlap, CSG_String sFileName, CSG_Parameter_File_Name *pFilePath, CSG_Parameter_PointCloud_List *pPointCloudList)
{
	m_iOutputs			= iOutputs;
	m_AOI				= AOI;
	m_pShapes			= pShapes;
	m_iFieldName		= iFieldName;
	m_bMultiple			= bMultiple;
	m_bAddOverlap		= bAddOverlap;
	m_dOverlap			= dOverlap;
	m_sFileName			= sFileName;
	m_pFilePath			= pFilePath;
	m_pPointCloudList	= pPointCloudList;

	return;
}


//---------------------------------------------------------
void CPointCloud_Get_Subset_SPCVF_Base::Finalise(void)
{
	m_pShapes			= NULL;
	m_pFilePath			= NULL;
	m_pPointCloudList	= NULL;

	return;
}


//---------------------------------------------------------
bool CPointCloud_Get_Subset_SPCVF_Base::Get_Subset(void)
{
	CSG_String		sPathSPCVF, sMethodPaths;
	double			dBBoxXMin, dBBoxYMin, dBBoxXMax, dBBoxYMax;
	CSG_Rect		BBoxSPCVF;
	CSG_MetaData	SPCVF;
	

	//-----------------------------------------------------
	if( !SPCVF.Create(m_sFileName) || SPCVF.Get_Name().CmpNoCase(SG_T("SPCVFDataset")) )
	{
		SG_UI_Msg_Add_Error(_TL("Please provide a valid *.scpvf file!"));
		return( false );
	}

	//-----------------------------------------------------
	SPCVF.Get_Property(SG_T("Paths"), sMethodPaths);

	if( !sMethodPaths.CmpNoCase(SG_T("absolute")) )
	{
		sPathSPCVF = SG_T("");
	}
	else if( !sMethodPaths.CmpNoCase(SG_T("relative")) )
	{
		sPathSPCVF = SG_File_Get_Path(m_sFileName);
		sPathSPCVF.Replace(SG_T("\\"), SG_T("/"));
	}
	else
	{
		SG_UI_Msg_Add_Error(_TL("Encountered invalid path description in *.spcvf file!"));
		return( false );
	}

	//-----------------------------------------------------
	SPCVF.Get_Child(SG_T("BBox"))->Get_Property(SG_T("XMin"), dBBoxXMin);
	SPCVF.Get_Child(SG_T("BBox"))->Get_Property(SG_T("YMin"), dBBoxYMin);
	SPCVF.Get_Child(SG_T("BBox"))->Get_Property(SG_T("XMax"), dBBoxXMax);
	SPCVF.Get_Child(SG_T("BBox"))->Get_Property(SG_T("YMax"), dBBoxYMax);
	BBoxSPCVF.Assign(dBBoxXMin, dBBoxYMin, dBBoxXMax, dBBoxYMax);


	//-----------------------------------------------------
	int				iDatasets = 0;
	CSG_PointCloud	*pPC_out = NULL;

	for(int iAOI=0; iAOI<m_iOutputs; iAOI++)
	{
		if( m_iOutputs > 1 )
		{
			SG_UI_Process_Set_Text(CSG_String::Format(_TL("Processing AOI %d ..."), iAOI + 1));
		}
		else
		{
			SG_UI_Process_Set_Text(_TL("Processing AOI ..."));
		}

		if( m_bMultiple )
		{
			iDatasets = 0;
		}

		if( m_pShapes != NULL )
		{
			if( m_pShapes->Get_Selection_Count() > 0 && !m_pShapes->is_Selected(iAOI) )
			{
				continue;
			}

			m_AOI = m_pShapes->Get_Shape(iAOI)->Get_Extent();
		}

		if( m_bAddOverlap )
		{
			m_AOI.Inflate(m_dOverlap, false);
		}
		else
		{
			m_dOverlap = 0.0;
		}
	
		if( m_AOI.Intersects(BBoxSPCVF) == INTERSECTION_None )
		{
			SG_UI_Msg_Add(_TL("AOI does not intersect bounding box of SPCVF, nothing to do!"), true);
			continue;
		}


		//-----------------------------------------------------
		CSG_MetaData	*pDatasets = SPCVF.Get_Child(SG_T("Datasets"));

		CSG_Strings		sFilePaths;

		for(int i=0; i<pDatasets->Get_Children_Count(); i++)
		{
			CSG_MetaData	*pDataset	= pDatasets->Get_Child(i);
			CSG_MetaData	*pBBox		= pDataset->Get_Child(SG_T("BBox"));

			pBBox->Get_Property(SG_T("XMin"), dBBoxXMin);
			pBBox->Get_Property(SG_T("YMin"), dBBoxYMin);
			pBBox->Get_Property(SG_T("XMax"), dBBoxXMax);
			pBBox->Get_Property(SG_T("YMax"), dBBoxYMax);

			CSG_Rect BBox(dBBoxXMin, dBBoxYMin, dBBoxXMax, dBBoxYMax);

			if( m_AOI.Intersects(BBox) > INTERSECTION_None )
			{
				if( m_pShapes != NULL && !m_bAddOverlap && m_pShapes->Get_Shape(iAOI)->Intersects(BBox) == INTERSECTION_None )
				{
					continue;
				}

				CSG_String sFilePath;

				pDataset->Get_Property(SG_T("File"), sFilePath);
				sFilePath.Prepend(sPathSPCVF);

				sFilePaths.Add(sFilePath);
			}
		}

		if( sFilePaths.Get_Count() == 0 )
		{
			SG_UI_Msg_Add(_TL("AOI does not intersect with any bounding box of the SPCVF datasets, nothing to do!"), true);
			continue;
		}


		//-----------------------------------------------------
		for(int i=0; i<sFilePaths.Get_Count() && SG_UI_Process_Set_Progress(i, sFilePaths.Get_Count()); i++)
		{
			CSG_PointCloud	*pPC = SG_Create_PointCloud(sFilePaths.Get_String(i));

			if( pPC_out == NULL && i == 0 )
			{
				pPC_out = SG_Create_PointCloud(pPC);
			}

			bool bFound = false;

			for(int iPoint=0; iPoint<pPC->Get_Count(); iPoint++)
			{
				if( m_AOI.Contains(pPC->Get_X(iPoint), pPC->Get_Y(iPoint)) )
				{
					if( m_pShapes != NULL && !m_bAddOverlap )
					{
						CSG_Shape_Polygon	*pPolygon	= (CSG_Shape_Polygon *)m_pShapes->Get_Shape(iAOI);

						if( !pPolygon->Contains(pPC->Get_X(iPoint), pPC->Get_Y(iPoint)) )
							continue;
					}
							
					pPC_out->Add_Point(pPC->Get_X(iPoint), pPC->Get_Y(iPoint), pPC->Get_Z(iPoint));

					for(int iField=0; iField<pPC->Get_Attribute_Count(); iField++)
					{
						pPC_out->Set_Attribute(iField, pPC->Get_Attribute(iPoint, iField));
					}

					bFound = true;
				}
			}

			if( bFound )
			{
				iDatasets++;
			}

			delete( pPC );
		}

		//---------------------------------------------------------
		if( m_bMultiple )
		{
			if( pPC_out != NULL && pPC_out->Get_Count() == 0 )
			{
				SG_UI_Msg_Add(_TL("AOI does not intersect with any point of the SPCVF datasets, nothing to do!"), true);
				delete( pPC_out );
				pPC_out = NULL;
				continue;
			}

			Write_Subset(pPC_out, iAOI, iDatasets);

			pPC_out = NULL;
		}
	}

	//---------------------------------------------------------
	if( !m_bMultiple )
	{
		if( pPC_out != NULL && pPC_out->Get_Count() == 0 )
		{
			SG_UI_Msg_Add(_TL("AOI does not intersect with any point of the SPCVF datasets, nothing to do!"), true);
			delete( pPC_out );
			return( true );
		}

		Write_Subset(pPC_out, 0, iDatasets);
	}

	return( true );
}


//---------------------------------------------------------
void CPointCloud_Get_Subset_SPCVF_Base::Write_Subset(CSG_PointCloud *pPC_out, int iAOI, int iDatasets)
{
	CSG_String	sPath = SG_T("");

	if( m_pFilePath != NULL )
	{
		sPath = m_pFilePath->asString();
		sPath += SG_T("/");
	}

	if( m_bMultiple )
	{
		if( m_iFieldName > -1 )
		{
			pPC_out->Set_Name(CSG_String::Format(SG_T("%s%s"), sPath.c_str(), m_pShapes->Get_Record(iAOI)->asString(m_iFieldName)));
		}
		else
		{
			pPC_out->Set_Name(CSG_String::Format(SG_T("%s%d_%d"), sPath.c_str(), (int)(m_AOI.Get_XMin() + m_dOverlap), (int)(m_AOI.Get_YMin() + m_dOverlap)));
		}
	}
	else
	{
		pPC_out->Set_Name(CSG_String::Format(SG_T("%spc_subset_%s"), sPath.c_str(), SG_File_Get_Name(m_sFileName, false).c_str()));
	}

	SG_UI_Msg_Add(CSG_String::Format(_TL("%d points from %d dataset(s) written to output point cloud %s."), pPC_out->Get_Count(), iDatasets, pPC_out->Get_Name()), true);

	if( m_pFilePath == NULL )
	{
		m_pPointCloudList->Add_Item(pPC_out);
	}
	else
	{
		pPC_out->Save(pPC_out->Get_Name());
		delete( pPC_out );
	}

	return;
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CPointCloud_Get_Subset_SPCVF::CPointCloud_Get_Subset_SPCVF(void)
{
	Set_Name		(_TL("Get Subset from Virtual Point Cloud"));

	Set_Author		(SG_T("V. Wichmann, LASERDATA GmbH (c) 2014"));

	Set_Description	(_TW(
		"The module allows to retrieve a point cloud from a virtual "
		"point cloud dataset by applying the provided area-of-interest "
		"(AOI). The extent of the AOI can be provided either as polygon "
		"shapefile, grid or by coordinates. Optionally, an overlap can "
		"be added to the AOI. In case an overlap is used and the AOI "
		"is provided as polygon shapfile, only the bounding boxes of the "
		"polygons are used.\n"
		"With polygon shapefiles additional functionality is available:\n"
		"* in case one or more polygons are selected, only the selected "
		"polygons are used.\n"
		"* in case the shapefile contains several polygons and the "
		"'One Point Cloud per Polygon' parameter is checked, a point "
		"cloud dataset is outputted for each polygon. In case the "
		"'Tilename' attribute is provided, the output files are named "
		"by this attribute. Otherwise the output file names are build "
		"from the lower left coordinate of each tile.\n"
		"The derived datasets can be outputted either as point cloud "
		"list or written to an output directory. For the latter, "
		"you must provide a valid file path with the 'Optional Output "
		"Filepath' parameter.\n"
		"A virtual point cloud dataset is a simple XML format "
		"with the file extension .spcvf, which can be created "
		"with the 'Create Virtual Point Cloud Dataset' module.\n\n"
	));


	//-----------------------------------------------------
	Parameters.Add_FilePath(
		NULL	, "FILENAME"		, _TL("Filename"),
		_TL("The full path and name of the .spcvf file"),
		CSG_String::Format(SG_T("%s|%s|%s|%s"),
			_TL("SAGA Point Cloud Virtual Format (*.spcvf)")	, SG_T("*.spcvf"),
			_TL("All Files")									, SG_T("*.*")
		)
 	);

	Parameters.Add_PointCloud_List(
		NULL	, "PC_OUT"			, _TL("Point Cloud"),
		_TL("The output point cloud(s)"),
		PARAMETER_OUTPUT_OPTIONAL
	);

	Parameters.Add_FilePath(
		NULL	, "FILEPATH"		, _TL("Optional Output Filepath"),
		_TL("The full path to which the output(s) should be written. Leave empty to output the datasets as point cloud list."),
		(const wchar_t *)0, (const wchar_t *)0,
		true, true, false
	);


	CSG_Parameter *pNode = Parameters.Add_Node(NULL, "NODE_AOI", _TL("AOI"), _TL("AOI Input Settings"));

	CSG_Parameter *pNodeField = Parameters.Add_Shapes(
		pNode	, "AOI_SHP"			, _TL("Shape"),
		_TL("Shapefile describing the AOI."),
		PARAMETER_INPUT_OPTIONAL, SHAPE_TYPE_Polygon
	);
	Parameters.Add_Table_Field(
		pNodeField	, "FIELD_TILENAME", _TL("Tilename"),
		_TL("Attribute used for naming the output file(s)"), true
	);

	Parameters.Add_Grid(
		pNode	, "AOI_GRID"		, _TL("Grid"),
		_TL("Grid describing the AOI."),
		PARAMETER_INPUT_OPTIONAL, false
	);

	Parameters.Add_Range(
		pNode	, "AOI_XRANGE"		, _TL("X-Extent"),
		_TL("Minimum and maximum x-coordinate of AOI."),
		0.0, 0.0
	);
	Parameters.Add_Range(
		pNode	, "AOI_YRANGE"		, _TL("Y-Extent"),
		_TL("Minimum and maximum y-coordinate of AOI."),
		0.0, 0.0
	);

	Parameters.Add_Value(
		pNode	, "AOI_ADD_OVERLAP"	, _TL("Add Overlap"),
		_TL("Add overlap to AOI"),
		PARAMETER_TYPE_Bool, false
	);
	Parameters.Add_Value(
		Parameters("AOI_ADD_OVERLAP")	, "OVERLAP"	, _TL("Overlap"),
		_TL("Overlap [map units]"),
		PARAMETER_TYPE_Double,
		50.0, 0.0, true
	);

	Parameters.Add_Value(
		pNode	, "ONE_PC_PER_POLYGON"	, _TL("One Point Cloud per Polygon"),
		_TL("Write one point cloud dataset for each polygon"),
		PARAMETER_TYPE_Bool, false
	);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CPointCloud_Get_Subset_SPCVF::On_Execute(void)
{
	CSG_String		sFileName;
	CSG_Parameter_PointCloud_List	*pPointCloudList = NULL;
	CSG_Parameter_File_Name	*pFilePath = NULL;
	CSG_Shapes		*pShapes;
	int				iFieldName;
	CSG_Grid		*pGrid = NULL;
	double			dAoiXMin, dAoiYMin, dAoiXMax, dAoiYMax;
	bool			bAddOverlap;
	double			dOverlap;
	bool			bMultiple;
	int				iOutputs = 1;
	CSG_Rect		AOI;


	//-----------------------------------------------------
	sFileName		= Parameters("FILENAME")->asString();
	pPointCloudList	= Parameters("PC_OUT")->asPointCloudList();
	pFilePath		= Parameters("FILEPATH")->asFilePath();
	pShapes			= Parameters("AOI_SHP")->asShapes();
	iFieldName		= Parameters("FIELD_TILENAME")->asInt();
	pGrid			= Parameters("AOI_GRID")->asGrid();
	dAoiXMin		= Parameters("AOI_XRANGE")->asRange()->Get_LoVal();
	dAoiXMax		= Parameters("AOI_XRANGE")->asRange()->Get_HiVal();
	dAoiYMin		= Parameters("AOI_YRANGE")->asRange()->Get_LoVal();
	dAoiYMax		= Parameters("AOI_YRANGE")->asRange()->Get_HiVal();

	bAddOverlap		= Parameters("AOI_ADD_OVERLAP")->asBool();
	dOverlap		= Parameters("OVERLAP")->asDouble();
	bMultiple		= Parameters("ONE_PC_PER_POLYGON")->asBool();

	if( pShapes == NULL )
	{
		bMultiple = false;
	}

	CSG_String	sPath = pFilePath->asString();
	
	if( sPath.Length() <= 1 )
	{
		pFilePath = NULL;
	}


	//-----------------------------------------------------
	if( pShapes == NULL && pGrid == NULL && (dAoiXMin == dAoiXMax || dAoiYMin == dAoiYMax) )
	{
		SG_UI_Msg_Add_Error(_TL("Please provide a valid AOI!"));
		return( false );
	}


	//-----------------------------------------------------
	if( pShapes != NULL )
	{
		iOutputs = pShapes->Get_Count();
	}
	else if( pGrid != NULL )
	{
		AOI = pGrid->Get_System().Get_Extent();
	}
	else
	{
		AOI.Assign(dAoiXMin, dAoiYMin, dAoiXMax, dAoiYMax);
	}

	m_Get_Subset_SPCVF.Initialise(iOutputs, AOI, pShapes, iFieldName, bMultiple, bAddOverlap, dOverlap, sFileName, pFilePath, pPointCloudList);

	bool bResult = m_Get_Subset_SPCVF.Get_Subset();

	//---------------------------------------------------------
	if( SG_UI_Get_Window_Main() )
	{
		for(int i=0; i<pPointCloudList->Get_Count(); i++)
		{
			CSG_Parameters	sParms;
			DataObject_Get_Parameters(pPointCloudList->asPointCloud(i), sParms);

			if( sParms("METRIC_ZRANGE") )
			{
				sParms("METRIC_ZRANGE")->asRange()->Set_Range(pPointCloudList->asPointCloud(i)->Get_Minimum(2), pPointCloudList->asPointCloud(i)->Get_Maximum(2));
			}

			DataObject_Set_Parameters(pPointCloudList->asPointCloud(i), sParms);
	
			DataObject_Update(pPointCloudList->asPointCloud(i));
		}
	}

	m_Get_Subset_SPCVF.Finalise();

	return( bResult );
}


//---------------------------------------------------------
int CPointCloud_Get_Subset_SPCVF::On_Parameters_Enable(CSG_Parameters *pParameters, CSG_Parameter *pParameter)
{
	if(	!SG_STR_CMP(pParameter->Get_Identifier(), SG_T("AOI_ADD_OVERLAP")) )
	{
		pParameters->Get_Parameter("OVERLAP")->Set_Enabled(pParameter->asBool());
	}

	if(	!SG_STR_CMP(pParameter->Get_Identifier(), SG_T("AOI_SHP")) )
	{
		pParameters->Get_Parameter("FIELD_TILENAME"		)->Set_Enabled(pParameter->asShapes() != NULL);
		pParameters->Get_Parameter("ONE_PC_PER_POLYGON"	)->Set_Enabled(pParameter->asShapes() != NULL);
	}
	
	//-----------------------------------------------------
	return (1);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CPointCloud_Get_Subset_SPCVF_Interactive::CPointCloud_Get_Subset_SPCVF_Interactive(void)
{
	Set_Name		(_TL("Get Subset from Virtual Point Cloud"));

	Set_Author		(SG_T("V. Wichmann, LASERDATA GmbH (c) 2014"));

	Set_Description	(_TW(
		"The module allows to retrieve a point cloud from a virtual "
		"point cloud dataset by dragging a box (AOI) in a Map View.\n"
		"A virtual point cloud dataset is a simple XML format "
		"with the file extension .spcvf, which can be created "
		"with the 'Create Virtual Point Cloud Dataset' module.\n\n"
	));


	//-----------------------------------------------------
	Parameters.Add_FilePath(
		NULL	, "FILENAME"		, _TL("Filename"),
		_TL("The full path and name of the .spcvf file"),
		CSG_String::Format(SG_T("%s|%s|%s|%s"),
			_TL("SAGA Point Cloud Virtual Format (*.spcvf)")	, SG_T("*.spcvf"),
			_TL("All Files")									, SG_T("*.*")
		)
 	);

	Parameters.Add_PointCloud_Output(
		NULL	, "PC_OUT"			, _TL("Point Cloud"),
		_TL("The output point cloud")
	);

}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CPointCloud_Get_Subset_SPCVF_Interactive::On_Execute(void)
{

	return( true );
}


//---------------------------------------------------------
bool CPointCloud_Get_Subset_SPCVF_Interactive::On_Execute_Position(CSG_Point ptWorld, TSG_Module_Interactive_Mode Mode)
{
	switch( Mode )
	{
	//-----------------------------------------------------
	case MODULE_INTERACTIVE_LDOWN:

		m_ptDown	= ptWorld;

		return( true );

	//-----------------------------------------------------
	case MODULE_INTERACTIVE_LUP:

		CSG_Rect	AOI(m_ptDown.Get_X(), m_ptDown.Get_Y(), ptWorld.Get_X(), ptWorld.Get_Y());

		CSG_Parameter_PointCloud_List	PointCloudList(NULL, PARAMETER_INFORMATION);

		// as long as this module only supports to drag a box, we initialize it with a fake overlap in order
		// to use CSG_Rect instead of CSG_Shape for point in polygon check in Get_Subset():
		m_Get_Subset_SPCVF.Initialise(1, AOI, NULL, -1, false, true, 0.0, Parameters("FILENAME")->asString(), NULL, &PointCloudList);

		bool bResult = m_Get_Subset_SPCVF.Get_Subset();

		if( bResult )
		{
			Parameters("PC_OUT")->Set_Value(PointCloudList.asPointCloud(0));
			CSG_PointCloud *pPC = Parameters("PC_OUT")->asPointCloud();

			CSG_Parameters	sParms;
			DataObject_Get_Parameters(pPC, sParms);

			if( sParms("METRIC_ZRANGE") )
			{
				sParms("METRIC_ZRANGE")->asRange()->Set_Range(pPC->Get_Minimum(2), pPC->Get_Maximum(2));
			}

			DataObject_Set_Parameters(pPC, sParms);
	
			DataObject_Update(pPC, SG_UI_DATAOBJECT_SHOW_LAST_MAP);
		}

		m_Get_Subset_SPCVF.Finalise();

		return( bResult );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------