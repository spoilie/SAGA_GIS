# Microsoft Developer Studio Project File - Name="saga_gui" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=saga_gui - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "saga_gui.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "saga_gui.mak" CFG="saga_gui - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "saga_gui - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "saga_gui - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "saga_gui - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vc_msw"
# PROP BASE Intermediate_Dir "vc_msw\saga_gui"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "./../../../bin/saga_vc"
# PROP Intermediate_Dir "./../../../bin/tmp/saga_vc/saga_gui"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GR /O2 /I ".\..\..\include" /I ".\..\..\lib\vc_lib\msw" /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "NOPCH" /Fd"vc_msw\saga_gui.pdb" /FD /EHsc /c
# ADD CPP /nologo /MD /W3 /GR /O2 /I "$(SAGA)/src/saga_core" /I "." /I "$(WXWIN)/include" /I "$(WXWIN)/lib/vc_lib/msw" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "NOPCH" /D "_TYPEDEF_BYTE" /D "_TYPEDEF_WORD" /D "_SAGA_MSW" /Fr /Fd"./../../../bin/tmp/saga_vc/saga_gui/saga_gui.pdb" /FD /EHsc /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i ".\..\..\include" /i ".\..\..\lib\vc_lib\msw" /i "." /i ".\..\..\samples" /d "__WXMSW__" /d "_WINDOWS" /d "NOPCH"
# ADD RSC /l 0x409 /i "./../../../wxWidgets/include" /i "./../../../wxWidgets/lib/vc_lib/msw" /d "__WXMSW__" /d "_WINDOWS" /d "NOPCH"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw26_core.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /subsystem:windows /machine:I386 /libpath:".\..\..\lib\vc_lib"
# ADD LINK32 wxbase26_xml.lib wxmsw26_html.lib wxmsw26_adv.lib wxmsw26_core.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /subsystem:windows /machine:I386 /libpath:"$(WXWIN)/lib/vc_lib" /libpath:"$(WXWIN)/wxDockIt/lib/vc_lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy res\saga_eng.txt ..\..\..\bin\saga_vc\saga.eng	copy res\saga_ger.txt ..\..\..\bin\saga_vc\saga.ger
# End Special Build Tool

!ELSEIF  "$(CFG)" == "saga_gui - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vc_mswd"
# PROP BASE Intermediate_Dir "vc_mswd\saga_gui"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "./../../../bin/saga_vc_dbg"
# PROP Intermediate_Dir "./../../../bin/tmp/saga_vc_dbg/saga_gui"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GR /Zi /Od /I ".\..\..\include" /I ".\..\..\lib\vc_lib\mswd" /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_WINDOWS" /D "NOPCH" /Fd"vc_mswd\saga_gui.pdb" /FD /GZ /EHsc /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /ZI /Od /I "$(SAGA)/src/saga_core" /I "." /I "$(WXWIN)/include" /I "$(WXWIN)/lib/vc_lib/mswd" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_WINDOWS" /D "NOPCH" /D "_TYPEDEF_BYTE" /D "_TYPEDEF_WORD" /D "_SAGA_MSW" /Fr /Fd"./../../../bin/tmp/saga_vc_dbg/saga_gui/saga_gui.pdb" /FD /GZ /EHsc /c
# ADD BASE MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i ".\..\..\include" /i ".\..\..\lib\vc_lib\mswd" /i "." /i ".\..\..\samples" /d "_DEBUG" /d "__WXMSW__" /d "__WXDEBUG__" /d "_WINDOWS" /d "NOPCH"
# ADD RSC /l 0x409 /i "./../../../wxWidgets/include" /i "./../../../wxWidgets/lib/vc_lib/mswd" /d "_DEBUG" /d "__WXMSW__" /d "__WXDEBUG__" /d "_WINDOWS" /d "NOPCH"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw26d_core.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:".\..\..\lib\vc_lib"
# ADD LINK32 wxbase26d_xml.lib wxmsw26d_html.lib wxmsw26d_adv.lib wxmsw26d_core.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:"$(WXWIN)/lib/vc_lib" /libpath:"$(WXWIN)/wxDockIt/lib/vc_lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy res\saga_eng.txt ..\..\..\bin\saga_vc_dbg\saga.eng	copy res\saga_ger.txt ..\..\..\bin\saga_vc_dbg\saga.ger
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "saga_gui - Win32 Release"
# Name "saga_gui - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\active.cpp
# End Source File
# Begin Source File

