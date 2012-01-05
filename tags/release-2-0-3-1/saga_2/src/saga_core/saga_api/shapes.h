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
//                       shapes.h                        //
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
#ifndef HEADER_INCLUDED__SAGA_API__shapes_H
#define HEADER_INCLUDED__SAGA_API__shapes_H


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "table.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
typedef enum ESG_Shape_Type
{
	SHAPE_TYPE_Undefined		= 0,
	SHAPE_TYPE_Point,
	SHAPE_TYPE_Points,
	SHAPE_TYPE_Line,
	SHAPE_TYPE_Polygon
}
TSG_Shape_Type;

//---------------------------------------------------------
SAGA_API_DLL_EXPORT const SG_Char *	SG_Get_ShapeType_Name	(TSG_Shape_Type Type);


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Shape
{
	friend class CSG_Shapes;

public:

	//-----------------------------------------------------
	virtual void				Destroy				(void);

	bool						Assign				(CSG_Shape *pShape, bool bAssign_Attributes = true);

	TSG_Shape_Type				Get_Type			(void);

	virtual bool				is_Valid			(void)											= 0;

	bool						is_Selected			(void)		{	return( m_pRecord->is_Selected() );	}

	//-----------------------------------------------------
	CSG_Table_Record *			Get_Record			(void)		{	return( m_pRecord );	}

	bool						Set_Value			(int           iField, const SG_Char *Value)		{	return( m_pRecord->Set_Value (iField, Value) );		}
	bool						Set_Value			(const SG_Char *Field, const SG_Char *Value)		{	return( m_pRecord->Set_Value ( Field, Value) );		}
	bool						Set_Value			(int           iField, double         Value)		{	return( m_pRecord->Set_Value (iField, Value) );		}
	bool						Set_Value			(const SG_Char *Field, double         Value)		{	return( m_pRecord->Set_Value ( Field, Value) );		}
	bool						Add_Value			(int           iField, double         Value)		{	return( m_pRecord->Add_Value (iField, Value) );		}
	bool						Add_Value			(const SG_Char *Field, double         Value)		{	return( m_pRecord->Add_Value ( Field, Value) );		}
	bool						Mul_Value			(int           iField, double         Value)		{	return( m_pRecord->Mul_Value (iField, Value) );		}
	bool						Mul_Value			(const SG_Char *Field, double         Value)		{	return( m_pRecord->Mul_Value ( Field, Value) );		}

	bool						Set_NoData			(int           iField)								{	return( m_pRecord->Set_NoData(iField) );			}
	bool						Set_NoData			(const SG_Char *Field)								{	return( m_pRecord->Set_NoData( Field) );			}
	bool						is_NoData			(int           iField)	const						{	return( m_pRecord->is_NoData (iField) );			}
	bool						is_NoData			(const SG_Char *Field)	const						{	return( m_pRecord->is_NoData ( Field) );			}

	const SG_Char *				asString			(int           iField, int Decimals = -1)	const	{	return( m_pRecord->asString  (iField, Decimals) );	}
	const SG_Char *				asString			(const SG_Char *Field, int Decimals = -1)	const	{	return( m_pRecord->asString  ( Field, Decimals) );	}
	SG_Char						asChar				(int           iField)	const						{	return( m_pRecord->asChar    (iField) );			}
	SG_Char						asChar				(const SG_Char *Field)	const						{	return( m_pRecord->asChar    ( Field) );			}
	short						asShort				(int           iField)	const						{	return( m_pRecord->asShort   (iField) );			}
	short						asShort				(const SG_Char *Field)	const						{	return( m_pRecord->asShort   ( Field) );			}
	int							asInt				(int           iField)	const						{	return( m_pRecord->asInt     (iField) );			}
	int							asInt				(const SG_Char *Field)	const						{	return( m_pRecord->asInt     ( Field) );			}
	float						asFloat				(int           iField)	const						{	return( m_pRecord->asFloat   (iField) );			}
	float						asFloat				(const SG_Char *Field)	const						{	return( m_pRecord->asFloat   ( Field) );			}
	double						asDouble			(int           iField)	const						{	return( m_pRecord->asDouble  (iField) );			}
	double						asDouble			(const SG_Char *Field)	const						{	return( m_pRecord->asDouble  ( Field) );			}

	//-----------------------------------------------------
	virtual int					Add_Point			(double x, double y,             int iPart = 0)	= 0;
	virtual int					Ins_Point			(double x, double y, int iPoint, int iPart = 0)	= 0;
	virtual int					Set_Point			(double x, double y, int iPoint, int iPart = 0)	= 0;
	virtual int					Del_Point			(                    int iPoint, int iPart = 0)	= 0;

	int							Add_Point			(TSG_Point Point,                int iPart = 0);
	int							Ins_Point			(TSG_Point Point,    int iPoint, int iPart = 0);
	int							Set_Point			(TSG_Point Point,    int iPoint, int iPart = 0);

	virtual int					Del_Part			(int iPart)										= 0;
	virtual int					Del_Parts			(void)											= 0;

	virtual int					Get_Part_Count		(void)											= 0;
	int							Get_Point_Count		(void);
	virtual int					Get_Point_Count		(int iPart)										= 0;
	virtual TSG_Point			Get_Point			(int iPoint, int iPart = 0)						= 0;


	//-----------------------------------------------------
	virtual const CSG_Rect &	Get_Extent			(void)											= 0;
	virtual const CSG_Rect &	Get_Extent			(int iPart)	{	return( Get_Extent() );		}

	int							Intersects			(TSG_Rect Extent);

	virtual double				Get_Distance		(TSG_Point Point)								= 0;
	virtual double				Get_Distance		(TSG_Point Point, int iPart)					= 0;
	virtual double				Get_Distance		(TSG_Point Point, TSG_Point &Next)				= 0;
	virtual double				Get_Distance		(TSG_Point Point, TSG_Point &Next, int iPart)	= 0;


protected:

	CSG_Table_Record			*m_pRecord;

	class CSG_Shapes			*m_pOwner;


	CSG_Shape(class CSG_Shapes *pOwner, CSG_Table_Record *pRecord);
	virtual ~CSG_Shape(void);

	virtual bool				On_Assign			(CSG_Shape *pShape)								= 0;
	virtual int					On_Intersects		(TSG_Rect Extent)								= 0;

	virtual void				_Invalidate			(void);

};


