/***************************************************************************
 *   Copyright (c) J�rgen Riegel          (juergen.riegel@web.de) 2002     *
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


#include "PreCompiled.h"

#ifndef _PreComp_
#endif


#include "Document.h"
#include "DocumentPy.h"
#include "Application.h"
#include "Feature.h"

#include <Base/PyExport.h>
#include <Base/Console.h>
#include <Base/Exception.h>
#include <Base/FileInfo.h>
#include <Base/Interpreter.h>
#include <Base/Reader.h>
#include <Base/Writer.h>

#include <Base/zipios/zipios-config.h>
#include <Base/zipios/zipfile.h>
#include <Base/zipios/zipinputstream.h>
#include <Base/zipios/zipoutputstream.h>
#include <Base/zipios/meta-iostreams.h>

#include "Application.h"

using Base::Console;
using Base::streq;
using Base::Writer;
using namespace App;
using namespace std;

using namespace zipios ;


PROPERTY_SOURCE(App::Document, App::PropertyContainer)

//--------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------
Document::Document(void)
: pActiveFeature(0)
{
  // Remark: In a constructor we should never increment a Python object as we cannot be sure
  // if the Python interpreter gets a reference of it. E.g. if we increment but Python don't 
  // get a reference then the object wouldn't get deleted in the destructor.
  // So, we must increment only if the interpreter gets a reference.
	_pcDocPy = new DocumentPy(this);

  Console().Log("+App::Document: %p\n",this,_pcDocPy);


  ADD_PROPERTY(Name,("Unnamed"));
  ADD_PROPERTY(FileName,(""));


}

Document::~Document()
{
  Console().Log("-App::Document: %s %p\n",getName(), this);

  std::map<std::string,FeatEntry>::iterator it;


  Console().Log("-Delete Features of %s \n",getName());

  for(it = FeatMap.begin(); it != FeatMap.end(); ++it)
  {
    delete(it->second.F);
  }

  _pcDocPy->DecRef(); // decrement by one
}




//--------------------------------------------------------------------------
// Exported functions
//--------------------------------------------------------------------------

void Document::Save (Writer &writer)
{
  writer << "<?xml version='1.0' encoding='utf-8'?>" << endl
      << "<!--" << endl
      << " FreeCAD Document, see http://free-cad.sourceforge.net for more informations..." << endl
      << "-->" << endl;

  writer << "<Document SchemaVersion=\"1\">" << endl;

  PropertyContainer::Save(writer);

  // writing the features types
  writer << writer.ind() << "<Features Count=\"" << FeatMap.size() <<"\">" << endl;

  std::map<std::string,FeatEntry>::iterator it;
  for(it = FeatMap.begin(); it != FeatMap.end(); ++it)
  {
    Feature* feat = it->second.F;
    writer << writer.ind() << "<Feature " 
                         << "type=\"" << feat->getTypeId().getName() << "\" "
                         << "name=\"" << feat->getName()             << "\" "
//                         << "typeOld=\"" << feat->type()             << "\" "
                         << "/>" << endl;    
  }

  writer << writer.ind() << "</Features>" << endl;
  
  // writing the features itself
  writer << writer.ind() << "<FeatureData Count=\"" << FeatMap.size() <<"\">" << endl;

  for(it = FeatMap.begin(); it != FeatMap.end(); ++it)
  {
    Feature* feat = it->second.F;
    writer << writer.ind() << "<Feature name=\"" << feat->getName() << "\">" << endl;   
    writer.unsetFilenames();
    feat->Save(writer);
    const std::vector<std::string>& fn = writer.getFilenames();
    writer << writer.ind() << "<Files Count=\"" << fn.size() <<"\">" << endl;
    for ( std::vector<std::string>::const_iterator iF = fn.begin(); iF != fn.end(); ++iF)
    {
      writer << writer.ind() << "<File name=\"" << (*iF) << "\" ></File>" << endl;
    }
    writer << writer.ind() << "</Files>" << endl;
    writer << writer.ind() << "</Feature>" << endl;
  }

  writer << writer.ind() << "</FeatureData>" << endl;
  writer << "</Document>" << endl;

}

void Document::Restore(Base::XMLReader &reader)
{
  int i,Cnt;

  reader.readElement("Document");

  // read the Document Properties
  PropertyContainer::Restore(reader);

  // read the feature types
  reader.readElement("Features");
  Cnt = reader.getAttributeAsInteger("Count");
  for(i=0 ;i<Cnt ;i++)
  {
    reader.readElement("Feature");
    string type = reader.getAttribute("type");
    string name = reader.getAttribute("name");

    addFeature(type.c_str(),name.c_str());
  }
  reader.readEndElement("Features");

  // read the features itself
  reader.readElement("FeatureData");
  Cnt = reader.getAttributeAsInteger("Count");
  for(i=0 ;i<Cnt ;i++)
  {
    reader.readElement("Feature");
    string name = reader.getAttribute("name");
    Feature* pFeat = getFeature(name.c_str());
    if(pFeat) // check if this feature has been registered
    {
      //FIXME: We must save/restore that state of a feature
      pFeat->Restore(reader);

      // restore the attached files to this feature
      reader.readElement("Files");
      int ctFiles = reader.getAttributeAsInteger("Count");
      for ( int iFiles = 0; iFiles < ctFiles; ++iFiles )
      {
        reader.readElement("File");
        string fn = reader.getAttribute("name");
        reinterpret_cast<Base::XMLZipReader&>(reader).addFile(fn.c_str(), name.c_str());
      }
      reader.readEndElement("Files");
    }
    reader.readEndElement("Feature");
  }
  reader.readEndElement("FeatureData");

  reader.readEndElement("Document");
}

// Save the Document under a new Name
void Document::saveAs (const char* name)
{
  Base::FileInfo File(name);

  //Base::ogzstream file(File.filePath().c_str());
  //ofstream file(File.filePath().c_str());

  std::string oldName = Name.getValue();
  Name.setValue(File.fileNamePure());
  FileName.setValue(File.filePath());

  Document::save();

  GetApplication().renameDocument(oldName.c_str(), Name.getValue());

  DocChanges DocChange;
  DocChange.Why = DocChanges::Rename;

  Notify(DocChange);
}

// Save the document under the name its been opened
bool Document::save (void)
{
  int compression = App::GetApplication().GetParameterGroupByPath("User parameter:BaseApp/Preferences/Document")->GetInt("CompressionLevel",3);

  if(*(FileName.getValue()) != '\0')
  {
    Base::Writer file(FileName.getValue());
    file.setComment("FreeCAD Document");
    file.setLevel( compression );

    file.putNextEntry("Document.xml");

    Document::Save(file);

    for ( Base::Writer::ConstIterator it = file.begin(); it != file.end(); ++it )
    {
      file.putNextEntry(it->FileName);
      it->Object->SaveDocFile( file );
    }

    file.close();

    return true;
  }

  return false;
}

// Open the document
bool Document::open (void)
{
  Base::Reader file(FileName.getValue());

  Base::XMLZipReader reader(FileName.getValue(), file);
  if ( reader.isValid() )
  {
    Document::Restore(reader);

    for ( Base::Reader::ConstIterator it = file.begin(); it != file.end(); ++it )
    {
      ConstEntryPointer entry = file.getNextEntry();
      if ( entry->isValid() && entry->getName() == it->FileName )
      {
        Feature* feat = getFeature( it->FeatName.c_str() );
        if ( feat )
        {
          try {
            feat->RestoreDocFile( file );
            feat->_eStatus = Feature::Valid;
            feat->touchTime.setToActual();
          } catch ( const Base::Exception& e) {
            feat->setError( e.what() );
            Base::Console().Log( e.what() );
          }
        }
      }
    }

    file.close();

    // notify all as new
    DocChanges DocChange;
    for(std::map<std::string,FeatEntry>::iterator It = FeatMap.begin();It != FeatMap.end();++It)
      DocChange.NewFeatures.insert(It->second.F);
    Notify(DocChange);

    return true;
  }

  file.close();
  return false;
}

bool Document::isSaved() const
{
  std::string name = FileName.getValue();
	return !name.empty();
}

/// Get the document name of a saved document
const char* Document::getName() const
{
  return Name.getValue();
}

/// Get the path of a saved document
const char* Document::getPath() const
{
  return FileName.getValue();//_hDoc->GetPath().ToExtString();
}

bool Document::Undo(void)					
{
  return false;//_hDoc->Undo() != 0; 
}

bool Document::Redo(void)
{
  return false;//_hDoc->Redo() != 0; 
}


/*
/// Get the Main Label of the document
FCPyHandle<FCLabel> Document::Main() 
{
	if(!_hcMain){
		_hcMain = new FCLabel(_hDoc->Main(),this);
		//_hcMain->IncRef();
		mcLabelMap[_hDoc->Main()] = _pcMain;
	}
	return  FCPyHandle<FCLabel>(_pcMain);
}

/// Test if the document is empty
bool Document::IsEmpty() const
{
  return false;//_hDoc->IsEmpty() != 0; 
}

/// Returns False if the  document  contains notified modifications.
bool Document::IsValid() const
{
  return true;//_hDoc->IsValid() != 0;
}
*/

