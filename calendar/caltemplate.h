/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2003-11-03
 * Description : template selection for calendar.
 *
 * Copyright (C) 2003-2005 by Renchi Raju <renchi@pooh.tam.uiuc.edu>
 * Copyright (C) 2007-2008 by Orgad Shaneh <orgads at gmail dot com>
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

#ifndef CALTEMPLATE_H
#define CALTEMPLATE_H

// Qt includes.

#include <QWidget>

// Local include.

#include "ui_caltemplate.h"

namespace KIPICalendarPlugin
{

class CalTemplate : public QWidget
{
public:

    CalTemplate(QWidget* parent);
    ~CalTemplate();

private:

    Ui::CalTemplate ui;
};

}  // NameSpace KIPICalendarPlugin

#endif // CALTEMPLATE_H