///////////////////////////////////////////////////////////
//														 //
//						Point							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Shape_Point : public CSG_Shape
{
	friend class CSG_Shapes;

public:

	virtual bool				is_Valid			(void)												{	return( true );				}

	virtual int					Add_Point			(double x, double y,             int iPart = 0);
	virtual int					Ins_Point			(double x, double y, int iPoint, int iPart = 0)		{	return( Add_Point(x, y) );	}
	virtual int					Set_Point			(double x, double y, int iPoint, int iPart = 0)		{	return( Add_Point(x, y) );	}
	virtual int					Del_Point			(                    int iPoint, int iPart = 0)		{	return( -1 );				}

	virtual int					Del_Part			(int iPart)											{	return( -1 );				}
	virtual int					Del_Parts			(void)												{	return( -1 );				}

	virtual int					Get_Part_Count		(void)												{	return( 1 );				}
	virtual int					Get_Point_Count		(int iPart)											{	return( 1 );				}
	virtual TSG_Point			Get_Point			(int iPoint, int iPart = 0)							{	return( m_Point );			}

	virtual const CSG_Rect &	Get_Extent			(void);

	virtual double				Get_Distance		(TSG_Point Point)									{	return( SG_Get_Distance(Point, m_Point) );	}
	virtual double				Get_Distance		(TSG_Point Point, int iPart)						{	return( SG_Get_Distance(Point, m_Point) );	}
	virtual double				Get_Distance		(TSG_Point Point, TSG_Point &Next)					{	Next = m_Point; return( SG_Get_Distance(Point, m_Point) );	}
	virtual double				Get_Distance		(TSG_Point Point, TSG_Point &Next, int iPart)		{	Next = m_Point; return( SG_Get_Distance(Point, m_Point) );	}


protected:

	CSG_Shape_Point(class CSG_Shapes *pOwner, CSG_Table_Record *pRecord);
	virtual ~CSG_Shape_Point(void);


	TSG_Point					m_Point;


	virtual bool				On_Assign			(CSG_Shape *pShape);
	virtual int					On_Intersects		(TSG_Rect Region);

};


