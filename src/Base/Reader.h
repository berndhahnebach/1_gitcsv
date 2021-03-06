/***************************************************************************
 *   Copyright (c) J�rgen Riegel          (juergen.riegel@web.de)          *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/

#ifndef __Reader_H__
#define __Reader_H__


#include <string>
#include <map>

#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>

#include "FileInfo.h"
#include "Writer.h"

class zipios::ZipInputStream;

XERCES_CPP_NAMESPACE_BEGIN
	class DefaultHandler;
XERCES_CPP_NAMESPACE_END

namespace Base
{


/** The XML reader class 
 * This is a important helper class for the store and retrivel system
 * of objects in FreeCAD. This classes a mainly inhereting the App::Persitance 
 * base class and implement the Restore() mthode.
 *  \par
 * The reader gets mainly initialized by the App::Document on retriving a 
 * document out of a file. From there supsequentli the Restore() mthode will
 * by called on all object stored. 
 *  \par
 * A simple Example is the Restore of App::PropertyString:
 *  \code
void PropertyString::Save (short indent,std::ostream &str)
{
  str << "<String value=\"" <<  _cValue.c_str() <<"\"/>" ;
}

void PropertyString::Restore(Base::Reader &reader)
{
  // read my Element
  reader.readElement("String");
  // get the value of my Attribute
  _cValue = reader.getAttribute("value");
}

 *  \endcode
 *  \par
 *  An more complicated example is the retrival of the App::PropertyContainer:
 *  \code
void PropertyContainer::Save (short indent,std::ostream &str)
{
  std::map<std::string,Property*> Map;
  getPropertyMap(Map);

  str << ind(indent) << "<Properties Count=\"" << Map.size() << "\">" << endl;
  std::map<std::string,Property*>::iterator it;
  for(it = Map.begin(); it != Map.end(); ++it)
  {
    str << ind(indent+1) << "<Property name=\"" << it->first << "\" type=\"" << it->second->getTypeId().getName() << "\">" ;    
    it->second->Save(indent+2,str);
    str << "</Property>" << endl;    
  }
  str << ind(indent) << "</Properties>" << endl;
}

void PropertyContainer::Restore(Base::Reader &reader)
{
  reader.readElement("Properties");
  int Cnt = reader.getAttributeAsInteger("Count");

  for(int i=0 ;i<Cnt ;i++)
  {
    reader.readElement("Property");
    string PropName = reader.getAttribute("name");
    Property* prop = getPropertyByName(PropName.c_str());
    if(prop)
      prop->Restore(reader);

    reader.readEndElement("Property");
  }
  reader.readEndElement("Properties");
}
 *  \endcode
 * \see App::Persistance
 * \author Juergen Riegel
 */
class BaseExport XMLReader : public XERCES_CPP_NAMESPACE_QUALIFIER DefaultHandler
{
public:
    /// opens the file and read the first element
  XMLReader(const char* FileName, std::istream&);
  ~XMLReader();

  bool isValid() const { return _valid; }

  /** @name Parser handling */
	//@{
  /// read the next element
  void read(void);
  /// get the local name of the current Element
  const char* localName(void);
  /// reads until it findes a start element (<name>) or start-end element (<name/>) (with special name if given)
  void readElement   (const char* ElementName=0);
  /// reads until it findes a end element (with special name if given)
  void readEndElement(const char* ElementName=0);
  /// reads until it findes characters
  void readCharacters(void);
	//@}

  /** @name Attribute handling */
	//@{
  /// get the numbers of attributes of the current Element
  unsigned int getAttributeCount    (void);
  /// check if the read element has a special attribute
  bool         hasAttribute (const char* AttrName);
  /// returns the named attribute as an interer (does type checking)
  long         getAttributeAsInteger(const char* AttrName);
  /// returns the named attribute as an double floating point (does type checking)
  double       getAttributeAsFloat  (const char* AttrName);
  /// returns the named attribute as an double floating point (does type checking)
  const char*  getAttribute         (const char* AttrName);
	//@}

   /** @name additional file reading */
  //@{
  /// add a read request of a Persitant object
  const char *addFile(const char* Name, Base::Persistance *Object);
  /// process the requested file writes
  void readFiles(zipios::ZipInputStream &zipstream);
  /// get all registered file names
  const std::vector<std::string>& getFilenames() const;
  //@{

private:

  // -----------------------------------------------------------------------
  //  Handlers for the SAX ContentHandler interface
  // -----------------------------------------------------------------------
  virtual void startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname, const XERCES_CPP_NAMESPACE_QUALIFIER Attributes& attrs);
  virtual void endElement  (const XMLCh* const uri, const XMLCh *const localname, const XMLCh *const qname);
  virtual void characters  (const XMLCh* const chars, const unsigned int length);
  virtual void ignorableWhitespace(const XMLCh* const chars, const unsigned int length);
  virtual void resetDocument();


  // -----------------------------------------------------------------------
  //  Handlers for the SAX ErrorHandler interface
  // -----------------------------------------------------------------------
	void warning(const XERCES_CPP_NAMESPACE_QUALIFIER SAXParseException& exc);
  void error(const XERCES_CPP_NAMESPACE_QUALIFIER SAXParseException& exc);
  void fatalError(const XERCES_CPP_NAMESPACE_QUALIFIER SAXParseException& exc);
  void resetErrors();


  std::string LocalName;
  std::string Characters;
  unsigned int CharacterCount;

  std::map<std::string,std::string> AttrMap;
  typedef std::map<std::string,std::string> AttrMapType;

  enum {
    None = 0,
    Chars,
    StartElement,
    StartEndElement,
    EndElement
  } ReadType;


  FileInfo _File;
  XERCES_CPP_NAMESPACE_QUALIFIER SAX2XMLReader* parser;
  XERCES_CPP_NAMESPACE_QUALIFIER XMLPScanToken token;
  bool _valid;

  struct FileEntry {
    std::string FileName;
    Base::Persistance *Object;
  };
  std::vector<FileEntry> FileList;
  std::vector<std::string> FileNames;

};

/**
 * Extended XML reader class the collects also the attached data files in a zip file.
 * @author Werner Mayer
 */
 /*
class BaseExport XMLZipReader : public XMLReader
{
public:
    /// opens the file and read the first element
  XMLZipReader(const char* FileName, Reader& reader)
    : XMLReader(FileName, reader), _reader(reader)
  {
  }

  ~XMLZipReader()
  {
  }

  void addFile(const char* FileName, const char* FeatName)
  {
    _reader.addFile(FileName,FeatName);
  }

private:
  Reader& _reader;
};
*/
}


#endif
