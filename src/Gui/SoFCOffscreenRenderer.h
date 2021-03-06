/***************************************************************************
 *   Copyright (c) 2005 Werner Mayer <werner.wm.mayer@gmx.de>              *
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


#ifndef COIN_SOFCOFFSCREENRENDERER_H
#define COIN_SOFCOFFSCREENRENDERER_H

#include <Inventor/SoOffscreenRenderer.h> 
#include <qstringlist.h>

class QImage;

namespace Gui {

/**
 * The SoFCOffscreenRenderer class is used for rendering scenes in offscreen buffers. 
 * @author Werner Mayer
 */
class GuiExport SoFCOffscreenRenderer : public SoOffscreenRenderer
{
public:
  /**
   * Constructor. Argument is the \a viewportregion we should use when rendering. An internal 
   * SoGLRenderAction will be constructed. 
   */
	SoFCOffscreenRenderer (const SbViewportRegion &viewportregion);
  /**
   * Constructor. Argument is the \a action we should apply to the scene graph when rendering the 
   * scene. Information about the viewport is extracted from the \a action. 
   */
	SoFCOffscreenRenderer (SoGLRenderAction *action);
  /**
   * Destructor. 
   */
	~SoFCOffscreenRenderer();
	/** 
	 * Writes the rendered image buffer directly into a QImage object
	 * instead of an image file.
	 */
	SbBool writeToImage (QImage&, const char * filetypeext="PNG") const;
	/** 
   * Saves the buffer to \a filename, in the filetype specified by \a filetypeextensions.
   *
   * Note that you must still specify the full filename for the first argument, i.e. the second argument will 
   * not automatically be attached to the filename -- it is only used to decide the filetype.
   * 
   * This does basically the same as writeToFile() unless that all QImage file formats are supported if not
   * directly supported by Coin3D.
   */
	SbBool writeToImageFile (const SbString &  filename, const SbName &  filetypeextension ) const;
  /**
   * This method returns all image file formats supported by Coin3D (see getWriteFiletypeInfo()) with all QImage file formats that are 
   * not directly supported by Coin3D, if so.
   */
  QStringList getWriteImageFiletypeInfo();
};

} // namespace Gui


#endif // COIN_SOFCOFFSCREENRENDERER_H 
