/*******************************************************************************
    IsochronesConst.h
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

class CFlowDepth : public CModule_Grid_Interactive {

private:
	CGrid *m_pDEM;
	CGrid *m_pFlowDepth;
	CGrid *m_pCatchArea;	
	CGrid *m_pSlope;
	CGrid *m_pAspect;
	CGrid *m_pBasinGrid;
	double m_fMaxFlowAcc;
	double m_dThreshold;
	double m_dFlow;

public:
	CFlowDepth(void);
	virtual ~CFlowDepth(void);	
	bool isHeader(int,int);
	double CalculateFlowDepth(int,int);
	//void MarkBuffer(int,int,double);
	bool getWetAreaAndPerimeter(int,int,double,double&,double&);

protected:
	virtual bool On_Execute(void);
	virtual bool On_Execute_Finish();
	virtual bool On_Execute_Position(CGEO_Point ptWorld, TModule_Interactive_Mode Mode);

	

};