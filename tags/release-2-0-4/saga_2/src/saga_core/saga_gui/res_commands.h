
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
//                    RES_Commands.h                     //
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
#ifndef _HEADER_INCLUDED__SAGA_GUI__RES_Commands_H
#define _HEADER_INCLUDED__SAGA_GUI__RES_Commands_H


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include <wx/event.h>

//---------------------------------------------------------
#include <saga_api/saga_api.h>


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define RECENT_COUNT				7

//---------------------------------------------------------
enum ID_COMMANDS
{
	ID_CMD_FIRST					= wxID_HIGHEST + 1000,

	ID_CMD_FRAME_CASCADE,
	ID_CMD_FRAME_TILE_HORZ,
	ID_CMD_FRAME_TILE_VERT,
	ID_CMD_FRAME_ARRANGEICONS,
	ID_CMD_FRAME_NEXT,
	ID_CMD_FRAME_PREVIOUS,
	ID_CMD_FRAME_CLOSE,
	ID_CMD_FRAME_CLOSE_ALL,

	ID_CMD_FRAME_QUIT,
	ID_CMD_FRAME_HELP,
	ID_CMD_FRAME_ABOUT,
	ID_CMD_FRAME_TIPS,

	ID_CMD_FRAME_INFO_SHOW,
	ID_CMD_FRAME_WKSP_SHOW,
	ID_CMD_FRAME_ACTIVE_SHOW,

	//-----------------------------------------------------
	ID_CMD_WKSP_FIRST,
	ID_CMD_WKSP_OPEN,
	ID_CMD_WKSP_ITEM_RETURN,
	ID_CMD_WKSP_ITEM_CLOSE,
	ID_CMD_WKSP_ITEM_SETTINGS_LOAD,
	ID_CMD_WKSP_ITEM_SHOW,

	ID_CMD_INFO_CLEAR,
	ID_CMD_INFO_COPY,

	ID_CMD_MODULES_FIRST,
	ID_CMD_MODULES_RECENT_FIRST,
	ID_CMD_MODULES_RECENT_LAST		= RECENT_COUNT + ID_CMD_MODULES_RECENT_FIRST,
	ID_CMD_MODULES_OPEN,
	ID_CMD_MODULES_SAVE_HTML,
	ID_CMD_MODULES_SAVE_SCRIPT,
	ID_CMD_MODULES_LAST,

	ID_CMD_DATA_FIRST,
	ID_CMD_DATA_PROJECT_RECENT_FIRST,
	ID_CMD_DATA_PROJECT_RECENT_LAST	= RECENT_COUNT + ID_CMD_DATA_PROJECT_RECENT_FIRST,
	ID_CMD_DATA_PROJECT_OPEN,
	ID_CMD_DATA_PROJECT_OPEN_ADD,
	ID_CMD_DATA_PROJECT_SAVE,
	ID_CMD_DATA_PROJECT_SAVE_AS,
	ID_CMD_DATA_LAST,

	ID_CMD_GRIDS_FIRST,
	ID_CMD_GRIDS_RECENT_FIRST,
	ID_CMD_GRIDS_RECENT_LAST		= RECENT_COUNT + ID_CMD_GRIDS_RECENT_FIRST,
	ID_CMD_GRIDS_OPEN,
	ID_CMD_GRIDS_SAVE,
	ID_CMD_GRIDS_SAVEAS,
	ID_CMD_GRIDS_SAVEAS_IMAGE,
	ID_CMD_GRIDS_SHOW,
	ID_CMD_GRIDS_HISTOGRAM,
	ID_CMD_GRIDS_SCATTERPLOT,
	ID_CMD_GRIDS_EQUALINTERVALS,
	ID_CMD_GRIDS_SET_LUT,
	ID_CMD_GRIDS_LAST,

	ID_CMD_SHAPES_FIRST,
	ID_CMD_SHAPES_RECENT_FIRST,
	ID_CMD_SHAPES_RECENT_LAST		= RECENT_COUNT + ID_CMD_SHAPES_RECENT_FIRST,
	ID_CMD_SHAPES_OPEN,
	ID_CMD_SHAPES_SAVE,
	ID_CMD_SHAPES_SAVEAS,
	ID_CMD_SHAPES_SHOW,
	ID_CMD_SHAPES_SET_LUT,
	ID_CMD_SHAPES_EDIT_SHAPE,
	ID_CMD_SHAPES_EDIT_SEL_INVERT,
	ID_CMD_SHAPES_EDIT_ADD_SHAPE,
	ID_CMD_SHAPES_EDIT_ADD_PART,
	ID_CMD_SHAPES_EDIT_DEL_SHAPE,
	ID_CMD_SHAPES_EDIT_DEL_PART,
	ID_CMD_SHAPES_EDIT_DEL_POINT,
	ID_CMD_SHAPES_LAST,

