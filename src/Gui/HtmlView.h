/***************************************************************************
                          HtmlView.h  -  description
                             -------------------
    begin                : 2002/10/19 10:47:44
    copyright            : (C) 2002 by Werner Mayer
    email                : werner.wm.mayer@gmx.de
 ***************************************************************************/

/** \file $RCSfile$
 *  \brief The Html view module
 *  \author Werner Mayer
 *  \version $Revision$
 *  \date    $Date$
 */


/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *   for detail see the LICENCE text file.                                 *
 *   Werner Mayer 2002                                                     *
 *                                                                         *
 ***************************************************************************/
 

#ifndef __HTML_VIEW_H__
#define __HTML_VIEW_H__
 
#include <qtextbrowser.h>
#include "window.h"

class FCTextBrowser : public QTextBrowser
{
  Q_OBJECT

  public:
    FCTextBrowser(QWidget * parent=0, const char * name=0);

    virtual void setText (const QString & contents, const QString & context=QString::null);
    virtual void setSource (const QString & name);

  signals:
    void showPopupMenu();
    void startExtBrowser(QString);

  protected:
    virtual void viewportMousePressEvent (QMouseEvent * e);
};

class FCComboBox : public QComboBox
{
  Q_OBJECT

  public:
    FCComboBox ( QWidget * parent=0, const char * name=0 );
    FCComboBox ( bool rw, QWidget * parent=0, const char * name=0 );

  signals:
    void returnPressed(QString text);

  protected:
    void keyPressEvent ( QKeyEvent * e );
    int iCt;
};

class GuiExport FCHtmlView : public FCDockWindow
{ 
    Q_OBJECT

  public:
    FCHtmlView( const QString& home_, QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~FCHtmlView();

    void SetEnableHistory  (bool b=true);
    void SetEnableBookmarks(bool b=true);
    bool SetMaxHistory (long lCnt);

  protected slots:
    void SetBackwardAvailable( bool );
    void SetForwardAvailable ( bool );
    void TextChanged();
    void OpenFile();
    void PathSelected( const QString & );
    void HistChosen( int );
    void BookmChosen( int );
    void AddBookmark();
    void CheckBookmarks();
    void RefreshPage();
    void PopupMenuAboutToShow();
    void ShowPopupMenu();
    void StartScriptOrBrowser(QString text);
    void StartExtBrowser(QString);

  protected:
    virtual QString GetRelativeURL (const QString& path) const;
    virtual QString GetAbsoluteURL (const QString& path) const;
    virtual QString GetHelpDirectory();
    virtual QString GetScriptDirectory();
    virtual void ReadHistory();
    virtual void ReadBookmarks();
    virtual void SaveHistory();
    virtual void SaveBookmarks();
    virtual void CreateHistoryPopup();
    virtual void CreateBookmarkPopup();
    virtual void AddToPath (const QString& path);
    virtual void StartScript (QString path, QString protocol);
    virtual void StartBrowser(QString path, QString protocol);

#	pragma warning( disable : 4251 )
    FCmap<int, QString> mHistory, mBookmarks;
    bool bBackward, bForward;
    bool bHistory, bBookm;
    QButtonGroup*  pclButtonGrp;
    QToolButton*   pclButtonBack;
    QToolButton*   pclButtonForward;
    QToolButton*   pclButtonHome;
    FCComboBox*    pclPathCombo;
    FCTextBrowser* pclBrowser;
    QString        selectedURL;
    QString        m_strDocDir;
    QString        m_strCaption;
    QGridLayout*   pclFormLayout;
    QHBoxLayout*   pclButtonGrpLayout;
    QPopupMenu*    pclPopup;
    QPopupMenu*    pclHistory;
    QPopupMenu*    pclBookm;
};

#endif // __HTML_VIEW_H__