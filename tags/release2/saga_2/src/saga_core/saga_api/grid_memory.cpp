
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
//                   grid_memory.cpp                     //
//                                                       //
//          Copyright (C) 2005 by Olaf Conrad            //
//                                                       //
//-------------------------------------------------------//
//                                                       //
// This file is part of 'SAGA - System for Automated     //
// Geoscientific Analyses'.                              //
//                                                       //
// This library is free software; you can redistribute   //
// it and/or modify it under the terms of the GNU Lesser //
// General Public License as published by the Free       //
// Software Foundation, version 2.1 of the License.      //
//                                                       //
// This library is distributed in the hope that it will  //
// be useful, but WITHOUT ANY WARRANTY; without even the //
// implied warranty of MERCHANTABILITY or FITNESS FOR A  //
// PARTICULAR PURPOSE. See the GNU Lesser General Public //
// License for more details.                             //
//                                                       //
// You should have received a copy of the GNU Lesser     //
// General Public License along with this program; if    //
// not, write to the Free Software Foundation, Inc.,     //
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
#include <memory.h>

#include "grid.h"
#include "parameters.h"


///////////////////////////////////////////////////////////
//														 //
//					Caching Options						 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
static CAPI_String	GRID_CACHE_Directory;

const char *		API_Grid_Cache_Get_Directory(void)
{
	return( GRID_CACHE_Directory );
}

void				API_Grid_Cache_Set_Directory(const char *Directory)
{
	if( API_Directory_isValid(Directory) )
	{
		GRID_CACHE_Directory.Printf(Directory);
	}
}

//---------------------------------------------------------
static bool			GRID_CACHE_bAutomatic	= false;

void				API_Grid_Cache_Set_Automatic(bool bOn)
{
	GRID_CACHE_bAutomatic	= bOn;
}

bool				API_Grid_Cache_Get_Automatic(void)
{
	return( GRID_CACHE_bAutomatic );
}

//---------------------------------------------------------
static int			GRID_CACHE_Confirm		= 1;

void				API_Grid_Cache_Set_Confirm(int Confirm)
{
	GRID_CACHE_Confirm		= Confirm;
}

int					API_Grid_Cache_Get_Confirm(void)
{
	return( GRID_CACHE_Confirm );
}

//---------------------------------------------------------
static int			GRID_CACHE_Threshold	= 40 * N_MEGABYTE_BYTES;

void				API_Grid_Cache_Set_Threshold(int nBytes)
{
	if( nBytes >= 0 )
	{
		GRID_CACHE_Threshold	= nBytes;
	}
}

void				API_Grid_Cache_Set_Threshold_MB(double nMegabytes)
{
	API_Grid_Cache_Set_Threshold((int)(nMegabytes * N_MEGABYTE_BYTES));
}

int					API_Grid_Cache_Get_Threshold(void)
{
	return( GRID_CACHE_Threshold );
}

double				API_Grid_Cache_Get_Threshold_MB(void)
{
	return( (double)GRID_CACHE_Threshold / (double)N_MEGABYTE_BYTES );
}


