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
# include <qdir.h>
# include <qfileinfo.h>
# include <qobjectlist.h>
# include <qstatusbar.h>
# include <qwhatsthis.h>
#endif

#include "Command.h"
#include "Action.h"
#include "Application.h"
#include "Document.h"
#include "ToolBoxBar.h"
#include "HelpView.h"
#include "Macro.h"
#include "DlgUndoRedo.h"
#include "BitmapFactory.h"

#include "../Base/Console.h"
#include "../Base/Exception.h"
#include "../Base/Interpreter.h"

using Base::Interpreter;
using namespace Gui;
using namespace Gui::Dialog;
using namespace Gui::DockWnd;

//===========================================================================
// Command
//===========================================================================


Command::Command(const char* name,CMD_Type eType)
  : sName(name),_pcAction(0),_eType(eType)
{
  sAppModule  = "";
  sGroup      = "";
}

Command::~Command()
{
  delete _pcAction;
}

bool Command::addTo(QWidget *pcWidget)
{
  if (!_pcAction)
    _pcAction = createAction();

  return _pcAction->addTo(pcWidget);
}

bool Command::removeFrom(QWidget *pcWidget)
{
  if (!_pcAction)
    return false;
  return _pcAction->removeFrom(pcWidget);
}

 
ApplicationWindow *Command::getAppWnd(void)
{
  return ApplicationWindow::Instance;
}

FCGuiDocument* Command::getActiveDocument(void)
{
  return getAppWnd()->GetActiveDocument();
}

App::Document*	   Command::getActiveOCCDocument(void)
{
  FCGuiDocument * pcDoc = getAppWnd()->GetActiveDocument();
  if(pcDoc)
    return pcDoc->GetDocument();
  else
    return 0l;
}

QAction* Command::getAction() 
{ 
  if (!_pcAction)
    _pcAction = createAction();

  return _pcAction; 
}

bool Command::isToggle(void)
{
  return (_eType&Cmd_Toggle) != 0; 
}


void Command::activated ()
{
  if(_eType == Cmd_Normal)
  {
    Base::Console().Log("Activate %s\n",_pcAction->text().latin1());
    // set the application module type for the macro
    getAppWnd()->GetMacroMngr()->SetModule(sAppModule.c_str());
    try{
      activated(0);
    }catch(...)
    {
      Base::Console().Error("Command::activated()  Activate of Cmd faild");
    };
  }
}
void Command::toggled (bool b)
{
  if(_eType == Cmd_Toggle)
  {
    Base::Console().Log("Toggled %s\n",_pcAction->text().latin1());
    if(b)
      activated(1);
    else
      activated(0);
  }
}

void Command::testActive(void)
{
  if(!_pcAction) return;

  bool bActive = isActive();
  _pcAction->setEnabled ( bActive );
}


//--------------------------------------------------------------------------
// UNDO REDO transaction handling  
//--------------------------------------------------------------------------
/** Open a new Undo transaction on the active document
 *  This methode open a new UNDO transaction on the active document. This transaction
 *  will later apear in the UNDO REDO dialog with the name of the command. If the user 
 *  recall the transaction everything changed on the document between OpenCommand() and 
 *  CommitCommand will be undone (or redone). You can use an alternetive name for the 
 *  operation default is the Command name.
 *  @see CommitCommand(),AbortCommand()
 */
void Command::openCommand(const char* sCmdName)
{
  // Using OpenCommand with no active document !
  assert(getAppWnd()->GetActiveDocument());

  if(sCmdName)
    getAppWnd()->GetActiveDocument()->OpenCommand(sCmdName);
  else
    getAppWnd()->GetActiveDocument()->OpenCommand(sName.c_str());
}

void Command::commitCommand(void)
{
  getAppWnd()->GetActiveDocument()->CommitCommand();
}

void Command::abortCommand(void)
{
  getAppWnd()->GetActiveDocument()->AbortCommand();
}

/// Run a App level Action 
void Command::doCommand(DoCmd_Type eType,const char* sCmd,...)
{
  // temp buffer
  char* format = (char*) malloc(strlen(sCmd)+1024);
    va_list namelessVars;
    va_start(namelessVars, sCmd);  // Get the "..." vars
    vsprintf(format, sCmd, namelessVars);
    va_end(namelessVars);

  if(eType == Gui)
    getAppWnd()->GetMacroMngr()->AddLine(FCMacroManager::Gui,format);
  else
    getAppWnd()->GetMacroMngr()->AddLine(FCMacroManager::Base,format);
  Interpreter().RunFCCommand(format);

  free (format);
}

