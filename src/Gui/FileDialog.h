/***************************************************************************
 *   Copyright (c) 2004 Werner Mayer <werner.wm.mayer@gmx.de>              *
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


#ifndef ___FILE_DIALOG_H__
#define ___FILE_DIALOG_H__

#ifndef _PreComp_
# include <qfiledialog.h>
# include <qpixmap.h>
# include <qscrollview.h>
#endif

class QCheckBox;

namespace Gui {

/**
 * The FileDialog class provides dialogs that allow users to select files or directories.
 * \author Werner Mayer
 */
class GuiExport FileDialog : public QFileDialog
{
  Q_OBJECT

public:
  static QString getSaveFileName ( const QString & startWith = QString::null, const QString & filter = QString::null, 
                                   QWidget* parent = 0, const char* name = 0, const QString & caption = QString::null );
  static QString getExistingDirectory( const QString & dir = QString::null, QWidget *parent = 0, 
                                       const char* name = 0, const QString& caption = QString::null, 
                                       bool dirOnly = true, bool resolveSymlinks = true );

public:
  FileDialog (Mode mode, QWidget* parent = 0, const char* name = 0, bool modal = false);
  FileDialog (Mode mode, const QString& dirName, const QString& filter = QString::null, QWidget* parent = 0, const char* name = 0, bool modal = false);
  virtual ~FileDialog();

  QString selectedFileName();

protected slots:
  virtual void accept();
};

// ======================================================================

/**
 * The ImagePreview class draws a pixmap on an area.
 * \author Werner Mayer
 */
class ImagePreview : public QScrollView
{
public:
  ImagePreview( QWidget *parent=0, const char* name=0 );

  void setPixmap( const QPixmap &pix );
  void drawContents( QPainter *p, int clipx, int clipy, int clipw, int cliph );

private:
  QPixmap _pixmap;
};

// ======================================================================

/**
 * The PreviewLabel class is a preview widget that can be used with FileDialogs.
 * \author Werner Mayer
 */
class PreviewLabel : public QWidget, public QFilePreview
{
public:
  PreviewLabel( QWidget *parent=0, const char* name =0 );

  void previewUrl( const QUrl &u );

private:
  ImagePreview* _preview;
  QCheckBox* _cbview;
};

// ======================================================================

/**
 * The FileIconProvider class provides icons for FileDialog to use.
 * \author Werner Mayer
 */
class FileIconProvider : public QFileIconProvider
{
public:
  FileIconProvider( QObject * parent = 0, const char * name = 0 );
  ~FileIconProvider();

  const QPixmap * pixmap ( const QFileInfo & info );
};

} // namespace Gui

#endif // ___FILE_DIALOG_H__
