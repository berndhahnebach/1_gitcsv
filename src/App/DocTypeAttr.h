/** \file $RCSfile$
 *  \brief The attribute module
 *  \author $Author$
 *  \version $Revision$
 *  \date    $Date$
 */


/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *   for detail see the LICENCE text file.                                 *
 *   J�rgen Riegel 2002                                                    *
 *                                                                         *
 ***************************************************************************/
 


#ifndef _Parameter_h_
#define _Parameter_h_

#include "Attribute.h"


class Standard_Transient;
class Handle_Standard_Type;
class Handle(TDF_Attribute);
class TDataStd_Name;
class FCDocType;


Standard_EXPORT Handle_Standard_Type& STANDARD_TYPE(TDataStd_Name);



class FCDocTypeAttr;


/** Handle class for FCDocTypeAttr
 */
class Handle_FCDocTypeAttr :public Handle_FCAttribute
{
public:
	AppExport void* operator new(size_t,void* anAddress){return anAddress;}
	AppExport void* operator new(size_t size){return Standard::Allocate(size);}
	AppExport void  operator delete(void *anAddress){if (anAddress) Standard::Free((Standard_Address&)anAddress);}
	AppExport Handle_FCDocTypeAttr():Handle(FCAttribute)() {} 
	AppExport Handle_FCDocTypeAttr(const Handle(FCDocTypeAttr)& aHandle) : Handle(FCAttribute)(aHandle){}

	AppExport Handle_FCDocTypeAttr(const FCDocTypeAttr* anItem) : Handle(FCAttribute)((FCAttribute *)anItem){}

	AppExport Handle_FCDocTypeAttr& operator=(const Handle(FCDocTypeAttr)& aHandle)
	{
		Assign(&(*aHandle));
		return *this;
	}

	AppExport Handle_FCDocTypeAttr& operator=(const FCDocTypeAttr* anItem)
	{
		Assign((Standard_Transient *)anItem);
		return *this;
	}

	AppExport FCDocTypeAttr* operator->() 
	{
		return (FCDocTypeAttr *)(ControlAccess());
	}

	AppExport FCDocTypeAttr* operator->() const{return(FCDocTypeAttr *)ControlAccess();}
	AppExport ~Handle_FCDocTypeAttr();
	AppExport static const Handle_FCDocTypeAttr DownCast(const Handle(Standard_Transient)& AnObject);
};



class FCDocTypeAttr : public FCAttribute 
{
public:

	/// Constructor
	AppExport FCDocTypeAttr(FCDocType *pcDocType = 0);
	/// Destructor
	AppExport ~FCDocTypeAttr();

	/// Delivers the type object of the Object
	AppExport FCDocType *GetType(void);

	/// Delivers the GUID of the Object
	AppExport static const Standard_GUID& GetID() ;


	/// Saving to a stream
	AppExport virtual  Standard_OStream& Dump(Standard_OStream& anOS) const;

	/// copy
	AppExport void Restore(const Handle(TDF_Attribute)& with) ;

	/// Set data
	AppExport void Set(FCDocType *pcDocType) ;
	/// Get data
	AppExport FCDocType *Get() const;

	/// not shure
	AppExport static  Handle_FCDocTypeAttr Set(const TDF_Label& label,FCDocType *pcDocType) ;

	/// Get the unique ID of the Attribute
	AppExport const Standard_GUID& ID() const;

	/// Get a empty instance
	AppExport Handle_TDF_Attribute NewEmpty() const;

	/// some kind of pasting ???
	AppExport void Paste(const Handle(TDF_Attribute)& into,const Handle(TDF_RelocationTable)& RT) const;


	/// needet for CasCade handles 
    AppExport void* operator new(size_t,void* anAddress)	{return anAddress;}
    AppExport void* operator new(size_t size)				{return Standard::Allocate(size);}
    AppExport void  operator delete(void *anAddress)		{if (anAddress) Standard::Free((Standard_Address&)anAddress);}
	AppExport friend Handle_Standard_Type& FCDocTypeAttr_Type_();
	AppExport const Handle(Standard_Type)& DynamicType() const;
	AppExport Standard_Boolean	       IsKind(const Handle(Standard_Type)&) const;

private: 

	/// Data member string
	FCDocType *_pcDocType;

};





#endif