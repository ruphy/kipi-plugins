/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2006-10-12
 * Description : a dialog to edit IPTC metadata
 *
 * Copyright (C) 2006-2009 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include "iptceditdialog.h"
#include "iptceditdialog.moc"

// Qt includes.

#include <QCloseEvent>
#include <QKeyEvent>

// KDE includes.

#include <kapplication.h>
#include <kconfig.h>
#include <kdebug.h>
#include <kguiitem.h>
#include <khelpmenu.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kmenu.h>
#include <kmessagebox.h>
#include <kpushbutton.h>
#include <ktoolinvocation.h>

// LibKIPI includes.

#include <libkipi/imagecollection.h>
#include <libkipi/imageinfo.h>
#include <libkipi/interface.h>
#include <libkipi/plugin.h>

// LibKExiv2 includes.

#include <libkexiv2/version.h>
#include <libkexiv2/kexiv2.h>

// Local includes.

#include "iptccategories.h"
#include "iptccontent.h"
#include "iptccredits.h"
#include "iptcenvelope.h"
#include "iptckeywords.h"
#include "iptcorigin.h"
#include "iptcproperties.h"
#include "iptcstatus.h"
#include "iptcsubjects.h"
#include "kpaboutdata.h"
#include "pluginsversion.h"

namespace KIPIMetadataEditPlugin
{

class IPTCEditDialogPrivate
{

public:

    IPTCEditDialogPrivate()
    {
        modified        = false;
        isReadOnly      = false;
        page_content    = 0;
        page_properties = 0;
        page_subjects   = 0;
        page_keywords   = 0;
        page_categories = 0;
        page_credits    = 0;
        page_status     = 0;
        page_origin     = 0;
        page_envelope   = 0;
        about           = 0;
        contentPage     = 0;
        propertiesPage  = 0;
        subjectsPage    = 0;
        keywordsPage    = 0;
        categoriesPage  = 0;
        creditsPage     = 0;
        statusPage      = 0;
        originPage      = 0;
        envelopePage    = 0;
    }

    bool                      modified;
    bool                      isReadOnly;

    QByteArray                exifData;
    QByteArray                iptcData;

    KPageWidgetItem          *page_content;
    KPageWidgetItem          *page_properties;
    KPageWidgetItem          *page_subjects;
    KPageWidgetItem          *page_keywords;
    KPageWidgetItem          *page_categories;
    KPageWidgetItem          *page_credits;
    KPageWidgetItem          *page_status;
    KPageWidgetItem          *page_origin;
    KPageWidgetItem          *page_envelope;

    KUrl::List                urls;

    KUrl::List::iterator      currItem;

    IPTCContent              *contentPage;
    IPTCProperties           *propertiesPage;
    IPTCSubjects             *subjectsPage;
    IPTCKeywords             *keywordsPage;
    IPTCCategories           *categoriesPage;
    IPTCCredits              *creditsPage;
    IPTCStatus               *statusPage;
    IPTCOrigin               *originPage;
    IPTCEnvelope             *envelopePage;

    KIPI::Interface          *interface;

