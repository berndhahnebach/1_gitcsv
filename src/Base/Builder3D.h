/***************************************************************************
 *   Copyright (c) YEAR YOUR NAME         <Your e-mail address>            *
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


#ifndef __BUILDER3D_H__
#define __BUILDER3D_H__

// Std. configurations

#include <sstream>
#include "Vector3D.h"

namespace Base
{
/** A Builder class for 3D representations on App level
 * On application level is nothing known of the visual representation of data.
 * Nevertheless it's often needed to see some 3D information, e.g. points, directions,
 * when you program or debug an algorithm. For that purpose Builder3D was made.
 * This class allows you to build up easily a 3D representation of some math and
 * algorithm internals. You can save this representation to a file and see it in an
 * Inventor viewer, or put it to the log. In case of the log and a debug FreeCAD
 * the representation will be loaded into the active viewer.
 *  \par
 * The usage is the following. Create the a Builder3D object and call the methods to insert
 * the graphical elements. After that call either saveToLog() or saveToFile().
 *  \par
 * Usage:
 *  \code
  Base::Builder3D log3D;
  for ( unsigned long i=0; i<pMesh->CountPoints(); i++ )
  {
    log3D.addSinglePoint(pMesh->GetPoint(i));
    log3D.addText(pMesh->GetPoint(i),"Point");
    ...
  }
  log3D.saveToLog();
 *  \endcode
 * \see Base::ConsoleSingleton
 */
class BaseExport Builder3D
{
public:
  /// Construction
  Builder3D();
  /// Destruction
  virtual ~Builder3D();

  /** @name point set handling */
  //@{
  /// starts a point set
  void startPoints(short pointSize=2.0, float color_r=1.0,float color_g=0.0,float color_b=0.0);
  /// insert a point in an point set
  void addPoint(float x, float y, float z);
  /// add a vector to a point set
  void addPoint(const Vector3f &vec);
  /// ends the points set operation 
  void endPoints(void);
  /// add a singular point (without startPoints() & endPoints() )
  void addSinglePoint(float x, float y, float z, short pointSize=2, float color_r=1.0,float color_g=1.0,float color_b=1.0);
  /// add a singular point (without startPoints() & endPoints() )
  void addSinglePoint(const Base::Vector3f &vec, short pointSize=2, float color_r=1.0,float color_g=1.0,float color_b=1.0);
  //@}

  /** @name line/direction handling */
  //@{
  /// add a line defined by 2 Vector3D
  void addSingleLine(Vector3f pt1, Vector3f pt2, short lineSize=2, float color_r=1.0,float color_g=1.0,float color_b=1.0, unsigned short linePattern = 0xffff);
  /// add a arrow (directed line) by 2 Vector3D. The arrow shows in direction of point 2.
  void addSingleArrow(Vector3f pt1, Vector3f pt2, short lineSize=2, float color_r=1.0,float color_g=1.0,float color_b=1.0, unsigned short linePattern = 0xffff);
  //@}

  /** @name triangle handling */
  //@{
  /// add a (filled) triangle defined by 3 vectors
  void addSingleTriangle(Vector3f pt0, Vector3f pt1, Vector3f pt2, bool filled = true, short lineSize=2, float color_r=1.0,float color_g=1.0,float color_b=1.0);
  //@}

  /** @name text handling */
  //@{
  /// add a text
  void addText(float pos_x, float pos_y , float pos_z,const char * text, float color_r=1.0,float color_g=1.0,float color_b=1.0);
  /// add a text
  void addText(const Base::Vector3f &vec,const char * text, float color_r=1.0,float color_g=1.0,float color_b=1.0);
  /// add a text
  void addText(const Base::Vector3f &vec, float color_r,float color_g,float color_b, const char * format, ...);
  //@}

  /// clear the string buffer
  void clear (void);

  /** @name write the result */
  //@{
  /// puts the result to the log and gui
  void saveToLog(void);
  /// save the result to a file (*.iv)
  void saveToFile(const char* FileName);
  //@}

private:
  /// the result string
  std::stringstream result;

  bool bStartEndOpen;

};

} //namespace Base

#endif // __BUILDER3D_H__