///////////////////////////////////////////////////////////
//														 //
//						Points							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Shape_Part
{
	friend class CSG_Shape_Points;
	friend class CSG_Shape_Line;
	friend class CSG_Shape_Polygon;

public:

	bool						Destroy				(void);

	bool						Assign				(CSG_Shape_Part *pPart);

	const CSG_Rect &			Get_Extent			(void)	{	_Update_Extent();	return( m_Extent );	}

	int							Get_Count			(void)	{	return( m_nPoints );	}

	TSG_Point					Get_Point			(int iPoint)
	{
		if( iPoint >= 0 && iPoint < m_nPoints )
		{
			return( m_Points[iPoint] );
		}

		return( CSG_Point(0.0, 0.0) );
	}

	int							Add_Point			(TSG_Point Point               )	{	return( Add_Point(Point.x, Point.y)         );	}
	int							Ins_Point			(TSG_Point Point,    int iPoint)	{	return( Ins_Point(Point.x, Point.y, iPoint) );	}
	int							Set_Point			(TSG_Point Point,    int iPoint)	{	return( Set_Point(Point.x, Point.y, iPoint) );	}

	int							Add_Point			(double x, double y            );
	int							Ins_Point			(double x, double y, int iPoint);
	int							Set_Point			(double x, double y, int iPoint);
	int							Del_Point			(                    int iPoint);


protected:

	CSG_Shape_Part(class CSG_Shape_Points *pOwner);
	virtual ~CSG_Shape_Part(void);


	bool						m_bUpdate;

	int							m_nPoints, m_nBuffer;

	TSG_Point					*m_Points;

	CSG_Rect					m_Extent;

	CSG_Shape_Points			*m_pOwner;


	bool						_Alloc_Memory		(int nPoints);

	virtual void				_Invalidate			(void);

	virtual void				_Update_Extent		(void);

};


///////////////////////////////////////////////////////////
//														 //
//						Points							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Shape_Points : public CSG_Shape
{
	friend class CSG_Shapes;
	friend class CSG_Shape_Part;

public:

	virtual void				Destroy				(void);

	virtual bool				is_Valid			(void)		{	return( m_nParts > 0 && m_pParts[0]->Get_Count() > 0 );	}

	virtual int					Add_Point			(double x, double y,             int iPart = 0);
	virtual int					Ins_Point			(double x, double y, int iPoint, int iPart = 0);
	virtual int					Set_Point			(double x, double y, int iPoint, int iPart = 0);
	virtual int					Del_Point			(                    int iPoint, int iPart = 0);

	virtual int					Del_Part			(int iPart);
	virtual int					Del_Parts			(void);

	virtual int					Get_Part_Count		(void)		{	return( m_nParts );		}
	virtual CSG_Shape_Part *	Get_Part			(int iPart)	{	return( iPart >= 0 && iPart < m_nParts ? m_pParts[iPart] : NULL );	}
	virtual int					Get_Point_Count		(int iPart)	{	return( iPart >= 0 && iPart < m_nParts ? m_pParts[iPart]->Get_Count() : 0 );	}

	virtual TSG_Point			Get_Point			(int iPoint, int iPart = 0)
	{
		if( iPart >= 0 && iPart < m_nParts && iPoint >= 0 && iPoint < m_pParts[iPart]->Get_Count() )
		{
			return( m_pParts[iPart]->Get_Point(iPoint) );
		}

		return( CSG_Point(0.0, 0.0) );
	}

	virtual const CSG_Rect &	Get_Extent			(void)		{	_Update_Extent();	return( m_Extent );	}

	virtual double				Get_Distance		(TSG_Point Point);
	virtual double				Get_Distance		(TSG_Point Point, int iPart);
	virtual double				Get_Distance		(TSG_Point Point, TSG_Point &Next);
	virtual double				Get_Distance		(TSG_Point Point, TSG_Point &Next, int iPart);


protected:

	CSG_Shape_Points(class CSG_Shapes *pOwner, CSG_Table_Record *pRecord);
	virtual ~CSG_Shape_Points(void);


	bool						m_bUpdate;

	int							m_nParts;

	CSG_Rect					m_Extent;

	CSG_Shape_Part				**m_pParts;


	int							_Add_Part			(void);

	virtual CSG_Shape_Part *	_Get_Part			(void)	{	return( new CSG_Shape_Part(this) );	}

	virtual void				_Invalidate			(void)
	{
		if( !m_bUpdate )
		{
			m_bUpdate	= true;

			CSG_Shape::_Invalidate();
		}
	}

	void						_Update_Extent		(void);

	virtual bool				On_Assign			(CSG_Shape *pShape);

	virtual int					On_Intersects		(TSG_Rect Region);

};


