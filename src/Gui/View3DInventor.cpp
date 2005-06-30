/***************************************************************************
 *   Copyright (c) 2004 J�rgen Riegel <juergen.riegel@web.de>              *
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
# include <qdragobject.h>
# include <qfileinfo.h>
# include <qvbox.h>
# include <TopoDS_Shape.hxx>
# include <Inventor/actions/SoWriteAction.h>
# include <Inventor/nodes/SoMaterial.h>
# include <Inventor/nodes/SoOrthographicCamera.h>
# include <Inventor/nodes/SoPerspectiveCamera.h>
# include <Inventor/nodes/SoShapeHints.h>
# include <Inventor/events/SoEvent.h>
#endif

#include "View3DInventor.h"
#include "View3DInventorViewer.h"
#include "Document.h"
#include "Application.h"
#include "MenuManager.h"
#include <App/Label.h>
#include <App/Feature.h>
#include <Base/Console.h>

// build in Inventor
#include "Inventor/Qt/viewers/SoQtExaminerViewer.h"
#include "Inventor/OCC/SoBrepShape.h"

#include "View3DInventorExamples.h"
#include "ViewProviderFeature.h"




#include "Icons/default_background.xpm"

using namespace Gui;


View3DInventor::View3DInventor( Gui::Document* pcDocument, QWidget* parent, const char* name, int wflags )
    :MDIView( pcDocument,parent, name, wflags)
{
  // important for highlighting 
  setMouseTracking(true);
  // accept drops on the window, get handled in dropEvent, dragEnterEvent   
  setAcceptDrops(true);

  pcActViewProvider = 0l;
  
  _viewer = new View3DInventorViewer(this);



  setViewerDefaults();
}

View3DInventor::~View3DInventor()
{
  delete _viewer;
  pcSepRoot->unref();
}

void View3DInventor::updatePrefs(void)
{

    // Importing the User special ++++++++++++++++++++++++++++++++++++++++++++++++
/*    SoInput in;
    SbBool ok = in.openFile(fn.latin1());
    if (!ok) throw FCException("FCView3DInventor::SetViewerDefaults() wrong file name for user special!");
    SoSeparator * pcSepUserSpecial = SoDB::readAll(&in);
  pcSepRoot->addChild(pcSepUserSpecial);
*/
}

#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoTransform.h>

void View3DInventor::contextMenuEvent ( QContextMenuEvent * e)
{
  MenuItem* view = new MenuItem;
//  view->setCommand( QObject::tr("&View") );
  *view << "Std_ViewCreateInventor" << "Std_ViewCreateOCC" << "Separator" << "Std_ViewFullScreen" 
        << "Separator" << "Std_ViewMenu";

  QPopupMenu ContextMenu(this);
  MenuManager::getInstance().setupContextMenu(view,ContextMenu);
  ContextMenu.exec(e->globalPos());

}


void View3DInventor::setViewerDefaults(void)
{
  // create the root group
  pcSepRoot = new SoSeparator();
  pcSepRoot->ref();

  // empty separator for user special
    SoSeparator * pcSepUserSpecial= new SoSeparator();
  pcSepRoot->addChild(pcSepUserSpecial);


  // shape aperance
  pcShapeHint = new SoShapeHints;
  pcShapeHint->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE; 

  // color management
//  QColor Col1(236,212,156); // light brown
//  QColor Col(128,128,128);  // grey

//  QColor DifCol=Col1;
//  QColor AmbCol=Col1;
//  QColor SpeCol=Col;
//  QColor EmCol(0,0,0);
/*
  pcShapeMaterial = new SoMaterial;
  pcShapeMaterial->diffuseColor.setValue(((float)DifCol.red())/256,((float)DifCol.green())/256,((float)DifCol.blue())/256);
  pcShapeMaterial->ambientColor.setValue(((float)AmbCol.red())/256,((float)AmbCol.green())/256,((float)AmbCol.blue())/256);
  pcShapeMaterial->specularColor.setValue(((float)SpeCol.red())/256,((float)SpeCol.green())/256,((float)SpeCol.blue())/256);
  pcShapeMaterial->emissiveColor.setValue(((float)EmCol.red())/256,((float)EmCol.green())/256,((float)EmCol.blue())/256);
  pcSepRoot->addChild(pcShapeMaterial);
*/
  pcSepRoot->renderCaching = SoSeparator::ON;
  pcSepRoot->addChild(pcShapeHint);


  pcSepRoot->addChild(createAxis());


  // create the root for the shape
  pcSepShapeRoot = new SoSeparator();
  pcSepRoot->addChild(pcSepShapeRoot);

//  _viewer->setSceneGraph(pcSepRoot);


//  _viewer->setFeedbackVisibility(true);
//  _viewer->setFeedbackSize(15);
//  _viewer->setBackgroundColor(SbColor(0.2f,0.2f,0.2f));
  _viewer->viewAll();
//  _viewer->setDecoration(false);

//  _viewer->setSceneGraph(pcSepRoot);

  //_viewer->setAntialiasing(true,2);
//  _viewer->setSceneGraph(new SoCone);
//  _viewer->show();

  _viewer->show();
}