	ID_CMD_TABLES_FIRST,
	ID_CMD_TABLES_RECENT_FIRST,
	ID_CMD_TABLES_RECENT_LAST		= RECENT_COUNT + ID_CMD_TABLES_RECENT_FIRST,
	ID_CMD_TABLES_OPEN,
	ID_CMD_TABLES_SAVE,
	ID_CMD_TABLES_SAVEAS,
	ID_CMD_TABLES_SHOW,
	ID_CMD_TABLES_DIAGRAM,
	ID_CMD_TABLES_SCATTERPLOT,
	ID_CMD_TABLES_LAST,

	ID_CMD_TIN_FIRST,
	ID_CMD_TIN_RECENT_FIRST,
	ID_CMD_TIN_RECENT_LAST			= RECENT_COUNT + ID_CMD_TIN_RECENT_FIRST,
	ID_CMD_TIN_OPEN,
	ID_CMD_TIN_SAVE,
	ID_CMD_TIN_SAVEAS,
	ID_CMD_TIN_SHOW,
	ID_CMD_TIN_LAST,

	ID_CMD_MAPS_FIRST,
	ID_CMD_MAPS_SHOW,
	ID_CMD_MAPS_3D_SHOW,
	ID_CMD_MAPS_LAYOUT_SHOW,
	ID_CMD_MAPS_SAVE_IMAGE,
	ID_CMD_MAPS_SAVE_TO_CLIPBOARD,
	ID_CMD_MAPS_SAVE_IMAGE_ON_CHANGE,
	ID_CMD_MAPS_SAVE_PDF_INDEXED,
	ID_CMD_MAPS_SAVE_INTERACTIVE_SVG,
	ID_CMD_MAPS_SYNCHRONIZE,
	ID_CMD_MAPS_LAYER_SHOW,
	ID_CMD_MAPS_MOVE_TOP,
	ID_CMD_MAPS_MOVE_BOTTOM,
	ID_CMD_MAPS_MOVE_UP,
	ID_CMD_MAPS_MOVE_DOWN,
	ID_CMD_MAPS_GRID_FITCOLORS,
	ID_CMD_MAPS_LAST,

	ID_CMD_WKSP_LAST,

	//-----------------------------------------------------
	ID_CMD_CHILD_FIRST,

	ID_CMD_MAP_FIRST,
	ID_CMD_MAP_CLOSE,
	ID_CMD_MAP_TOOLBAR,
	ID_CMD_MAP_3D_SHOW,
	ID_CMD_MAP_LAYOUT_SHOW,
	ID_CMD_MAP_SAVE_IMAGE,
	ID_CMD_MAP_SAVE_IMAGE_ON_CHANGE,
	ID_CMD_MAP_SAVE_PDF_INDEXED,
	ID_CMD_MAP_SAVE_INTERACTIVE_SVG,
	ID_CMD_MAP_SYNCHRONIZE,
	ID_CMD_MAP_LEGEND_VERTICAL,
	ID_CMD_MAP_LEGEND_HORIZONTAL,
	ID_CMD_MAP_ZOOM_FULL,
	ID_CMD_MAP_ZOOM_BACK,
	ID_CMD_MAP_ZOOM_FORWARD,
	ID_CMD_MAP_ZOOM_ACTIVE,
	ID_CMD_MAP_ZOOM_SELECTION,
	ID_CMD_MAP_ZOOM_EXTENT,
	ID_CMD_MAP_MODE_ZOOM,
	ID_CMD_MAP_MODE_PAN,
	ID_CMD_MAP_MODE_SELECT,
	ID_CMD_MAP_MODE_DISTANCE,
	ID_CMD_MAP_LAST,

