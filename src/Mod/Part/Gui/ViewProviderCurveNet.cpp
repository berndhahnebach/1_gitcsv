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
# include <BRep_Tool.hxx>
# include <Geom_Curve.hxx>
# include <TColgp_Array1OfPnt.hxx>
# include <TopoDS.hxx>
# include <TopExp_Explorer.hxx>
# include <Inventor/nodes/SoComplexity.h>
# include <Inventor/nodes/SoCoordinate3.h>
# include <Inventor/nodes/SoDrawStyle.h>
# include <Inventor/nodes/SoLineSet.h>
# include <Inventor/nodes/SoLocateHighlight.h>
# include <Inventor/nodes/SoMaterial.h>
# include <Inventor/nodes/SoTransform.h>
# include <Inventor/nodes/SoSphere.h>
# include <Inventor/events/SoMouseButtonEvent.h>
# include <Inventor/events/SoKeyboardEvent.h>
# include <Inventor/events/SoLocation2Event.h>
#endif

/// Here the FreeCAD includes sorted by Base,App,Gui......
#include <Base/Console.h>
#include <Base/Parameter.h>
#include <Base/Exception.h>
#include <Base/Sequencer.h>
#include <Gui/View3DInventorViewer.h>
#include <Gui/SoFCSelection.h>
#include <App/Application.h>

#include <Mod/Part/App/PartFeature.h>

#include "ViewProviderCurveNet.h"



using namespace PartGui;


//**************************************************************************
// Construction/Destruction

PROPERTY_SOURCE(PartGui::ViewProviderCurveNet,PartGui::ViewProviderPart)

       
ViewProviderCurveNet::ViewProviderCurveNet()
:bInEdit(false),bMovePointMode(false)
{
  /*
  hGrp = App::GetApplication().GetParameterGroupByPath("User parameter:BaseApp/Preferences/Mod/Part");

  fMeshDeviation      = hGrp->GetFloat("MeshDeviation",0.2);
  bNoPerVertexNormals = hGrp->GetBool("NoPerVertexNormals",false);
  lHilightColor       = hGrp->GetInt ("HilightColor",0);
  bQualityNormals     = hGrp->GetBool("QualityNormals",false);
  */
}

ViewProviderCurveNet::~ViewProviderCurveNet()
{

}

void ViewProviderCurveNet::attach(App::DocumentObject *pcFeat)
{
  pcObject = pcFeat;

  // copy the material properties of the feature
  setMatFromObject();

  if ( getAsFeature()->getStatus() ==  App::AbstractFeature::Error )
    return; // feature is invalid

//  TopoDS_Shape cShape = (dynamic_cast<Part::PartFeature*>(pcFeature))->getShape();

  /*
  try{
    computeEdges   (pcRoot,cShape);
    computeVertices(pcRoot,cShape);
  } catch (...){
    Base::Console().Error("ViewProviderPart::create() Cannot compute Inventor representation for the actual shape");
  }
*/

  // setup the root and material for the edges
  EdgeRoot = new SoSeparator();
  pcRoot->addChild(EdgeRoot);
  EdgeRoot->addChild(pcLineStyle);  
  EdgeRoot->addChild(pcLineMaterial);  


  // setup the root and material for the vertexes
  VertexRoot = new SoSeparator();
  pcRoot->addChild(VertexRoot);
  VertexRoot->addChild(pcPointMaterial);
  SoComplexity *copl = new SoComplexity();
  copl->value = (float)0.2;
  VertexRoot->addChild(copl);



}



// **********************************************************************************


void ViewProviderCurveNet::setEdit(void)
{
  bInEdit = true;
  //getWidget()->setCursor( QCursor( 13 /*ArrowCursor*/) )

}

void ViewProviderCurveNet::unsetEdit(void)
{
  bInEdit = false;

}

