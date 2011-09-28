
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                  Geostatistics_Grid                   //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//              GSGrid_Zonal_Statistics.cpp              //
//                                                       //
//                 Copyright (C) 2005 by                 //
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
// 59 Temple Place - Suite 330, Boston, MA 02111-1307,   //
// USA.                                                  //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//    e-mail:     volker.wichmann@ku-eichstaett.de       //
//                                                       //
//    contact:    Volker Wichmann                        //
//                Research Associate                     //
//                Chair of Physical Geography		     //
//				  KU Eichstaett-Ingolstadt				 //
//                Ostenstr. 18                           //
//                85072 Eichstaett                       //
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
#include "GSGrid_Zonal_Statistics.h"


///////////////////////////////////////////////////////////
//														 //
//				Construction/Destruction				 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CGSGrid_Zonal_Statistics::CGSGrid_Zonal_Statistics(void)
{
	//-----------------------------------------------------
	// Place information about your module here...

	Set_Name(_TL("Zonal Grid Statistics"));

	Set_Author(_TL("Copyrights (c) 2005 by Volker Wichmann"));

	Set_Description(_TL(
		"The module can be used to create a contingency table of unique condition units (UCUs). These "
		"units are delineated from a zonal grid (e.g. sub catchments) and optional categorial grids (e.g. "
		"landcover, soil, ...). It is possible to calculate simple statistics (min, max, mean, standard "
		"deviation and sum) for each UCU from optional grids with continious data (e.g. slope). The number "
		"of input grids is only limited by available memory. The module has four different modes of "
		"application: (1) only a zonal grid is used as input. This results in a simple contingency table with "
		"the number of grid cells in each zone. (2) a zonal grid and additional categorial grids are used as "
		"input. This results in a contingency table with the number of cells in each UCU. (3) a zonal grid "
		"and additional grids with continuous data are used as input. This results in a contingency table "
		"with the number of cells in each zone and some simple statistics for each zone. The statistics are "
		"calculated for each continuous grid. (4) a zonal grid, additional categorial grids and additional "
		"grids with continuous data are used as input. This results in a contingency table with the number "
		"of cells in each UCU and the corresponding statistics for each continuous grid.\n"
		"\n"
		"Depending on the mode of application, the output table contains information about the category "
		"combination of each UCU, the number of cells in each UCU and the statistics for each UCU. A "
		"typical output table may look like this:\n"
		"<table border=\"1\">"
		"<tr><td>ID Zone</td><td>ID 1stCat</td><td>ID 2ndCat</td><td>Count UCU</td><td>MIN 1stCont</td><td>MAX 1stCont</td><td>MEAN 1stCont</td><td>STDDEV 1stCont</td><td>SUM 1stCont</td></tr>"
		"<tr><td>0      </td><td>2        </td><td>6        </td><td>6        </td><td>708.5      </td><td>862.0      </td><td>734.5       </td><td>62.5          </td><td>4406.8     </td></tr>"
		"<tr><td>0      </td><td>3        </td><td>4        </td><td>106      </td><td>829.1      </td><td>910.1      </td><td>848.8       </td><td>28.5          </td><td>89969.0    </td></tr>"
		"</table>")
	);


	Parameters.Add_Grid(
		NULL, "ZONES"		, _TL("Zone Grid"),
		_TL("Grid defining the zones to analyse. This grid also acts as a mask. Coding: no-data / categorial values."),
		PARAMETER_INPUT
	);

	Parameters.Add_Grid_List(
		NULL, "CATLIST"		, _TL("Categorial Grids"),
		_TL("Grids used to delineate the UCUs. Coding: no-data / categorial values."),
		PARAMETER_INPUT_OPTIONAL
	);

	Parameters.Add_Grid_List(
		NULL, "STATLIST"	, _TL("Grids to analyse"),
		_TL("Grids with continuous data, statistics are calculated for each grid. Coding: no-data / continuous values."),
		PARAMETER_INPUT_OPTIONAL
	);

	Parameters.Add_Table(
		NULL, "OUTTAB"		, _TL("Result Table"),
		_TL("Summary table."),
		PARAMETER_OUTPUT
	);
}