SoSeparator *View3DInventor::createAxis(void)
{
  // create axis
  pcSepAxis = new SoSeparator();

  // x Axis
  SoMaterial *pcMat = new SoMaterial();
  pcMat->diffuseColor.setValue(1.0,0,0);
  pcSepAxis->addChild(pcMat);
  SoCylinder *pcCylinder = new SoCylinder();
  pcCylinder->height.setValue(100);

  SoTransform *pcTransform = new SoTransform();
  pcTransform->rotation.setValue(  SbVec3f (1,0,0), 90);
  pcSepAxis->addChild(pcTransform);

  // y axis
  pcMat = new SoMaterial();
  pcMat->diffuseColor.setValue(0,1.0,0);
  pcSepAxis->addChild(pcMat);
  pcCylinder = new SoCylinder();
  pcCylinder->height.setValue(100);

  return pcSepAxis;
}

/*
void View3DInventor::setShape(void)
{
  TopoDS_Shape cShape;
    SoBrepShape cShapeConverter;


  assert(getAppDocument());


    //TDF_Label L = GetAppDocument()->Main()->GetOCCLabel();




  // Open the argument file..
    //cShapeConverter.SetFile(fn.latin1());

  bool nurbs_enable = false;
  bool selection = false;
  bool strip_enable = false;
  bool strip_color = false;

  cShapeConverter.SetRenderOptions(strip_enable,strip_color,nurbs_enable,selection);
  cShapeConverter.Compute(pcSepShapeRoot);
}
*/

void View3DInventor::onUpdate(void)
{
#ifdef FC_LOGUPDATECHAIN
  Base::Console().Log("Acti: Gui::View3DInventor::onUpdate()");
#endif
/*
  App::Feature *pcActFeature = getAppDocument()->GetActiveFeature();

  if(pcActFeature)
  {
    std::string cName = pcActFeature->Type();
    // check if still the same ViewProvider
    if(cName == cViewProviderName && pcActViewProvider)
    {
      // if the same just calculate the new representation
      pcActViewProvider->attache(pcActFeature);
      SoNode* Node = pcActViewProvider->getRoot();
      pcSepRoot->addChild(Node);
      //_viewer->setSceneGraph(Node);
    }else{
      // if not create the new ViewProvider
      if (pcActViewProvider) 
        delete pcActViewProvider;
      pcActViewProvider = ViewProviderInventorFeatureFactory().Produce(pcActFeature->Type());
      if(pcActViewProvider)
      {
        // if succesfully created set the right name an calculate the view
        cViewProviderName = pcActFeature->Type();
        pcActViewProvider->attache(pcActFeature);
        SoNode* Node = pcActViewProvider->getRoot();
        pcSepRoot->addChild(Node);
        //_viewer->setSceneGraph(Node);
      }else{
        Base::Console().Warning("Gui::View3DInventorEx::onUpdate() no view provider for the Feature %s found\n",pcActFeature->Type());
        cViewProviderName = "";
      }
    }

  }

*/
  
}
void View3DInventor::hideEvent ( QHideEvent * cEvent )
{
 // ApplicationWindow::Instance->setPaneText(2, QString(" Dimension"));
}

void View3DInventor::showDimension (void) const
{
/*  if ( _hView.IsNull() )
    return; // no valid view

  Quantity_Length fWidth, fHeight;
  _hView->Size(fWidth, fHeight);

  float fLog = float(log10(fWidth)), fFactor;
  int   nExp = int(fLog);
  char  szDim[20];

  if (nExp >= 6)
  {
    fFactor = 1.0e+6f;
    strcpy(szDim, "km");
  }
  else if (nExp >= 3)
  {
    fFactor = 1.0e+3f;
    strcpy(szDim, "m");
  }
  else if ((nExp >= 0) && (fLog > 0.0f))
  {
    fFactor = 1.0e+0f;
    strcpy(szDim, "mm");
  }
  else if (nExp >= -3)
  {
    fFactor = 1.0e-3f;
    strcpy(szDim, "um");
  }
  else 
  {
    fFactor = 1.0e-6f;
    strcpy(szDim, "nm");
  }

  char szSize[100];
  sprintf(szSize, " %.2f x %.2f %s", fWidth / fFactor, fHeight / fFactor, szDim);

  ApplicationWindow::Instance->setPaneText(2, QString(szSize));
  */
}



