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


#ifndef __TREE_H_
#define __TREE_H_

#include <qlistview.h>

#include <Base/PyExport.h>
#include <App/Document.h>
#include <App/Application.h>

#include "Selection.h"
#include "DockWindow.h"

/// Forwards
class FCLabel; 

using namespace std;

namespace Gui {
class Document;
class TreeView;
class ViewProviderDocumentObject;

struct GUIDDefs {
    char  *Guid;
    char  *Name;
};

/** The link between the Tree and a document.
 *  Every (shown) document in the Application get it 
 *  associated DocItem which controls the visibility 
 *  and the functions of the Document.
 *  \author J�rgen Riegel
 */
class ObjectItem : public QListViewItem
{
public:
  /// Constructor
  ObjectItem( QListViewItem* parent,Gui::ViewProviderDocumentObject* pcViewProvider);

  /// This is the class Id to distinguish from QListViewItem itself or from other QListViewItem-subclasses. 
  int rtti () const { return 3100; }
  /// Opens the Leafs and generate them.
  void setOpen( bool );
  /// Im not realy sure whats this method do ;-).
  void setup();

  void highlightFeature(bool);

  void selectFeature(bool);

  void paintCell ( QPainter * p, const QColorGroup & cg, int column, int width, int align );

  /// Delivers the pixmap that is shown.
//    const QPixmap *pixmap( int i ) const;
  /// Sets the pixmap that will be shown.
//    void setPixmap( QPixmap *p );

  void update(void);

  void buildUp(void);

  bool testStatus(void);
  void displayStatusInfo();

  QColor BaseColor, TextColor, HighlightColor;

  friend class DocItem;

protected:
  void activate (); 

  bool bHighlight;
  bool bSelected;

  int FeatStatus;


  Gui::ViewProviderDocumentObject*  _pcViewProvider;
};


/** The link between the Tree and a document.
 *  Every (shown) document in the Application get it 
 *  associated DocItem which controls the visibility 
 *  and the functions of the Document.
 *  \author J�rgen Riegel
 */
class DocItem : public QListViewItem//, public App::Document::ObserverType
{
public:
  /// Constructor
  DocItem( QListViewItem* parent,QListViewItem * after,Gui::Document* doc);
  virtual ~DocItem();

  /// This is the class Id to distinguish from QListViewItem itself or from other QListViewItem-subclasses. 
  int rtti () const { return 3000; }
  /// Opens the Leafs and generate them.
  void setOpen( bool );
  /// Im not realy sure whats this method do ;-).
  void setup();

  /// Delivers the pixmap that is shown.
//    const QPixmap *pixmap( int i ) const;
  /// Sets the pixmap that will be shown.
//    void setPixmap( QPixmap *p );

  /** Adds a view provider to the document item and returns true.
   * If this view provider is already added nothing happens and false is returned.
   */
  bool addViewProviderDocumentObject(ViewProviderDocumentObject*);
  /** Removes a view provider from the document item and returns true.
   * If this view provider is not added nothing happens and false is returned.
   */
  bool removeViewProviderDocumentObject(ViewProviderDocumentObject*);

  void highlightFeature(const char*, bool);

  void selectFeature(const char*, bool);
  void clearSelection(void);
  void isSelectionUptodate(void);

  void update(void);

  void buildUp(void);

  bool testStatus(void);

  Gui::Document*  getDocument(void){return _pcDocument;}

  void rename(void);

  /// Observer message from the App doc
//  virtual void OnChange(App::Document::SubjectType &rCaller,App::Document::MessageType Reason);
  void paintCell ( QPainter * p, const QColorGroup & cg, int column, int width, int align );


protected:
  void activate (); 

  bool bHighlight;
  bool bSelected;


  Gui::Document*  _pcDocument;
  std::map<std::string,ObjectItem*> FeatMap;

private:
  QListViewItem* _lastFeaItem;
};
 

 


/** 
 *  \author J�rgen Riegel
 */
class TreeView :public Gui::DockView, public Gui::SelectionSingleton::ObserverType
{
  Q_OBJECT

public:
  TreeView(Gui::Document*  pcDocument,QWidget *parent=0,const char *name=0);
  // App_Tree();

  ~TreeView();

  bool onMsg(const char* pMsg);

  virtual const char *getName(void){return "TreeView";}

    /// Observer message from the Selection
  virtual void OnChange(Gui::SelectionSingleton::SubjectType &rCaller,Gui::SelectionSingleton::MessageType Reason);


  //void InitCascade(Handle(TDocStd_Document) hDoc);
  friend class DocItem;
  friend class ObjectItem;

  /// is called when the above function is called to handle document change stuff
  virtual void onNewDocument(Gui::Document* pcOldDocument,Gui::Document* pcNewDocument);
  /// get called when the document is changed or updated
  virtual void onUpdate(void);
  /// get called when the document is renamed
  virtual void onRename(Gui::Document *);


	/// This method get called when a new doc appears
	DocItem *  NewDoc(Gui::Document*);
	/// This method get called when a new doc will be deleted
  void DeleteDoc(Gui::Document*);

  QListViewItem* getMainItem(void){return _pcMainItem;}

  void testStatus(void);

public slots:

  void onSelectionChanged(void);
  void onItem(QListViewItem*);

protected:

  void contextMenuEvent ( QContextMenuEvent * e );

  QListView*  _pcListView;
  QListViewItem*  _pcMainItem;

  static QPixmap *pcDocumentPixmap;

  map<string,DocItem*> DocMap;

  bool bFromOutside;

private:
  QListViewItem* _lastDocItem;
};

} // namespace Gui

#endif // __TREE_H_

