/*******************************************************************************
    ProtectionIndex.cpp
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

#include "ProtectionIndex.h"

#define NO_DATA -1

CProtectionIndex::CProtectionIndex(void){

	Parameters.Set_Name(_TL("Morphometric Protection Index"));
	Parameters.Set_Description(_TW(
		"(c) 2005 by Victor Olaya."));

	Parameters.Add_Grid(NULL, 
						"DEM",
						_TL("Elevation"), 						
						_TL(""), 
						PARAMETER_INPUT);

	Parameters.Add_Grid(NULL, 
						"PROTECTION", 
						_TL("Protection Index"), 
						_TL(""), 
						PARAMETER_OUTPUT, 
						true, 
						GRID_TYPE_Float);

	Parameters.Add_Value(NULL, 
						"RADIUS", 
						_TL("Radius"), 
						_TL(""), 
						PARAMETER_TYPE_Double, 2000, 
						0.0, 
						true);

}//constructor

CProtectionIndex::~CProtectionIndex(void)
{}

bool CProtectionIndex::On_Execute(void){
	
	int x,y;
	double dProtectionIndex;
	CSG_Grid* pProtectionIndex = Parameters("PROTECTION")->asGrid();
	
	m_dRadius = Parameters("RADIUS")->asDouble();
	m_pDEM = Parameters("DEM")->asGrid(); 

    for(y=0; y<Get_NY() && Set_Progress(y); y++){		
		for(x=0; x<Get_NX(); x++){
			dProtectionIndex = getProtectionIndex(x,y);
			if (dProtectionIndex == NO_DATA){
				pProtectionIndex->Set_NoData(x,y);
			}//if
			else{
				pProtectionIndex->Set_Value(x,y, dProtectionIndex);
			}//else
		}//for
	}//for	
	
	return true;

}//method

double CProtectionIndex::getProtectionIndex(int x, int y){

	int i,j;
	int iDifX[] = {0,1,1,1,0,-1,-1,-1};
	int iDifY[] = {1,1,0,-1,-1,-1,0,1};
	double dDifHeight;
	double dDist;
	double dAngle;
	double dProtectionIndex = 0;
	double *pAngle = new double[8];

	for (i = 0; i < 8; i++){
		j = 1;
		pAngle[i] = 0;
		dDist = M_GET_LENGTH(iDifX[i], iDifY[i]) * j * m_pDEM->Get_Cellsize();
		while (dDist < m_dRadius){
			if (m_pDEM->is_InGrid(x + iDifX[i] * j, y + iDifY[i] * j)){
				dDifHeight = m_pDEM->asDouble(x,y);
			}//if
			else{
				return NO_DATA;
			}
			dDifHeight = m_pDEM->asDouble(x + iDifX[i] * j, y + iDifY[i] * j) 
						 - m_pDEM->asDouble(x,y);
			dAngle = atan (dDifHeight / dDist);
			if (dAngle > pAngle[i]){
				pAngle[i] = dAngle;
			}//if
			j++;
			dDist = M_GET_LENGTH(iDifX[i], iDifY[i]) * j * m_pDEM->Get_Cellsize();
		}//while
		dProtectionIndex+=pAngle[i];
	}//while

	delete [] pAngle;
	return (dProtectionIndex / 8.);

}//method