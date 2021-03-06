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
# include <Inventor/actions/SoWriteAction.h>
# include <Inventor/nodes/SoMaterial.h>
# include <Inventor/nodes/SoOrthographicCamera.h>
# include <Inventor/nodes/SoPerspectiveCamera.h>
# include <Inventor/nodes/SoSeparator.h>
# include <Inventor/nodes/SoShapeHints.h>
# include <Inventor/events/SoEvent.h>
#endif

#include "View3DInventor.h"
#include "View3DInventorViewer.h"
#include "Document.h"
#include "Application.h"
#include "MenuManager.h"
#include <App/Feature.h>
#include <Base/Console.h>

// build in Inventor
#include "Inventor/Qt/viewers/SoQtExaminerViewer.h"

#include "View3DInventorExamples.h"
#include "ViewProviderFeature.h"

#include <locale>


using namespace Gui;


View3DInventor::View3DInventor( Gui::Document* pcDocument, QWidget* parent, const char* name, int wflags )
    :MDIView( pcDocument,parent, name, wflags)
{
  // important for highlighting 
  setMouseTracking(true);
  // accept drops on the window, get handled in dropEvent, dragEnterEvent   
  setAcceptDrops(true);

  pcActViewProvider = 0l;
  
  // attache Parameter Observer
  hGrp = App::GetApplication().GetParameterGroupByPath("User parameter:BaseApp/Preferences/View");
  hGrp->Attach(this);

  // create the inventor widget and set the defaults
  _viewer = new View3DInventorViewer(this);
  setViewerDefaults();
  // check whether a perspective or orthogrphic camera should be set
  if ( hGrp->GetBool("Orthographic", false) )
    _viewer->setCameraType(SoOrthographicCamera::getClassTypeId());
  else
    _viewer->setCameraType(SoPerspectiveCamera::getClassTypeId());
  _viewer->show();


}

View3DInventor::~View3DInventor()
{
  hGrp->Detach(this);

  delete _viewer;
//  pcSepRoot->unref();
}


#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoTransform.h>

void View3DInventor::setViewerDefaults(void)
{
  Base::Console().Log("Acti: View3DInventor::setViewerDefaults()\n");

  _viewer->setStereoOffset(hGrp->GetFloat("EyeDistance"      ,65.0));
  _viewer->bDrawAxisCross = hGrp->GetBool("CornerCoordSystem",true);
  _viewer->bAllowSpining =  hGrp->GetBool("UseAutoRotation"  ,true);
  _viewer->setGradientBackgroud( (hGrp->GetBool("Gradient",true)) );
  long col = hGrp->GetInt("BackgroundColor",0);
  float r,g,b;
  r = (col & 0xff) / 255.0;
  g = ((col >> 8) & 0xff) / 255.0;
  b = ((col >> 16) & 0xff) / 255.0;
  _viewer->setBackgroundColor(SbColor(r, g, b));

  long col2 = hGrp->GetInt("BackgroundColor2",13467519); // default color (lila)
  long col3 = hGrp->GetInt("BackgroundColor3",16777215); // default color (white)
  float r2,g2,b2,r3,g3,b3;
  r2 = (col2 & 0xff) / 255.0; g2 = ((col2 >> 8) & 0xff) / 255.0; b2 = ((col2 >> 16) & 0xff) / 255.0;
  r3 = (col3 & 0xff) / 255.0; g3 = ((col3 >> 8) & 0xff) / 255.0; b3 = ((col3 >> 16) & 0xff) / 255.0;
  _viewer->setGradientBackgroudColor( SbColor(r2, g2, b2), SbColor(r3, g3, b3) );

  if(hGrp->GetBool("UseAntialiasing"  ,false))
    _viewer->getGLRenderAction()->setSmoothing(true);
  else
    _viewer->getGLRenderAction()->setSmoothing(false);
}

