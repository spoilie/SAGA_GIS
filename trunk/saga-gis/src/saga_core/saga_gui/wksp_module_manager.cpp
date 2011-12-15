/**********************************************************
 * Version $Id$
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    User Interface                     //
//                                                       //
//                    Program: SAGA                      //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                WKSP_Module_Manager.cpp                //
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
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include <wx/dir.h>
#include <wx/filename.h>

#include <saga_api/saga_api.h>
#include <saga_odbc/saga_odbc.h>

#include "saga.h"

#include "res_commands.h"
#include "res_dialogs.h"

#include "helper.h"

#include "wksp_module_manager.h"
#include "wksp_module_library.h"
#include "wksp_module_menu.h"
#include "wksp_module.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CWKSP_Module_Manager	*g_pModules	= NULL;


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CWKSP_Module_Manager::CWKSP_Module_Manager(void)
{
	g_pModules	= this;

	m_pMenu		= new CWKSP_Module_Menu;

	//-----------------------------------------------------
	m_Parameters.Create(this, _TL(""), _TL(""));

	m_Parameters.Add_Value(
		NULL	, "BEEP"		, _TL("[CAP] Beep when finished"),
		_TL(""),
		PARAMETER_TYPE_Bool	, true
	);

	m_Parameters.Add_Choice(
		NULL	, "START_LOGO"	, _TL("Show Logo at Start Up"),
		_TL(""),
		CSG_String::Format(SG_T("%s|%s|%s|%s|"),
			_TL("do not show"),
			_TL("only during start up phase"),
			_TL("20 seconds"),
			_TL("until user closes it")
		), 1
	);

	m_Parameters.Add_Choice(
		NULL	, "HELP_SOURCE"	, _TL("Module Description Source"),
		_TL(""),
		CSG_String::Format(SG_T("%s|%s|"),
			_TL("built-in"),
			_TL("online")
		), 0
	);

	m_Parameters.Add_Value(
		NULL	, "PROC_FREQ"	, _TL("Process Update Frequency [milliseconds]"),
		_TL(""),
		PARAMETER_TYPE_Int	, 0, 0, true
	);

	m_Parameters.Add_FilePath(
		NULL	, "LNG_FILE_DIC", _TL("Language Translations"),
		_TL("Dictionary for translations from built-in (English) to local language (editable text table)"),
		CSG_String::Format(SG_T("%s|*.lng|%s|*.txt|%s|*.*"),
			_TL("Dictionary Files (*.lng)"),
			_TL("Text Table (*.txt)"),
			_TL("All Files")
		)
	);

	m_Parameters.Add_FilePath(
		NULL	, "CRS_FILE_SRS", _TL("CRS Database"),
		_TL("Database with Coordinate Reference System (CRS) definitions. You have to restart SAGA to make changes take affect!"),
		CSG_String::Format(SG_T("%s|*.srs|%s|*.*"),
			_TL("Spatial Reference System Files (*.srs)"),
			_TL("All Files")
		)
	);

	m_Parameters.Add_FilePath(
		NULL	, "CRS_FILE_DIC", _TL("CRS Dictionary"),
		_TL("Dictionary for Proj.4/OGC WKT translations. You have to restart SAGA to make changes take affect!"),
		CSG_String::Format(SG_T("%s|*.dic|%s|*.*"),
			_TL("Dictionary Files (*.dic)"),
			_TL("All Files")
		)
	);
}

//---------------------------------------------------------
CWKSP_Module_Manager::~CWKSP_Module_Manager(void)
{
	delete(m_pMenu);

	g_pModules	= NULL;
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Module_Manager::Initialise(void)
{
	_Config_Read();

	if( Get_Count() == 0 )
	{
#if defined(_SAGA_LINUX)
	if( _Open_Directory(wxT(MODULE_LIBRARY_PATH)) == 0 )
#endif
		_Open_Directory(g_pSAGA->Get_App_Path(), true);
	}

	return( true );
}

//---------------------------------------------------------
bool CWKSP_Module_Manager::Finalise(void)
{
	SG_ODBC_Get_Connection_Manager().Destroy();

	_Config_Write();

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
wxString CWKSP_Module_Manager::Get_Name(void)
{
	return( _TL("[CAP] Module Libraries") );
}

//---------------------------------------------------------
wxString CWKSP_Module_Manager::Get_Description(void)
{
	wxString	s;

	//-----------------------------------------------------
	s	+= wxString::Format(wxT("<b>%s</b>"), _TL("Module Library"));

	s	+= wxT("<table border=\"0\">");

	DESC_ADD_INT(_TL("Loaded Libraries")	, Get_Count());
	DESC_ADD_INT(_TL("Available Modules")	, Get_Items_Count());

	s	+= wxT("</table>");

	//-----------------------------------------------------
	s	+= wxString::Format(wxT("<hr><b>%s:</b><table border=\"1\">"), _TL("Module Libraries"));

	s	+= wxString::Format(wxT("<tr><th>%s</th><th>%s</th><th>%s</th><th>%s</th></tr>"), _TL("Library"), _TL("Modules"), _TL("Name"), _TL("Location"));

	for(int i=0; i<Get_Count(); i++)
	{
		s	+= wxString::Format(wxT("<tr><td>%s</td><td>%d</td><td>%s</td><td>%s</td></tr>"),
				SG_File_Get_Name(Get_Library(i)->Get_File_Name(), false).w_str(),
				Get_Library(i)->Get_Count(),
				Get_Library(i)->Get_Name(),
				SG_File_Get_Path(Get_Library(i)->Get_File_Name()).w_str()
			);
	}

	s	+= wxT("</table>");

	//-----------------------------------------------------
	return( s );
}

//---------------------------------------------------------
wxMenu * CWKSP_Module_Manager::Get_Menu(void)
{
	wxMenu	*pMenu;

	pMenu	= new wxMenu(_TL("[CAP] Module Libraries"));

	CMD_Menu_Add_Item(pMenu, false, ID_CMD_MODULES_OPEN);

	if( Get_Count() > 0 )
	{
		CMD_Menu_Add_Item(pMenu, false, ID_CMD_WKSP_ITEM_CLOSE);
		pMenu->AppendSeparator();
		CMD_Menu_Add_Item(pMenu, false, ID_CMD_WKSP_ITEM_SEARCH);
	}

	return( pMenu );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Module_Manager::On_Command(int Cmd_ID)
{
	switch( Cmd_ID )
	{
	default:
		return( CWKSP_Base_Manager::On_Command(Cmd_ID) );

	case ID_CMD_MODULES_OPEN:
		Open();
		break;
	}

	return( true );
}

//---------------------------------------------------------
bool CWKSP_Module_Manager::On_Command_UI(wxUpdateUIEvent &event)
{
	switch( event.GetId() )
	{
	default:
		return( CWKSP_Base_Manager::On_Command_UI(event) );

	case ID_CMD_WKSP_ITEM_CLOSE:
		event.Enable(Get_Count() > 0 && g_pModule == NULL);
		break;
	}

	return( true );
}

//---------------------------------------------------------
void CWKSP_Module_Manager::On_Execute(wxCommandEvent &event)
{
	CWKSP_Module	*pModule;

	if( (pModule = Get_Module_byID(m_pMenu->Get_ID_Translated(event.GetId()))) != NULL )
	{
		pModule->On_Command(ID_CMD_WKSP_ITEM_RETURN);
	}
}

//---------------------------------------------------------
void CWKSP_Module_Manager::On_Execute_UI(wxUpdateUIEvent &event)
{
	if( g_pModule )
	{
		if( g_pModule->Get_Menu_ID() == m_pMenu->Get_ID_Translated(event.GetId()) )
		{
			event.Enable(true);
			event.Check(true);
		}
		else
		{
			event.Enable(false);
			event.Check(false);
		}
	}
	else
	{
		event.Enable(true);
		event.Check(false);
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CWKSP_Module_Manager::Parameters_Changed(void)
{
	g_pSAGA->Process_Set_Frequency(m_Parameters("PROC_FREQ")->asInt());

	CWKSP_Base_Item::Parameters_Changed();
}

//---------------------------------------------------------
bool CWKSP_Module_Manager::Do_Beep(void)
{
	return( m_Parameters("BEEP")->asBool() );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define CFG_LIBS	wxT("/LIBS")
#define CFG_LIBF	wxT("LIB_%03d")

//---------------------------------------------------------
void CWKSP_Module_Manager::_Config_Read(void)
{
	bool		bValue;
	long		lValue;
	wxString	sValue;

	if( CONFIG_Read(wxT("/MODULES"), wxT("BEEP")		, bValue) )
	{
		m_Parameters("BEEP")		->Set_Value(bValue);
	}

	if( CONFIG_Read(wxT("/MODULES"), wxT("START_LOGO")	, lValue) )
	{
		m_Parameters("START_LOGO")	->Set_Value((int)lValue);
	}

	if( CONFIG_Read(wxT("/MODULES"), wxT("HELP_SOURCE")	, lValue) )
	{
		m_Parameters("HELP_SOURCE")	->Set_Value((int)lValue);
	}

	if( CONFIG_Read(wxT("/MODULES"), wxT("PROC_FREQ")	, lValue) )
	{
		m_Parameters("PROC_FREQ")	->Set_Value((int)lValue);
	}

	if( CONFIG_Read(wxT("/MODULES"), wxT("LNG_FILE_DIC"), sValue) )
	{
		m_Parameters("LNG_FILE_DIC")->Set_Value(&sValue);
	}

	if( CONFIG_Read(wxT("/MODULES"), wxT("CRS_FILE_SRS"), sValue) )
	{
		m_Parameters("CRS_FILE_SRS")->Set_Value(&sValue);
	}

	if( CONFIG_Read(wxT("/MODULES"), wxT("CRS_FILE_DIC"), sValue) )
	{
		m_Parameters("CRS_FILE_DIC")->Set_Value(&sValue);
	}

	for(int i=0; CONFIG_Read(CFG_LIBS, wxString::Format(CFG_LIBF, i), sValue); i++)
	{
		Open(sValue);
	}

	m_pMenu->Update();
}

//---------------------------------------------------------
void CWKSP_Module_Manager::_Config_Write(void)
{
	CONFIG_Write(wxT("/MODULES"), wxT("BEEP")		 ,       m_Parameters("BEEP")        ->asBool());
	CONFIG_Write(wxT("/MODULES"), wxT("START_LOGO")	 , (long)m_Parameters("START_LOGO")  ->asInt());
	CONFIG_Write(wxT("/MODULES"), wxT("HELP_SOURCE") , (long)m_Parameters("HELP_SOURCE") ->asInt());
	CONFIG_Write(wxT("/MODULES"), wxT("PROC_FREQ")	 , (long)m_Parameters("PROC_FREQ")   ->asInt());
	CONFIG_Write(wxT("/MODULES"), wxT("LNG_FILE_DIC"),       m_Parameters("LNG_FILE_DIC")->asString());
	CONFIG_Write(wxT("/MODULES"), wxT("CRS_FILE_SRS"),       m_Parameters("CRS_FILE_SRS")->asString());
	CONFIG_Write(wxT("/MODULES"), wxT("CRS_FILE_DIC"),       m_Parameters("CRS_FILE_DIC")->asString());

	CONFIG_Delete(CFG_LIBS);

	for(int i=0; i<Get_Count(); i++)
	{
		CONFIG_Write(CFG_LIBS, wxString::Format(CFG_LIBF, i), Get_Library(i)->Get_File_Name());
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CWKSP_Module_Manager::_Open_Directory(const wxChar *sDirectory, bool bOnlySubDirectories)
{
	int			nOpened	= 0;
	wxDir		Dir;
	wxString	FileName;

	if( Dir.Open(sDirectory) )
	{
		if( !bOnlySubDirectories && Dir.GetFirst(&FileName, wxEmptyString, wxDIR_FILES) )
		{
			do
			{	if( FileName.Find(wxT("saga_")) < 0 && FileName.Find(wxT("wx")) < 0 && FileName.Find(wxT("mingw")) < 0 )
				if( Open(SG_File_Make_Path(Dir.GetName(), FileName, NULL).w_str()) )
				{
					nOpened++;
				}
			}
			while( Dir.GetNext(&FileName) );
		}

		if( Dir.GetFirst(&FileName, wxEmptyString, wxDIR_DIRS) )
		{
			do
			{
				if( FileName.CmpNoCase(wxT("dll")) )
				{
					nOpened	+= _Open_Directory(SG_File_Make_Path(Dir.GetName(), FileName, NULL));
				}
			}
			while( Dir.GetNext(&FileName) );
		}
	}

	return( nOpened );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CWKSP_Module_Manager::Open(void)
{
	wxArrayString	File_Paths;

	if( DLG_Open(File_Paths, ID_DLG_MODULES_OPEN) )
	{
		MSG_General_Add_Line();

		for(size_t i=0; i<File_Paths.GetCount(); i++)
		{
			Open(File_Paths[i]);
		}

		m_pMenu->Update();
	}
}

//---------------------------------------------------------
bool CWKSP_Module_Manager::Open(const wxString &File_Name)
{
	if( SG_Get_Module_Library_Manager().Add_Library(File_Name) )
	{
		return( Add_Item(new CWKSP_Module_Library(SG_Get_Module_Library_Manager().Get_Library(SG_Get_Module_Library_Manager().Get_Count() - 1))) );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Module_Manager::Exists(CWKSP_Module *pModule)
{
	for(int i=0; i<Get_Count(); i++)
	{
		if( Get_Library(i)->Get_Module(pModule) != NULL )
		{
			return( true );
		}
	}

	return( false );
}

//---------------------------------------------------------
CWKSP_Module * CWKSP_Module_Manager::Get_Module_byID(int CMD_ID)
{
	CWKSP_Module	*pModule;

	for(int i=0; i<Get_Count(); i++)
	{
		if( (pModule = Get_Library(i)->Get_Module_byID(CMD_ID)) != NULL )
		{
			return( pModule );
		}
	}

	return( NULL );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------