SOURCE=.\active_attributes.cpp
# End Source File
# Begin Source File

SOURCE=.\active_description.cpp
# End Source File
# Begin Source File

SOURCE=.\ACTIVE_HTMLExtraInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\active_legend.cpp
# End Source File
# Begin Source File

SOURCE=.\active_parameters.cpp
# End Source File
# Begin Source File

SOURCE=.\callback.cpp
# End Source File
# Begin Source File

SOURCE=.\dc_helper.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_about.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_about_logo.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_base.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_colors.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_colors_control.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_list_base.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_list_grid.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_list_shapes.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_list_table.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_list_tin.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_parameters.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_table.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_text.cpp
# End Source File
# Begin Source File

SOURCE=.\helper.cpp
# End Source File
# Begin Source File

SOURCE=.\info.cpp
# End Source File
# Begin Source File

SOURCE=.\info_messages.cpp
# End Source File
# Begin Source File

SOURCE=.\parameters_control.cpp
# End Source File
# Begin Source File

SOURCE=.\parameters_properties.cpp
# End Source File
# Begin Source File

SOURCE=.\project.cpp
# End Source File
# Begin Source File

SOURCE=.\res_commands.cpp
# End Source File
# Begin Source File

SOURCE=.\res_controls.cpp
# End Source File
# Begin Source File

SOURCE=.\res_dialogs.cpp
# End Source File
# Begin Source File

SOURCE=.\res_images.cpp
# End Source File
# Begin Source File

SOURCE=.\saga.cpp
# End Source File
# Begin Source File

SOURCE=.\saga_frame.cpp
# End Source File
# Begin Source File

SOURCE=.\saga_frame_droptarget.cpp
# End Source File
# Begin Source File

SOURCE=.\saga_frame_layout.cpp
# End Source File
# Begin Source File

SOURCE=.\SVG_Interactive_Map.cpp
# End Source File
# Begin Source File

SOURCE=.\view_base.cpp
# End Source File
# Begin Source File

SOURCE=.\view_histogram.cpp
# End Source File
# Begin Source File

SOURCE=.\view_layout.cpp
# End Source File
# Begin Source File

SOURCE=.\view_layout_control.cpp
# End Source File
# Begin Source File

SOURCE=.\view_layout_info.cpp
# End Source File
# Begin Source File

SOURCE=.\view_layout_printout.cpp
# End Source File
# Begin Source File

SOURCE=.\view_map.cpp
# End Source File
# Begin Source File

SOURCE=.\view_map_3d.cpp
# End Source File
# Begin Source File

SOURCE=.\view_map_3d_image.cpp
# End Source File
# Begin Source File

SOURCE=.\view_map_control.cpp
# End Source File
# Begin Source File

SOURCE=.\view_ruler.cpp
# End Source File
# Begin Source File

SOURCE=.\view_scatterplot.cpp
# End Source File
# Begin Source File

SOURCE=.\view_table.cpp
# End Source File
# Begin Source File

SOURCE=.\view_table_control.cpp
# End Source File
# Begin Source File

SOURCE=.\view_table_diagram.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_base_control.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_base_item.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_base_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_data_control.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_data_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_data_menu_file.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_data_menu_files.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_grid.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_grid_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_grid_system.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_layer.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_layer_classify.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_layer_legend.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_map.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_map_control.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_map_dc.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_map_layer.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_map_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_module.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_module_control.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_module_library.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_module_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_module_menu.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_shapes.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_shapes_edit.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_shapes_line.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_shapes_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_shapes_point.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_shapes_points.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_shapes_polygon.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_shapes_type.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_table.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_table_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_tin.cpp
# End Source File
# Begin Source File

SOURCE=.\wksp_tin_manager.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ACTIVE.h
# End Source File
# Begin Source File

SOURCE=.\ACTIVE_Attributes.h
# End Source File
# Begin Source File