/// Observer message from the ParameterGrp
void View3DInventor::OnChange(ParameterGrp::SubjectType &rCaller,ParameterGrp::MessageType Reason)
{
  setViewerDefaults();
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


void View3DInventor::onUpdate(void)
{
#ifdef FC_LOGUPDATECHAIN
  Base::Console().Log("Acti: Gui::View3DInventor::onUpdate()");
#endif
  update();  
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
  if(strcmp("ViewFit",pMsg) == 0 ){
    _viewer->viewAll();
    return true;
// comment out on older Inventor
#if (SOQT_MAJOR_VERSION >= 1 && SOQT_MINOR_VERSION >= 2)
  }else if(strcmp("SetStereoRedGreen",pMsg) == 0 ){
    _viewer->setStereoType(SoQtViewer::STEREO_ANAGLYPH);
    return true;
  }else if(strcmp("SetStereoQuadBuff",pMsg) == 0 ){
    _viewer->setStereoType(SoQtViewer::STEREO_QUADBUFFER );
    return true;
  }else if(strcmp("SetStereoInterleavedRows",pMsg) == 0 ){
    _viewer->setStereoType(SoQtViewer::STEREO_INTERLEAVED_ROWS );
    return true;
  }else if(strcmp("SetStereoInterleavedColumns",pMsg) == 0 ){
    _viewer->setStereoType(SoQtViewer::STEREO_INTERLEAVED_COLUMNS  );
    return true;
  }else if(strcmp("SetStereoOff",pMsg) == 0 ){
    _viewer->setStereoType(SoQtViewer::STEREO_NONE );
    return true;
#else
  }else if(strcmp("SetStereoRedGreen",pMsg) == 0 ){
    Base::Console().Warning("Use SoQt 1.2.x or later!\n");
    return true;
  }else if(strcmp("SetStereoQuadBuff",pMsg) == 0 ){
    Base::Console().Warning("Use SoQt 1.2.x or later!\n");
    return true;
  }else if(strcmp("SetStereoInterleavedRows",pMsg) == 0 ){
    Base::Console().Warning("Use SoQt 1.2.x or later!\n");
    return true;
  }else if(strcmp("SetStereoInterleavedColumns",pMsg) == 0 ){
    Base::Console().Warning("Use SoQt 1.2.x or later!\n");
    return true;
  }else if(strcmp("SetStereoOff",pMsg) == 0 ){
    Base::Console().Warning("Use SoQt 1.2.x or later!\n");
    return true;
#endif
  }else if(strcmp("Example1",pMsg) == 0 ){
    SoSeparator * root = new SoSeparator;
    Texture3D(root);
    _viewer->setSceneGraph(root);
    return true;
  }else if(strcmp("Example2",pMsg) == 0 ){
    SoSeparator * root = new SoSeparator;
    LightManip(root);
    _viewer->setSceneGraph(root);
    return true;
  }else if(strcmp("Example3",pMsg) == 0 ){
    SoSeparator * root = new SoSeparator;
    AnimationTexture(root);
    _viewer->setSceneGraph(root);
    return true;
  }else if(strcmp("GetCamera",pMsg) == 0 ){
    SoCamera * Cam = _viewer->getCamera();
    *ppReturn = writeNodesToString(Cam).c_str();
    return true;
  }else if(strncmp("SetCamera",pMsg,9) == 0 ){
    return setCamera(pMsg+10);
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
  }else if(strcmp("OrthographicCamera",pMsg) == 0 ){
    _viewer->setCameraType(SoOrthographicCamera::getClassTypeId());
    return true;
  }else if(strcmp("PerspectiveCamera",pMsg) == 0 ){
    _viewer->setCameraType(SoPerspectiveCamera::getClassTypeId());
    return true;
  }

  return false;
}


bool View3DInventor::onHasMsg(const char* pMsg)
{
  if(strcmp("SetStereoRedGreen",pMsg) == 0 ){
    return true;
  }else if(strcmp("SetStereoQuadBuff",pMsg) == 0 ){
    return true;
  }else if(strcmp("SetStereoInterleavedRows",pMsg) == 0 ){
    return true;
  }else if(strcmp("SetStereoInterleavedColumns",pMsg) == 0 ){
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
    Base::Console().Error("setCamera failed to read: %s\n",pCamera);
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
  _viewer->getWidget()->setCursor(aCursor);
}

void View3DInventor::dump()
{

}

/**
 * Drops the event \a e and writes the right Python command.
 */
void View3DInventor::dropEvent ( QDropEvent * e )
{
  if ( QUriDrag::canDecode(e) )
  {
    QStringList fn;
    QUriDrag::decodeLocalFiles(e, fn);

    App::Document* pDoc = getAppDocument();
    if ( pDoc ) 
    {
      for ( QStringList::Iterator it = fn.begin(); it != fn.end(); ++it ) {

        QFileInfo info(*it);
        if ( info.exists() && info.isFile() )
        {
          if ( App::GetApplication().hasOpenType( info.extension().latin1() ) )
            Application::Instance->import(info.absFilePath().latin1(), pDoc->getName());
        }
      }
    }
  }else
    MDIView::dropEvent(e);
}

void View3DInventor::dragEnterEvent ( QDragEnterEvent * e )
{
  // Here we must allow uri drafs and check them in dropEvent
  if ( QUriDrag::canDecode(e) )
    e->accept();
  else
    e->ignore();
}



#include "moc_View3DInventor.cpp"