/// Activate an other Commands
void Command::activateCommand(const char* sCmdName)
{
  Command* pcCmd = getAppWnd()->GetCommandManager().getCommandByName(sCmdName);
  if(pcCmd)
  {
    assert(!(pcCmd->isToggle()));
    pcCmd->activated(0);
  }
}

/// Toggles other Commands
void Command::toggleCommand(const char* sCmdName,bool bToggle)
{
  Command* pcCmd = getAppWnd()->GetCommandManager().getCommandByName(sCmdName);
  if(pcCmd)
  {
    assert(pcCmd->isToggle());
    pcCmd->_pcAction->setOn(bToggle?1:0);
  }
}

/// Updates the (active) document (propagate changes)
void Command::updateActive(void)
{
  getAppWnd()->GetActiveDocument()->GetDocument()->Recompute();
  //GetAppWnd()->UpdateActive();
}

/// Updates the (all or listed) documents (propagate changes)
void Command::updateAll(std::list<FCGuiDocument*> cList)
{
  if(cList.size()>0)
  {
    for(std::list<FCGuiDocument*>::iterator It= cList.begin();It!=cList.end();It++)
      (*It)->Update();
  }else{
    getAppWnd()->Update();
  }
}

//--------------------------------------------------------------------------
// Online help handling  
//--------------------------------------------------------------------------

/// returns the begin of a online help page
const char * Command::beginCmdHelp(void)
{
  return  "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">\n"
      "<html>\n"
      "<head>\n"
      "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=ISO-8859-1\">\n"
      "<title>FreeCAD Main Index</title>\n"
      "</head>\n"
      "<body bgcolor=\"#ffffff\">\n\n";
}
/// returns the end of a online help page
const char * Command::endCmdHelp(void)
{
  return "</body></html>\n\n";
}

//===========================================================================
// FCCppCommand 
//===========================================================================

CppCommand::CppCommand(const char* name,CMD_Type eType)
  :Command(name,eType)
{
  sMenuText     = "";
  sToolTipText  = "";
  sWhatsThis    = "";
  sStatusTip    = "";
  sPixmap       = QString::null;
  iAccel        = 0;
}

std::string CppCommand::getResource(const char* sName)
{
  return "";
}


QAction * CppCommand::createAction(void)
{
  QAction *pcAction;

  pcAction = new Action(this,ApplicationWindow::Instance,sName.c_str(),(_eType&Cmd_Toggle) != 0);
  pcAction->setText(QObject::tr(sMenuText));
  pcAction->setMenuText(QObject::tr(sMenuText));
  pcAction->setToolTip(QObject::tr(sToolTipText));
  pcAction->setStatusTip(QObject::tr(sStatusTip));
  pcAction->setWhatsThis(QObject::tr(sWhatsThis));
  if(sPixmap)
    pcAction->setIconSet(Gui::BitmapFactory().pixmap(sPixmap));
  pcAction->setAccel(iAccel);

  return pcAction;
}

void CppCommand::languageChange()
{
  if ( _pcAction )
  {
    _pcAction->setText(QObject::tr(sMenuText));
    _pcAction->setMenuText(QObject::tr(sMenuText));
    _pcAction->setToolTip(QObject::tr(sToolTipText));
    _pcAction->setStatusTip(QObject::tr(sStatusTip));
    _pcAction->setWhatsThis(QObject::tr(sWhatsThis));
  }
}


//===========================================================================
// MacroCommand 
//===========================================================================

MacroCommand::MacroCommand(const char* name)
  :CppCommand(name,Cmd_Normal)
{
  sAppModule    = "";
  sGroup        = "Macros";
}

void MacroCommand::activated(int iMsg)
{
//  OpenCommand("Excecute Macro");
//
//  DoCommand(Doc,"execfile(%s)",_sScriptName.c_str());
//
//  void CommitCommand(void);
  std::string cMacroPath = App::GetApplication().GetParameterGroupByPath
    ("User parameter:BaseApp/Preferences/Macro")->GetASCII("MacroPath",
    App::GetApplication().GetHomePath());

  QDir d( cMacroPath.c_str() );
  QFileInfo fi( d, scriptName );
  ApplicationWindow::Instance->GetMacroMngr()->Run(FCMacroManager::File,( fi.filePath() ).latin1());
}