SOURCE=.\ACTIVE_Description.h
# End Source File
# Begin Source File

SOURCE=.\ACTIVE_HTMLExtraInfo.h
# End Source File
# Begin Source File

SOURCE=.\active_legend.h
# End Source File
# Begin Source File

SOURCE=.\ACTIVE_Parameters.h
# End Source File
# Begin Source File

SOURCE=.\Callback.h
# End Source File
# Begin Source File

SOURCE=.\DC_Helper.h
# End Source File
# Begin Source File

SOURCE=.\dlg_about.h
# End Source File
# Begin Source File

SOURCE=.\dlg_about_logo.h
# End Source File
# Begin Source File

SOURCE=.\DLG_Base.h
# End Source File
# Begin Source File

SOURCE=.\DLG_Colors.h
# End Source File
# Begin Source File

SOURCE=.\DLG_Colors_Control.h
# End Source File
# Begin Source File

SOURCE=.\DLG_List_Base.h
# End Source File
# Begin Source File

SOURCE=.\DLG_List_Grid.h
# End Source File
# Begin Source File

SOURCE=.\DLG_List_Shapes.h
# End Source File
# Begin Source File

SOURCE=.\DLG_List_Table.h
# End Source File
# Begin Source File

SOURCE=.\DLG_List_TIN.h
# End Source File
# Begin Source File

SOURCE=.\DLG_Parameters.h
# End Source File
# Begin Source File

SOURCE=.\DLG_Table.h
# End Source File
# Begin Source File

SOURCE=.\DLG_Text.h
# End Source File
# Begin Source File

SOURCE=.\Helper.h
# End Source File
# Begin Source File

SOURCE=.\INFO.h
# End Source File
# Begin Source File

SOURCE=.\INFO_Messages.h
# End Source File
# Begin Source File

SOURCE=.\Parameters_Control.h
# End Source File
# Begin Source File

SOURCE=.\Parameters_Properties.h
# End Source File
# Begin Source File

SOURCE=.\Project.h
# End Source File
# Begin Source File

SOURCE=.\RES_Commands.h
# End Source File
# Begin Source File

SOURCE=.\RES_Controls.h
# End Source File
# Begin Source File

SOURCE=.\RES_Dialogs.h
# End Source File
# Begin Source File

SOURCE=.\RES_Images.h
# End Source File
# Begin Source File

SOURCE=.\SAGA.h
# End Source File
# Begin Source File

SOURCE=.\SAGA_Frame.h
# End Source File
# Begin Source File

SOURCE=.\SAGA_Frame_DropTarget.h
# End Source File
# Begin Source File

SOURCE=.\saga_frame_layout.h
# End Source File
# Begin Source File

SOURCE=.\SVG_Interactive_Map.h
# End Source File
# Begin Source File

SOURCE=.\VIEW_Base.h
# End Source File
# Begin Source File

SOURCE=.\VIEW_Histogram.h
# End Source File
# Begin Source File

SOURCE=.\VIEW_Layout.h
# End Source File
# Begin Source File

SOURCE=.\VIEW_Layout_Control.h
# End Source File
# Begin Source File

SOURCE=.\VIEW_Layout_Info.h
# End Source File
# Begin Source File

SOURCE=.\VIEW_Layout_Printout.h
# End Source File
# Begin Source File

SOURCE=.\VIEW_Map.h
# End Source File
# Begin Source File

SOURCE=.\VIEW_Map_3D.h
# End Source File
# Begin Source File

SOURCE=.\view_map_3d_image.h
# End Source File
# Begin Source File

SOURCE=.\VIEW_Map_Control.h
# End Source File
# Begin Source File

SOURCE=.\VIEW_Ruler.h
# End Source File
# Begin Source File

SOURCE=.\VIEW_ScatterPlot.h
# End Source File
# Begin Source File

SOURCE=.\VIEW_Table.h
# End Source File
# Begin Source File

SOURCE=.\VIEW_Table_Control.h
# End Source File
# Begin Source File

SOURCE=.\VIEW_Table_Diagram.h
# End Source File
# Begin Source File

