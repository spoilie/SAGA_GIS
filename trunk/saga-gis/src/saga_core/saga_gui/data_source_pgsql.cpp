/**********************************************************
 * Version $Id: Data_Source.cpp 911 2011-02-14 16:38:15Z reklov_w $
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
//                 data_source_pgsql.cpp                 //
//                                                       //
//          Copyright (C) 2013 by Olaf Conrad            //
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
// 51 Franklin Street, 5th Floor, Boston, MA 02110-1301, //
// USA.                                                  //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//    contact:    Olaf Conrad                            //
//                Institute of Geography                 //
//                University of Hamburg                  //
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
#include <wx/menu.h>

#include "helper.h"

#include "res_controls.h"
#include "res_commands.h"
#include "res_dialogs.h"
#include "res_images.h"

#include "wksp.h"
#include "wksp_data_manager.h"

#include "data_source_pgsql.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
enum
{
	TYPE_ROOT	= 0,
	TYPE_SOURCE,
	TYPE_TABLE,
	TYPE_SHAPES,
	TYPE_GRIDS,
	TYPE_GRID
};

//---------------------------------------------------------
enum
{
	IMG_ROOT	= 0,
	IMG_SRC_CLOSED,
	IMG_SRC_OPENED,
	IMG_TABLE,
	IMG_POINT,
	IMG_POINTS,
	IMG_LINE,
	IMG_POLYGON,
	IMG_GRIDS,
	IMG_GRID
};

//---------------------------------------------------------
enum
{
	DB_PGSQL_Get_Connections	= 0,
	DB_PGSQL_Get_Connection		= 1,
	DB_PGSQL_Del_Connection		= 2,
	DB_PGSQL_Del_Connections	= 3,
	DB_PGSQL_Transaction_Start	= 4,
	DB_PGSQL_Transaction_Stop	= 5,
	DB_PGSQL_Execute_SQL		= 6,

	DB_PGSQL_Table_List			= 10,
	DB_PGSQL_Table_Info			= 11,
	DB_PGSQL_Table_Load			= 12,
	DB_PGSQL_Table_Save			= 13,
	DB_PGSQL_Table_Drop			= 14,
	DB_PGSQL_Table_Query		= 15,

	DB_PGSQL_Shapes_Load		= 20,
	DB_PGSQL_Shapes_Save		= 21,
	DB_PGSQL_Shapes_SRID_Update	= 22,

	DB_PGSQL_Raster_Load		= 30,
	DB_PGSQL_Raster_Load_Band	= 33,
	DB_PGSQL_Raster_Save		= 31,
	DB_PGSQL_Raster_SRID_Update	= 32
};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define RUN_MODULE(MODULE, bManager, CONDITION)	bool bResult = false;\
{\
	CSG_Module	*pModule	= SG_Get_Module_Library_Manager().Get_Module("db_pgsql", MODULE);\
	\
	if(	pModule )\
	{\
		pModule->Settings_Push(bManager ? &SG_Get_Data_Manager() : NULL);\
		bResult	= pModule->On_Before_Execution() && (CONDITION) && pModule->Execute();\
		pModule->Settings_Pop();\
	}\
}

//---------------------------------------------------------
#define SET_PARAMETER(IDENTIFIER, VALUE)	pModule->Set_Parameter(IDENTIFIER, VALUE)


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool	is_Connected	(const CSG_String &Server)
{
	CSG_Table	Connections;

	RUN_MODULE(DB_PGSQL_Get_Connections, false, SET_PARAMETER("CONNECTIONS", &Connections));	// CGet_Connections

	for(int i=0; bResult && i<Connections.Get_Count(); i++)
	{
		if( !Server.Cmp(Connections[i].asString(0)) )
		{
			return( true );
		}
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CData_Source_PgSQL_Data : public wxTreeItemData
{
public:
    CData_Source_PgSQL_Data(int Type, const CSG_String &Value = "", const CSG_String &Server = "", const CSG_String &Username = "", const CSG_String &Password = "")
		: m_Type(Type), m_Value(Value), m_Server(Server), m_Username(Username), m_Password(Password)
	{}

	int						Get_Type		(void)	const	{	return( m_Type     );	}
	const CSG_String &		Get_Value		(void)	const	{	return( m_Value    );	}
	const CSG_String &		Get_Server		(void)	const	{	return( m_Server   );	}

	void					Set_Username	(const SG_Char *Username)	{	m_Username	= Username;	}
	const CSG_String &		Get_Username	(void)	const	{	return( m_Username     );	}
	void					Set_Password	(const SG_Char *Password)	{	m_Password	= Password;	}
	const CSG_String &		Get_Password	(void)	const	{	return( m_Password );	}

	CSG_String				Get_Host		(void)  const	{	return( m_Server.AfterLast ('[').BeforeFirst(':') );	}
	CSG_String				Get_Port		(void)  const	{	return( m_Server.AfterLast (':').BeforeFirst(']') );	}
	CSG_String				Get_DBName		(void)  const	{	CSG_String s(m_Server.BeforeLast('[')); s.Trim(true); return( s );	}

	bool					is_Connected	(void)	const	{	return( ::is_Connected(m_Server) );	}


private:

    int						m_Type;

	CSG_String				m_Value, m_Server, m_Username, m_Password;

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
BEGIN_EVENT_TABLE(CData_Source_PgSQL, wxTreeCtrl)
	EVT_MENU					(ID_CMD_DB_REFRESH				, CData_Source_PgSQL::On_Refresh)
	EVT_MENU					(ID_CMD_DB_SOURCE_OPEN			, CData_Source_PgSQL::On_Source_Open)
	EVT_MENU					(ID_CMD_DB_SOURCE_CLOSE			, CData_Source_PgSQL::On_Source_Close)
	EVT_MENU					(ID_CMD_DB_SOURCE_CLOSE_ALL		, CData_Source_PgSQL::On_Sources_Close)
	EVT_MENU					(ID_CMD_DB_SOURCE_DELETE		, CData_Source_PgSQL::On_Source_Delete)
	EVT_MENU					(ID_CMD_DB_TABLE_OPEN			, CData_Source_PgSQL::On_Table_Open)
	EVT_MENU					(ID_CMD_DB_TABLE_RENAME			, CData_Source_PgSQL::On_Table_Rename)
	EVT_MENU					(ID_CMD_DB_TABLE_INFO			, CData_Source_PgSQL::On_Table_Info)
	EVT_MENU					(ID_CMD_DB_TABLE_DELETE			, CData_Source_PgSQL::On_Table_Drop)

	EVT_TREE_ITEM_ACTIVATED		(ID_WND_DATA_SOURCE_DATABASE	, CData_Source_PgSQL::On_Item_Activated)
	EVT_TREE_ITEM_RIGHT_CLICK	(ID_WND_DATA_SOURCE_DATABASE	, CData_Source_PgSQL::On_Item_RClick)
	EVT_TREE_ITEM_MENU			(ID_WND_DATA_SOURCE_DATABASE	, CData_Source_PgSQL::On_Item_Menu)
END_EVENT_TABLE()


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define CFG_PGSQL_DIR	wxT("/PGSQL")
#define CFG_PGSQL_SRC	wxT("SRC_%03d")

//---------------------------------------------------------
CData_Source_PgSQL::CData_Source_PgSQL(wxWindow *pParent)
	: wxTreeCtrl(pParent, ID_WND_DATA_SOURCE_DATABASE)
{
	AssignImageList(new wxImageList(IMG_SIZE_TREECTRL, IMG_SIZE_TREECTRL, true, 0));
	IMG_ADD_TO_TREECTRL(ID_IMG_WKSP_DB_SOURCES    );	// IMG_ROOT
	IMG_ADD_TO_TREECTRL(ID_IMG_WKSP_DB_SOURCE_OFF );	// IMG_SRC_CLOSED
	IMG_ADD_TO_TREECTRL(ID_IMG_WKSP_DB_SOURCE_ON  );	// IMG_SRC_OPENED
	IMG_ADD_TO_TREECTRL(ID_IMG_WKSP_DB_TABLE      );	// IMG_TABLE
	IMG_ADD_TO_TREECTRL(ID_IMG_WKSP_SHAPES_POINT  );	// IMG_POINT
	IMG_ADD_TO_TREECTRL(ID_IMG_WKSP_SHAPES_POINTS );	// IMG_POINTS
	IMG_ADD_TO_TREECTRL(ID_IMG_WKSP_SHAPES_LINE   );	// IMG_LINE
	IMG_ADD_TO_TREECTRL(ID_IMG_WKSP_SHAPES_POLYGON);	// IMG_POLYGON
	IMG_ADD_TO_TREECTRL(ID_IMG_WKSP_GRID_MANAGER  );	// IMG_GRIDS
	IMG_ADD_TO_TREECTRL(ID_IMG_WKSP_GRID          );	// IMG_GRID

	AddRoot(_TL("PostgreSQL Sources"), IMG_ROOT, IMG_ROOT, new CData_Source_PgSQL_Data(TYPE_ROOT));

	//-----------------------------------------------------
	SG_UI_Msg_Lock(true);

	wxString	Server;

	for(int i=0; CONFIG_Read(CFG_PGSQL_DIR, wxString::Format(CFG_PGSQL_SRC, i), Server); i++)
	{
		wxString	User, Password;

		if( Server.Find("|") > 0 )
		{
			User     = Server.AfterFirst ('|').BeforeFirst('|');
			Password = Server.AfterLast  ('|');
			Server   = Server.BeforeFirst('|');
		}

		CData_Source_PgSQL_Data	*pData	= new CData_Source_PgSQL_Data(TYPE_SOURCE, &Server, &Server, &User, &Password);

		Update_Source(AppendItem(GetRootItem(), Server.c_str(), IMG_SRC_CLOSED, IMG_SRC_CLOSED, pData));
	}

	Update_Sources();

	SG_UI_Msg_Lock(false);
}

//---------------------------------------------------------
CData_Source_PgSQL::~CData_Source_PgSQL(void)
{
	wxTreeItemIdValue	Cookie;
	wxTreeItemId		Item	= GetFirstChild(GetRootItem(), Cookie);

	long Reopen	= 0;

	CONFIG_Read("/DATA", "PROJECT_DB_REOPEN", Reopen);

	CONFIG_Delete(CFG_PGSQL_DIR);

	for(int i=0; Item.IsOk(); )
	{
		CData_Source_PgSQL_Data	*pData	= (CData_Source_PgSQL_Data *)GetItemData(Item);

		if( pData && pData->Get_Type() == TYPE_SOURCE )
		{
			CSG_String	Connection	= pData->Get_Server().c_str();

			if( Reopen != 0 && pData->is_Connected() && !pData->Get_Username().is_Empty() )	// store user and password
			{
				Connection	+= "|" + pData->Get_Username() + "|" + pData->Get_Password();
			}

			CONFIG_Write(CFG_PGSQL_DIR, wxString::Format(CFG_PGSQL_SRC, i++), Connection.c_str());
		}

		Item	= GetNextChild(Item, Cookie);
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CData_Source_PgSQL::Autoconnect(void)
{
	long Reopen	= 0;

	CONFIG_Read("/DATA", "PROJECT_DB_REOPEN", Reopen);

	if( Reopen != 0 )
	{
		wxTreeItemIdValue	Cookie;

		wxTreeItemId	Item	= GetFirstChild(GetRootItem(), Cookie);

		while( Item.IsOk() )
		{
			CData_Source_PgSQL_Data	*pData	= Item.IsOk() ? (CData_Source_PgSQL_Data *)GetItemData(Item) : NULL; if( pData == NULL )	return;

			if( pData->Get_Type() == TYPE_SOURCE && !pData->Get_Username().is_Empty() )
			{
				Source_Open(pData, false);
			}

			Item	= GetNextChild(Item, Cookie);
		}
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CData_Source_PgSQL::On_Refresh(wxCommandEvent &WXUNUSED(event))
{
	Update_Item(GetSelection());
}

//---------------------------------------------------------
void CData_Source_PgSQL::On_Source_Open(wxCommandEvent &WXUNUSED(event))
{
	Source_Open(GetSelection());
}

//---------------------------------------------------------
void CData_Source_PgSQL::On_Source_Close(wxCommandEvent &WXUNUSED(event))
{
	Source_Close(GetSelection(), false);
}

//---------------------------------------------------------
void CData_Source_PgSQL::On_Sources_Close(wxCommandEvent &WXUNUSED(event))
{
	Sources_Close();
}

//---------------------------------------------------------
void CData_Source_PgSQL::On_Source_Delete(wxCommandEvent &WXUNUSED(event))
{
	Source_Close(GetSelection(), true);
}

//---------------------------------------------------------
void CData_Source_PgSQL::On_Table_Open(wxCommandEvent &WXUNUSED(event))
{
	Table_Open(GetSelection());
}

//---------------------------------------------------------
void CData_Source_PgSQL::On_Table_Rename(wxCommandEvent &WXUNUSED(event))
{
	Table_Rename(GetSelection());
}

//---------------------------------------------------------
void CData_Source_PgSQL::On_Table_Info(wxCommandEvent &WXUNUSED(event))
{
	Table_Info(GetSelection());
}

//---------------------------------------------------------
void CData_Source_PgSQL::On_Table_Drop(wxCommandEvent &WXUNUSED(event))
{
	Table_Drop(GetSelection());
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CData_Source_PgSQL::On_Item_Activated(wxTreeEvent &event)
{
	CData_Source_PgSQL_Data	*pData	= event.GetItem().IsOk() ? (CData_Source_PgSQL_Data *)GetItemData(event.GetItem()) : NULL; if( pData == NULL )	return;

	switch( pData->Get_Type() )
	{
	case TYPE_ROOT:
		Update_Sources();
		break;

	case TYPE_SOURCE:
		Source_Open(event.GetItem());
		break;

	case TYPE_TABLE:
	case TYPE_SHAPES:
	case TYPE_GRIDS:
	case TYPE_GRID:
		Table_Open(event.GetItem());
		break;
	}
}

//---------------------------------------------------------
void CData_Source_PgSQL::On_Item_RClick(wxTreeEvent &event)
{
	SelectItem(event.GetItem());

	event.Skip();
}

//---------------------------------------------------------
void CData_Source_PgSQL::On_Item_Menu(wxTreeEvent &event)
{
	CData_Source_PgSQL_Data	*pData	= event.GetItem().IsOk() ? (CData_Source_PgSQL_Data *)GetItemData(event.GetItem()) : NULL; if( pData == NULL )	return;

	wxMenu	Menu;

	switch( pData->Get_Type() )
	{
	case TYPE_ROOT:
		CMD_Menu_Add_Item(&Menu, false, ID_CMD_DB_REFRESH);
		CMD_Menu_Add_Item(&Menu, false, ID_CMD_DB_SOURCE_OPEN);
		CMD_Menu_Add_Item(&Menu, false, ID_CMD_DB_SOURCE_CLOSE_ALL);
		break;

	case TYPE_SOURCE:
		if( !pData->is_Connected() )
		{
			CMD_Menu_Add_Item(&Menu, false, ID_CMD_DB_REFRESH);
			CMD_Menu_Add_Item(&Menu, false, ID_CMD_DB_SOURCE_OPEN);
			CMD_Menu_Add_Item(&Menu, false, ID_CMD_DB_SOURCE_DELETE);
		}
		else
		{
			CMD_Menu_Add_Item(&Menu, false, ID_CMD_DB_REFRESH);
			CMD_Menu_Add_Item(&Menu, false, ID_CMD_DB_SOURCE_CLOSE);
			CMD_Menu_Add_Item(&Menu, false, ID_CMD_DB_SOURCE_DELETE);
			CMD_Menu_Add_Item(&Menu, false, ID_CMD_DB_TABLE_FROM_QUERY);
		}
		break;

	case TYPE_TABLE:
	case TYPE_SHAPES:
	case TYPE_GRIDS:
		CMD_Menu_Add_Item(&Menu, false, ID_CMD_DB_TABLE_OPEN);
		CMD_Menu_Add_Item(&Menu, false, ID_CMD_DB_TABLE_RENAME);
		CMD_Menu_Add_Item(&Menu, false, ID_CMD_DB_TABLE_DELETE);
		CMD_Menu_Add_Item(&Menu, false, ID_CMD_DB_TABLE_INFO);
		break;

	case TYPE_GRID:
		CMD_Menu_Add_Item(&Menu, false, ID_CMD_DB_TABLE_OPEN);
		CMD_Menu_Add_Item(&Menu, false, ID_CMD_DB_TABLE_RENAME);
		CMD_Menu_Add_Item(&Menu, false, ID_CMD_DB_TABLE_DELETE);
		break;
	}

	if( Menu.GetMenuItemCount() > 0 )
	{
		PopupMenu(&Menu);
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
wxTreeItemId CData_Source_PgSQL::Find_Source(const wxString &Server)
{
	wxTreeItemIdValue	Cookie;

	wxTreeItemId	Item	= GetFirstChild(GetRootItem(), Cookie);

	while( Item.IsOk() && Server.Cmp(GetItemText(Item)) )
	{
		Item	= GetNextChild(Item, Cookie);
	}

	return( Item );
}

//---------------------------------------------------------
void CData_Source_PgSQL::Update_Item(const wxTreeItemId &Item)
{
	CData_Source_PgSQL_Data	*pData	= Item.IsOk() ? (CData_Source_PgSQL_Data *)GetItemData(Item) : NULL; if( pData == NULL )	return;

	switch( pData->Get_Type() )
	{
	case TYPE_ROOT:		Update_Sources();		break;
	case TYPE_SOURCE:	Update_Source(Item);	break;
	}
}

//---------------------------------------------------------
void CData_Source_PgSQL::Update_Sources(void)
{
	Freeze();

	//-----------------------------------------------------
	wxTreeItemIdValue	Cookie;

	wxTreeItemId	Item	= GetFirstChild(GetRootItem(), Cookie);

	while( Item.IsOk() )
	{
		Update_Source(Item);

		Item	= GetNextChild(Item, Cookie);
	}

	//-----------------------------------------------------
	CSG_Table	Connections;

	RUN_MODULE(DB_PGSQL_Get_Connections, false, SET_PARAMETER("CONNECTIONS", &Connections));	// CGet_Connections

	for(int i=0; i<Connections.Get_Count(); i++)
	{
		if( !Find_Source(Connections[i].asString(0)) )
		{
			Update_Source(Connections[i].asString(0));
		}
	}

	//-----------------------------------------------------
	SortChildren(GetRootItem());
	Expand      (GetRootItem());

	Thaw();
}

//---------------------------------------------------------
void CData_Source_PgSQL::Update_Source(const wxString &Server)
{
	if( Server.IsEmpty() )
	{
		Update_Sources();

		return;
	}

	wxTreeItemId	Item	= Find_Source(Server);

	if( !Item.IsOk() && is_Connected(&Server) )
	{
		Item	= AppendItem(GetRootItem(), Server.c_str(), IMG_SRC_OPENED, IMG_SRC_OPENED,
			new CData_Source_PgSQL_Data(TYPE_SOURCE, &Server, &Server)
		);
	}

	Update_Source(Item);
}

//---------------------------------------------------------
void CData_Source_PgSQL::Update_Source(const wxTreeItemId &Item)
{
	CData_Source_PgSQL_Data	*pData	= Item.IsOk() ? (CData_Source_PgSQL_Data *)GetItemData(Item) : NULL; if( pData == NULL )	return;

	if( pData->Get_Type() != TYPE_SOURCE )
	{
		return;
	}

	Freeze();

	DeleteChildren(Item);

	//-----------------------------------------------------
	if( !pData->is_Connected() )
	{
		SetItemImage(Item, IMG_SRC_CLOSED, wxTreeItemIcon_Normal);
		SetItemImage(Item, IMG_SRC_CLOSED, wxTreeItemIcon_Selected);
	}
	else
	{
		SetItemImage(Item, IMG_SRC_OPENED, wxTreeItemIcon_Normal);
		SetItemImage(Item, IMG_SRC_OPENED, wxTreeItemIcon_Selected);

		CSG_Table	Tables;

		RUN_MODULE(DB_PGSQL_Table_List, false,	// CTable_List
				SET_PARAMETER("CONNECTION", pData->Get_Value())
			&&	SET_PARAMETER("TABLES"    , &Tables)
		);

		Tables.Set_Index(1, TABLE_INDEX_Ascending, 0, TABLE_INDEX_Ascending);

		for(int i=0; i<Tables.Get_Count(); i++)
		{
			CSG_String	s(Tables[i].asString(1));

			TSG_Shape_Type	Shape;
			TSG_Vertex_Type	Vertex;

			if( CSG_Shapes_OGIS_Converter::to_ShapeType(s, Shape, Vertex) )
			{
				switch( Shape )
				{
				case SHAPE_TYPE_Point:   Append_Table(Item, Tables[i].asString(0), TYPE_SHAPES, IMG_POINT  ); break;
				case SHAPE_TYPE_Points:  Append_Table(Item, Tables[i].asString(0), TYPE_SHAPES, IMG_POINTS ); break;
				case SHAPE_TYPE_Line:    Append_Table(Item, Tables[i].asString(0), TYPE_SHAPES, IMG_LINE   ); break;
				case SHAPE_TYPE_Polygon: Append_Table(Item, Tables[i].asString(0), TYPE_SHAPES, IMG_POLYGON); break;
				}
			}
			else if( !s.Cmp("RASTER" ) ) Append_Table(Item, Tables[i].asString(0), TYPE_GRIDS , IMG_GRIDS  );
			else if( !s.Cmp("TABLE"  ) ) Append_Table(Item, Tables[i].asString(0), TYPE_TABLE , IMG_TABLE  );
		}

		Expand(Item);
	}

	Thaw();
}

//---------------------------------------------------------
void CData_Source_PgSQL::Append_Table(const wxTreeItemId &Parent, const SG_Char *Name, int Type, int Image)
{
	CData_Source_PgSQL_Data	*pData	= Parent.IsOk() ? (CData_Source_PgSQL_Data *)GetItemData(Parent) : NULL; if( pData == NULL )	return;

	wxTreeItemId	Item	= AppendItem(Parent, Name, Image, Image, new CData_Source_PgSQL_Data(Type, Name, pData->Get_Server()));

	if( Type == TYPE_GRIDS )
	{
		CSG_Table	Grids;

		RUN_MODULE(DB_PGSQL_Table_Query, false,	// CTable_Query
				SET_PARAMETER("CONNECTION", pData->Get_Server())
			&&	SET_PARAMETER("TABLES"    , Name)
			&&	SET_PARAMETER("TABLE"     , &Grids)
			&&  SET_PARAMETER("FIELDS"    , SG_T("rid, name"))
		);

		if( bResult )
		{
			for(int i=0; i<Grids.Get_Count(); i++)
			{
				AppendItem(Item, Grids[i].asString(1), IMG_GRID, IMG_GRID,
					new CData_Source_PgSQL_Data(TYPE_GRID, CSG_String::Format("%s:rid=%d", Name, Grids[i].asInt(0)), pData->Get_Server())
				);
			}
		}
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CData_Source_PgSQL::Source_Open(CData_Source_PgSQL_Data *pData, bool bDialog)
{
	static	wxString	Username = "postgres", Password = "postgres";

	if( bDialog )
	{
		if( !DLG_Login(Username, Password) )
		{
			return( false );
		}

		pData->Set_Username(Username);
		pData->Set_Password(Password);
	}

	//-----------------------------------------------------
	MSG_General_Add(wxString::Format("%s: %s...", _TL("Connect to Database"), pData->Get_Server().c_str()), true, true);

	RUN_MODULE(DB_PGSQL_Get_Connection, false,	// CGet_Connection
			SET_PARAMETER("PG_HOST", pData->Get_Host    ())
		&&	SET_PARAMETER("PG_PORT", pData->Get_Port    ())
		&&	SET_PARAMETER("PG_NAME", pData->Get_DBName  ())
		&&	SET_PARAMETER("PG_USER", pData->Get_Username())
		&&	SET_PARAMETER("PG_PWD" , pData->Get_Password())
	);

	if( bResult )
	{
		MSG_General_Add(_TL("okay"), false, false, SG_UI_MSG_STYLE_SUCCESS);

		return( true );
	}

	MSG_General_Add(_TL("failed"), false, false, SG_UI_MSG_STYLE_FAILURE);

	return( false );
}

//---------------------------------------------------------
void CData_Source_PgSQL::Source_Open(const wxTreeItemId &Item)
{
	CData_Source_PgSQL_Data	*pData	= Item.IsOk() ? (CData_Source_PgSQL_Data *)GetItemData(Item) : NULL; if( pData == NULL )	return;

	if( pData->Get_Type() == TYPE_ROOT )
	{
		CSG_Module	*pModule	= SG_Get_Module_Library_Manager().Get_Module("db_pgsql", DB_PGSQL_Get_Connection);	// CGet_Connection

		if(	pModule )
		{
			if( pModule->On_Before_Execution() && DLG_Parameters(pModule->Get_Parameters()) )
			{
				pModule->Execute();
			}
		}
	}
	else if( pData->is_Connected() )
	{
		Update_Source(Item);
	}
	else if( !Source_Open(pData, true) )
	{
		DLG_Message_Show_Error(_TL("Could not connect to data source."), _TL("Connect to PostgreSQL"));
	}
}

//---------------------------------------------------------
void CData_Source_PgSQL::Source_Close(const wxTreeItemId &Item, bool bDelete)
{
	CData_Source_PgSQL_Data	*pData	= Item.IsOk() ? (CData_Source_PgSQL_Data *)GetItemData(Item) : NULL; if( pData == NULL )	return;

	if( pData->is_Connected() )
	{
		RUN_MODULE(DB_PGSQL_Del_Connection, true, SET_PARAMETER("CONNECTION", pData->Get_Server()));
	}

	if( bDelete )
	{
		Delete(Item);
	}
	else
	{
		pData->Set_Username(SG_T(""));
		pData->Set_Password(SG_T(""));
	}
}

//---------------------------------------------------------
void CData_Source_PgSQL::Sources_Close(void)
{
	RUN_MODULE(DB_PGSQL_Del_Connections, true, true);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CData_Source_PgSQL::Table_Open(const wxTreeItemId &Item)
{
	CData_Source_PgSQL_Data	*pData	= Item.IsOk() ? (CData_Source_PgSQL_Data *)GetItemData(Item) : NULL; if( pData == NULL )	return;

	//-----------------------------------------------------
	if( pData->Get_Type() == TYPE_TABLE )
	{
		CSG_Table	*pTable	= SG_Create_Table();

		RUN_MODULE(DB_PGSQL_Table_Load, true,	// CTable_Load
				SET_PARAMETER("CONNECTION", pData->Get_Server())
			&&	SET_PARAMETER("TABLES"    , pData->Get_Value ())
			&&	SET_PARAMETER("TABLE"     , pTable)
		);

		if( bResult )
		{
			g_pData->Show(pTable, 0);
		}
		else
		{
			delete(pTable);
		}
	}

	//-----------------------------------------------------
	if( pData->Get_Type() == TYPE_SHAPES )
	{
		CSG_Shapes	*pShapes	= SG_Create_Shapes();

		RUN_MODULE(DB_PGSQL_Shapes_Load, true,	// CPGIS_Shapes_Load
				SET_PARAMETER("CONNECTION", pData->Get_Server())
			&&	SET_PARAMETER("TABLES"    , pData->Get_Value ())
			&&	SET_PARAMETER("SHAPES"    , pShapes)
		);

		if( bResult )
		{
		//	g_pData->Show(pShapes, SG_UI_DATAOBJECT_SHOW_NEW_MAP);
		}
		else
		{
			delete(pShapes);
		}
	}

	//-----------------------------------------------------
	if( pData->Get_Type() == TYPE_GRIDS )
	{
		RUN_MODULE(DB_PGSQL_Raster_Load, true,
				SET_PARAMETER("CONNECTION", pData->Get_Server())
			&&	SET_PARAMETER("TABLES"    , pData->Get_Value ())
		);
	}

	//-----------------------------------------------------
	if( pData->Get_Type() == TYPE_GRID )
	{
		RUN_MODULE(DB_PGSQL_Raster_Load, true,
				SET_PARAMETER("CONNECTION", pData->Get_Server())
			&&	SET_PARAMETER("TABLES"    , pData->Get_Value ().BeforeFirst(':'))
			&&	SET_PARAMETER("WHERE"     , pData->Get_Value ().AfterFirst (':'))
		);
	}
}

//---------------------------------------------------------
void CData_Source_PgSQL::Table_Rename(const wxTreeItemId &Item)
{
	CData_Source_PgSQL_Data	*pData	= Item.IsOk() ? (CData_Source_PgSQL_Data *)GetItemData(Item) : NULL; if( pData == NULL )	return;

	wxString	Name	= GetItemText(Item);

	switch( pData->Get_Type() )
	{
	//-----------------------------------------------------
	case TYPE_GRID:
		if( DLG_Get_Text(Name, _TL("Rename Raster Band"), _TL("Name")) )
		{
			CSG_String	Table	= pData->Get_Value().BeforeFirst(':');
			CSG_String	rid		= pData->Get_Value().AfterFirst (':');
			CSG_String	SQL	= "UPDATE \"" + Table + "\" SET name='" + CSG_String(&Name) + "' WHERE " + rid + ";";

			RUN_MODULE(DB_PGSQL_Execute_SQL, false,
					SET_PARAMETER("CONNECTION", pData->Get_Server())
				&&	SET_PARAMETER("SQL"       , SQL)
			);

			if( bResult )
			{
				SetItemText(Item, Name);
			}
		}
		break;

	//-----------------------------------------------------
	default:
		if( DLG_Get_Text(Name, _TL("Rename Table"), _TL("Name")) )
		{
			CSG_String	SQL	= "ALTER TABLE \"" + pData->Get_Value() + "\" RENAME TO \"" + CSG_String(&Name) + "\";";

			RUN_MODULE(DB_PGSQL_Execute_SQL, false,
					SET_PARAMETER("CONNECTION", pData->Get_Server())
				&&	SET_PARAMETER("SQL"       , SQL)
			);

			if( bResult )
			{
				SetItemText(Item, Name);
			}
		}
		break;
	}
}

//---------------------------------------------------------
void CData_Source_PgSQL::Table_Info(const wxTreeItemId &Item)
{
	CData_Source_PgSQL_Data	*pData	= Item.IsOk() ? (CData_Source_PgSQL_Data *)GetItemData(Item) : NULL; if( pData == NULL )	return;

	CSG_Table	*pTable	= SG_Create_Table();

	RUN_MODULE(DB_PGSQL_Table_Info, true,	// CTable_Info
			SET_PARAMETER("CONNECTION", pData->Get_Server())
		&&	SET_PARAMETER("TABLES"    , pData->Get_Value ())
		&&	SET_PARAMETER("TABLE"     , pTable)
	);

	if( bResult )
	{
		g_pData->Show(pTable, 0);
	}
	else
	{
		delete(pTable);
	}
}

//---------------------------------------------------------
void CData_Source_PgSQL::Table_Drop(const wxTreeItemId &Item)
{
	CData_Source_PgSQL_Data	*pData	= Item.IsOk() ? (CData_Source_PgSQL_Data *)GetItemData(Item) : NULL; if( pData == NULL )	return;

	wxString	Name	= GetItemText(Item);

	switch( pData->Get_Type() )
	{
	//-----------------------------------------------------
	case TYPE_GRID:
		if( DLG_Message_Confirm(wxString::Format("%s [%s]", _TL("Do you really want to delete this raster band"), Name.c_str()), _TL("Raster Band Deletion")) )
		{
			MSG_General_Add(wxString::Format("%s: [%s] %s...", _TL("Deleting raster band"), pData->Get_Server().c_str(), Name.c_str()), true, true);

			CSG_String	Table	= pData->Get_Value().BeforeFirst(':');
			CSG_String	rid		= pData->Get_Value().AfterFirst (':');
			CSG_String	SQL	= "DELETE FROM \"" + Table + "\" WHERE " + rid + ";";

			RUN_MODULE(DB_PGSQL_Execute_SQL, false,
					SET_PARAMETER("CONNECTION", pData->Get_Server())
				&&	SET_PARAMETER("SQL"       , SQL)
			);

			if( bResult )
			{
				Delete(Item);

				MSG_General_Add(_TL("okay"), false, false, SG_UI_MSG_STYLE_SUCCESS);
			}
			else
			{
				MSG_General_Add(_TL("failed"), false, false, SG_UI_MSG_STYLE_FAILURE);
			}
		}
		break;

	//-----------------------------------------------------
	default:
		if( DLG_Message_Confirm(wxString::Format("%s [%s]", _TL("Do you really want to delete the table"), pData->Get_Value().c_str()), _TL("Table Deletion")) )
		{
			MSG_General_Add(wxString::Format("%s: [%s] %s...", _TL("Deleting table"), pData->Get_Server().c_str(), pData->Get_Value().c_str()), true, true);

			RUN_MODULE(DB_PGSQL_Table_Drop, false,	// CTable_Drop
					SET_PARAMETER("CONNECTION", pData->Get_Server())
				&&	SET_PARAMETER("TABLES"    , pData->Get_Value())
			);

			if( bResult )
			{
				Delete(Item);

				MSG_General_Add(_TL("okay"), false, false, SG_UI_MSG_STYLE_SUCCESS);
			}
			else
			{
				MSG_General_Add(_TL("failed"), false, false, SG_UI_MSG_STYLE_FAILURE);
			}
		}
		break;
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