/// Remove all modifications. After this call The document becomesagain Valid.
void Document::PurgeModified()
{
  //_hDoc->PurgeModified(); 
}

/// New Command (Short cut for Commit and Open transaction)
void Document::NewCommand() 
{
  //_hDoc->NewCommand(); 
}

/// returns True if a Command transaction is open
bool Document::HasOpenCommand() const
{
  return false;//hDoc->HasOpenCommand() != 0; 
}

/** Open a new command transaction.
 *  Raise a OCC Exception If a Command is already open.
 *  You may   check  it with the   previous method <HasOpenCommand>.
 */
void Document::OpenCommand()
{
  //_hDoc->OpenCommand(); 
}

/// Commit the Command transaction. Do nothing If there is no Command transaction open.
void Document::CommitCommand()
{
  //_hDoc->CommitCommand();
}

/// Abort the  Command  transaction. Do nothing If there is no Command transaction open.
void Document::AbortCommand()
{
  //_hDoc->AbortCommand(); 
}

/// The current limit on the number of undos
int Document::GetUndoLimit() const
{
  return 1000;//_hDoc->GetUndoLimit(); 
}

/** Set the limit on the number of Undo Deltas stored.
 *  0 will disable Undo on the document A negative value
 *  means no limit. Note that by default Undo is disabled.
 *  Enabling it will take effect with the next call to
 *  NewCommand. Of course this limit is the same for Redo.
 */