//---------------------------------------------------------
CGSGrid_Zonal_Statistics::~CGSGrid_Zonal_Statistics(void)
{}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CGSGrid_Zonal_Statistics::On_Execute(void)
{
	int						x, y, nCatGrids, nStatGrids, iGrid, zoneID, catID, NDcount, catLevel, NDcountStat;
	double					statID;
	const char				*Gridname;

	CGrid					*pZones, *pGrid;
	CParameter_Grid_List	*pCatList;
	CParameter_Grid_List	*pStatList;

	CList_Conti				*newZone, *startList, *runList, *newSub, *parent, *runSub, *subList;
	CList_Stat				*runStats;
	CTable					*pOutTab;
	CTable_Record			*pRecord;


	pZones		= Parameters("ZONES")		->asGrid();
	pCatList	= Parameters("CATLIST")		->asGridList();
	pStatList	= Parameters("STATLIST")	->asGridList();
	pOutTab		= Parameters("OUTTAB")		->asTable();

	nCatGrids	= pCatList	->Get_Count();
	nStatGrids	= pStatList	->Get_Count();
	
	NDcount		= 0;						// NoData Counter (ZoneGrid)
	NDcountStat	= 0;						// NoData Counter (StatGrids)

	
	newZone		= new CList_Conti();								// create first list entry (dummy)
	startList	= newZone;

	for(y=0; y<Get_NY() && Set_Progress(y); y++)
	{
		for(x=0; x<Get_NX(); x++)
		{	
			if( !pZones->is_NoData(x, y) )
			{
				runList		= startList;
				zoneID		= pZones->asInt(x, y);								// get zone ID

				while( runList->next != NULL && runList->cat < zoneID )		// search for last entry in list or insert point
				{
					runList = runList->next;
				}

				if( runList->cat == zoneID )
					runList = runList;											// zoneID found				
				else if( runList->cat == 0 ) 
					runList->cat = zoneID;										// first list entry, just write
				else if( runList->next == NULL )								// append zoneID
				{
					newZone = new CList_Conti();
					newZone->previous	= runList;
					runList->next		= newZone;

					newZone->cat	= zoneID;									//		... and write info
					runList			= newZone;
				}
				else															// insert new entry
				{
					newZone = new CList_Conti();

					newZone->next		= runList;
					if( runList->previous != NULL )
					{
						newZone->previous = runList->previous;
						runList->previous->next = newZone;
					}
					runList->previous	= newZone;
					
					if( runList == startList )
						startList = newZone;									// if new entry is first element, update startList pointer

					newZone->cat	= zoneID;									//		... and write info
					runList			= newZone;
				}


				for(iGrid=0; iGrid<nCatGrids; iGrid++)								// collect categories
				{
					parent  = runList;
					if( runList->sub == NULL )										// no sub class found
					{
						newSub = new CList_Conti();
						runList->sub = newSub;
					}

					runList = runList->sub;

					pGrid	= pCatList->asGrid(iGrid);
					if( !pGrid->is_NoData(x, y) )
						catID	= pGrid->asInt(x, y);
					else
						catID	= (int)pGrid->Get_NoData_Value();


					while( runList->next != NULL && runList->cat < catID )		// search for last entry in list or insert point
					{
						runList = runList->next;
					}

					if( runList->cat == catID )
						runList = runList;											// zoneID found, all infos already written
					else if( runList->cat == 0 ) 
					{
						runList->cat = catID;										// first list entry, just write
						runList->parent = parent;
					}
					else if( runList->next == NULL && runList->cat < catID)								// append zoneID
					{
						newSub = new CList_Conti();
						newSub->cat		= catID;									//		... and write info
						newSub->previous	= runList;
						newSub->parent		= parent;
						runList->next		= newSub;

						runList			= newSub;
					}
					else															// insert new entry
					{
						newSub = new CList_Conti();
						newSub->cat		= catID;									//		... and write info
						newSub->next		= runList;
						newSub->parent		= parent;
						if( runList->previous != NULL )
						{
							newSub->previous = runList->previous;
							runList->previous->next = newSub;
						}
						else
							parent->sub		= newSub;
							
						runList->previous	= newSub;

						runList			= newSub;
					}
				}


				for(iGrid=0; iGrid<nStatGrids; iGrid++)									// collect statistics for StatGrids
				{
					if( iGrid == 0 )
					{
						if( runList->stats == NULL )
							runList->stats = new CList_Stat();
						
						runStats	= runList->stats;
					}
					else
					{
						if( runStats->next == NULL )
							runStats->next = new CList_Stat();

						runStats = runStats->next;
					}
					if( !pStatList->asGrid(iGrid)->is_NoData(x, y) )
					{
						statID		= pStatList->asGrid(iGrid)->asDouble(x, y);
						runStats->sum += statID;
						if( runStats->min == 0.0 || statID < runStats->min )
							runStats->min = statID;
						if( statID > runStats->max )
							runStats->max = statID;
						runStats->dev += pow(statID, 2);
					}
					else
						NDcountStat += 1;
				}
				

				runList->count += 1;												// sum up unique condition area
			}
			else
				NDcount += 1;														// sum up NoData values of zone grid
		}
	}


	Gridname = pZones->Get_Name();													// Create fields in output table (1st = Zone, 2nd = Catgrid1, 3rd = Catgrid 2, ...)
	pOutTab->Add_Field(Gridname, TABLE_FIELDTYPE_Int);
	for(iGrid=0; iGrid<nCatGrids; iGrid++)
	{
		Gridname = pCatList->asGrid(iGrid)->Get_Name();
		pOutTab->Add_Field(Gridname, TABLE_FIELDTYPE_Int);
	}
	pOutTab->Add_Field(_TL("Count"), TABLE_FIELDTYPE_Int);
	for(iGrid=0; iGrid<nStatGrids; iGrid++)
	{
		Gridname = pStatList->asGrid(iGrid)->Get_Name();

		pOutTab->Add_Field(CAPI_String::Format(_TL("%s_MIN")   , Gridname), TABLE_FIELDTYPE_Double);
		pOutTab->Add_Field(CAPI_String::Format(_TL("%s_MAX")   , Gridname), TABLE_FIELDTYPE_Double);
		pOutTab->Add_Field(CAPI_String::Format(_TL("%s_MEAN")  , Gridname), TABLE_FIELDTYPE_Double);
		pOutTab->Add_Field(CAPI_String::Format(_TL("%s_STDDEV"), Gridname), TABLE_FIELDTYPE_Double);
		pOutTab->Add_Field(CAPI_String::Format(_TL("%s_SUM")   , Gridname), TABLE_FIELDTYPE_Double);
	}




	while( startList != NULL )														// scan zone layer list and write cat values in table
	{
		runList = startList;
		while( runList->sub != NULL )												// fall down to lowest layer
			runList = runList->sub;
		
		subList = runList;															// use pointer to scan horizontal

		while( subList != NULL )													// move forward and read all categories of this layer (including the parent layers)
		{
			runSub = subList;
			catLevel = nCatGrids;
			pRecord	= pOutTab->Add_Record();										// create new record in table
			pRecord->Set_Value((catLevel+1), runSub->count);						// read/write field count			

			for(iGrid=0; iGrid<nStatGrids; iGrid++)									// read/write statistics
			{
				if( iGrid == 0 )
					runStats = runSub->stats;
				else
					runStats = runStats->next;

				pRecord->Set_Value(catLevel+2+iGrid*5, runStats->min);
				pRecord->Set_Value(catLevel+3+iGrid*5, runStats->max);
				pRecord->Set_Value(catLevel+4+iGrid*5, runStats->sum/runSub->count);
				pRecord->Set_Value(catLevel+5+iGrid*5, sqrt((runStats->dev - runSub->count*pow(runStats->sum/runSub->count, 2)) / (runSub->count - 1))); // sample
				//pRecord->Set_Value(catLevel+5+iGrid*5, sqrt((runStats->dev - pow(runStats->sum/runSub->count, 2)) / runSub->count)); // population
				pRecord->Set_Value(catLevel+6+iGrid*5, runStats->sum);
			}			
			
			while( runSub != NULL )													// read/write categories
			{
				pRecord->Set_Value(catLevel, runSub->cat);
				runSub = runSub->parent;
				catLevel -= 1;
			}
			subList = subList->next;
		}

		while( runList->parent != NULL && runList->parent->next == NULL )			// move up to next 'Caterory with -> next'
			runList = runList->parent;

		if( runList->parent != NULL )												// if not upper layer (zones)
		{	
			runList = runList->parent;												// move to parent of next 'Caterory with -> next'
			if( runList->next != NULL && runList->parent != NULL )
				runList->parent->sub = runList->next;								// redirect pointer to category which is next 'Categora with -> next' next
			else if (runList->parent == NULL && runList->next != NULL )				
				startList = runList->next;											// when upper layer (zones) is reached, move to next zone
			else
				startList = NULL;													// reading finished
			
			if( runList->parent == NULL )
				startList = runList->next;											// ?? when upper layer is reached, move to next zone
			else
				runList->sub = runList->sub->next;									// on sub layers redirect pointer to ->next
		}
		else
		{
			if( nCatGrids == 0 )
				startList = NULL;
			else
				startList = runList->next;												// ?? upper layer is reached, move to next zone
		}


		runList->next = NULL;					
		delete (runList);															// delete disconneted part of the list

	}

	pRecord	= pOutTab->Add_Record();												// write NoData record
	pRecord->Set_Value(0, pZones->Get_NoData_Value());
	pRecord->Set_Value((nCatGrids+1), NDcount);

	if( NDcountStat > 0 )
	{
		Message_Add(CAPI_String::Format(_TL("\n\n\nWARNING: Encountered %d no-data value(s) in statistic grid(s)!\n\n\n"), NDcountStat));
	}

	return (true);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------