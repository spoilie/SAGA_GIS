/*******************************************************************************
    IsochronesVar.h
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

class CIsochronesVar : public CModule_Grid_Interactive {

private:
	CGrid *m_pDEM;
	CGrid *m_pTime;
	CGrid *m_pSpeed;
	CGrid *m_pManning;
	CGrid *m_pCN;
	CGrid *m_pCatchArea;
	CGrid *m_pSlope;
	double m_dManning;
	double m_dCN;
	double m_dRainfall;
	double m_dMixedThresh;
	double m_dChannelThresh;
	double m_dChannelSlope;
	double m_dMinSpeed;
	void writeTimeOut(int,int,int,int);
	void ZeroToNoData(void);
	double Runoff(double, double);


public:
	CIsochronesVar(void);
	virtual ~CIsochronesVar(void);


protected:
	virtual bool On_Execute(void);
	virtual bool On_Execute_Finish();
	virtual bool On_Execute_Position(CGEO_Point ptWorld, TModule_Interactive_Mode Mode);


};