void Document::SetUndoLimit(const int L)
{
  //_hDoc->SetUndoLimit(L); 
}

/// Remove all stored Undos and Redos
void Document::ClearUndos()
{
  //_hDoc->ClearUndos(); 
}

/// Returns the  number  of stored Undos. If greater than 0 Undo will be effective.
int Document::GetAvailableUndos() const
{
  return 0;//_hDoc->GetAvailableUndos(); 
}

/// Returns the number   of stored Redos. If greater than 0 Redo will be effective.
int Document::GetAvailableRedos() const
{
  return 0;//_hDoc->GetAvailableRedos(); 
}


/// Recompute if the document was  not valid and propagate the reccorded modification.
void Document::Recompute()
{  
  int iSentinel = 20;
  bool goOn;
  DocChanges DocChange;
  DocChange.Why = DocChanges::Recompute;

  std::set<Feature*>::iterator i;
  std::set<Feature*> tempErr;
  
//  TDF_MapIteratorOfLabelMap It;
  Base::Console().Log("Solv: Start recomputation of document: \"%s\"\n",getName());

  do{
    goOn = false;
    tempErr = DocChange.ErrorFeatures;

    std::map<std::string,FeatEntry>::iterator It;

    for(It = FeatMap.begin();It != FeatMap.end();++It)
    {
      // map the new features
      if (It->second.F->getStatus() == Feature::New)
        DocChange.NewFeatures.insert(It->second.F);

		  if (It->second.F->MustExecute())
		  {
        // if a feature change a other (earlier) Feature could be impacted, so go on ...
        goOn = true;

        _RecomputeFeature(It->second.F);

        if(It->second.F->getStatus() == Feature::Error)
          DocChange.ErrorFeatures.insert(It->second.F);
        
        if(It->second.F->getStatus() == Feature::Valid)
        {
          DocChange.UpdatedFeatures.insert(It->second.F);
          DocChange.ErrorFeatures.erase(It->second.F);
        }
		  }
    }
    // for detecting recursions
    iSentinel --;

  }while(iSentinel > 0 && goOn && tempErr != DocChange.ErrorFeatures);

  if(iSentinel <= 0)
    Base::Console().Warning("Document::Recompute(): bailing out with to high solver count, possible recursion!\n");
  
  // remove the new features from the update set, get updated anyway
  for(i = DocChange.NewFeatures.begin();i!=DocChange.NewFeatures.end();++i)
    DocChange.UpdatedFeatures.erase(*i);

  //_hDoc->Recompute(); 

  Notify(DocChange);

  for(i = DocChange.ErrorFeatures.begin();i!=DocChange.ErrorFeatures.end();++i)
    Base::Console().Log("Error in Feature \"%s\": %s\n",(*i)->getName(),(*i)->getErrorString());

  Base::Console().Log("Solv: Recomputation of Document \"%s\" with %d new, %d Updated and %d errors finished\n",
                      getName(),
                      DocChange.NewFeatures.size(),
                      DocChange.UpdatedFeatures.size(),
                      DocChange.ErrorFeatures.size());
  
}

