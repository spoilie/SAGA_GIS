/*******************************************************************************
    GPSBabel.cpp
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
#include "GPSBabel.h"

CAPI_String	sGPSBabelID[]	=
{
	"geo",
	"gpsman",
	"gpx",
	"magellan",
	"mapsend",
	"pcx",
	"mapsource",
	"gpsutil",
	"tiger",
	"csv",
	"xmap",
	"dna",
	"psp",
	"cetus",
	"gpspilot",
	"magnav",
	"garmin",
	"mxf",
	"holux",
	"ozi",
	"tpg",
	"tmpro"
};

CGPSBabel::CGPSBabel(){

	Parameters.Set_Name(_TL("GPSBabel"));

	Parameters.Set_Description(_TL(
		"An interface to the GPSBabel software"
		"(c) 2005 by Victor Olaya\r\nemail: volaya@ya.com"));

	Parameters.Add_FilePath(NULL,
							"BASEPATH",
							_TL("GPSBabel path"),
							_TL("GPSBabel path"),
							"",
							"",
							false,
							true);

	Parameters.Add_FilePath(NULL, 
							"INPUT", 
							_TL("Input file"),
							"",
							_TL("All Files|*.*|"));

	Parameters.Add_Choice(NULL, 
						"FORMATIN", 
						_TL("Input format"), 
						"", 
						"Geocaching.com .loc|"
						"GPSman|"
						"GPX XML|"
						"Magellan protocol|"
						"Magellan Mapsend|"
						"Garmin PCX5|"
						"Garmin Mapsource|"
						"gpsutil|"
						"U.S. Census Bureau Tiger Mapping Service|"
						"Comma separated values|"
						"Delorme Topo USA4/XMap Conduit|"
						"Navitrak DNA marker format|"
						"MS PocketStreets 2002 Pushpin|"
						"Cetus for Palm/OS|"
						"GPSPilot Tracker for Palm/OS|"
						"Magellan NAV Companion for PalmOS|"
						"Garmin serial protocol|"
						"MapTech Exchange Format|"
						"Holux (gm-100) .wpo Format|"
						"OziExplorer Waypoint|"
						"National Geographic Topo .tpg|"
						"TopoMapPro Places File|",
						0);

	Parameters.Add_FilePath(NULL, 
							"OUTPUT", 
							_TL("Output file"),
							"",
							_TL("All Files|*.*|"),
							"",
							true,
							false);

	Parameters.Add_Choice(NULL, 
						"FORMATOUT", 
						_TL("Output format"), 
						"", 
						"Geocaching.com .loc|"
						"GPSman|"
						"GPX XML|"
						"Magellan protocol|"
						"Magellan Mapsend|"
						"Garmin PCX5|"
						"Garmin Mapsource|"
						"gpsutil|"
						"U.S. Census Bureau Tiger Mapping Service|"
						"Comma separated values|"
						"Delorme Topo USA4/XMap Conduit|"
						"Navitrak DNA marker format|"
						"MS PocketStreets 2002 Pushpin|"
						"Cetus for Palm/OS|"
						"GPSPilot Tracker for Palm/OS|"
						"Magellan NAV Companion for PalmOS|"
						"Garmin serial protocol|"
						"MapTech Exchange Format|"
						"Holux (gm-100) .wpo Format|"
						"OziExplorer Waypoint|"
						"National Geographic Topo .tpg|"
						"TopoMapPro Places File|",
						0);

}//constructor

CGPSBabel::~CGPSBabel(){

}//destructor

bool CGPSBabel::On_Execute(void){

	CAPI_String sCmd;
	CAPI_String sInputFile = Parameters("INPUT")->asString();
	CAPI_String sOutputFile = Parameters("OUTPUT")->asString();
	CAPI_String sBasePath = Parameters("BASEPATH")->asString();
	int iInputFormat = Parameters("FORMATIN")->asInt();
	int iOutputFormat = Parameters("FORMATOUT")->asInt();

	sCmd = sBasePath + "\\" + "gpsbabel.exe " 
			+ "-i " + sGPSBabelID[iInputFormat] + " "
			+ "-f " + sInputFile + " "
			+ "-o " + sGPSBabelID[iOutputFormat] + " "
			+ "-F " + sOutputFile + " ";

	system(sCmd.c_str());

	return true;

}//method