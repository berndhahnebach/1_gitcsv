/** \file DlgParameterImp.cpp
 *  \brief  
 *  \author $Author$
 *  \version $Revision$
 *  \date    $Date$
 *   
 */

/***************************************************************************
 *   (c) J�rgen Riegel (juergen.riegel@web.de) 2002                        *   
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License (LGPL)   *
 *   as published by the Free Software Foundation; either version 2 of     *
 *   the License, or (at your option) any later version.                   *
 *   for detail see the LICENCE text file.                                 *
 *                                                                         *
 *   FreeCAD is distributed in the hope that it will be useful,            *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        * 
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with FreeCAD; if not, write to the Free Software        * 
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  *
 *   USA                                                                   *
 *                                                                         *
 *   Juergen Riegel 2002                                                   *
 ***************************************************************************/


/** Precompiled header stuff
 *  on some compilers the precompiled header option gain significant compile 
 *  time! So every external header (libs and system) should included in 
 *  Precompiled.h. For systems without precompilation the header needed are
 *  included in the else fork.
 */
#include "../Config.h"
#ifdef _PreComp_
#	include "PreCompiled.h"
#else
#	include <qmessagebox.h>
#	include <qiconview.h> 
#	include <qfiledialog.h>
#	include <qcombobox.h>
#       include <Python.h>
#endif

#include "DlgParameterImp.h"
#include "../Base/Parameter.h"
#include "../App/Application.h"

#include "Icons/ParamDlgImgs.h"


//**************************************************************************
//**************************************************************************
// DlgParameter
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/* 
 *  Constructs a DlgParameter 
 */
DlgParameter::DlgParameter( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : ParameterDialog( parent, name, modal, fl ),_pcMainLabel(0L),FCWindow(name)
{
	const std::map<std::string,FCParameterManager *> rcList = GetApplication().GetParameterSetList();

	for( std::map<std::string,FCParameterManager *>::const_iterator It= rcList.begin();It!=rcList.end();It++)
	{
		if(It->first == "User parameter")
			SetNameComboBox->insertItem(It->first.c_str(),1);
		else
			SetNameComboBox->insertItem(It->first.c_str(),-1);
	}

	QString cStr("System parameter");
	OnParameterSetChange(cStr);
}

/*  
 *  Destroys the object and frees any allocated resources
 */
DlgParameter::~DlgParameter()
{
    // no need to delete child widgets, Qt does it all for us
}

void DlgParameter::OnLoadParameterSet()
{ 
	puts("button pressed");
}

void DlgParameter::OnAddText()
{

}

void DlgParameter::DeleteItem()
{

}

void DlgParameter::OnAddFloat()
{

}

void DlgParameter::OnAddGroup()
{

}

void DlgParameter::OnAddInteger()
{

}

void DlgParameter::OnCleatValue()
{

}

void DlgParameter::OnInsertFromFile()
{

}



void DlgParameter::OnParameterSetChange(const QString& rcString)
{
	FCParameterManager &rcParMngr = GetApplication().GetParameterSet(rcString.latin1());

	if(_pcMainLabel) delete _pcMainLabel;
	// remove all labels
	ParameterView->clear();

	// root label
	_pcMainLabel = new ParameterLabelGroup(ParameterView,rcParMngr.GetGroup("BaseApp"));

	_pcMainLabel->setOpen(true);

	ParameterView->triggerUpdate(); 
}

//**************************************************************************
//**************************************************************************
// ParameterLabelGroup
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 

ParameterLabelGroup::ParameterLabelGroup( ParameterLabelGroup * parent, const FCHandle<FCParameterGrp> &hcGrp )
    : QListViewItem( parent ),
	_hcGrp(hcGrp)
{
	FillUp();	
}

ParameterLabelGroup::ParameterLabelGroup( QListView* parent, const FCHandle<FCParameterGrp> &hcGrp)
    : QListViewItem( parent ),
	_hcGrp(hcGrp)
{
	FillUp();	
}

void ParameterLabelGroup::FillUp(void)
{
	//setPixmap( folderOpen );
	// filing up groups
	std::vector<FCHandle<FCParameterGrp> > vhcParamGrp = _hcGrp->GetGroups();

	setText(0,_hcGrp->GetGroupName());

	for(std::vector<FCHandle<FCParameterGrp> >::iterator It=vhcParamGrp.begin();It!=vhcParamGrp.end();It++)
		new ParameterLabelGroup(this,*It);
	
	// filling up Text nodes
	std::map<std::string,std::string> mcTextMap = _hcGrp->GetASCIIMap();

	QListViewItem *pcItem;
	for(std::map<std::string,std::string>::iterator It2=mcTextMap.begin();It2!=mcTextMap.end();It2++)
	{
		pcItem = new QListViewItem(this,It2->first.c_str(),It2->second.c_str());
		pcItem->setPixmap(0,QPixmap(Param_Text) );
	}

	// filling up Int nodes
	std::map<std::string,long> mcIntMap = _hcGrp->GetIntMap();

	for(std::map<std::string,long>::iterator It3=mcIntMap.begin();It3!=mcIntMap.end();It3++)
	{
		char b[255];
		sprintf(b,"%d",It3->second);
		pcItem = new QListViewItem(this,It3->first.c_str(),b);
		pcItem->setPixmap(0,QPixmap(Param_Int) );
	}

	// filling up Float nodes
	std::map<std::string,double> mcFloatMap = _hcGrp->GetFloatMap();

	for(std::map<std::string,double>::iterator It4=mcFloatMap.begin();It4!=mcFloatMap.end();It4++)
	{
		char b[255];
		sprintf(b,"%f",It4->second);
		pcItem = new QListViewItem(this,It4->first.c_str(),b);
		pcItem->setPixmap(0,QPixmap(Param_Float) );
	}

	// filling up bool nodes
	std::map<std::string,bool> mcBoolMap = _hcGrp->GetBoolMap();

	for(std::map<std::string,bool>::iterator It5=mcBoolMap.begin();It5!=mcBoolMap.end();It5++)
	{
		pcItem = new QListViewItem(this,It5->first.c_str(),It5->second?"true":"false");
		pcItem->setPixmap(0,QPixmap(Param_Float) );
	}
}

void ParameterLabelGroup::activate ()
{
	puts("Activated");
}


#include "DlgParameter.cpp"

#include "moc_DlgParameter.cpp"
#include "moc_DlgParameterImp.cpp"