void Document::RecomputeFeature(Feature* Feat)
{
  DocChanges DocChange;
  DocChange.Why = DocChanges::Recompute;

  _RecomputeFeature(Feat);

  if(Feat->getStatus() == Feature::Error)
    DocChange.ErrorFeatures.insert(Feat);

  if(Feat->getStatus() == Feature::Valid)
    DocChange.UpdatedFeatures.insert(Feat);

  Notify(DocChange);

}

// call the recompute of the Feature and handle the exceptions and errors.
void Document::_RecomputeFeature(Feature* Feat)
{
  Base::Console().Log("Solv: Executing Feature: %s\n",Feat->getName());

  Feat->_eStatus = Feature::Recompute;
  int  succes;
  try{
    succes = Feat->execute();
  }catch(Base::AbortException &e){
    e.ReportException();
    succes = 4;
  }catch(const Base::MemoryException& e){
    Base::Console().Error("Memory exception in feature '%s' thrown: %s\n",Feat->getName(),e.what());
    Feat->setError(e.what());
    succes = 3;
  }catch(Base::Exception &e){
    e.ReportException();
    succes = 3;
  }catch(std::exception &e){
    Base::Console().Warning("CasCade exception in Feature \"%s\" thrown: %s\n",Feat->getName(),e.what());
    Feat->setError(e.what());
    succes = 3;
  }
#ifndef FC_DEBUG
  catch(...){
    Base::Console().Error("App::Document::_RecomputeFeature(): Unknown exception in Feature \"%s\" thrown\n",Feat->getName());
    succes = 3;
  }
#endif

  // special error code to avoid to execute a feature twice
  if (succes == 4){
    Feat->_eStatus = Feature::Inactive;
  }else if(succes > 0){
    Feat->_eStatus = Feature::Error;
  }else {
    // set the time of change
    Feat->_eStatus = Feature::Valid;

    Feat->touchTime.setToActual();
  }
}


Feature *Document::addFeature(const char* sType, const char* pFeatName)
{
  App::Feature* pcFeature = (App::Feature*) Base::Type::createInstanceByName(sType,true);
  assert(pcFeature->getTypeId().isDerivedFrom(App::Feature::getClassTypeId()));


  string FeatName;

	if(pcFeature)
	{
    pcFeature->setDocument(this);
    // get Unique name
    if(pFeatName)
      FeatName = getUniqueFeatureName(pFeatName);
    else
      FeatName = getUniqueFeatureName(sType);

    // set the status of the feature to New
    pcFeature->_eStatus = Feature::New;

    //_LogBook.SetTouched(FeatureLabel);
    //TouchState(FeatureLabel);
		// update the pointer
		pActiveFeature = pcFeature;

    // remember name (for faster search)
    FeatEntry e;
    e.F = pcFeature;
//    e.L = FeatureLabel;
    FeatMap[FeatName] = FeatEntry(e);

    pcFeature->_Name = FeatName;
	
    // return the feature
		return pcFeature;

	}else return 0;

}


/// Remove a feature out of the document
void Document::remFeature(const char* sName)
{
  DocChanges DocChange;
  DocChange.Why = DocChanges::Recompute;

  std::map<std::string,FeatEntry>::iterator pos;
  
  pos = FeatMap.find(sName);

  // name not found?
  if(pos == FeatMap.end())
    return;

  FeatEntry e = pos->second;

  DocChange.DeletedFeatures.insert(e.F);

  Notify(DocChange);

  // remove Feature Attribute
  //e.L.ForgetAttribute (FeatureAttr::GetID()); 

  // finely delete the Feature
  delete e.F;
}


Feature *Document::getActiveFeature(void)
{
  
  return pActiveFeature;
}

Feature *Document::getFeature(const char *Name)
{
  std::map<std::string,FeatEntry>::iterator pos;
  
  pos = FeatMap.find(Name);

  if(pos != FeatMap.end())
    return pos->second.F;
  else
    return 0;
}