SOURCE=.\WKSP.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_Base_Control.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_Base_Item.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_Base_Manager.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_Data_Control.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_Data_Manager.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_Data_Menu_File.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_Data_Menu_Files.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_Grid.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_Grid_Manager.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_Grid_System.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_Layer.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_Layer_Classify.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_Layer_Legend.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_Map.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_Map_Control.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_Map_DC.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_Map_Layer.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_Map_Manager.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_Module.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_Module_Control.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_Module_Library.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_Module_Manager.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_Module_Menu.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_Shapes.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_Shapes_Line.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_Shapes_Manager.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_Shapes_Point.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_Shapes_Points.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_Shapes_Polygon.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_Shapes_Type.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_Table.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_Table_Manager.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_TIN.h
# End Source File
# Begin Source File

SOURCE=.\WKSP_TIN_Manager.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "rc;xpm"
# Begin Group "XPM"

# PROP Default_Filter "xpm"
# Begin Source File

SOURCE=.\RES\XPM\CRS_EDIT_POINT_ADD.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\CRS_EDIT_POINT_MOVE.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\CRS_HAND.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\CRS_HAND_GRAP.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\CRS_INFO.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\CRS_SELECT.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\DEFAULT.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\NB_ACTIVE_ATTRIBUTES.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\NB_ACTIVE_DESCRIPTION.xpm
# End Source File
# Begin Source File

SOURCE=.\res\xpm\nb_active_htmlextrainfo.xpm
# End Source File
# Begin Source File

SOURCE=.\res\xpm\NB_ACTIVE_LEGEND.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\NB_ACTIVE_PARAMETERS.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\NB_INFO_ERROR.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\NB_INFO_EXECUTION.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\NB_INFO_MESSAGES.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\NB_WKSP_DATA.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\NB_WKSP_MAPS.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\NB_WKSP_MODULES.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\SAGA_ICON_16.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\SAGA_ICON_32.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\SAGA_SPLASH.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_ACTIVE.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_DELETE.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_DIAGRAM_PARAMETERS.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_EDIT_SHAPE_ADD.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_EDIT_SHAPE_ADD_PART.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_EDIT_SHAPE_DEL.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_EDIT_SHAPE_DEL_PART.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_EDIT_SHAPE_DEL_POINT.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_EDIT_SHAPE_SELECT.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_HELP.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_HISTOGRAM_AS_TABLE.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_HISTOGRAM_CUMULATIVE.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_INFO.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_LAYOUT_PAGE_SETUP.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_LAYOUT_PRINT.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_LAYOUT_PRINT_PREVIEW.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_LAYOUT_PRINT_SETUP.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_MAP3D_CENTRAL_LESS.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_MAP3D_CENTRAL_MORE.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_MAP3D_EXAGGERATE_LESS.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_MAP3D_EXAGGERATE_MORE.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_MAP3D_INTERPOLATED.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_MAP3D_PROPERTIES.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_MAP3D_ROTATE_X_LESS.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_MAP3D_ROTATE_X_MORE.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_MAP3D_ROTATE_Z_LESS.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_MAP3D_ROTATE_Z_MORE.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_MAP3D_SHIFT_X_LESS.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_MAP3D_SHIFT_X_MORE.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_MAP3D_SHIFT_Y_LESS.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_MAP3D_SHIFT_Y_MORE.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_MAP3D_SHIFT_Z_LESS.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_MAP3D_SHIFT_Z_MORE.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_MAP3D_STEREO.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_MAP_3D_SHOW.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_MAP_LAYOUT_SHOW.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_MAP_LEGEND_HORIZONTAL.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_MAP_LEGEND_VERTICAL.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_MAP_MODE_DISTANCE.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_MAP_MODE_PAN.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_MAP_MODE_SELECT.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_MAP_MODE_ZOOM.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_MAP_SYNCHRONIZE.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_MAP_ZOOM_ACTIVE.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_MAP_ZOOM_FULL.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_MAP_ZOOM_LAST.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_MAP_ZOOM_SELECTION.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_OPEN.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_OPEN_GRID.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_OPEN_MODULE.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_OPEN_SHAPES.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_OPEN_TABLE.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_OPEN_TIN.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_SAVE.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_SCATTERPLOT_PARAMETERS.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_SHOW.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_TABLE_ROW_ADD.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_TABLE_ROW_DEL.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_TABLE_ROW_INS.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\TB_WKSP.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\WKSP_DATA_MANAGER.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\WKSP_GRID.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\WKSP_GRID_MANAGER.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\WKSP_GRID_SYSTEM.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\WKSP_MAP.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\WKSP_MAP_MANAGER.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\WKSP_MODULE.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\WKSP_MODULE_LIBRARY.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\WKSP_MODULE_MANAGER.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\WKSP_NOITEMS.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\WKSP_SHAPES_LINE.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\WKSP_SHAPES_MANAGER.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\WKSP_SHAPES_POINT.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\WKSP_SHAPES_POINTS.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\WKSP_SHAPES_POLYGON.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\WKSP_TABLE.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\WKSP_TABLE_MANAGER.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\WKSP_TIN.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\WKSP_TIN_MANAGER.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\WND_DIAGRAM.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\WND_HISTOGRAM.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\WND_LAYOUT.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\WND_MAP.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\WND_MAP3D.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\WND_SCATTERPLOT.xpm
# End Source File
# Begin Source File

