/*******************************************************************************
    AHP.cpp
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

#include "Grid_AHP.h"

CAHP::CAHP(void){

	Parameters.Set_Name(_TL("Analytical Hierarchy Process"));
	Parameters.Set_Description(_TL(
		"(c) 2004 by Victor Olaya. Analytical Hierarchy Process"));

	Parameters.Add_Grid_List(NULL,
						"GRIDS", 
						_TL("Input Grids"),
						"Input Grids",
						PARAMETER_INPUT);

	Parameters.Add_Table(NULL,
						"TABLE", 
						_TL("Pairwise Comparisons Table "),
						"",	
						PARAMETER_INPUT);

	Parameters.Add_Grid(NULL,
						"OUTPUT",
						_TL("Output Grid"),
						"",
						PARAMETER_OUTPUT);

}//constructor


CAHP::~CAHP(void){}

bool CAHP::On_Execute(void){

	int i,j;	
	int x,y;	
	float *pCoefs;	
	float fValue;
	float **pMatrix;
	float fSum;
	CGrid *pOutputGrid;
	CGrid **pGrids;
	CTable_Record *pRecord;
	CTable *pTable;
	CParameter_Grid_List* pGridsList;
	CAPI_String sMessage;

	pTable = Parameters("TABLE")->asTable();
	pOutputGrid = Parameters("OUTPUT")->asGrid();

	if( (pGridsList = (CParameter_Grid_List *)Parameters("GRIDS")->Get_Data()) != 
			NULL && pGridsList->Get_Count() > 0 ){
		if (pTable->Get_Field_Count() != pGridsList->Get_Count() ||
				pTable->Get_Record_Count() < pGridsList->Get_Count()){
			Message_Add(_TL("Error : Wrong table. Check table dimensions"));
			return false;
		}//if
		pMatrix = new float*[pGridsList->Get_Count()];
		for (i = 0; i<pGridsList->Get_Count(); i++){
			pMatrix[i] = new float[pGridsList->Get_Count()];
			pRecord = pTable->Get_Record(i);
			for (j = 0; j<pGridsList->Get_Count(); j++){
				pMatrix[i][j] = pRecord->asFloat(j);
			}//for
		}//for

		for (i = 0; i<pGridsList->Get_Count(); i++){
			fSum = 0;
			for (j = 0; j<pGridsList->Get_Count(); j++){
				fSum += pMatrix[j][i];
			}//for
			for (j = 0; j<pGridsList->Get_Count(); j++){
				pMatrix[j][i] /= fSum;
			}//for
		}//for

		pCoefs = new float[pGridsList->Get_Count()];
		for (i = 0; i<pGridsList->Get_Count(); i++){
			fSum = 0;
			for (j = 0; j<pGridsList->Get_Count(); j++){
				fSum += pMatrix[i][j];
			}//for
			pCoefs[i] = fSum / (float) pGridsList->Get_Count();
			sMessage = _TL("Weight for grid ") + API_Get_String(i,0) + " = " + API_Get_String(pCoefs[i]);
			Message_Add(sMessage.c_str());
		}//for

		pGrids = new CGrid* [pGridsList->Get_Count()];
		for (i = 0; i<pGridsList->Get_Count(); i++){
			pGrids[i] = pGridsList->asGrid(i); 
		}//for
		for(y=0; y<Get_NY() && Set_Progress(y); y++){
			for(x=0; x<Get_NX(); x++){
				fValue = 0;
				for (i = 0; i<pGridsList->Get_Count(); i++){
					fValue = pCoefs[i] * pGrids[i]->asFloat(x,y);
				}//for
				pOutputGrid->Set_Value(x,y,fValue);
			}//for
		}//for
	}//if

	for (i = 0; i<pGridsList->Get_Count(); i++){
		delete [] pMatrix[i];
	}//for
	delete []pMatrix;
	delete[] pCoefs;

	return true;

}//method