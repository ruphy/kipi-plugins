/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2006-10-18
 * Description : EXIF lens settings page.
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

#ifndef EXIF_LENS_H
#define EXIF_LENS_H

// Qt includes.

#include <QWidget>
#include <QByteArray>

namespace KIPIMetadataEditPlugin
{

class EXIFLensPriv;

class EXIFLens : public QWidget
{
    Q_OBJECT

public:

    EXIFLens(QWidget* parent);
    ~EXIFLens();

    void applyMetadata(QByteArray& exifData);
    void readMetadata(QByteArray& exifData);

signals:

    void signalModified();

private:

    EXIFLensPriv* const d;
};

}  // namespace KIPIMetadataEditPlugin

#endif // EXIF_LENS_H 