void View3DInventor::resizeEvent ( QResizeEvent * e)
{
  MDIView::resizeEvent(e);
//  _pcFrame->resize(e->size());
}

const char *View3DInventor::getName(void)
{
  return "View3DInventor";
}

// buffer acrobatics for inventor ****************************************************
static char * buffer;
static size_t buffer_size = 0;
static std::string cReturnString;

static void *
buffer_realloc(void * bufptr, size_t size)
{
  buffer = (char *)realloc(bufptr, size);
  buffer_size = size;
  return buffer;
}
 
const std::string &View3DInventor::writeNodesToString(SoNode * root)
{
  SoOutput out;
  buffer = (char *)malloc(1024);
  buffer_size = 1024;
  out.setBuffer(buffer, buffer_size, buffer_realloc);

  SoWriteAction wa(&out);
  wa.apply(root);

  cReturnString =buffer;
  free(buffer);
  return cReturnString;
}

// **********************************************************************************

bool View3DInventor::onMsg(const char* pMsg, const char** ppReturn)
{
  if(strcmp("SetStereoOn",pMsg) == 0 ){
    _viewer->setStereoViewing(true);
    _viewer->setStereoOffset(10);
    return true;
  }else if(strcmp("SetStereoOff",pMsg) == 0 ){
    _viewer->setStereoViewing(false);
    return true;
  }else if(strcmp("Example1",pMsg) == 0 ){
    SoSeparator * root = new SoSeparator;;
    Texture3D(root);
    _viewer->setSceneGraph(root);
    return true;
  }else if(strcmp("Example2",pMsg) == 0 ){
    SoSeparator * root = new SoSeparator;;
    LightManip(root);
    _viewer->setSceneGraph(root);
    return true;
  }else if(strcmp("Example3",pMsg) == 0 ){
    SoSeparator * root = new SoSeparator;;
    AnimationTexture(root);
    _viewer->setSceneGraph(root);
    return true;
  }else if(strcmp("GetCamera",pMsg) == 0 ){
    SoCamera * Cam = _viewer->getCamera();
    *ppReturn = writeNodesToString(Cam).c_str();
    return true;
  }else if(strncmp("SetCamera",pMsg,9) == 0 ){
    return setCamera(pMsg+10);
  }else if(strcmp("ViewFit",pMsg) == 0 ){
    _viewer->viewAll();
    return true;
  }else if(strcmp("ViewBottom",pMsg) == 0 ){
    SoCamera* cam = _viewer->getCamera();
    cam->orientation.setValue(-1, 0, 0, 0);
    _viewer->viewAll();
    return true;
  }else if(strcmp("ViewFront",pMsg) == 0 ){
    float root = (float)(sqrt(2.0)/2.0);
    SoCamera* cam = _viewer->getCamera();
    cam->orientation.setValue(-root, 0, 0, -root);
    _viewer->viewAll();
    return true;
  }else if(strcmp("ViewLeft",pMsg) == 0 ){
    SoCamera* cam = _viewer->getCamera();
    cam->orientation.setValue(0.5, 0.5, 0.5, 0.5);
    _viewer->viewAll();
    return true;
  }else if(strcmp("ViewRear",pMsg) == 0 ){
    float root = (float)(sqrt(2.0)/2.0);
    SoCamera* cam = _viewer->getCamera();
    cam->orientation.setValue(0, root, root, 0);
    _viewer->viewAll();
    return true;
  }else if(strcmp("ViewRight",pMsg) == 0 ){
    SoCamera* cam = _viewer->getCamera();
    cam->orientation.setValue(-0.5, 0.5, 0.5, -0.5);
    _viewer->viewAll();
    return true;
  }else if(strcmp("ViewTop",pMsg) == 0 ){
    SoCamera* cam = _viewer->getCamera();
    cam->orientation.setValue(0, 0, 0, 1);
    _viewer->viewAll();
    return true;
  }else if(strcmp("ViewAxo",pMsg) == 0 ){
    float root = (float)(sqrt(3.0)/4.0);
    SoCamera* cam = _viewer->getCamera();
    cam->orientation.setValue(-0.333333f, -0.166666f, -0.333333f, -root);
    _viewer->viewAll();
    return true;
  }

  return false;
}