    KIPIPlugins::KPAboutData *about;
};

IPTCEditDialog::IPTCEditDialog(QWidget* parent, KUrl::List urls, KIPI::Interface *iface)
              : KPageDialog(parent), d(new IPTCEditDialogPrivate)
{
    d->urls      = urls;
    d->interface = iface;
    d->currItem  = d->urls.begin();

    setButtons(d->urls.count() > 1 ? Help|User1|User2|Ok|Apply|Close
                                   : Help|Ok|Apply|Close);
    setDefaultButton(Ok);
    setButtonIcon(User1, KIcon("go-next"));
    setButtonIcon(User2, KIcon("go-previous"));
    setButtonText(User1, i18n("Next"));
    setButtonText(User2, i18n("Previous"));
    setFaceType(List);
    setModal(true);

    // ---------------------------------------------------------------

    d->contentPage   = new IPTCContent(this);
    d->page_content  = addPage(d->contentPage, i18n("Content"));
    d->page_content->setHeader(i18n("<qt>Content Information<br/>"
                     "<i>Use this panel to describe the visual content of the image</i></qt>"));
    d->page_content->setIcon(KIcon("help-contents"));

    d->originPage  = new IPTCOrigin(this);
    d->page_origin = addPage(d->originPage, i18n("Origin"));
    d->page_origin->setHeader(i18n("<qt>Origin Information<br/>"
                    "<i>Use this panel for formal descriptive information about the image</i></qt>"));
    d->page_origin->setIcon(KIcon("applications-internet"));

    d->creditsPage  = new IPTCCredits(this);
    d->page_credits = addPage(d->creditsPage, i18n("Credits"));
    d->page_credits->setHeader(i18n("<qt>Credits Information<br/>"
                     "<i>Use this panel to record copyright information about the image</i></qt>"));
    d->page_credits->setIcon(KIcon("view-pim-contacts"));

    d->subjectsPage  = new IPTCSubjects(this);
    d->page_subjects = addPage(d->subjectsPage, i18n("Subjects"));
    d->page_subjects->setHeader(i18n("<qt>Subjects Information<br/>"
                      "<i>Use this panel to record subjects about the image</i></qt>"));
    d->page_subjects->setIcon(KIcon("feed-subscribe"));

    d->keywordsPage  = new IPTCKeywords(this);
    d->page_keywords = addPage(d->keywordsPage, i18n("Keywords"));
    d->page_keywords->setHeader(i18n("<qt>Keywords Information<br/>"
                      "<i>Use this panel to record keywords about the image</i></qt>"));
    d->page_keywords->setIcon(KIcon("bookmarks"));

    d->categoriesPage  = new IPTCCategories(this);
    d->page_categories = addPage(d->categoriesPage, i18n("Categories"));
    d->page_categories->setHeader(i18n("<qt>Categories Information<br/>"
                        "<i>Use this panel to record categories about the image</i></qt>"));
    d->page_categories->setIcon(KIcon("folder-image"));

    d->statusPage  = new IPTCStatus(this);
    d->page_status = addPage(d->statusPage, i18n("Status"));
    d->page_status->setHeader(i18n("<qt>Status Information<br/>"
                    "<i>Use this panel to record workflow description</i></qt>"));
    d->page_status->setIcon(KIcon("view-pim-tasks"));

    d->propertiesPage  = new IPTCProperties(this);
    d->page_properties = addPage(d->propertiesPage, i18n("Properties"));
    d->page_properties->setHeader(i18n("<qt>Status Properties<br/>"
                      "<i>Use this panel to record workflow properties</i></qt>"));
    d->page_properties->setIcon(KIcon("draw-freehand"));

    d->envelopePage  = new IPTCEnvelope(this);
    d->page_envelope = addPage(d->envelopePage, i18n("Envelope"));
    d->page_envelope->setHeader(i18n("<qt>Envelope Information<br/>"
                      "<i>Use this panel to record editorial description</i></qt>"));
    d->page_envelope->setIcon(KIcon("view-pim-mail"));

    // ---------------------------------------------------------------
    // About data and help button.

    d->about = new KIPIPlugins::KPAboutData(ki18n("Edit Metadata"),
                                            0,
                                            KAboutData::License_GPL,
                                            ki18n("A Plugin to edit pictures metadata"),
                                            ki18n("(c) 2006-2009, Gilles Caulier"));

    d->about->addAuthor(ki18n("Gilles Caulier"), ki18n("Author and Maintainer"),
                        "caulier dot gilles at gmail dot com");

    disconnect(this, SIGNAL(helpClicked()),
               this, SLOT(slotHelp()));

    KHelpMenu* helpMenu = new KHelpMenu(this, d->about, false);
    helpMenu->menu()->removeAction(helpMenu->menu()->actions().first());
    QAction *handbook   = new QAction(i18n("Handbook"), this);
    connect(handbook, SIGNAL(triggered(bool)),
            this, SLOT(slotHelp()));
    helpMenu->menu()->insertAction(helpMenu->menu()->actions().first(), handbook);
    button(Help)->setMenu(helpMenu->menu());

    // ------------------------------------------------------------

    connect(d->contentPage, SIGNAL(signalModified()),
            this, SLOT(slotModified()));

    connect(d->propertiesPage, SIGNAL(signalModified()),
            this, SLOT(slotModified()));

    connect(d->subjectsPage, SIGNAL(signalModified()),
            this, SLOT(slotModified()));

    connect(d->keywordsPage, SIGNAL(signalModified()),
            this, SLOT(slotModified()));

    connect(d->categoriesPage, SIGNAL(signalModified()),
            this, SLOT(slotModified()));

    connect(d->creditsPage, SIGNAL(signalModified()),
            this, SLOT(slotModified()));

    connect(d->statusPage, SIGNAL(signalModified()),
            this, SLOT(slotModified()));

    connect(d->originPage, SIGNAL(signalModified()),
            this, SLOT(slotModified()));

    connect(d->envelopePage, SIGNAL(signalModified()),
            this, SLOT(slotModified()));

    // ------------------------------------------------------------

    connect(this, SIGNAL(user1Clicked()),
            this, SLOT(slotUser1()));

    connect(this, SIGNAL(user2Clicked()),
            this, SLOT(slotUser2()));

    connect(this, SIGNAL(applyClicked()),
            this, SLOT(slotApply()));

    connect(this, SIGNAL(closeClicked()),
            this, SLOT(slotClose()));

    connect(this, SIGNAL(okClicked()),
            this, SLOT(slotOk()));

    // ------------------------------------------------------------

    readSettings();
    slotItemChanged();
}

IPTCEditDialog::~IPTCEditDialog()
{
    delete d->about;
    delete d;
}

void IPTCEditDialog::slotHelp()
{
    KToolInvocation::invokeHelp("metadataedit", "kipi-plugins");
}

void IPTCEditDialog::closeEvent(QCloseEvent *e)
{
    if (!e) return;
    saveSettings();
    e->accept();
}

void IPTCEditDialog::slotClose()
{
    saveSettings();
    close();
}

void IPTCEditDialog::readSettings()
{
    KConfig config("kipirc");
    KConfigGroup group = config.group("Metadata Edit Settings");
    showPage(group.readEntry("IPTC Edit Page", 0));
    d->contentPage->setCheckedSyncJFIFComment(group.readEntry("Sync JFIF Comment", true));
    d->contentPage->setCheckedSyncHOSTComment(group.readEntry("Sync Host Comment", true));
    d->contentPage->setCheckedSyncEXIFComment(group.readEntry("Sync EXIF Comment", true));
    d->originPage->setCheckedSyncHOSTDate(group.readEntry("Sync Host Date", true));
    d->originPage->setCheckedSyncEXIFDate(group.readEntry("Sync EXIF Date", true));
    KConfigGroup group2 = config.group(QString("IPTC Edit Dialog"));
    restoreDialogSize(group2);
}

void IPTCEditDialog::saveSettings()
{
    KConfig config("kipirc");
    KConfigGroup group = config.group("Metadata Edit Settings");
    group.writeEntry("IPTC Edit Page", activePageIndex());
    group.writeEntry("Sync JFIF Comment", d->contentPage->syncJFIFCommentIsChecked());
    group.writeEntry("Sync Host Comment", d->contentPage->syncHOSTCommentIsChecked());
    group.writeEntry("Sync EXIF Comment", d->contentPage->syncEXIFCommentIsChecked());
    group.writeEntry("Sync Host Date", d->originPage->syncHOSTDateIsChecked());
    group.writeEntry("Sync EXIF Date", d->originPage->syncEXIFDateIsChecked());
    KConfigGroup group2 = config.group(QString("IPTC Edit Dialog"));
    saveDialogSize(group2);
    config.sync();
}

void IPTCEditDialog::slotItemChanged()
{
    KExiv2Iface::KExiv2 exiv2Iface;
    exiv2Iface.load((*d->currItem).path());
    d->exifData = exiv2Iface.getExif();
    d->iptcData = exiv2Iface.getIptc();
    d->contentPage->readMetadata(d->iptcData);
    d->originPage->readMetadata(d->iptcData);
    d->creditsPage->readMetadata(d->iptcData);
    d->subjectsPage->readMetadata(d->iptcData);
    d->keywordsPage->readMetadata(d->iptcData);
    d->categoriesPage->readMetadata(d->iptcData);
    d->statusPage->readMetadata(d->iptcData);
    d->propertiesPage->readMetadata(d->iptcData);
    d->envelopePage->readMetadata(d->iptcData);

    d->isReadOnly = !KExiv2Iface::KExiv2::canWriteIptc((*d->currItem).path());
    d->page_content->setEnabled(!d->isReadOnly);
    d->page_origin->setEnabled(!d->isReadOnly);
    d->page_credits->setEnabled(!d->isReadOnly);
    d->page_subjects->setEnabled(!d->isReadOnly);
    d->page_keywords->setEnabled(!d->isReadOnly);
    d->page_categories->setEnabled(!d->isReadOnly);
    d->page_status->setEnabled(!d->isReadOnly);
    d->page_properties->setEnabled(!d->isReadOnly);
    d->page_envelope->setEnabled(!d->isReadOnly);
    enableButton(Apply, !d->isReadOnly);

    setCaption(QString("%1 (%2/%3) - %4")
               .arg((*d->currItem).fileName())
               .arg(d->urls.indexOf(*(d->currItem))+1)
               .arg(d->urls.count())
               .arg(i18n("Edit IPTC Metadata")) +
               (d->isReadOnly ? QString(" - ") + i18n("(read only)") : QString::null));
    enableButton(User1, *(d->currItem) != d->urls.last());
    enableButton(User2, *(d->currItem) != d->urls.first());
    enableButton(Apply, false);
}

void IPTCEditDialog::slotApply()
{
    if (d->modified && !d->isReadOnly)
    {
        KIPI::ImageInfo info = d->interface->info(*d->currItem);

        if (d->contentPage->syncHOSTCommentIsChecked())
        {
            info.setDescription(d->contentPage->getIPTCCaption());
        }
        d->contentPage->applyMetadata(d->exifData, d->iptcData);

        if (d->originPage->syncHOSTDateIsChecked())
        {
            info.setTime(d->originPage->getIPTCCreationDate());
        }
        d->originPage->applyMetadata(d->exifData, d->iptcData);

        d->creditsPage->applyMetadata(d->iptcData);
        d->subjectsPage->applyMetadata(d->iptcData);
        d->keywordsPage->applyMetadata(d->iptcData);
        d->categoriesPage->applyMetadata(d->iptcData);
        d->statusPage->applyMetadata(d->iptcData);
        d->propertiesPage->applyMetadata(d->iptcData);
        d->envelopePage->applyMetadata(d->iptcData);

        KExiv2Iface::KExiv2 exiv2Iface;
        exiv2Iface.setWriteRawFiles(d->interface->hostSetting("WriteMetadataToRAW").toBool());

#if KEXIV2_VERSION >= 0x000600
        exiv2Iface.setUpdateFileTimeStamp(d->interface->hostSetting("WriteMetadataUpdateFiletimeStamp").toBool());
#endif

        exiv2Iface.load((*d->currItem).path());
        exiv2Iface.setExif(d->exifData);
        exiv2Iface.setIptc(d->iptcData);
        exiv2Iface.save((*d->currItem).path());
        d->modified = false;
    }
}

void IPTCEditDialog::slotUser1()
{
    slotApply();
    d->currItem++;
    slotItemChanged();
}

void IPTCEditDialog::slotUser2()
{
    slotApply();
    d->currItem--;
    slotItemChanged();
}

void IPTCEditDialog::slotModified()
{
    if (!d->isReadOnly)
    {
        enableButton(Apply, true);
        d->modified = true;
    }
}

void IPTCEditDialog::slotOk()
{
    slotApply();
    saveSettings();
    accept();
}

bool IPTCEditDialog::eventFilter(QObject *, QEvent *e)
{
    if ( e->type() == QEvent::KeyPress )
    {
        QKeyEvent *k = (QKeyEvent *)e;

        if (k->modifiers() == Qt::ControlModifier &&
            (k->key() == Qt::Key_Enter || k->key() == Qt::Key_Return))
        {
            slotApply();

            if (isButtonEnabled(User1))
                slotUser1();

            return true;
        }
        else if (k->modifiers() == Qt::ShiftModifier &&
                 (k->key() == Qt::Key_Enter || k->key() == Qt::Key_Return))
        {
            slotApply();

            if (isButtonEnabled(User2))
                slotUser2();

            return true;
        }

        return false;
    }

    return false;
}

void IPTCEditDialog::showPage(int page)
{
    switch(page)
    {
        case 0:
            setCurrentPage(d->page_content);
            break;
        case 1:
            setCurrentPage(d->page_origin);
            break;
        case 2:
            setCurrentPage(d->page_credits);
            break;
        case 3:
            setCurrentPage(d->page_subjects);
            break;
        case 4:
            setCurrentPage(d->page_keywords);
            break;
        case 5:
            setCurrentPage(d->page_categories);
            break;
        case 6:
            setCurrentPage(d->page_status);
            break;
        case 7:
            setCurrentPage(d->page_properties);
            break;
        case 8:
            setCurrentPage(d->page_envelope);
            break;
        default:
            setCurrentPage(d->page_content);
            break;
    }
}

int IPTCEditDialog::activePageIndex()
{
    KPageWidgetItem *cur = currentPage();

    if (cur == d->page_content)    return 0;
    if (cur == d->page_origin)     return 1;
    if (cur == d->page_credits)    return 2;
    if (cur == d->page_subjects)   return 3;
    if (cur == d->page_keywords)   return 4;
    if (cur == d->page_categories) return 5;
    if (cur == d->page_status)     return 6;
    if (cur == d->page_properties) return 7;
    if (cur == d->page_envelope)   return 8;

    return 0;
}

}  // namespace KIPIMetadataEditPlugin