SOURCE=.\RES\XPM\WND_TABLE.xpm
# End Source File
# End Group
# Begin Source File

SOURCE=.\RES\colours.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\resource.h
# End Source File
# Begin Source File

SOURCE=.\RES\SAGA.ico
# End Source File
# Begin Source File

SOURCE=.\RES\saga_eng.txt
# End Source File
# Begin Source File

SOURCE=.\RES\saga_ger.txt
# End Source File
# Begin Source File

SOURCE=.\res\saga_gui.rc
# End Source File
# End Group
# Begin Group "Control Extension Files"

# PROP Default_Filter ""
# Begin Group "propgrid"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\wx\propgrid\advprops.cpp
# End Source File
# Begin Source File

SOURCE=.\wx\propgrid\extras.cpp
# End Source File
# Begin Source File

SOURCE=.\wx\propgrid\manager.cpp
# End Source File
# Begin Source File

SOURCE=.\wx\propgrid\odcombo.cpp
# End Source File
# Begin Source File

SOURCE=.\wx\propgrid\propgrid.cpp
# End Source File
# Begin Source File

SOURCE=.\wx\propgrid\props.cpp
# End Source File
# End Group
# Begin Group "dockit"

# PROP Default_Filter ""
# Begin Group "msw"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\wx\dockit\msw\dockwindow_msw.cpp
# End Source File
# Begin Source File

SOURCE=.\wx\dockit\msw\gdi_msw.cpp
# End Source File
# Begin Source File

SOURCE=.\wx\dockit\msw\pane_msw.cpp
# End Source File
# Begin Source File

SOURCE=.\wx\dockit\msw\toolbutton_msw.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\wx\dockit\generic\barholder.cpp
# End Source File
# Begin Source File

SOURCE=.\wx\dockit\generic\dockhost.cpp
# End Source File
# Begin Source File

SOURCE=.\wx\dockit\generic\dockpanel.cpp
# End Source File
# Begin Source File

SOURCE=.\wx\dockit\generic\dockwindow.cpp
# End Source File
# Begin Source File

SOURCE=.\wx\dockit\generic\exsplitter.cpp
# End Source File
# Begin Source File

SOURCE=.\wx\dockit\generic\gdi.cpp
# End Source File
# Begin Source File

SOURCE=.\wx\dockit\generic\gripper.cpp
# End Source File
# Begin Source File

SOURCE=.\wx\dockit\generic\layoutmanager.cpp
# End Source File
# Begin Source File

SOURCE=.\wx\dockit\generic\pane.cpp
# End Source File
# Begin Source File

SOURCE=.\wx\dockit\generic\slidebar.cpp
# End Source File
# Begin Source File

SOURCE=.\wx\dockit\generic\toolbutton.cpp
# End Source File
# Begin Source File

SOURCE=.\wx\dockit\generic\util.cpp
# End Source File
# End Group
# End Group
# End Target
# End Project