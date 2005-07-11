/***************************************************************************
 *   Copyright (c) 2002 J�rgen Riegel <juergen.riegel@web.de>              *
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
# include <qcombobox.h>
# include <qiconview.h>
# include <qfiledialog.h>
# include <qlineedit.h>
#endif

#include "DlgDisplayPropertiesImp.h"
#include "Command.h"
#include "Application.h"
#include "Widgets.h"
#include "Selection.h"
#include "Document.h"
#include "ViewProvider.h"

#include "../App/Application.h"

using namespace Gui::Dialog;

/**
 *  Constructs a DlgDisplayPropertiesImp which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
DlgDisplayPropertiesImp::DlgDisplayPropertiesImp(  Gui::Command* pcCmd, QWidget* parent,  const char* name, bool modal, WFlags fl )
    : DlgDisplayProperties( parent, name, modal, fl ),_pcCmd(pcCmd),Sel(Gui::Selection().Selection())
{
//  const std::set<App::Feature*> &Sel = Gui::Selection().Selection();
  std::set<std::string> ModeList;

  for(std::set<App::Feature*>::const_iterator It=Sel.begin();It!=Sel.end();It++)
  {
    ViewProviderInventor *pcProv = pcCmd->getActiveDocument()->getViewProvider(*It);
    std::vector<std::string> Modes = pcProv->getModes();

    for(std::vector<std::string>::iterator It2= Modes.begin();It2!=Modes.end();It2++)
      ModeList.insert(*It2);

    Provider.push_back(pcProv);
  }


  for(std::set<std::string>::iterator It3= ModeList.begin();It3!=ModeList.end();It3++)
    ModeBox->insertItem(It3->c_str()); 

}

/** 
 *  Destroys the object and frees any allocated resources
 */
DlgDisplayPropertiesImp::~DlgDisplayPropertiesImp()
{
  // no need to delete child widgets, Qt does it all for us
}

void DlgDisplayPropertiesImp::onChangeMaterial(const QString&s)
{
  Base::Console().Log("Material = %s\n",s.latin1());

}
void DlgDisplayPropertiesImp::onChangeMode(const QString&s)
{
  Base::Console().Log("Mode = %s\n",s.latin1());
  for( std::vector<ViewProviderInventor*>::iterator It= Provider.begin();It!=Provider.end();It++)
    (*It)->setMode(s.latin1()); 
  _pcCmd->getActiveDocument()->onUpdate();
}

void DlgDisplayPropertiesImp::onChangePlot(const QString&s)
{
  Base::Console().Log("Plot = %s\n",s.latin1());
}

void DlgDisplayPropertiesImp::onOK(void)
{
  accept();
}

void DlgDisplayPropertiesImp::onColorChange()
{
  Base::Console().Log("Color");

}

void DlgDisplayPropertiesImp::onCancel()
{
  reject();
}

void DlgDisplayPropertiesImp::onChangeTrans(int i)
{
  for( std::vector<ViewProviderInventor*>::iterator It= Provider.begin();It!=Provider.end();It++)
    (*It)->setTransparency( i/100.0); 
  _pcCmd->getActiveDocument()->onUpdate();
}



/*void DlgDisplayPropertiesImp::onChooseFile()
{
  QString s ( QFileDialog::getOpenFileName( QString::null, "FreeCAD Templates (*.FCTempl)", this ) );
  if ( s.isEmpty() )
    return;
  puts(s);
}
void DlgDisplayPropertiesImp::onValidate()
{
  QString sTemplate = LineEdit1->text();

  if (sTemplate.isEmpty())
    _pcCmd->doCommand(Gui::Command::Doc,"FreeCAD.New()");		
  else
    _pcCmd->doCommand(Gui::Command::Doc,"FreeCAD.New(\"%s\")",LineEdit1->text().latin1());

  accept();
}

void DlgDisplayPropertiesImp::onIconDoubleClicked( QIconViewItem* Item )
{
  LineEdit1->setText(Item->text());
  onValidate();
}

void DlgDisplayPropertiesImp::onViewChanged( QIconViewItem* Item )
{
  LineEdit1->setText(Item->text());
}

void DlgDisplayPropertiesImp::onEditFile()
{
  qWarning( "DlgDisplayPropertiesImp::EditFile() not yet implemented!" ); 
}
*/


#include "DlgDisplayProperties.cpp"
#include "moc_DlgDisplayProperties.cpp"
#include "moc_DlgDisplayPropertiesImp.cpp"
