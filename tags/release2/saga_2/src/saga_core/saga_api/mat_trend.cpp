
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//           Application Programming Interface           //
//                                                       //
//                  Library: SAGA_API                    //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                     mat_trend.cpp                     //
//                                                       //
//                 Copyright (C) 2006 by                 //
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
//    contact:    Olaf Conrad                            //
//                Institute of Geography                 //
//                University of Goettingen               //
//                Goldschmidtstr. 5                      //
//                37077 Goettingen                       //
//                Germany                                //
//                                                       //
//    e-mail:     oconrad@saga-gis.org                   //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//  Based on the LMFit.h/cpp, Fit.h/cpp source codes of  //
//    A. Ringeler (see 'table_calculus' sub project).    //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "mat_tools.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define SWAP(a, b)	{	temp = (a); (a) = (b); (b) = temp;	}

//---------------------------------------------------------
#define EPSILON		0.001


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CMAT_Trend::CFncParams::CFncParams(void)
{
	m_Count		= 0;
}

//---------------------------------------------------------
CMAT_Trend::CFncParams::~CFncParams(void)
{
	Destroy();
}

//---------------------------------------------------------
bool CMAT_Trend::CFncParams::Create(const char *Variables, int nVariables)
{
	int		i;

	if( m_Count != nVariables )
	{
		Destroy();

		m_Count		= nVariables;

		m_Variables	= (char    *)API_Calloc(m_Count, sizeof(char));
		m_A			= (double  *)API_Calloc(m_Count, sizeof(double));
		m_Atry		= (double  *)API_Calloc(m_Count, sizeof(double));
		m_Beta		= (double  *)API_Calloc(m_Count, sizeof(double));
		m_dA		= (double  *)API_Calloc(m_Count, sizeof(double));
		m_dA2		= (double  *)API_Calloc(m_Count, sizeof(double));
		m_Alpha		= (double **)API_Calloc(m_Count, sizeof(double *));
		m_Covar		= (double **)API_Calloc(m_Count, sizeof(double *));

		for(i=0; i<m_Count; i++)
		{
			m_Alpha[i]		= (double *)API_Calloc(m_Count, sizeof(double));
			m_Covar[i]		= (double *)API_Calloc(m_Count, sizeof(double));
		}
	}

	for(i=0; i<m_Count; i++)
	{
		m_Variables[i]	= Variables[i];
		m_A[i]			= 1.0;
	}

	return( true );
}