///////////////////////////////////////////////////////////
//														 //
//						Memory							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CGrid::_Memory_Create(TGrid_Memory_Type Memory_Type)
{
	if( m_System.is_Valid() && m_Type > 0 && m_Type < GRID_TYPE_Count )
	{
		_Memory_Destroy();

		Set_Buffer_Size(GRID_CACHE_Threshold);

		if(	Memory_Type != GRID_MEMORY_Cache && GRID_CACHE_bAutomatic && Get_NCells() * Get_nValueBytes() > GRID_CACHE_Threshold )
		{
			switch( GRID_CACHE_Confirm )
			{
			default:
				Memory_Type	= GRID_MEMORY_Cache;
				break;

			case 1:
				{
					CAPI_String	s;

					s.Printf("%s\n%s\n%s: %.2fMB",
						LNG("Shall I activate file caching for new grid."),
						m_System.Get_Name(),
						LNG("Total memory size"),
						(Get_NCells() * Get_nValueBytes()) / (double)N_MEGABYTE_BYTES
					);

					if( API_Callback_Dlg_Continue(s, LNG("Activate Grid File Cache?")) )
					{
						Memory_Type	= GRID_MEMORY_Cache;
					}
				}
				break;

			case 2:
				{
					CParameters	p(NULL, LNG("Activate Grid File Cache?"), "");

					p.Add_Value(
						NULL	, "BUFFERSIZE"	, LNG("Buffer Size [MB]"),
						"",
						PARAMETER_TYPE_Double, API_Grid_Cache_Get_Threshold_MB(), 0.0, true
					);

					if( API_Callback_Dlg_Parameters(&p, LNG("Activate Grid File Cache?")) )
					{
						Memory_Type	= GRID_MEMORY_Cache;

						Set_Buffer_Size((int)(p("BUFFERSIZE")->asDouble() * N_MEGABYTE_BYTES));
					}
				}
				break;
			}
		}

		//-------------------------------------------------
		switch( Memory_Type )
		{
		case GRID_MEMORY_Normal:
			return( _Array_Create() );

		case GRID_MEMORY_Cache:
			return( _Cache_Create() );

		case GRID_MEMORY_Compression:
			return( _Compr_Create() );
		}
	}

	return( false );
}

//---------------------------------------------------------
void CGrid::_Memory_Destroy(void)
{
	Sort_Discard();

	switch( m_Memory_Type )
	{
	case GRID_MEMORY_Normal:		_Array_Destroy();		break;
	case GRID_MEMORY_Cache:			_Cache_Destroy(false);	break;
	case GRID_MEMORY_Compression:	_Compr_Destroy(false);	break;
	}

	_LineBuffer_Destroy();

	m_Memory_Type	= GRID_MEMORY_Normal;
}


///////////////////////////////////////////////////////////
//														 //
//						LineBuffer						 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CGrid::_LineBuffer_Create(void)
{
	_LineBuffer_Destroy();

	LineBuffer			= (TGrid_Memory_Line *)API_Malloc(LineBuffer_Count * sizeof(TGrid_Memory_Line));

	for(int i=0; i<LineBuffer_Count; i++)
	{
		LineBuffer[i].Data		= (char *)API_Malloc(_LineBuffer_Get_nBytes());
		LineBuffer[i].y			= -1;
		LineBuffer[i].bModified	= false;
	}
}

//---------------------------------------------------------
void CGrid::_LineBuffer_Destroy(void)
{
	if( LineBuffer )
	{
		for(int i=0; i<LineBuffer_Count; i++)
		{
			if( LineBuffer[i].Data )
			{
				API_Free(LineBuffer[i].Data);
			}
		}

		API_Free(LineBuffer);

		LineBuffer	= NULL;
	}
}

