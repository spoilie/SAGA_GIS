
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                Command Line Interface                 //
//                                                       //
//                   Program: SAGA_CMD                   //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                   module_library.h                    //
//                                                       //
//          Copyright (C) 2005 by Olaf Conrad            //
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
///////////////////////////////////////////////////////////

//---------------------------------------------------------


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#ifndef _HEADER_INCLUDED__SAGA_CMD__Module_Library_H
#define _HEADER_INCLUDED__SAGA_CMD__Module_Library_H


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include <wx/dynlib.h>


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CModule_Library  
{
public:
	CModule_Library(void);
	virtual ~CModule_Library(void);

	bool						Create					(const char *FileName, const char *FilePath);
	void						Destroy					(void);

	int							Get_Count				(void)		{	return( m_nModules );	}
	class CModule *				Get_Module				(int i)		{	return( i >= 0 && i < m_nModules ? m_Modules[i] : NULL );	}

	bool						is_Valid				(void)		{	return( m_nModules > 0 );	}

	wxString					Get_FileName			(void)		{	return( m_FileName );	}

	class CModule *				Select					(const char *ModuleName);
	class CModule *				Get_Selected			(void)		{	return( m_pSelected );	}

	bool						Execute					(int argc, char *argv[]);

	bool						Get_Parameters			(class CParameters *pParameters);
	bool						Add_DataObject			(class CDataObject *pObject);


private:

	int							m_nModules;

	class CModule				**m_Modules, *m_pSelected;

	wxString					m_FileName;

	wxDynamicLibrary			m_Library;

	class wxCmdLineParser		*m_pCMD;


	void						_Set_CMD				(class wxCmdLineParser *pCMD, class CParameters *pParameters);

	bool						_Get_CMD				(class wxCmdLineParser *pCMD, class CParameters *pParameters, wxString cmdLine);
	bool						_Get_CMD				(class wxCmdLineParser *pCMD, class CParameters *pParameters, int argc, char *argv[]);
	bool						_Get_CMD				(class wxCmdLineParser *pCMD, class CParameters *pParameters);

	bool						_Create_DataObjects		(class wxCmdLineParser *pCMD, class CParameters *pParameters);
	bool						_Create_DataObject_List	(class CParameter *pParameter, wxString sList);

	bool						_Destroy_DataObjects	(bool bSave);
	bool						_Destroy_DataObjects	(bool bSave, class wxCmdLineParser *pCMD, class CParameters *pParameters);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef _HEADER_INCLUDED__SAGA_CMD__Module_Library_H