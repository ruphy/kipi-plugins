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

#ifndef IPTCEDITDIALOG_H
#define IPTCEDITDIALOG_H

// KDE includes.

#include <kpagedialog.h>
#include <kurl.h>

class QCloseEvent;
class QEvent;
class QObject;

namespace KIPI
{
class Interface;
}

namespace KIPIMetadataEditPlugin
{

class IPTCEditDialogPrivate;

class IPTCEditDialog : public KPageDialog
{
    Q_OBJECT

public:

    IPTCEditDialog(QWidget* parent, KUrl::List urls, KIPI::Interface *iface);
    ~IPTCEditDialog();

public slots:

    void slotModified();

protected slots:

    void slotOk();
    void slotHelp();
    void slotClose();

protected:

    void closeEvent(QCloseEvent *);
    bool eventFilter(QObject *, QEvent *);

private slots:

    void slotItemChanged();
    void slotApply();
    void slotUser1();
    void slotUser2();

private:

    void readSettings();
    void saveSettings();

    int  activePageIndex();
    void showPage(int page);

private:

    IPTCEditDialogPrivate* const d;
};

}  // namespace KIPIMetadataEditPlugin

#endif /* IPTCEDITDIALOG_H */