//---------------------------------------------------------
bool CMAT_Trend::CFncParams::Destroy(void)
{
	if( m_Count > 0 )
	{
		for(int i=0; i<m_Count; i++)
		{
			API_Free(m_Alpha[i]);
			API_Free(m_Covar[i]);
		}

		API_Free(m_Variables);
		API_Free(m_A);
		API_Free(m_Atry);
		API_Free(m_Beta);
		API_Free(m_dA);
		API_Free(m_dA2);
		API_Free(m_Alpha);
		API_Free(m_Covar);

		m_Count	= 0;
	}

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CMAT_Trend::CMAT_Trend(void)
{
	m_Lambda_Max	= 10000;
	m_Iter_Max		= 1000;

//	Set_Formula("a * x + b");
}

//---------------------------------------------------------
CMAT_Trend::~CMAT_Trend(void)
{
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CMAT_Trend::Set_Formula(const char *Formula)
{
	m_bOkay	= false;

	if( m_Formula.Set_Formula(Formula) )
	{
		CAPI_String	vars, uvars(m_Formula.Get_Used_Var());

		for(unsigned int i=0; i<uvars.Length(); i++)
		{
			if( uvars.c_str()[i] >= 'a' && uvars.c_str()[i] <= 'z' && uvars.c_str()[i] != 'x' )
			{
				vars.Append(uvars.c_str()[i]);
			}
		}

		return( m_Params.Create(vars.c_str(), vars.Length()) );
	}

	m_Params.Destroy();

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CMAT_Trend::Clr_Data(void)
{
	m_Data.Clear();

	m_bOkay	= false;
}

//---------------------------------------------------------
void CMAT_Trend::Set_Data(double *xData, double *yData, int nData, bool bAdd)
{
	if( !bAdd )
	{
		m_Data.Clear();
	}

	for(int i=0; i<nData; i++)
	{
		m_Data.Add(xData[i], yData[i]);
	}

	m_bOkay	= false;
}

//---------------------------------------------------------
void CMAT_Trend::Set_Data(const CAPI_dPoints &Data, bool bAdd)
{
	if( !bAdd )
	{
		m_Data.Clear();
	}

	for(int i=0; i<Data.Get_Count(); i++)
	{
		m_Data.Add(Data.Get_X(i), Data.Get_Y(i));
	}

	m_bOkay	= false;
}

//---------------------------------------------------------
void CMAT_Trend::Add_Data(double x, double y)
{
	m_Data.Add(x, y);

	m_bOkay	= false;
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CMAT_Trend::Set_Max_Iterations(int Iterations)
{
	if( Iterations > 0 )
	{
		m_Iter_Max		= Iterations;

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
bool CMAT_Trend::Set_Max_Lambda(double Lambda)
{
	if( Lambda > 0.0 )
	{
		m_Lambda_Max	= Lambda;

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
bool CMAT_Trend::Get_Trend(double *xData, double *yData, int nData, const char *Formula)
{
	Set_Data(xData, yData, false);

	if( Formula )
	{
		Set_Formula(Formula);
	}

	return( Get_Trend() );
}

//---------------------------------------------------------
bool CMAT_Trend::Get_Trend(const CAPI_dPoints &Data, const char *Formula)
{
	Set_Data(Data, false);

	if( Formula )
	{
		Set_Formula(Formula);
	}

	return( Get_Trend() );
}

//---------------------------------------------------------
bool CMAT_Trend::Get_Trend(void)
{
	if( !m_Formula.Get_Error(NULL, NULL) )
	{
		int		i;

		m_bOkay	= true;

		//-------------------------------------------------
		if( m_Data.Get_Count() > 1 )
		{
			if( m_Params.m_Count > 0 )
			{
				m_Lambda	= 0.001;

				_Get_mrqcof(m_Params.m_A, m_Params.m_Alpha, m_Params.m_Beta);

				m_ChiSqr_o	= m_ChiSqr;

				for(i=0; i<m_Params.m_Count; i++)
				{
					m_Params.m_Atry[i]	= m_Params.m_A[i];
				}

				//-----------------------------------------
				for(i=0; i<m_Iter_Max && m_Lambda<m_Lambda_Max && m_bOkay && API_Callback_Process_Get_Okay(false); i++)
				{
					m_bOkay	= _Fit_Function();
				}

				//-----------------------------------------
				for(i=0; i<m_Params.m_Count; i++)
				{
					m_Formula.Set_Variable(m_Params.m_Variables[i], m_Params.m_A[i]);
				}
			}

			//---------------------------------------------
			double	y_m, y_o, y_t;

			for(i=0, y_m=0.0; i<m_Data.Get_Count(); i++)
			{
				y_m	+= m_Data.Get_Y(i);
			}

			y_m	/= m_Data.Get_Count();

			for(i=0, y_o=0.0, y_t=0.0; i<m_Data.Get_Count(); i++)
			{
				y_o	+= MAT_Square(y_m - m_Data.Get_Y(i));
				y_t	+= MAT_Square(y_m - m_Formula.Val(m_Data.Get_X(i)));
			}

			m_ChiSqr_o	= y_o > 0.0 ? y_t / y_o : 1.0;
		}
	}

	return( m_bOkay );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CAPI_String CMAT_Trend::Get_Error(void)
{
	int			Position;
	const char	*Message;
	CAPI_String	s;

	if( m_bOkay )
	{
	}
	else if( m_Formula.Get_Error(&Position, &Message) )
	{
		s.Printf("%s [%s] %s: %d. \"%s\"", LNG("Error in formula"), m_Formula.Get_Formula().c_str(), LNG("Position"), Position + 1, Message);
	}
	else
	{
		s.Printf("%s", LNG("Error in Trend Calculation"));
	}

	return( s );
}

//---------------------------------------------------------
CAPI_String CMAT_Trend::Get_Formula(void)
{
	CAPI_String	s;

	s.Printf(CAPI_String::Format("%s\n", m_Formula.Get_Formula().c_str()));

	if( m_bOkay )
	{
		for(int i=0; i<m_Params.m_Count; i++)
		{
			s.Append(CAPI_String::Format("%c = %g\n", m_Params.m_Variables[i], m_Params.m_A[i]));
		}
	}

	return( s );
}

//---------------------------------------------------------
double CMAT_Trend::Get_ChiSquare(void)
{
	if( m_bOkay )
	{
//		return( sqrt(m_ChiSqr / m_Data.Get_Count()) );	// RMS of Residuals (stdfit)
		return( m_ChiSqr );
	}

	return( 0.0 );
}

//---------------------------------------------------------
double CMAT_Trend::Get_R2(void)
{
	if( m_bOkay )
	{
		return( m_ChiSqr_o );
	}

	return( 0.0 );
}

//---------------------------------------------------------
double CMAT_Trend::Get_Value(double x)
{
	if( m_bOkay )
	{
		return( m_Formula.Val(x) );
	}

	return( 0.0 );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CMAT_Trend::_Fit_Function(void)
{
	int		i, j;

	//-----------------------------------------------------
	for(i=0; i<m_Params.m_Count; i++)
	{
		for(j=0; j<m_Params.m_Count; j++)
		{
			m_Params.m_Covar[i][j]	= m_Params.m_Alpha[i][j];
		}

		m_Params.m_Covar[i][i]	= m_Params.m_Alpha[i][i] * (1.0 + m_Lambda);
		m_Params.m_dA2  [i]		= m_Params.m_Beta [i];
	}

	//-----------------------------------------------------
	if( _Get_Gaussj() == false )
	{
		return( false );
	}

	//-----------------------------------------------------
	for(i=0; i<m_Params.m_Count; i++)
	{
		m_Params.m_dA[i]	= m_Params.m_dA2[i];
	}

	//-----------------------------------------------------
	if( m_Lambda == 0.0 )
	{
		double	temp;

		for(i=m_Params.m_Count-1; i>0; i--)
		{
			for(j=0; j<m_Params.m_Count; j++)
			{
				SWAP(m_Params.m_Covar[j][i], m_Params.m_Covar[j][i-1]);
			}

			for(j=0; j<m_Params.m_Count; j++)
			{
				SWAP(m_Params.m_Covar[i][j], m_Params.m_Covar[i-1][j]);
			}
		}
	}
	else
	{
		for(i=0; i<m_Params.m_Count; i++)
		{
			m_Params.m_Atry[i]	= m_Params.m_A[i] + m_Params.m_dA[i];
		}

		_Get_mrqcof(m_Params.m_Atry, m_Params.m_Covar, m_Params.m_dA);

		if( m_ChiSqr < m_ChiSqr_o )
		{
			m_Lambda	*= 0.1;
			m_ChiSqr_o	 = m_ChiSqr;

			for(i=0; i<m_Params.m_Count; i++)
			{
				for(j=0; j<m_Params.m_Count; j++)
				{
					m_Params.m_Alpha[i][j]	= m_Params.m_Covar[i][j];
				}

				m_Params.m_Beta[i]	= m_Params.m_dA[i];
			}

			for(i=0; i<m_Params.m_Count; i++)	// Achtung!! in aelteren Versionen war hier ein Fehler
			{
				m_Params.m_A[i]		= m_Params.m_Atry[i];
			}
		}
		else 
		{
			m_Lambda	*= 10.0;
			m_ChiSqr	 = m_ChiSqr_o;
		}
	}

	//-----------------------------------------------------
	return( true );
}

//---------------------------------------------------------
bool CMAT_Trend::_Get_Gaussj(void)
{
	int		i, j, k, iCol, iRow, *indxc, *indxr, *ipiv;
	double	big, pivinv, temp;

	//-----------------------------------------------------
	indxc	= (int *)API_Calloc(m_Params.m_Count, sizeof(int));
	indxr	= (int *)API_Calloc(m_Params.m_Count, sizeof(int));
	ipiv	= (int *)API_Calloc(m_Params.m_Count, sizeof(int));
	
	for(i=0; i<m_Params.m_Count; i++)
	{
		ipiv[i]	= 0;
	}

	//-----------------------------------------------------
	for(i=0, iCol=-1, iRow=-1; i<m_Params.m_Count; i++)
	{
		for(j=0, big=0.0; j<m_Params.m_Count; j++)
		{
			if( ipiv[j] != 1 )
			{
				for(k=0; k<m_Params.m_Count; k++)
				{
					if( ipiv[k] == 0 )
					{
						if( fabs(m_Params.m_Covar[j][k]) >= big )
						{
							big		= fabs(m_Params.m_Covar[j][k]);
							iRow	= j;
							iCol	= k;
						}
					}
					else if( ipiv[k] > 1 )
					{
						API_Free(indxc);	API_Free(indxr);	API_Free(ipiv);	return( false );	// singular matrix...
					}
				}
			}
		}

		if( iCol < 0 || iRow < 0 )
		{
			API_Free(indxc);	API_Free(indxr);	API_Free(ipiv);	return( false );	// singular matrix...
		}

		//-------------------------------------------------
		ipiv[iCol]++;

		if( iRow != iCol )
		{
			for(j=0; j<m_Params.m_Count; j++)
			{
				SWAP(m_Params.m_Covar[iRow][j], m_Params.m_Covar[iCol][j]);
			}

			SWAP(m_Params.m_dA2[iRow], m_Params.m_dA2[iCol]);
		}

		indxr[i]	= iRow;
		indxc[i]	= iCol;

		if( fabs(m_Params.m_Covar[iCol][iCol]) < 1E-300 )
		{
			API_Free(indxc);	API_Free(indxr);	API_Free(ipiv);	return( false );	// singular matrix...
		}

		//-------------------------------------------------
		pivinv		= 1.0 / m_Params.m_Covar[iCol][iCol];
		m_Params.m_Covar[iCol][iCol]	= 1.0;

		for(j=0; j<m_Params.m_Count; j++)
		{
			m_Params.m_Covar[iCol][j]	*= pivinv;
		}

		m_Params.m_dA2[iCol]	*= pivinv;

		for(j=0; j<m_Params.m_Count; j++)
		{
			if( j != iCol )
			{
				temp	= m_Params.m_Covar[j][iCol];
				m_Params.m_Covar[j][iCol]	= 0.0;

				for(k=0; k<m_Params.m_Count; k++)
				{
					m_Params.m_Covar[j][k]	-= m_Params.m_Covar[iCol][k] * temp;
				}

				m_Params.m_dA2[j]	-= m_Params.m_dA2[iCol] * temp;
			}
		}
	}

	//-----------------------------------------------------
	for(i=m_Params.m_Count-1; i>=0; i--)
	{
        if( indxr[i] != indxc[i] )
		{
			for(j=0; j<m_Params.m_Count; j++)
			{
				SWAP(m_Params.m_Covar[j][indxr[i]], m_Params.m_Covar[j][indxc[i]]);
			}
		}
	}

	//-----------------------------------------------------
	API_Free(indxc);
	API_Free(indxr);
	API_Free(ipiv);

	return( true );
}

//---------------------------------------------------------
bool CMAT_Trend::_Get_mrqcof(double *Parameters, double **Alpha, double *Beta)
{
	int		i, j, k;
	double	y, dy, *dy_da;

	//-----------------------------------------------------
	for(i=0; i<m_Params.m_Count; i++)
	{
		for(j=0; j<=i; j++)
		{
			Alpha[i][j] = 0.0;
		}

		Beta[i]		= 0.0;
	}

	//-----------------------------------------------------
	dy_da	= (double *)API_Calloc(m_Params.m_Count, sizeof(double));

	for(k=0, m_ChiSqr=0.0; k<m_Data.Get_Count(); k++)
	{
		_Get_Function(m_Data[k].x, Parameters, y, dy_da);

		dy	= m_Data[k].y - y;

		for(i=0; i<m_Params.m_Count; i++)
		{
			for(j=0; j<=i; j++)
			{
				Alpha[i][j]	+= dy_da[i] * dy_da[j];
			}

			Beta[i]		+= dy * dy_da[i];
		}

		m_ChiSqr	+= dy * dy;
	}

	API_Free(dy_da);

	//-----------------------------------------------------
	for(i=1; i<m_Params.m_Count; i++)
	{
		for(j=0; j<i; j++)
		{
			Alpha[j][i]	= Alpha[i][j];
		}
	}

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CMAT_Trend::_Get_Function(double x, double *Parameters, double &y, double *dy_da)
{
	int		i;

	//-----------------------------------------------------
	for(i=0; i<m_Params.m_Count; i++)
	{
		m_Formula.Set_Variable(m_Params.m_Variables[i], Parameters[i]);
	}

	y	= m_Formula.Val(x);

	//-----------------------------------------------------
	for(i=0; i<m_Params.m_Count; i++)
	{
		m_Formula.Set_Variable(m_Params.m_Variables[i], Parameters[i] + EPSILON);

		dy_da[i]	 = m_Formula.Val(x);
		dy_da[i]	-= y;
		dy_da[i]	/= EPSILON;

		m_Formula.Set_Variable(m_Params.m_Variables[i], Parameters[i] - EPSILON);
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------