//---------------------------------------------------------
bool CGrid::Set_Buffer_Size(int Size)
{
	int		i;

	if( m_System.is_Valid() && m_Type > 0 && m_Type < GRID_TYPE_Count )
	{
		Size	/= _LineBuffer_Get_nBytes();

		if( Size < 1 )
		{
			Size	= 1;
		}
		else if( Size >= Get_NY() )
		{
			Size	= Get_NY() - 1;
		}

		if( Size != LineBuffer_Count )
		{
			if( LineBuffer )
			{
				if( Size > LineBuffer_Count )
				{
					LineBuffer			= (TGrid_Memory_Line *)API_Realloc(LineBuffer, Size * sizeof(TGrid_Memory_Line));

					for(i=LineBuffer_Count; i<Size; i++)
					{
						LineBuffer[i].Data		= (char *)API_Malloc(_LineBuffer_Get_nBytes());
						LineBuffer[i].y			= -1;
						LineBuffer[i].bModified	= false;
					}
				}
				else
				{
					for(i=Size; i<LineBuffer_Count; i++)
					{
						if( LineBuffer[i].Data )
						{
							API_Free(LineBuffer[i].Data);
						}
					}

					LineBuffer			= (TGrid_Memory_Line *)API_Realloc(LineBuffer, Size * sizeof(TGrid_Memory_Line));
				}
			}

			LineBuffer_Count	= Size;
		}

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
void CGrid::_LineBuffer_Flush(void)
{
	if( LineBuffer )
	{
		for(int i=0; i<LineBuffer_Count; i++)
		{
			switch( m_Memory_Type )
			{
		    default:
		        break;

			case GRID_MEMORY_Cache:
				_Cache_LineBuffer_Save(LineBuffer + i);
				break;

			case GRID_MEMORY_Compression:
				_Compr_LineBuffer_Save(LineBuffer + i);
				break;
			}
		}
	}
}

//---------------------------------------------------------
CGrid::TGrid_Memory_Line * CGrid::_LineBuffer_Get_Line(int y)
{
	int					i, iLine;
	TGrid_Memory_Line	tmp_Line;

	if( LineBuffer && y >= 0 && y < Get_NY() )
	{
		//-------------------------------------------------
		if( y != LineBuffer[0].y )
		{
			for(i=1, iLine=0; i<LineBuffer_Count && !iLine; i++)
			{
				if( y == LineBuffer[i].y )
				{
					iLine	= i;
				}
			}

			//---------------------------------------------
			if( !iLine )
			{
				iLine	= LineBuffer_Count - 1;

				switch( m_Memory_Type )
				{
				default:
					break;

				case GRID_MEMORY_Cache:
					_Cache_LineBuffer_Save(LineBuffer + iLine);
					_Cache_LineBuffer_Load(LineBuffer + iLine, y);
					break;

				case GRID_MEMORY_Compression:
					_Compr_LineBuffer_Save(LineBuffer + iLine);
					_Compr_LineBuffer_Load(LineBuffer + iLine, y);
					break;
				}
			}

			//---------------------------------------------
			tmp_Line		= LineBuffer[iLine];

			for(i=iLine; i>0; i--)
			{
				LineBuffer[i]	= LineBuffer[i - 1];
			}	// memmove(LineBuffer + 1, LineBuffer, (iLine - 1) * sizeof(TGrid_Memory_Line));

			LineBuffer[0]	= tmp_Line;
		}

		//-------------------------------------------------
		return( LineBuffer );
	}

	return( NULL );
}

//---------------------------------------------------------
void CGrid::_LineBuffer_Set_Value(int x, int y, double Value)
{
	TGrid_Memory_Line	*pLine;

	if( (pLine = _LineBuffer_Get_Line(y)) != NULL )
	{
		switch( m_Type )
		{
		default:
			break;

		case GRID_TYPE_Byte:
			((BYTE   *)pLine->Data)[x]	= (BYTE  )Value;
			break;

		case GRID_TYPE_Char:
			((char   *)pLine->Data)[x]	= (char  )Value;
			break;

		case GRID_TYPE_Word:
			((WORD   *)pLine->Data)[x]	= (WORD  )Value;
			break;

		case GRID_TYPE_Short:
			((short  *)pLine->Data)[x]	= (short )Value;
			break;

		case GRID_TYPE_DWord:
			((DWORD  *)pLine->Data)[x]	= (DWORD )Value;
			break;

		case GRID_TYPE_Int:
			((int    *)pLine->Data)[x]	= (int   )Value;
			break;

		case GRID_TYPE_Long:
			((long   *)pLine->Data)[x]	= (long  )Value;
			break;

		case GRID_TYPE_Float:
			((float  *)pLine->Data)[x]	= (float )Value;
			break;

		case GRID_TYPE_Double:
			((double *)pLine->Data)[x]	= (double)Value;
			break;
		}

		pLine->bModified	= true;
	}
}

//---------------------------------------------------------
double CGrid::_LineBuffer_Get_Value(int x, int y)
{
	TGrid_Memory_Line	*pLine;

	if( (pLine = _LineBuffer_Get_Line(y)) != NULL )
	{
		switch( m_Type )
		{
		default:
			break;

		case GRID_TYPE_Byte:
			return( ((BYTE   *)pLine->Data)[x] );

		case GRID_TYPE_Char:
			return( ((char   *)pLine->Data)[x] );

		case GRID_TYPE_Word:
			return( ((WORD   *)pLine->Data)[x] );

		case GRID_TYPE_Short:
			return( ((short  *)pLine->Data)[x] );

		case GRID_TYPE_DWord:
			return( ((DWORD  *)pLine->Data)[x] );

		case GRID_TYPE_Int:
			return( ((int    *)pLine->Data)[x] );

		case GRID_TYPE_Long:
			return( ((long   *)pLine->Data)[x] );

		case GRID_TYPE_Float:
			return( ((float  *)pLine->Data)[x] );

		case GRID_TYPE_Double:
			return( ((double *)pLine->Data)[x] );
		}
	}

	return( 0.0 );
}


///////////////////////////////////////////////////////////
//														 //
//						Array							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CGrid::_Array_Create(void)
{
	if( m_System.is_Valid() && m_Type > 0 && m_Type < GRID_TYPE_Count )
	{
		_Array_Destroy();

		m_Values	= (void **)API_Malloc(Get_NY() * sizeof(void *));

		for(int y=0; y<Get_NY(); y++)
		{
			m_Values[y]	= (void *)API_Calloc(1, _Get_nLineBytes());
		}

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
void CGrid::_Array_Destroy(void)
{
	if( m_Values )
	{
		for(int y=0; y<Get_NY(); y++)
		{
			if( m_Values[y] )
			{
				API_Free(m_Values[y]);
			}
		}

		API_Free(m_Values);

		m_Values	= NULL;
	}
}


///////////////////////////////////////////////////////////
//														 //
//						Cache							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CGrid::Set_Cache(bool bOn)
{
	return( bOn ? _Cache_Create() : _Cache_Destroy(true) );
}

//---------------------------------------------------------
bool CGrid::is_Cached(void)
{
	return( m_Memory_Type == GRID_MEMORY_Cache );
}


///////////////////////////////////////////////////////////
//														 //
//				Cache: Create / Destroy					 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CGrid::_Cache_Create(const char *FilePath, TGrid_Type File_Type, long Offset, bool bSwap, bool bFlip)
{
	if( m_System.is_Valid() && m_Type > 0 && m_Type < GRID_TYPE_Count && m_Memory_Type == GRID_MEMORY_Normal )
	{
		Cache_Path.Printf(FilePath);

		if( m_Type == File_Type && (Cache_Stream = fopen(Cache_Path, "rb+")) != NULL )
		{
			m_Memory_bLock	= true;

			Cache_bTemp		= false;

			Cache_Offset	= Offset;
			Cache_bSwap		= bSwap;
			Cache_bFlip		= bFlip;

			_LineBuffer_Create();

			m_Memory_bLock	= false;
			m_Memory_Type	= GRID_MEMORY_Cache;

			_Array_Destroy();
		}
	}

	return( is_Cached() );
}

//---------------------------------------------------------
bool CGrid::_Cache_Create(void)
{
	TGrid_Memory_Line	Line;

	if( m_System.is_Valid() && m_Type > 0 && m_Type < GRID_TYPE_Count && m_Memory_Type == GRID_MEMORY_Normal )
	{
		Cache_Path	= API_Get_Temp_File_Name("SAGA_GRID", API_Grid_Cache_Get_Directory());

		if( (Cache_Stream = fopen(Cache_Path, "wb+")) != NULL )
		{
			m_Memory_bLock	= true;

			Cache_bTemp		= true;

			Cache_Offset	= 0;
			Cache_bSwap		= false;
			Cache_bFlip		= false;

			_LineBuffer_Create();

			if( m_Values )
			{
				Line.Data	= (char *)API_Malloc(_LineBuffer_Get_nBytes());

				for(Line.y=0; Line.y<Get_NY() && API_Callback_Process_Set_Progress(Line.y, Get_NY()); Line.y++)
				{
					Line.bModified	= true;
					memcpy(Line.Data, m_Values[Line.y], _LineBuffer_Get_nBytes());
					_Cache_LineBuffer_Save(&Line);
					API_Free(m_Values[Line.y]);
				}

				API_Free(Line.Data);

				API_Free(m_Values);
				m_Values	= NULL;

				API_Callback_Process_Set_Ready();
			}

			m_Memory_bLock	= false;
			m_Memory_Type	= GRID_MEMORY_Cache;
		}
	}

	return( is_Cached() );
}

//---------------------------------------------------------
bool CGrid::_Cache_Destroy(bool bMemory_Restore)
{
	int					y;
	TGrid_Memory_Line	*pLine;

	if( is_Valid() && m_Memory_Type == GRID_MEMORY_Cache )
	{
		m_Memory_bLock	= true;

		if( !Cache_bTemp )
		{
			_LineBuffer_Flush();
		}

		if( bMemory_Restore && _Array_Create() )
		{
			for(y=0; y<Get_NY() && API_Callback_Process_Set_Progress(y, Get_NY()); y++)
			{
				if( (pLine = _LineBuffer_Get_Line(y)) != NULL )
				{
					memcpy(m_Values[y], pLine->Data, _LineBuffer_Get_nBytes());
				}
			}

			API_Callback_Process_Set_Ready();
		}

		_LineBuffer_Destroy();

		m_Memory_bLock	= false;
		m_Memory_Type	= GRID_MEMORY_Normal;

		//-------------------------------------------------
		fclose(Cache_Stream);
		Cache_Stream	= NULL;

		if( Cache_bTemp )
		{
			API_Remove_File(Cache_Path);
		}

		return( true );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//					Cache: Save / Load					 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CGrid::_Cache_LineBuffer_Save(TGrid_Memory_Line *pLine)
{
	int		x, y, Line_Pos, Line_Size;
	char	*pValue;

	if( pLine && pLine->bModified )
	{
		pLine->bModified	= false;

		if( pLine->y >= 0 && pLine->y < Get_NY() )
		{
			y			= Cache_bFlip ? Get_NY() - 1 - pLine->y : pLine->y;

			Line_Size	= _LineBuffer_Get_nBytes();
			Line_Pos	= Cache_Offset + y * Line_Size;

			//-------------------------------------------------
			if( Cache_bSwap && m_Type != GRID_TYPE_Bit )
			{
				for(x=0, pValue=pLine->Data; x<Get_NX(); x++, pValue+=Get_nValueBytes())
				{
					_Swap_Bytes(pValue, Get_nValueBytes());
				}
			}

			fseek(Cache_Stream, Line_Pos, SEEK_SET);
			fwrite(pLine->Data, sizeof(char), Line_Size, Cache_Stream);
			fflush(Cache_Stream);

			if( Cache_bSwap && m_Type != GRID_TYPE_Bit )
			{
				for(x=0, pValue=pLine->Data; x<Get_NX(); x++, pValue+=Get_nValueBytes())
				{
					_Swap_Bytes(pValue, Get_nValueBytes());
				}
			}
		}
	}
}

//---------------------------------------------------------
void CGrid::_Cache_LineBuffer_Load(TGrid_Memory_Line *pLine, int y)
{
	int		x, Line_Pos, Line_Size;
	char	*pValue;

	if( pLine )
	{
		pLine->bModified	= false;
		pLine->y			= y;

		if( pLine->y >= 0 && pLine->y < Get_NY() )
		{
			y			= Cache_bFlip ? Get_NY() - 1 - pLine->y : pLine->y;

			Line_Size	= _LineBuffer_Get_nBytes();
			Line_Pos	= Cache_Offset + y * Line_Size;

			//-------------------------------------------------
			fseek(Cache_Stream, Line_Pos, SEEK_SET);
			fread(pLine->Data, sizeof(char), Line_Size, Cache_Stream);

			if( Cache_bSwap && m_Type != GRID_TYPE_Bit )
			{
				for(x=0, pValue=pLine->Data; x<Get_NX(); x++, pValue+=Get_nValueBytes())
				{
					_Swap_Bytes(pValue, Get_nValueBytes());
				}
			}
		}
	}
}


///////////////////////////////////////////////////////////
//														 //
//					RTL - Compression					 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CGrid::Set_Compression(bool bOn)
{
	return( bOn ? _Compr_Create() : _Compr_Destroy(true) );
}

//---------------------------------------------------------
bool CGrid::is_Compressed(void)
{
	return( m_Memory_Type == GRID_MEMORY_Compression );
}

//---------------------------------------------------------
double CGrid::Get_Compression_Ratio(void)
{
	int		y;
	long	nCompression, nNoCompression;

	if( is_Compressed() )
	{
		for(y=0, nCompression=0; y<Get_NY(); y++)
		{
			nCompression	+= *((int *)m_Values[y]);
		}

		if( (nNoCompression = Get_NCells() * Get_nValueBytes()) > 0 )
		{
			return( (double)nCompression / (double)nNoCompression );
		}
	}

	return( 1.0 );
}


///////////////////////////////////////////////////////////
//														 //
//			RTL - Compression: Create / Destroy			 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CGrid::_Compr_Create(void)
{
	TGrid_Memory_Line	Line;

	if( m_System.is_Valid() && m_Type > 0 && m_Type < GRID_TYPE_Count && m_Memory_Type == GRID_MEMORY_Normal )
	{
		m_Memory_bLock	= true;

		Line.Data		= (char *)API_Calloc(1, _Get_nLineBytes());

		if( m_Values )	// compress loaded data...
		{
			for(Line.y=0; Line.y<Get_NY() && API_Callback_Process_Set_Progress(Line.y, Get_NY()); Line.y++)
			{
				memcpy(Line.Data, m_Values[Line.y], _LineBuffer_Get_nBytes());
				Line.bModified	= true;
				_Compr_LineBuffer_Save(&Line);
			}
		}
		else			// create empty grid...
		{
			m_Values	= (void **)API_Malloc(Get_NY() * sizeof(void *));

			for(Line.y=0; Line.y<Get_NY() && API_Callback_Process_Set_Progress(Line.y, Get_NY()); Line.y++)
			{
				m_Values[Line.y]	= (void *)API_Calloc(1, _LineBuffer_Get_nBytes());
				Line.bModified	= true;
				_Compr_LineBuffer_Save(&Line);
			}
		}

		API_Free(Line.Data);

		_LineBuffer_Create();

		m_Memory_bLock	= false;
		m_Memory_Type	= GRID_MEMORY_Compression;

		API_Callback_Process_Set_Ready();
	}

	return( is_Compressed() );
}

//---------------------------------------------------------
bool CGrid::_Compr_Destroy(bool bMemory_Restore)
{
	TGrid_Memory_Line	Line;

	if( is_Valid() && m_Memory_Type == GRID_MEMORY_Compression )
	{
		m_Memory_bLock	= true;

		if( bMemory_Restore )
		{
			_LineBuffer_Flush();

			Line.Data		= (char *)API_Calloc(1, _Get_nLineBytes());

			for(int y=0; y<Get_NY() && API_Callback_Process_Set_Progress(y, Get_NY()); y++)
			{
				_Compr_LineBuffer_Load(&Line, y);
				m_Values[y]	= (void *)API_Realloc(m_Values[y], _Get_nLineBytes());
				memcpy(m_Values[y], Line.Data, _LineBuffer_Get_nBytes());
			}

			API_Free(Line.Data);

			API_Callback_Process_Set_Ready();
		}
		else
		{
			_Array_Destroy();
		}

		_LineBuffer_Destroy();

		m_Memory_bLock	= false;
		m_Memory_Type	= GRID_MEMORY_Normal;

		return( true );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//			RTL - Compression: Save / Load				 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CGrid::_Compr_LineBuffer_Save(TGrid_Memory_Line *pLine)
{
	char	*pResult, *pNoCompress, *pCompress, *pNext;
	int		x, ix, Threshold, nCompress, nNoCompress, nBytesTotal, Line_Pos;

	if( pLine && pLine->bModified )
	{
		pLine->bModified	= false;

		if( pLine->y >= 0 && pLine->y < Get_NY() )
		{
			Threshold	= 1 + (sizeof(WORD) + sizeof(bool) + Get_nValueBytes()) / Get_nValueBytes();

			nBytesTotal	= Line_Pos	= sizeof(nBytesTotal);
			pResult		= (char *)API_Malloc(nBytesTotal);
			pNoCompress	= pLine->Data;
			nNoCompress	= 0;

			//---------------------------------------------
			for(x=0; x<Get_NX(); )
			{
				nCompress	= 1;
				pCompress	= pLine->Data + x * Get_nValueBytes();
				pNext		= pCompress + Get_nValueBytes();

				for(ix=x+1; ix<Get_NX(); ix++, pNext+=Get_nValueBytes())
				{
					if( memcmp(pCompress, pNext, Get_nValueBytes()) )
					{
						break;
					}
					else
					{
						nCompress++;
					}
				}

				//-----------------------------------------
				if( nCompress <= Threshold )
				{
					nNoCompress++;
					x++;
				}
				else
				{
					nBytesTotal	+= sizeof(WORD) + sizeof(bool) + Get_nValueBytes();

					if( nNoCompress > 0 )
					{
						nBytesTotal	+= sizeof(WORD) + sizeof(bool) + nNoCompress * Get_nValueBytes();
						pResult	= (char *)API_Realloc(pResult, nBytesTotal);

						*((WORD *)(pResult + Line_Pos))	= nNoCompress;
						Line_Pos	+= sizeof(WORD);
						*((bool *)(pResult + Line_Pos))	= false;
						Line_Pos	+= sizeof(bool);

						memcpy(pResult + Line_Pos, pNoCompress, nNoCompress * Get_nValueBytes());
						Line_Pos	+= nNoCompress * Get_nValueBytes();

						nNoCompress	= 0;
					}
					else
					{
						pResult	= (char *)API_Realloc(pResult, nBytesTotal);
					}

					*((WORD *)(pResult + Line_Pos))	= nCompress;
					Line_Pos	+= sizeof(WORD);
					*((bool *)(pResult + Line_Pos))	= true;
					Line_Pos	+= sizeof(bool);

					memcpy(pResult + Line_Pos, pCompress, Get_nValueBytes());
					Line_Pos	+= Get_nValueBytes();

					pNoCompress	= pCompress + nCompress * Get_nValueBytes();
					x			+= nCompress;
				}
			}

			//---------------------------------------------
			if( nNoCompress > 0 )
			{
				nBytesTotal	+= sizeof(WORD) + sizeof(bool) + nNoCompress * Get_nValueBytes();
				pResult	= (char *)API_Realloc(pResult, nBytesTotal);

				*((WORD *)(pResult + Line_Pos))	= nNoCompress;
				Line_Pos	+= sizeof(WORD);
				*((bool *)(pResult + Line_Pos))	= false;
				Line_Pos	+= sizeof(bool);

				memcpy(pResult + Line_Pos, pNoCompress, nNoCompress * Get_nValueBytes());
			}

			//-------------------------------------------------
			memcpy(pResult, &nBytesTotal, sizeof(nBytesTotal));

			if( m_Values[pLine->y] )
			{
				API_Free(m_Values[pLine->y]);
			}

			m_Values[pLine->y]	= pResult;
		}
	}
}

//---------------------------------------------------------
void CGrid::_Compr_LineBuffer_Load(TGrid_Memory_Line *pLine, int y)
{
	bool	bCompressed;
	char	*pData, *pValue;
	int		x, iValue;
	WORD	nValues;

	if( pLine )
	{
		pLine->bModified	= false;
		pLine->y			= y;

		if( pLine->y >= 0 && pLine->y < Get_NY() )
		{
			pValue		= (char *)m_Values[y] + sizeof(int);
			pData		= pLine->Data;

			for(x=0; x<Get_NX(); )
			{
				nValues		= *((WORD *)pValue);
				pValue		+= sizeof(WORD);
				bCompressed	= *((bool *)pValue);
				pValue		+= sizeof(bool);

				if( bCompressed )
				{
					for(iValue=0; iValue<nValues && x<Get_NX(); iValue++, x++, pData+=Get_nValueBytes())
					{
						memcpy(pData, pValue, Get_nValueBytes());
					}

					pValue	+= Get_nValueBytes();
				}
				else
				{
					memcpy(pData, pValue, Get_nValueBytes() * nValues);
					x		+= nValues;
					pData	+= Get_nValueBytes() * nValues;
					pValue	+= Get_nValueBytes() * nValues;
				}
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