/**************************************************************************\
 *
 *  This file is part of the Coin GUI toolkit libraries.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation.  See the
 *  file LICENSE.LGPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you want to use this library with software that is incompatible
 *  licensewise with the LGPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

// @configure_input@

#ifndef SOQT_FLYVIEWER_H
#define SOQT_FLYVIEWER_H

#include <Inventor/Qt/viewers/SoQtConstrainedViewer.h>

// ************************************************************************

class SOQT_DLL_API SoQtFlyViewer : public SoQtConstrainedViewer {
  SOQT_OBJECT_HEADER(SoQtFlyViewer, SoQtConstrainedViewer);

public:
  SoQtFlyViewer(QWidget* parent = NULL,
                   const char * name = NULL, 
                   SbBool embed = TRUE, 
                   SoQtFullViewer::BuildFlag flag = BUILD_ALL,
                   SoQtViewer::Type type = BROWSER);
  ~SoQtFlyViewer();

  virtual void setViewing(SbBool enable);
  virtual void viewAll(void);
  virtual void resetToHomePosition(void);
  virtual void setCamera(SoCamera * camera);
  virtual void setCursorEnabled(SbBool enable);
  virtual void setCameraType(SoType type);

protected:
  SoQtFlyViewer(QWidget* parent,
                   const char * const name, 
                   SbBool embed, 
                   SoQtFullViewer::BuildFlag flag, 
                   SoQtViewer::Type type, 
                   SbBool build);

  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

  virtual SbBool processSoEvent(const SoEvent * const event);
  virtual void setSeekMode(SbBool enable);
  virtual void actualRedraw(void);

  virtual void rightWheelMotion(float value);

  virtual void afterRealizeHook(void);

private:
  class SoQtFlyViewerP * pimpl;
#ifndef DOXYGEN_SKIP_THIS
  friend class SoQtFlyViewerP;
#endif // DOXYGEN_SKIP_THIS
};

// ************************************************************************

#endif // ! SOQT_FLYVIEWER_H
