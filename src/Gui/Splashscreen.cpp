/***************************************************************************
                          Splashscreen.cpp  -  description
                             -------------------
    begin                : 2002/08/19 21:11:52
    copyright            : (C) 2002 by Werner Mayer
    email                : werner.wm.mayer@gmx.de
 ***************************************************************************/

/** \file Splashscreen.cpp
 *  \brief The splash screen module
 *  \author Werner Mayer
 *  \version 0.1
 *  \date    2003/01/06
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




#include "PreCompiled.h"

#include "../Version.h"
#ifndef _PreComp_
#	include <qapplication.h>
#	include <qmessagebox.h>
#	include <qiconview.h>
#	include <qfiledialog.h>
#	include <qimage.h>
#	include <qlabel.h>
#	include <qlayout.h>
#	include <qpixmap.h>
#	include <qprogressbar.h>
#	include <qpushbutton.h>
#	include <qsplashscreen.h>
#	include <qstylefactory.h>
#	include <qtextbrowser.h>
#	include <qtextview.h>
#	include <qthread.h>
#	include <qtooltip.h>
#	include <qvariant.h>
#	include <qwhatsthis.h>
#	include <qcheckbox.h>
#endif

#include "Splashscreen.h"
#include "Icons/developers.h"
#include "../Base/Console.h"


class FCSplashObserver : public FCConsoleObserver
{
  public:
    FCSplashObserver(QSplashScreen* splasher=0, const char* name=0)
			: splash(splasher)
		{
		  GetConsole().AttacheObserver(this);
		}
    
		virtual ~FCSplashObserver()
		{
		  GetConsole().DetacheObserver(this);
		}

    void Warning(const char * s)
		{
			Log(s);
		}
	  void Message(const char * s)
		{
			Log(s);
		}
    void Error  (const char * s)
		{
			Log(s);
		}
	  void Log (const char * s)
		{
			QString msg(s);
			// remove leading blanks and carriage returns
			while (msg[0] == '\n' || msg[0] == ' ')
			{
				msg = msg.mid(1);
			}
			msg = QString("\n %1").arg(msg);
			splash->message( msg, Qt::AlignTop|Qt::AlignLeft, Qt::black );
			qApp->processEvents();
			QWaitCondition().wait(50);
		}

		private:
			QSplashScreen* splash;
};

// ------------------------------------------------------------------------------

FCSplashScreen::FCSplashScreen(  const QPixmap & pixmap , WFlags f )
  : QSplashScreen( pixmap, f), progBar(0L)
{
	// write the messages to splasher
	messages = new FCSplashObserver(this);

	// append also a progressbar for visual feedback
  progBar = new QProgressBar( this, "SplasherProgress" );
  progBar->setProperty( "progress", 0 );
  progBar->setStyle(QStyleFactory::create("motif"));
	progBar->setFixedSize(width()-6, 15);

	// make the splasher a bit higher
	resize(width(), height()+progBar->height());
	progBar->move(3, height()-(progBar->height()));
}

FCSplashScreen::~FCSplashScreen()
{
	delete messages;
	delete progBar;
}

void FCSplashScreen::drawContents ( QPainter * painter )
{
	if (progBar)
	  progBar->setProgress(progBar->progress() + 6);
	QSplashScreen::drawContents(painter);
}

// ------------------------------------------------------------------------------

AboutDlg::AboutDlg( QWidget* parent, const char* name )
	: QDialog( parent, name, true,	QLabel::WStyle_Customize  | 
	                                QLabel::WStyle_NoBorder   | 
	                                QLabel::WType_Modal       ),
      image0( (const char **) splash_screen )
{
  if ( !name )
		setName( "AboutDlg" );
	Form1Layout = new QGridLayout( this, 1, 1, 11, 6, "AboutDlgLayout"); 

	pixmapLabel1 = new QLabel( this, "pixmapLabel1" );
	pixmapLabel1->setPixmap( image0 );
	pixmapLabel1->setScaledContents( TRUE );

	Form1Layout->addWidget( pixmapLabel1, 0, 0 );

	layout1 = new QHBoxLayout( 0, 0, 6, "layout1"); 
	QSpacerItem* spacer = new QSpacerItem( 116, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout1->addItem( spacer );

	pushButton1 = new QPushButton( this, "pushButton1" );
	pushButton1->setDefault( TRUE );
	layout1->addWidget( pushButton1 );
	QSpacerItem* spacer_2 = new QSpacerItem( 116, 31, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout1->addItem( spacer_2 );

	Form1Layout->addLayout( layout1, 2, 0 );

	textLabel1 = new QLabel( this, "textLabel1" );
	textLabel1->setAlignment( int( QLabel::AlignCenter ) );

	Form1Layout->addWidget( textLabel1, 1, 0 );
	languageChange();
	clearWState( WState_Polished );

	connect(pushButton1, SIGNAL(clicked()), this, SLOT(accept()));
}

/*
 *  Destroys the object and frees any allocated resources
 */
AboutDlg::~AboutDlg()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void AboutDlg::languageChange()
{
    pushButton1->setText( tr( "&OK" ) );
    pushButton1->setAccel( QKeySequence( tr( "Alt+O" ) ) );
	  QString SplasherText = QString(
    "<!DOCTYPE HTML PUBLIC ""-//W3C//DTD HTML 4.0 Transitional//EN"">"
    "<html>"
    "<body bgcolor=""#ffffff"">"
    "<p>"
    "<table cellpadding=2 cellspacing=1 border=0  width=100% bgcolor=#E5E5E5 >"
    "<tr>"
    "<th bgcolor=#E5E5E5 width=33%>"
    "<ul>"
    "<a href=""FreeCAD"">FreeCAD (c) 2001 J�rgen Riegel</a>\n"
    "</ul>"
    "</th>"
    "</tr>"
    "<tr>"
    "<td>"
    "<p>Version&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>%1.%2 </b></p>"
    "<p>Build number&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;                             <b>%3</b></p>"
    "<p>Build date&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <b>%4</b></p>"
    "</td>"
    "</tr>"
    "<tr>"
    "<td>"
    "<ul>"
    "</ul>"
    "</td>"
    "</tr>"
    "</table>"
    "</body></html>").arg(FCVersionMajor).arg(FCVersionMinor).arg(FCVersionBuild).arg(FCVersionDisDa);
    textLabel1->setText( SplasherText );

}

#include "moc_Splashscreen.cpp"
