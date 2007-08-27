/*******************************************************************************
    Simulate.h
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

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MLB_Interface.h"
#include "firelib.h"

class CSimulateVariableWind : public CSG_Module_Grid {

private:

	CSG_Grid *m_pDEM;    
    CSG_Grid *m_pIgnGrid;           /* ptr to ignition time map (minutes) */
    CSG_Grid **m_pWindSpdGrids;     /* ptr to wind speed map (km/h) */
    CSG_Grid **m_pWindDirGrids;     /* ptr to wind direction map (deg from north) */
    CSG_Grid *m_pM1Grid;            /* ptr to 1-hr dead fuel moisture map (in %)*/
    CSG_Grid *m_pM10Grid;           /* ptr to 10-hr dead fuel moisture map */
    CSG_Grid *m_pM100Grid;          /* ptr to 100-hr dead fuel moisture map */
    CSG_Grid *m_pMHerbGrid;         /* ptr to live herbaceous fuel moisture map */
    CSG_Grid *m_pMWoodGrid;         /* ptr to live stem fuel moisture map */
	CSG_Grid *m_pFuelGrid;          /* ptr to fuel model map */
	CSG_Grid *m_pValueGrid;

	CSG_Grid *m_pFlameGrid;         /* ptr to flame length map (m) */
	CSG_Grid *m_pIntensityGrid;     
	CSG_Grid *m_pReactionIntensityGrid;
	CSG_Grid *m_pEffectiveWindGrid;
	CSG_Grid *m_pHeatPerUnitAreaGrid;

    CSG_Grid *m_pSlopeGrid;         /* ptr to slope map (rise/reach) */
    CSG_Grid *m_pAspectGrid;        /* ptr to aspect map (degrees from north) */

	CSG_Grid *m_pTimeGrid;

	int m_iWindDirGrids;
	int m_iWindSpdGrids;

	float m_fIgnTime;
	float m_fInterval;

	bool m_bDeleteWindSpdGrid;
	bool m_bDeleteWindDirGrid;

	FuelCatalogPtr m_Catalog;    /* fuel catalog handle */

	int m_iLength;

	CSG_Points_Int	m_CentralPoints;
	CSG_Points_Int	m_AdjPoints;

	int *m_pLength; 
	int m_fTimeLimit; //in min

	float m_fWorldX, m_fWorldY;
	int m_iGridX, m_iGridY;

	float m_fDeadFuelMoisture;
	float *m_pMeanWindSpd;
	float *m_pMeanWindDir;
	float m_fSlope;
	float m_fAspect;
	float m_fMeanSpeed;
	float m_fHeatPerUnitArea;
	float m_fReactionIntensity;
	float m_fEffectiveWind;
	float m_fIntensity;
	float m_fFlameHeight;
	float m_fMaxSpreadDir;
	float m_fArea;
	float *m_pAreaByFuelModel;
	float m_fPerimeter;
	float m_fFrontDistance;
	float m_fRearDistance;

	bool AssignParameters();
	void CalculateFire();
	int CalculateFireSpreading(float);
	void DeleteObjects();

	float getWindSpeed(int,int,float);
	float getWindDirection(int,int,float);
	float getAzimuth(float,float);

	void CreateReport();
	void CalculateReportParameters();

public:

	CSimulateVariableWind(void);
	virtual ~CSimulateVariableWind(void);

protected:

	virtual bool On_Execute(void);

};