void MacroCommand::setScriptName ( const QString& s )
{
  scriptName = s;
}

void MacroCommand::setWhatsThis( const QString& s )
{
  sWhatsThis = s;
  if ( _pcAction )
    _pcAction->setWhatsThis(QObject::tr(sWhatsThis));
}

void MacroCommand::setMenuText( const QString& s )
{
  sMenuText = s;
  if ( _pcAction )
  {
    _pcAction->setText    (QObject::tr(sMenuText));
    _pcAction->setMenuText(QObject::tr(sMenuText));
  }
}

void MacroCommand::setToolTipText( const QString& s )
{
  sToolTipText = s;
  if ( _pcAction )
    _pcAction->setToolTip(QObject::tr(sToolTipText));
}

void MacroCommand::setStatusTip( const QString& s )
{
  sStatusTip = s;
  if ( _pcAction )
    _pcAction->setStatusTip(QObject::tr(sStatusTip));
}

void MacroCommand::setPixmap( const QString& s )
{
  sPixmap = s;
  if ( _pcAction )
  {
    if ( sPixmap )
      _pcAction->setIconSet(Gui::BitmapFactory().pixmap(sPixmap));
    else
      _pcAction->setIconSet(QPixmap());
  }
}

void MacroCommand::setAccel(int i)
{
  iAccel = i;
  if ( _pcAction )
    _pcAction->setAccel(iAccel);
}

//===========================================================================
// FCPythonCommand
//===========================================================================


PythonCommand::PythonCommand(const char* name,PyObject * pcPyCommand)
  :Command(name),_pcPyCommand(pcPyCommand)
{
  Py_INCREF(_pcPyCommand);

  // call the methode "GetResources()" of the command object
  _pcPyResourceDict = Interpreter().RunMethodObject(_pcPyCommand, "GetResources");
  // check if the "GetResources()" methode returns a Dict object
  if(! PyDict_Check(_pcPyResourceDict) )
    throw Base::Exception("FCPythonCommand::FCPythonCommand(): Methode GetResources() of the python command object returns the wrong type (has to be Py Dictonary)");
}

std::string PythonCommand::getResource(const char* sName)
{
  PyObject* pcTemp;
  Base::PyBuf ResName(sName);


  // get the "MenuText" resource string
  pcTemp = PyDict_GetItemString(_pcPyResourceDict,ResName.str);
  if(! pcTemp )
    return std::string();
  if(! PyString_Check(pcTemp) )
    throw Base::Exception("FCPythonCommand::FCPythonCommand(): Methode GetResources() of the python command object returns a dictionary which holds not only strings");

  return std::string(PyString_AsString(pcTemp) );
}


void PythonCommand::activated(int iMsg)
{
  try{
    Interpreter().RunMethodVoid(_pcPyCommand, "Activated");
  }catch (Base::Exception e){
    Base::Console().Error("Running the python command %s failed,try to resume",sName.c_str());
  }
}

bool PythonCommand::isActive(void)
{
  return true;
}

std::string PythonCommand::cmdHelpURL(void)
{
  PyObject* pcTemp;

  pcTemp = Interpreter().RunMethodObject(_pcPyCommand, "CmdHelpURL"); 

  if(! pcTemp ) 
    return std::string();
  if(! PyString_Check(pcTemp) ) 
    throw Base::Exception("FCPythonCommand::CmdHelpURL(): Methode CmdHelpURL() of the python command object returns no string");
  
  return std::string( PyString_AsString(pcTemp) );
}

void PythonCommand::cmdHelpPage(std::string &rcHelpPage)
{
  PyObject* pcTemp;

  pcTemp = Interpreter().RunMethodObject(_pcPyCommand, "CmdHelpPage"); 

  if(! pcTemp ) 
    return ;
  if(! PyString_Check(pcTemp) ) 
    throw Base::Exception("FCPythonCommand::CmdHelpURL(): Methode CmdHelpURL() of the python command object returns no string");

  rcHelpPage = PyString_AsString(pcTemp) ;
}

