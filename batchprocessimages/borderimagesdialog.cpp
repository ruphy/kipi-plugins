/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2004-10-01
 * Description : a kipi plugin to batch process images
 *
 * Copyright (C) 2004-2007 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

// Include files for Qt

#include <q3groupbox.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qpushbutton.h>

// Include files for KDE

#include <klocale.h>
#include <kconfig.h>
#include <kmessagebox.h>
#include <knuminput.h>
#include <kprocess.h>
#include <kcolorbutton.h>
#include <kapplication.h>
#include <khelpmenu.h>
#include <kiconloader.h>
#include <kmenu.h>
#include <ktoolinvocation.h>

// Local includes

#include "dialogutils.h"
#include "pluginsversion.h"
#include "kpaboutdata.h"
#include "outputdialog.h"
#include "imagepreview.h"
#include "borderoptionsdialog.h"
#include "borderimagesdialog.h"
#include "borderimagesdialog.moc"

namespace KIPIBatchProcessImagesPlugin
{

BorderImagesDialog::BorderImagesDialog( KUrl::List urlList, KIPI::Interface* interface, QWidget *parent )
                  : BatchProcessImagesDialog( urlList, interface, i18n("Batch-Bordering Images"), parent )
{
    // About data and help button.

    m_about = new KIPIPlugins::KPAboutData(ki18n("Batch Image-bordering"),
                                           QByteArray(),
                                           KAboutData::License_GPL,
                                           ki18n("A Kipi plugin for batch bordering images\n"
                                                      "This plugin use the \"convert\" program from \"ImageMagick\" package."),
                                           ki18n("(c) 2003-2008, Gilles Caulier"));

    m_about->addAuthor(ki18n("Gilles Caulier"), ki18n("Author and maintainer"),
                     "caulier dot gilles at gmail dot com");
 
    DialogUtils::setupHelpButton(this, m_about);
    //---------------------------------------------

    m_nbItem = m_selectedImageFiles.count();
    
    //---------------------------------------------

    groupBox1->setTitle( i18n("Images Bordering Options") );

    m_labelType->setText( i18n("Border:") );

    m_Type->insertItem(i18n("Solid"));
    
    // Niepce is Real name. This is the first guy in the world to have build a camera.
    m_Type->insertItem("Niepce");

    m_Type->insertItem(i18n("Raise"));
    m_Type->insertItem(i18n("Frame"));
    m_Type->setCurrentText(i18n("Niepce"));
    whatsThis = i18n("<p>Select here the border type for your images:<p>"
                     "<b>Solid</b>: just surround the images with a line.<p>"
                     "<b>Niepce</b>: surround the images with a fine line and a large border "
                     "(ideal for black and white pictures).<p>"
                     "<b>Raise</b>: creating a 3D effect around the images.<p>"
                     "<b>Frame</b>: surround the images with an ornamental border.<p>");

    m_Type->setWhatsThis(whatsThis );

    //---------------------------------------------

    readSettings();
    listImageFiles();
}

BorderImagesDialog::~BorderImagesDialog()
{
    delete m_about;
}

void BorderImagesDialog::slotHelp( void )
{
    KToolInvocation::invokeHelp("borderimages", "kipi-plugins");
}

void BorderImagesDialog::slotOptionsClicked(void)
{
    int Type = m_Type->currentItem();
    BorderOptionsDialog *optionsDialog = new BorderOptionsDialog(this, Type);

    if ( Type == 0 )  // Solid
       {
       optionsDialog->m_solidBorderWidth->setValue(m_solidWidth);
       optionsDialog->m_button_solidBorderColor->setColor(m_solidColor);
       }
    if ( Type == 1 ) // Niepce
       {
       optionsDialog->m_lineNiepceBorderWidth->setValue(m_lineNiepceWidth);
       optionsDialog->m_button_lineNiepceBorderColor->setColor(m_lineNiepceColor);
       optionsDialog->m_NiepceBorderWidth->setValue(m_NiepceWidth);
       optionsDialog->m_button_NiepceBorderColor->setColor(m_NiepceColor);
       }
    if ( Type == 2 ) // Raise
       {
       optionsDialog->m_raiseBorderWidth->setValue(m_raiseWidth);
       }
    if ( Type == 3 ) // Frame
       {
       optionsDialog->m_frameBorderWidth->setValue(m_frameWidth);
       optionsDialog->m_frameBevelBorderWidth->setValue(m_bevelWidth);
       optionsDialog->m_button_frameBorderColor->setColor(m_frameColor);
       }

    if ( optionsDialog->exec() == KMessageBox::Ok )
       {
       if ( Type == 0 ) // Solid
          {
          m_solidWidth = optionsDialog->m_solidBorderWidth->value();
          m_solidColor = optionsDialog->m_button_solidBorderColor->color();
          }
       if ( Type == 1 ) // Niepce
          {
          m_lineNiepceWidth = optionsDialog->m_lineNiepceBorderWidth->value();
          m_lineNiepceColor = optionsDialog->m_button_lineNiepceBorderColor->color();
          m_NiepceWidth = optionsDialog->m_NiepceBorderWidth->value();
          m_NiepceColor = optionsDialog->m_button_NiepceBorderColor->color();
          }
       if ( Type == 2 ) // Raise
          {
          m_raiseWidth = optionsDialog->m_raiseBorderWidth->value();
          }
       if ( Type == 3 ) // Frame
          {
          m_frameWidth = optionsDialog->m_frameBorderWidth->value();
          m_bevelWidth = optionsDialog->m_frameBevelBorderWidth->value();
          m_frameColor = optionsDialog->m_button_frameBorderColor->color();
          }
       }

   delete optionsDialog;
}

void BorderImagesDialog::readSettings(void)
{
    // Read all settings from configuration file.

    KConfig config("kipirc");
    KConfigGroup group = config.group("BorderImages Settings");

    m_Type->setCurrentItem(group.readEntry("BorderType", 1));     // Niepce per default.
    m_solidWidth = group.readEntry("SolidWidth", 25);
    m_solidColor = group.readEntry("SolidColor", QColor(Qt::black));

    m_lineNiepceWidth = group.readEntry("LineNiepceWidth", 10);
    m_lineNiepceColor = group.readEntry("LineNiepceColor", QColor(Qt::black));
    m_NiepceWidth = group.readEntry("NiepceWidth", 100);
    m_NiepceColor = group.readEntry("NiepceColor", QColor(Qt::white));

    m_raiseWidth = group.readEntry("RaiseWidth", 50);

    m_frameWidth = group.readEntry("FrameWidth", 25);
    m_bevelWidth = group.readEntry("BevelWidth", 10);
    m_frameColor = group.readEntry("FrameColor", QColor(Qt::black));

    if (group.readEntry("SmallPreview", "true") == "true")
        m_smallPreview->setChecked( true );
    else
        m_smallPreview->setChecked( false );

    m_overWriteMode->setCurrentItem(group.readEntry("OverWriteMode", 2));  // 'Rename' per default...

    if (group.readEntry("RemoveOriginal", "false") == "true")
        m_removeOriginal->setChecked( true );
    else
        m_removeOriginal->setChecked( false );
}

void BorderImagesDialog::saveSettings(void)
{
    // Write all settings in configuration file.

    KConfig config("kipirc");
    KConfigGroup group = config.group("BorderImages Settings");
    group.writeEntry("BorderType", m_Type->currentItem());

    group.writeEntry("SolidWidth", m_solidWidth);
    group.writeEntry("SolidColor", m_solidColor);

    group.writeEntry("LineNiepceWidth", m_lineNiepceWidth);
    group.writeEntry("LineNiepceColor", m_lineNiepceColor);
    group.writeEntry("NiepceWidth", m_NiepceWidth);
    group.writeEntry("NiepceColor", m_NiepceColor);

    group.writeEntry("RaiseWidth", m_raiseWidth);

    group.writeEntry("FrameWidth", m_frameWidth);
    group.writeEntry("BevelWidth", m_bevelWidth);
    group.writeEntry("FrameColor", m_frameColor);

    group.writeEntry("SmallPreview", m_smallPreview->isChecked());
    group.writeEntry("OverWriteMode", m_overWriteMode->currentItem());
    group.writeEntry("RemoveOriginal", m_removeOriginal->isChecked());


}

void BorderImagesDialog::initProcess(KProcess* proc, BatchProcessImagesItem *item,
                                        const QString& albumDest, bool previewMode)
{
    *proc << "convert";

    if ( previewMode && m_smallPreview->isChecked() )    // Preview mode and small preview enabled !
       {
       *m_PreviewProc << "-crop" << "300x300+0+0";
       m_previewOutput.append( " -crop 300x300+0+0 ");
       }

    if (m_Type->currentItem() == 0) // Solid
       {
       *proc << "-border";
       QString Temp, Temp2;
       Temp2 = Temp.setNum( m_solidWidth ) + "x";
       Temp2.append(Temp.setNum( m_solidWidth ));
       *proc << Temp2;
       *proc << "-bordercolor";
       Temp2 = "rgb(" + Temp.setNum(m_solidColor.red()) + ",";
       Temp2.append(Temp.setNum(m_solidColor.green()) + ",");
       Temp2.append(Temp.setNum(m_solidColor.blue()) + ")");
       *proc << Temp2;
       }

    if (m_Type->currentItem() == 1) // Niepce
       {
       QString Temp, Temp2;

       *proc << "-border";
       Temp2 = Temp.setNum( m_lineNiepceWidth ) + "x";
       Temp2.append(Temp.setNum( m_lineNiepceWidth ));
       *proc << Temp2;

       *proc << "-bordercolor";
       Temp2 = "rgb(" + Temp.setNum(m_NiepceColor.red()) + ",";
       Temp2.append(Temp.setNum(m_NiepceColor.green()) + ",");
       Temp2.append(Temp.setNum(m_NiepceColor.blue()) + ")");
       *proc << Temp2;

       *proc << "-border";
       Temp2 = Temp.setNum( m_NiepceWidth ) + "x";
       Temp2.append(Temp.setNum( m_NiepceWidth ));
       *proc << Temp2;

       *proc << "-bordercolor";
       Temp2 = "rgb(" + Temp.setNum(m_lineNiepceColor.red()) + ",";
       Temp2.append(Temp.setNum(m_lineNiepceColor.green()) + ",");
       Temp2.append(Temp.setNum(m_lineNiepceColor.blue()) + ")");
       *proc << Temp2;
       }

    if (m_Type->currentItem() == 2) // Raise
       {
       *proc << "-raise";
       QString Temp, Temp2;
       Temp2 = Temp.setNum( m_raiseWidth ) + "x";
       Temp2.append(Temp.setNum( m_raiseWidth ));
       *proc << Temp2;
       }

    if (m_Type->currentItem() == 3) // Frame
       {
       *proc << "-frame";
       QString Temp, Temp2;
       Temp2 = Temp.setNum( m_frameWidth ) + "x";
       Temp2.append(Temp.setNum( m_frameWidth ) + "+");
       Temp2.append(Temp.setNum( m_bevelWidth ) + "+");
       Temp2.append(Temp.setNum( m_bevelWidth ) );
       *proc << Temp2;
       *proc << "-mattecolor";
       Temp2 = "rgb(" + Temp.setNum(m_frameColor.red()) + ",";
       Temp2.append(Temp.setNum(m_frameColor.green()) + ",");
       Temp2.append(Temp.setNum(m_frameColor.blue()) + ")");
       *proc << Temp2;
       }

    *proc << "-verbose";

    *proc << item->pathSrc();

    if ( !previewMode )   // No preview mode !
       *proc << albumDest + "/" + item->nameDest();
}

}  // NameSpace KIPIBatchProcessImagesPlugin