///////////////////////////////////////////////////////////
//														 //
//						Line							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Shape_Line : public CSG_Shape_Points
{
	friend class CSG_Shapes;

public:

	virtual bool				is_Valid			(void)	{	return( m_nParts > 0 && m_pParts[0]->Get_Count() > 1 );	}

	double						Get_Length			(void);
	double						Get_Length			(int iPart);

	virtual double				Get_Distance		(TSG_Point Point, TSG_Point &Next, int iPart);


protected:

	CSG_Shape_Line(class CSG_Shapes *pOwner, CSG_Table_Record *pRecord);
	virtual ~CSG_Shape_Line(void);

	virtual int					On_Intersects		(TSG_Rect Region);

};


///////////////////////////////////////////////////////////
//														 //
//						Polygon							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Shape_Polygon_Part : public CSG_Shape_Part
{
	friend class CSG_Shape_Polygon;

public:

	bool						is_Clockwise		(void)	{	_Update_Area();	return( m_bClockwise == 1 );	}

	double						Get_Perimeter		(void)	{	_Update_Area();	return( m_Perimeter );	}

	double						Get_Area			(void)	{	_Update_Area();	return( m_Area );	}

	const TSG_Point &			Get_Centroid		(void)	{	_Update_Area();	return( m_Centroid );	}

	bool						is_Containing		(const TSG_Point &Point);
	bool						is_Containing		(double x, double y);

	double						Get_Distance		(TSG_Point Point, TSG_Point &Next);


protected:

	CSG_Shape_Polygon_Part(class CSG_Shape_Points *pOwner);
	virtual ~CSG_Shape_Polygon_Part(void);


	int							m_bClockwise, m_bLake;

	double						m_Area, m_Perimeter;

	TSG_Point					m_Centroid;


	virtual void				_Invalidate			(void);

	void						_Update_Area		(void);

};

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Shape_Polygon : public CSG_Shape_Points
{
	friend class CSG_Shapes;

public:

	virtual bool				is_Valid			(void)		{	return( m_nParts > 0 && m_pParts[0]->Get_Count() > 2 );	}


	CSG_Shape_Polygon_Part *	Get_Polygon_Part	(int iPart)	{	return( (CSG_Shape_Polygon_Part *)Get_Part(iPart) );	}

	bool						is_Lake				(int iPart);

	bool						is_Clockwise		(int iPart);

	double						Get_Perimeter		(int iPart);
	double						Get_Perimeter		(void);

	double						Get_Area			(int iPart);
	double						Get_Area			(void);

	TSG_Point					Get_Centroid		(int iPart);
	TSG_Point					Get_Centroid		(void);

	bool						is_Containing		(const TSG_Point &Point, int iPart);
	bool						is_Containing		(const TSG_Point &Point);
	bool						is_Containing		(double x, double y, int iPart);
	bool						is_Containing		(double x, double y);

	virtual double				Get_Distance		(TSG_Point Point, TSG_Point &Next, int iPart);


protected:

	CSG_Shape_Polygon(class CSG_Shapes *pOwner, CSG_Table_Record *pRecord);
	virtual ~CSG_Shape_Polygon(void);


	int							m_bUpdate_Lakes;


	virtual CSG_Shape_Part *	_Get_Part			(void)	{	return( new CSG_Shape_Polygon_Part(this) );	}

	virtual void				_Invalidate			(void);

	virtual int					On_Intersects		(TSG_Rect Region);

};