QAction * PythonCommand::createAction(void)
{
  QAction *pcAction;

  pcAction = new Action(this,ApplicationWindow::Instance,sName.c_str(),(_eType&Cmd_Toggle) != 0);
  pcAction->setText(sName.c_str());
  pcAction->setMenuText(getResource("MenuText").c_str());
  pcAction->setToolTip(getResource("ToolTip").c_str());
  pcAction->setStatusTip(getResource("StatusTip").c_str());
  pcAction->setWhatsThis(getResource("WhatsThis").c_str());
  if(getResource("Pixmap") != "")
    pcAction->setIconSet(Gui::BitmapFactory().pixmap(getResource("Pixmap").c_str()));

  return pcAction;
}



//===========================================================================
// CommandManager 
//===========================================================================


void CommandManager::addCommand(Command* pCom)
{
  _sCommands[pCom->getName()] = pCom;//	pCom->Init();
}

void CommandManager::removeCommand(Command* pCom)
{
  std::map <std::string,Command*>::iterator It = _sCommands.find(pCom->getName());
  if (It != _sCommands.end())
  {
    delete It->second;
    _sCommands.erase(It);
  }
}

void CommandManager::addTo(const char* Name,QWidget *pcWidget)
{
  if (_sCommands.find(Name) == _sCommands.end())
  {
    Base::Console().Error("CommandManager::AddTo() try to add an unknown command (%s) to a widget!\n",Name);
  }
  else
  {
    Command* pCom = _sCommands[Name];
    pCom->addTo(pcWidget);
  }
}

void CommandManager::removeFrom(const char* Name,QWidget *pcWidget)
{
  if (_sCommands.find(Name) != _sCommands.end())
  {
    Command* pCom = _sCommands[Name];
    pCom->removeFrom(pcWidget);
  }
}

std::vector <Command*> CommandManager::getModuleCommands(const char *sModName)
{
  std::vector <Command*> vCmds;

  for( std::map<std::string, Command*>::iterator It= _sCommands.begin();It!=_sCommands.end();It++)
  {
    if( strcmp(It->second->getAppModuleName(),sModName) == 0)
      vCmds.push_back(It->second);
  }

  return vCmds;
}

std::string CommandManager::getAppModuleName(QAction* pAction)
{
  for( std::map<std::string, Command*>::iterator It= _sCommands.begin();It!=_sCommands.end();++It)
  {
    if ( It->second->getAction() == pAction )
      return It->second->getAppModuleName();
  }

  return "Not found";
}

std::string CommandManager::getAppModuleNameByName(const char* sName)
{
  for( std::map<std::string, Command*>::iterator It= _sCommands.begin();It!=_sCommands.end();++It)
  {
    if ( strcmp(It->second->getName(), sName) == 0 )
      return It->second->getAppModuleName();
  }

  return "Not found";
}

std::vector <Command*> CommandManager::getAllCommands(void)
{
  std::vector <Command*> vCmds;

  for( std::map<std::string, Command*>::iterator It= _sCommands.begin();It!=_sCommands.end();It++)
  {
    vCmds.push_back(It->second);
  }

  return vCmds;
}

std::vector <Command*> CommandManager::getGroupCommands(const char *sGrpName)
{
  std::vector <Command*> vCmds;

  for( std::map<std::string, Command*>::iterator It= _sCommands.begin();It!=_sCommands.end();It++)
  {
    if( strcmp(It->second->getGroupName(),sGrpName) == 0)
      vCmds.push_back(It->second);
  }

  return vCmds;
}

Command* CommandManager::getCommandByName(const char* sName)
{
  Command* pCom = NULL;
  if (_sCommands.find(sName) != _sCommands.end())
  {
    pCom = _sCommands[sName];
  }

  return pCom;
}

Command* CommandManager::getCommandByActionText(const char* sName)
{
  for( std::map<std::string, Command*>::iterator It= _sCommands.begin();It!=_sCommands.end();It++)
  {
    if (It->second->getAction())
    {
      if (It->second->getAction()->text() == sName)
      {
        return It->second;
      }
    }
  }

  return NULL;
}

void CommandManager::runCommandByName (const char* sName)
{
  Command* pCmd = getCommandByName(sName);

  if (pCmd)
    pCmd->activated();
}

void CommandManager::testActive(void)
{
  for( std::map<std::string, Command*>::iterator It= _sCommands.begin();It!=_sCommands.end();It++)
  {
    It->second->testActive();
  }
}