const char *Document::getFeatureName(Feature *pFeat)
{
  std::map<std::string,FeatEntry>::iterator pos;

  for(pos = FeatMap.begin();pos != FeatMap.end();++pos)
    if(pos->second.F == pFeat)
      return pos->first.c_str();

  return 0;
}

string Document::getUniqueFeatureName(const char *Name)
{
  std::map<std::string,FeatEntry>::iterator pos;

  // name in use?
  pos = FeatMap.find(Name);

  if (pos == FeatMap.end())
    // if not, name is OK
    return Name;
  else
  {
    // find highes sufix
    int nSuff = 0;  
    for(pos = FeatMap.begin();pos != FeatMap.end();++pos)
    {
      const string &rclObjName = pos->first;
      int nPos = rclObjName.find_last_not_of("0123456789");
      if (rclObjName.substr(0, nPos + 1) == Name)  // Prefix gleich
      {
        string clSuffix(rclObjName.substr(nPos + 1));
        if (clSuffix.size() > 0)
          nSuff = max<int>(nSuff, atol(clSuffix.c_str()));
      }
    }
    char szName[200];
    sprintf(szName, "%s%d", Name, nSuff + 1);
	
    return string(szName);
  }
	
}

std::vector<Feature*> Document::getFeatures() const
{
  std::vector<Feature*> features;
  for( std::map<std::string,FeatEntry>::const_iterator it = FeatMap.begin(); it != FeatMap.end(); ++it )
    features.push_back( it->second.F );
  return features;
}

std::vector<Feature*> Document::getFeaturesOfType(const Base::Type& typeId) const
{
  std::vector<Feature*> features;
  for( std::map<std::string,FeatEntry>::const_iterator it = FeatMap.begin(); it != FeatMap.end(); ++it )
  {
    if ( it->second.F->getTypeId().isDerivedFrom( typeId ) )
      features.push_back( it->second.F );
  }
  return features;
}

int Document::countFeaturesOfType(const Base::Type& typeId) const
{
  int ct=0;
  for( std::map<std::string,FeatEntry>::const_iterator it = FeatMap.begin(); it != FeatMap.end(); ++it )
  {
    if ( it->second.F->getTypeId().isDerivedFrom( typeId ) )
      ct++;
  }

  return ct;
}

/*
void Document::Init (void)
{
	TDF_Label lMain = Main();

	_lBase    = lMain.FindChild(1);
	_lPos     = lMain.FindChild(2);
	_lFeature = lMain.FindChild(3);


	TDataStd_Name::Set(_lBase,    TCollection_ExtendedString((Standard_CString)"Base"));
	TDataStd_Name::Set(_lPos,     TCollection_ExtendedString((Standard_CString)"Pos"));
	TDataStd_Name::Set(_lFeature, TCollection_ExtendedString((Standard_CString)"Features"));

	_iNextFreeFeature = 1;
//	_lActiveFeature; 
}
/// Returns the storage string of the document.
const short* Document::storageFormat() const
{
  return _hDoc->StorageFormat().ToExtString(); 
}

/// Change the storage format of the document.
void Document::changeStorageFormat(const short* sStorageFormat) 
{
  _hDoc->ChangeStorageFormat((Standard_ExtString)sStorageFormat); 
}
*/

/*
void Document::TouchState(const TDF_Label &l)
{
  _LogBook.SetTouched(l);
}

void Document::ImpactState(const TDF_Label &l)
{
  _LogBook.SetImpacted(l);
}

*/

/*
FCLabel *Document::HasLabel(TDF_Label cLabel)
{
	FCLabel *pcL;
	std::map <TDF_Label,FCLabel*,LabelLess>::iterator It;
	
	// find a FCLabel if possible
	It = mcLabelMap.find(cLabel);

	// if not
	if(It == mcLabelMap.end())
	{
		// create a new label and append it to the map
		pcL = new FCLabel(cLabel,this);
		mcLabelMap[cLabel] = pcL;
		// taking care that python not delete the object
		pcL->IncRef();
		return pcL;
	}
	// if yes return the found FCLabel
	return It->second;

}


void Document::Dump(void)
{
  _hDoc->Main().Dump(std::cout);
}
*/

Base::PyObjectBase * Document::GetPyObject(void)
{
  _pcDocPy->IncRef();
  return _pcDocPy;
}