///////////////////////////////////////////////////////////
//														 //
//						Shapes							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Shapes : public CSG_Data_Object
{
	friend class CSG_Shape;

public:

	CSG_Shapes(void);

								CSG_Shapes	(const CSG_Shapes &Shapes);
	bool						Create		(const CSG_Shapes &Shapes);

								CSG_Shapes	(const SG_Char *File_Name);
	bool						Create		(const SG_Char *File_Name);

								CSG_Shapes	(TSG_Shape_Type Type, const SG_Char *Name = NULL, CSG_Table *pStructure = NULL);
	bool						Create		(TSG_Shape_Type Type, const SG_Char *Name = NULL, CSG_Table *pStructure = NULL);

	virtual ~CSG_Shapes(void);

	virtual bool				Destroy					(void);

	virtual TSG_Data_Object_Type	Get_ObjectType		(void)	const		{	return( DATAOBJECT_TYPE_Shapes );	}

	virtual bool				Assign					(CSG_Data_Object *pObject);

	virtual bool				Save					(const SG_Char *File_Name, int Format = 0);

	virtual bool				is_Valid				(void)	const		{	return( m_Type != SHAPE_TYPE_Undefined && m_nShapes >= 0 );				}

	TSG_Shape_Type				Get_Type				(void)	const		{	return( m_Type );		}

	CSG_Table &					Get_Table				(void)				{	return( m_Table );		}

	const CSG_Rect &			Get_Extent				(void)				{	_Update_Extent();	return( m_Extent );	}

	//-----------------------------------------------------
	CSG_Shape *					Add_Shape				(void);
	CSG_Shape *					Add_Shape				(CSG_Table_Record *pValues);
	CSG_Shape *					Add_Shape				(CSG_Shape *pShape, bool bCopyAttributes = false);
	bool						Del_Shape				(int iShape);
	bool						Del_Shape				(CSG_Shape *pShape);
	bool						Del_Shapes				(void);

	int							Get_Count				(void)				const	{	return( m_nShapes );	}
	CSG_Shape *					Get_Shape				(int iShape)		const	{	return( iShape >= 0 && iShape < m_nShapes ? m_Shapes[iShape] : NULL );	}
	int							Get_Shape_Index			(CSG_Shape *pShape)	const	{	return( pShape ? pShape->Get_Record()->Get_Index() : -1 );	}

	CSG_Shape *					Get_Shape				(TSG_Point Point, double Epsilon = 0.0);


	//-----------------------------------------------------
	bool						Set_Index				(int Field_1, TSG_Table_Index_Order Order_1, int Field_2 = -1, TSG_Table_Index_Order Order_2 = TABLE_INDEX_None, int Field_3 = -1, TSG_Table_Index_Order Order_3 = TABLE_INDEX_None);

	bool						Del_Index				(void)				{	return( m_Table.Del_Index() );			}
	bool						Toggle_Index			(int iField)		{	return( m_Table.Toggle_Index(iField) );	}
	bool						is_Indexed				(void)	const		{	return( m_Table.is_Indexed() );			}

	int							Get_Index_Field			(int i)	const		{	return( m_Table.Get_Index_Field(i) );	}
	TSG_Table_Index_Order		Get_Index_Order			(int i)	const		{	return( m_Table.Get_Index_Order(i) );	}

	CSG_Shape *					Get_Shape_byIndex		(int Index)			const
	{
		if( Index >= 0 && Index < m_nShapes )
		{
			if( m_Table.m_Index != NULL )
			{
				return( m_Shapes[m_Table.m_Index[Index]] );
			}

			return( m_Shapes[Index] );
		}

		return( NULL );
	}

	//-----------------------------------------------------
	int							Get_Selection_Count		(void)				{	return( m_Table.m_nSelected );	}
	CSG_Shape *					Get_Selection			(int Index = 0);
	const CSG_Rect &			Get_Selection_Extent	(void);

	bool						Select					(TSG_Rect Extent         , bool bAdd = false);
	bool						Select					(int iShape              , bool bAdd = false);
	bool						Select					(CSG_Shape *pShape = NULL, bool bAdd = false);

	int							Del_Selection			(void);
	int							Inv_Selection			(void);


protected:

	bool						m_bUpdate;

	int							m_nShapes, m_nBuffer;

	TSG_Shape_Type				m_Type;

	CSG_Rect					m_Extent, m_Extent_Selected;

	CSG_Table					m_Table;

	CSG_Shape					**m_Shapes;


	void						_On_Construction		(void);

	void						_Invalidate				(void)				{	m_bUpdate	= true;	}
	void						_Update_Extent			(void);

	bool						_Inc_Array				(void);
	bool						_Dec_Array				(void);

	CSG_Shape *					_Add_Shape				(CSG_Table_Record *pRecord);

	bool						_Load_ESRI				(const SG_Char *File_Name);
	bool						_Save_ESRI				(const SG_Char *File_Name);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
/** Safe shapes construction */
SAGA_API_DLL_EXPORT CSG_Shapes *	SG_Create_Shapes	(void);

/** Safe shapes construction */
SAGA_API_DLL_EXPORT CSG_Shapes *	SG_Create_Shapes	(const CSG_Shapes &Shapes);

/** Safe shapes construction */
SAGA_API_DLL_EXPORT CSG_Shapes *	SG_Create_Shapes	(const SG_Char *FileName);

/** Safe shapes construction */
SAGA_API_DLL_EXPORT CSG_Shapes *	SG_Create_Shapes	(TSG_Shape_Type Type, const SG_Char *Name = NULL, CSG_Table *pStructure = NULL);


///////////////////////////////////////////////////////////
//														 //
//					Search Engine						 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Shapes_Search
{
public:
	CSG_Shapes_Search(void);
	CSG_Shapes_Search(CSG_Shapes *pPoints);

	virtual ~CSG_Shapes_Search(void);

	bool						Create				(CSG_Shapes *pPoints);
	void						Destroy				(void);

	bool						is_Valid			(void)	{	return( m_nPoints > 0 );	}

	CSG_Shape *					Get_Point_Nearest	(double x, double y);
	CSG_Shape *					Get_Point_Nearest	(double x, double y, int iQuadrant);

	int							Select_Radius		(double x, double y, double Radius, bool bSort = false, int MaxPoints = -1);
	int							Get_Selected_Count	(void)	{	return( m_nSelected );		}

	CSG_Shape *					Get_Selected_Point	(int iSelected)
	{
		if( iSelected >= 0 && iSelected < m_nSelected )
		{
			return( m_nSelected == m_Selected_Idx.Get_Count()
				? m_Selected[m_Selected_Idx[iSelected]]
				: m_Selected               [iSelected]
			);
		}

		return( NULL );
	}


protected:

	bool						m_bDestroy;

	int							m_nPoints, m_nSelected, m_Selected_Buf;

	double						*m_Selected_Dst;

	TSG_Point					*m_Pos;

	CSG_Index					m_Idx, m_Selected_Idx;

	CSG_Shape					**m_Selected;

	CSG_Shapes					*m_pPoints;


	void						_On_Construction	(void);

	int							_Get_Index_Next		(double Position);
	int							_Get_Point_Nearest	(double x, double y, int iQuadrant);
	void						_Select_Add			(CSG_Shape *pPoint, double Distance);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__SAGA_API__shapes_H