bool View3DInventor::onHasMsg(const char* pMsg)
{
  if(strcmp("SetStereoOn",pMsg) == 0 ){
    return true;
  }else if(strcmp("SetStereoOff",pMsg) == 0 ){
    return true;
  }else if(strcmp("SetStereoOff",pMsg) == 0 ){
    return true;
  }else if(strcmp("Example1",pMsg) == 0 ){
    return true;
  }else if(strcmp("Example2",pMsg) == 0 ){
    return true;
  }else if(strcmp("Example3",pMsg) == 0 ){
    return true;
  }else if(strcmp("ViewFit",pMsg) == 0 ){
    return true;
  }else if(strcmp("ViewBottom",pMsg) == 0 ){
    return true;
  }else if(strcmp("ViewFront",pMsg) == 0 ){
    return true;
  }else if(strcmp("ViewLeft",pMsg) == 0 ){
    return true;
  }else if(strcmp("ViewRear",pMsg) == 0 ){
    return true;
  }else if(strcmp("ViewRight",pMsg) == 0 ){
    return true;
  }else if(strcmp("ViewTop",pMsg) == 0 ){
    return true;
  }else if(strcmp("ViewAxo",pMsg) == 0 ){
    return true;
  }else if(strcmp("GetCamera",pMsg) == 0 ){
    return true;
  }else if(strncmp("SetCamera",pMsg,9) == 0 ){
    return true;
  }
  return false;
}

#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>

bool View3DInventor::setCamera(const char* pCamera)
{
  SoCamera * CamViewer = _viewer->getCamera();
  if(!CamViewer)
    Base::Console().Warning("setCamera because no camera set so far....");
  SoPerspectiveCamera  * CamViewerP = 0;
  SoOrthographicCamera * CamViewerO = 0;

  if (CamViewer->getTypeId() == SoPerspectiveCamera::getClassTypeId()) {
    CamViewerP = (SoPerspectiveCamera *)CamViewer;  // safe downward cast, knows the type
  }else if (CamViewer->getTypeId() == SoOrthographicCamera::getClassTypeId()) {
    CamViewerO = (SoOrthographicCamera *)CamViewer;  // safe downward cast, knows the type
  }

  SoInput in;
  in.setBuffer((void*)pCamera,strlen(pCamera));

  SoNode * Cam;
  SoDB::read(&in,(SoNode*&)Cam);

  if (!Cam){
    Base::Console().Error("setCamera faild to read: %s\n",pCamera);
    return true;
  }

  if (Cam->getTypeId() == SoPerspectiveCamera::getClassTypeId()) {
    if(CamViewerP){
      CamViewerP->position      = ((SoPerspectiveCamera *)Cam)->position;
      CamViewerP->orientation   = ((SoPerspectiveCamera *)Cam)->orientation;
      CamViewerP->nearDistance  = ((SoPerspectiveCamera *)Cam)->nearDistance;
      CamViewerP->farDistance   = ((SoPerspectiveCamera *)Cam)->farDistance;
      CamViewerP->focalDistance = ((SoPerspectiveCamera *)Cam)->focalDistance;
    }
    else
      Base::Console().Error("Camera type missmatch");
  }else if (Cam->getTypeId() == SoOrthographicCamera::getClassTypeId()) {
    if(CamViewerO){
      CamViewerO->viewportMapping  = ((SoOrthographicCamera *)Cam)->viewportMapping;
      CamViewerO->position         = ((SoOrthographicCamera *)Cam)->position;
      CamViewerO->orientation      = ((SoOrthographicCamera *)Cam)->orientation;
      CamViewerO->nearDistance     = ((SoOrthographicCamera *)Cam)->nearDistance;
      CamViewerO->farDistance      = ((SoOrthographicCamera *)Cam)->farDistance;
      CamViewerO->focalDistance    = ((SoOrthographicCamera *)Cam)->focalDistance;
      CamViewerO->aspectRatio      = ((SoOrthographicCamera *)Cam)->aspectRatio ;
      CamViewerO->height           = ((SoOrthographicCamera *)Cam)->height;
    }
    else
      Base::Console().Error("Camera type missmatch");
  }

  return true;
}


void View3DInventor::onWindowActivated ()
{
  //myOperations->onSelectionChanged();
}

void View3DInventor::setCursor(const QCursor& aCursor)
{
  //_viewer->setCursor(aCursor);
}

void View3DInventor::dump()
{

}

/**
 * Drops the event \a e and writes the right Python command.
 */
void View3DInventor::dropEvent ( QDropEvent      * e )
{
  if ( QUriDrag::canDecode(e) )
  {
    QStringList fn;
    QUriDrag::decodeLocalFiles(e, fn);

    for ( QStringList::Iterator it = fn.begin(); it != fn.end(); ++it ) {

      QFileInfo info(*it);
      if ( info.exists() && info.isFile() )
      {
          ApplicationWindow::Instance->import(info.absFilePath().latin1());
      }
    }
  }else
    MDIView::dropEvent(e);
}



#include "moc_View3DInventor.cpp"