	ID_CMD_MAP3D_FIRST,
	ID_CMD_MAP3D_PARAMETERS,
	ID_CMD_MAP3D_ROTATE_X_LESS,
	ID_CMD_MAP3D_ROTATE_X_MORE,
	ID_CMD_MAP3D_ROTATE_Y_LESS,
	ID_CMD_MAP3D_ROTATE_Y_MORE,
	ID_CMD_MAP3D_ROTATE_Z_LESS,
	ID_CMD_MAP3D_ROTATE_Z_MORE,
	ID_CMD_MAP3D_SHIFT_X_LESS,
	ID_CMD_MAP3D_SHIFT_X_MORE,
	ID_CMD_MAP3D_SHIFT_Y_LESS,
	ID_CMD_MAP3D_SHIFT_Y_MORE,
	ID_CMD_MAP3D_SHIFT_Z_LESS,
	ID_CMD_MAP3D_SHIFT_Z_MORE,
	ID_CMD_MAP3D_EXAGGERATE_LESS,
	ID_CMD_MAP3D_EXAGGERATE_MORE,
	ID_CMD_MAP3D_CENTRAL,
	ID_CMD_MAP3D_CENTRAL_LESS,
	ID_CMD_MAP3D_CENTRAL_MORE,
	ID_CMD_MAP3D_STEREO,
	ID_CMD_MAP3D_STEREO_LESS,
	ID_CMD_MAP3D_STEREO_MORE,
	ID_CMD_MAP3D_INTERPOLATED,
	ID_CMD_MAP3D_SRC_RES_LESS,
	ID_CMD_MAP3D_SRC_RES_MORE,
	ID_CMD_MAP3D_SAVE,
	ID_CMD_MAP3D_SEQ_POS_ADD,
	ID_CMD_MAP3D_SEQ_POS_DEL,
	ID_CMD_MAP3D_SEQ_POS_DEL_ALL,
	ID_CMD_MAP3D_SEQ_POS_EDIT,
	ID_CMD_MAP3D_SEQ_PLAY,
	ID_CMD_MAP3D_SEQ_PLAY_LOOP,
	ID_CMD_MAP3D_SEQ_SAVE,
	ID_CMD_MAP3D_LAST,

	ID_CMD_TABLE_FIRST,
	ID_CMD_TABLE_FIELD_ADD,
	ID_CMD_TABLE_FIELD_DEL,
	ID_CMD_TABLE_FIELD_SORT,
	ID_CMD_TABLE_FIELD_RENAME,
	ID_CMD_TABLE_RECORD_ADD,
	ID_CMD_TABLE_RECORD_INS,
	ID_CMD_TABLE_RECORD_DEL,
	ID_CMD_TABLE_RECORD_DEL_ALL,
	ID_CMD_TABLE_AUTOSIZE_COLS,
	ID_CMD_TABLE_AUTOSIZE_ROWS,
	ID_CMD_TABLE_LAST,

	ID_CMD_DIAGRAM_FIRST,
	ID_CMD_DIAGRAM_PARAMETERS,
	ID_CMD_DIAGRAM_LAST,

	ID_CMD_SCATTERPLOT_FIRST,
	ID_CMD_SCATTERPLOT_PARAMETERS,
	ID_CMD_SCATTERPLOT_UPDATE,
	ID_CMD_SCATTERPLOT_LAST,

	ID_CMD_HISTOGRAM_FIRST,
	ID_CMD_HISTOGRAM_TOOLBAR,
	ID_CMD_HISTOGRAM_CUMULATIVE,
	ID_CMD_HISTOGRAM_AS_TABLE,
	ID_CMD_HISTOGRAM_LAST,

	ID_CMD_LAYOUT_FIRST,
	ID_CMD_LAYOUT_TOOLBAR,
	ID_CMD_LAYOUT_PRINT_SETUP,
	ID_CMD_LAYOUT_PAGE_SETUP,
	ID_CMD_LAYOUT_PRINT,
	ID_CMD_LAYOUT_PRINT_PREVIEW,
	ID_CMD_LAYOUT_FIT_SCALE,
	ID_CMD_LAYOUT_LAST,

	ID_CMD_CHILD_LAST,

	//-----------------------------------------------------
	ID_CMD_MODULE_FIRST,
	ID_CMD_MODULE_RECENT_FIRST,
	ID_CMD_MODULE_RECENT_LAST		= RECENT_COUNT + ID_CMD_MODULE_RECENT_FIRST,
	ID_CMD_MODULE_START,
	ID_CMD_MODULE_LAST				= ID_CMD_MODULE_START + 1000
};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
const wxChar *			CMD_Get_Name				(int Cmd_ID);
const wxChar *			CMD_Get_Help				(int Cmd_ID);
int						CMD_Get_ImageID				(int Cmd_ID);

void					CMD_Menu_Add_Item			(class wxMenu *pMenu, bool bCheck, int Cmd_ID);
void					CMD_Menu_Ins_Item			(class wxMenu *pMenu, bool bCheck, int Cmd_ID, int Position);

class wxToolBarBase *	CMD_ToolBar_Create			(int ID);
void					CMD_ToolBar_Add				(class wxToolBarBase *pToolBar, const wxChar *Name);
void					CMD_ToolBar_Add_Item		(class wxToolBarBase *pToolBar, bool bCheck, int Cmd_ID);
void					CMD_ToolBar_Add_Separator	(class wxToolBarBase *pToolBar);


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef _HEADER_INCLUDED__SAGA_GUI__RES_Commands_H