#ifndef __PRECOMPILED_GUI__
#define __PRECOMPILED_GUI__

#include "../FCConfig.h"
#ifdef _PreComp_

/// here get the warnings of to long specifieres disabled (needed for VC6)
#ifdef _MSC_VER
# pragma warning( disable : 4251 )
# pragma warning( disable : 4503 )
# pragma warning( disable : 4786 )  // specifier longer then 255 chars
#endif

// standard
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <ctype.h>
#include <typeinfo>
#include <float.h>
#include <limits.h>

#ifdef FC_OS_WIN32
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#  include <io.h>
#endif

// streams
#include <iostream>
#include <iomanip>


// STL
#include <vector>
#include <map>
#include <string>
#include <list>
#include <set>
#include <algorithm>
#include <stack>
#include <queue>
#include <sstream>

// Python
#include <Python.h>

// Xerces
#include <xercesc/util/XercesDefs.hpp>


// Open Inventor
#include <GL/gl.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/SbClip.h>
#include <Inventor/SbColor.h>
#include <Inventor/SoInput.h>
#include <Inventor/actions/SoHandleEventAction.h> 
#include <Inventor/actions/SoBoxHighlightRenderAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoLineHighlightRenderAction.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/draggers/SoTransformerDragger.h>
#include <Inventor/events/SoEvent.h>
#include <Inventor/fields/SoMFColor.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/manips/SoTransformerManip.h> 
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoCallback.h> 
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoCoordinate4.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoEnvironment.h>
#include <Inventor/nodes/SoExtSelection.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoImage.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoIndexedTriangleStripSet.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoLocateHighlight.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoNurbsSurface.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoQuadMesh.h>
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTexture3.h>
#include <Inventor/nodes/SoTextureCoordinate3.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoProfile.h>
#include <Inventor/nodes/SoProfileCoordinate2.h>
#include <Inventor/nodes/SoProfileCoordinate3.h>
#include <Inventor/events/SoMotion3Event.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/projectors/SbSphereSheetProjector.h>
#include <Inventor/projectors/SbSpherePlaneProjector.h>
#include <Inventor/actions/SoRayPickAction.h> 
#include <Inventor/SoPickedPoint.h> 
#include <Inventor/SoInteraction.h> 



// Qt Toolkit
#include <qaccel.h>
#include <qaction.h>
#include <qapplication.h>
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qclipboard.h>
#include <qcolordialog.h>
#include <qcombobox.h>
#include <qcursor.h>
#include <qdatetimeedit.h>
#include <qdockarea.h>
#include <qdockwindow.h>
#include <qdragobject.h>
#include <qdrawutil.h>
#include <qevent.h>
#include <qeventloop.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qframe.h>
#include <qfontdatabase.h>
#include <qfontdialog.h>
#include <qftp.h>
#include <qgl.h>
#include <qhbox.h>
#include <qheader.h>
#include <qhttp.h>
#include <qiconview.h>
#include <qimage.h>
#include <qinputdialog.h>
#include <qkeycode.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qlistbox.h>
#include <qlistview.h>
#include <qmainwindow.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qmetaobject.h>
#include <qmime.h>
#include <qmovie.h>
#include <qmultilineedit.h>
#include <qnamespace.h>
#include <qobjectlist.h>
#include <qpaintdevicemetrics.h>
#include <qpainter.h>
#include <qpalette.h>
#include <qpixmap.h>
#include <qpopupmenu.h>
#include <qprinter.h>
#include <qprocess.h>
#include <qprogressbar.h>
#include <qprogressdialog.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qrangecontrol.h>
#include <qrect.h>
#include <qregexp.h>
#include <qscrollbar.h>
#include <qsimplerichtext.h>
#include <qslider.h>
#include <qspinbox.h>
#include <qsplashscreen.h>
#include <qsplitter.h>
#include <qstatusbar.h>
#include <qstyle.h>
#include <qstylefactory.h>
#include <qstylesheet.h>
#include <qsyntaxhighlighter.h>
#include <qtabbar.h>
#include <qtabwidget.h>
#include <qtextcodec.h>
#include <qtextedit.h>
#include <qtextbrowser.h>
#include <qtextstream.h>
#include <qthread.h>
#include <qtimer.h>
#include <qtoolbar.h>
#include <qtoolbox.h>
#include <qtoolbutton.h>
#include <qtooltip.h>
#include <qtranslator.h>
#include <qurl.h>
#include <qvalidator.h>
#include <qvaluestack.h>
#include <qvariant.h>
#include <qvbox.h>
#include <qwhatsthis.h>
#include <qwidget.h>
#include <qwidgetfactory.h>
#include <qwidgetstack.h>
#include <qworkspace.h>
#include <private/qrichtext_p.h>


#endif //_PreComp_


#endif // __PRECOMPILED_GUI__
