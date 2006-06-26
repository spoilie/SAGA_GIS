/*******************************************************************************
    GPX2SHP.cpp
    Copyright (C) Victor Olaya

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*******************************************************************************/
#include "gpx2shp.h"

CGPX2SHP::CGPX2SHP(){
				   
	Parameters.Set_Name(_TL("GPX to shapefile"));

	Parameters.Set_Description(_TL(
		"Converts a GPX file into a Shapefile (.shp)"
		"(c) 2005 by Victor Olaya\r\nemail: volaya@ya.com")
	);

	Parameters.Add_FilePath(NULL,
							"BASEPATH",
							_TL("Gpx2shp path"),
							_TL("Gpx2shp path"),
							"",
							"",
							false,
							true);

	Parameters.Add_FilePath(NULL, 
							"FILE", 
							_TL("GPX file"),
							"",
							_TL("GPX files (*.gpx)|*.gpx|All Files|*.*|")
	);
	
	Parameters.Add_Value(NULL,
						"TRACKPOINTS", 
						_TL("Convert track points"),
						_TL("Convert track points"),
						PARAMETER_TYPE_Bool,
						true);
	
	Parameters.Add_Value(NULL,
						"WAYPOINTS", 
						_TL("Convert way points"),
						_TL("Convert way points"),
						PARAMETER_TYPE_Bool,
						true);

	Parameters.Add_Value(NULL,
						"ROUTES", 
						_TL("Convert routes"),
						_TL("Convert routes"),
						PARAMETER_TYPE_Bool,
						true);
	
	Parameters.Add_Value(NULL,
						"ADD", 
						_TL("Load shapefile"),
						_TL("Load shapefile after conversion"),
						PARAMETER_TYPE_Bool,
						true);

}//constructor

CGPX2SHP::~CGPX2SHP(){

}//destructor

bool CGPX2SHP::On_Execute(void){

	CAPI_String sCmd;
	CAPI_String sFile = Parameters("FILE")->asString();
	CAPI_String sBasePath = Parameters("BASEPATH")->asString();
	CAPI_String sShapefile;
	bool bWaypoints = Parameters("WAYPOINTS")->asBool();
	bool bTrackpoints = Parameters("TRACKPOINTS")->asBool();
	bool bRoutes = Parameters("ROUTES")->asBool();
	bool bAdd = Parameters("ADD")->asBool();
	CShapes *pShapes;

	sCmd = sBasePath + "\\gpx2shp ";

	if (bWaypoints){
		sCmd += "-w ";
	}//if
	if (bTrackpoints){
		sCmd += "-t ";
	}//if
	if (bRoutes){
		sCmd += "-r ";
	}//if
	
	sCmd += sFile;

	system(sCmd.c_str());

	if (bAdd){
		if (bWaypoints){
			pShapes	= API_Create_Shapes(API_Make_File_Path(NULL, sFile, "_wpt.shp"));
			DataObject_Add(pShapes, false);
		}//if
		if (bTrackpoints){
			pShapes	= API_Create_Shapes(API_Make_File_Path(NULL, sFile, "_trk.shp"));
			DataObject_Add(pShapes, false);
		}//if
		if (bRoutes){
			pShapes	= API_Create_Shapes(API_Make_File_Path(NULL, sFile, "_rte.shp"));
			DataObject_Add(pShapes, false);
		}//if					
	}//if
	
	return true;

}//method