bool ViewProviderCurveNet::handleEvent(const SoEvent * const ev, Gui::View3DInventorViewer &Viewer)
{
  SbVec3f point, norm;

  // get the position of the mouse
  const SbVec2s pos(ev->getPosition());

  // Keybooard events
  if (ev->getTypeId().isDerivedFrom(SoKeyboardEvent::getClassTypeId())) {
    SoKeyboardEvent * ke = (SoKeyboardEvent *)ev;
    switch (ke->getKey()) {
    case SoKeyboardEvent::LEFT_ALT:
    case SoKeyboardEvent::RIGHT_ALT:
    case SoKeyboardEvent::LEFT_CONTROL:
    case SoKeyboardEvent::RIGHT_CONTROL:
    case SoKeyboardEvent::LEFT_SHIFT:
    case SoKeyboardEvent::RIGHT_SHIFT:
      break;
    default:
      break;
    }
  }

  // switching the mouse buttons
  if (ev->getTypeId().isDerivedFrom(SoMouseButtonEvent::getClassTypeId())) {

    const SoMouseButtonEvent * const event = (const SoMouseButtonEvent *) ev;
    const int button = event->getButton();
    const SbBool press = event->getState() == SoButtonEvent::DOWN ? TRUE : FALSE;

    // which button pressed?
    switch (button) {
    case SoMouseButtonEvent::BUTTON1:
      if(press)
      {
        Base::Console().Log("ViewProviderCurveNet::handleEvent() press left\n");

        bool bIsNode =  false;
        for(std::list<Node>::iterator It = NodeList.begin();It != NodeList.end(); It++)
        {
          if(It->pcHighlight->isHighlighted())
          {
            bIsNode = true;
            PointToMove = *It;
            break;
          }
        }

        if(bIsNode)
        {
          // set the provider in point move mode and remember the point
          bMovePointMode = true;
          // PointToMove = *It; ### Error 'It' is out of scope->move inside the loop


          return true;
        }else if(Viewer.pickPoint(pos,point,norm))
        {
          Node n;
          Base::Console().Log("Picked(%f,%f,%f)\n",point[0],point[1],point[2]);

          SoSeparator *TransRoot = new SoSeparator();
          n.pcTransform          = new SoTransform();
          TransRoot->addChild(n.pcTransform);
          n.pcTransform->translation.setValue(point);
          n.pcHighlight          = new Gui::SoFCSelection();
          SoSphere * sphere      = new SoSphere;
          sphere->radius = (float)pcLineStyle->pointSize.getValue();
          n.pcHighlight->addChild(sphere);
          TransRoot->addChild(n.pcHighlight);
          VertexRoot->addChild(TransRoot);

          NodeList.push_back(n);
          
          return true;
          
        }

      } else // if(pressd)..
      {
        if(bMovePointMode)
        {
          bMovePointMode = false;
          return true;
        }
      }
      break;
    }

  }

    // Mouse Movement handling
  if (ev->getTypeId().isDerivedFrom(SoLocation2Event::getClassTypeId())) {
//    const SoLocation2Event * const event = (const SoLocation2Event *) ev;

    if(bMovePointMode && Viewer.pickPoint(pos,point,norm) ){
      PointToMove.pcTransform->translation.setValue(point);     
      return true;
    }
  }

  // event not processed
  return false;
}





Standard_Boolean ViewProviderCurveNet::computeEdges   (SoSeparator* root, const TopoDS_Shape &myShape)
{
  unsigned long ulNbOfPoints = 50;

  TopExp_Explorer ex;
  SoSeparator *EdgeRoot = new SoSeparator();
  root->addChild(EdgeRoot);

  EdgeRoot->addChild(pcLineStyle);  
  EdgeRoot->addChild(pcLineMaterial);  

  for (ex.Init(myShape, TopAbs_EDGE); ex.More(); ex.Next())
  {
    // get the shape and mesh it
    const TopoDS_Edge& aEdge = TopoDS::Edge(ex.Current());

    Standard_Real fBegin, fEnd;
    SbVec3f* vertices = new SbVec3f[ulNbOfPoints];

    Handle(Geom_Curve) hCurve = BRep_Tool::Curve(aEdge,fBegin,fEnd);
    float fLen   = float(fEnd - fBegin);

    for (unsigned long i = 0; i < ulNbOfPoints; i++)
    {
      gp_Pnt gpPt = hCurve->Value(fBegin + (fLen * float(i)) / float(ulNbOfPoints-1));
      //rclPoints.push_back(Vector3D(gpPt.X(),gpPt.Y(),gpPt.Z()));
      vertices[i].setValue((float)(gpPt.X()),(float)(gpPt.Y()),(float)(gpPt.Z()));
    }

    // define vertices
    SoCoordinate3 * coords = new SoCoordinate3;
    coords->point.setValues(0,ulNbOfPoints, vertices);
    EdgeRoot->addChild(coords);

    // define the indexed face set
    SoLocateHighlight* h = new SoLocateHighlight();
    h->color.setValue((float)0.2,(float)0.5,(float)0.2);

    SoLineSet * lineset = new SoLineSet;
    h->addChild(lineset);
    EdgeRoot->addChild(h);

  }

  return true;
}


Standard_Boolean ViewProviderCurveNet::computeVertices(SoSeparator* root, const TopoDS_Shape &myShape)
{
  TopExp_Explorer ex;
  SoSeparator *VertexRoot = new SoSeparator();
  root->addChild(VertexRoot);

  VertexRoot->addChild(pcPointMaterial);

  SoComplexity *copl = new SoComplexity();
  copl->value = (float)0.2;
  VertexRoot->addChild(copl);

  for (ex.Init(myShape, TopAbs_VERTEX); ex.More(); ex.Next()) {

    // get the shape 
		const TopoDS_Vertex& aVertex = TopoDS::Vertex(ex.Current());
    gp_Pnt gpPt = BRep_Tool::Pnt (aVertex);

    SoSeparator *TransRoot = new SoSeparator();
    SoTransform *Trans     = new SoTransform();
    TransRoot->addChild(Trans);
    Trans->translation.setValue((float)(gpPt.X()),(float)(gpPt.Y()),(float)(gpPt.Z()));

    SoLocateHighlight* h = new SoLocateHighlight();
    h->color.setValue((float)0.2,(float)0.5,(float)0.2);

    SoSphere * sphere = new SoSphere;
    sphere->radius = (float)pcPointStyle->pointSize.getValue();

    h->addChild(sphere);
    TransRoot->addChild(h);
    VertexRoot->addChild(TransRoot);


  }

  return true;
}





