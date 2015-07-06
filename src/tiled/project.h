/*
 * project.h
 * Copyright 2015, Thorbjørn Lindeijer <bjorn@lindeijer.nl>
 *
 * This file is part of Tiled.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TILED_INTERNAL_PROJECT_H
#define TILED_INTERNAL_PROJECT_H

#include <QList>
#include <QObject>

namespace Tiled {
namespace Internal {

class Project : public QObject
{
    Q_OBJECT

public:
    explicit Project(QObject *parent = 0);

    bool save(const QString &fileName);
    bool load(const QString &fileName);

    const QString &fileName() const;

    const QString &tilesetsFolder() const;
    void setTilesetsFolder(const QString &tilesetsFolder);

    const QString &mapsFolder() const;
    void setMapsFolder(const QString &mapsFolder);

    const QString &stampsFolder() const;
    void setStampsFolder(const QString &stampsFolder);

    static Project *current();
    static void deleteCurrent();

signals:

public slots:

private:
    QString m_fileName;

    QString m_tilesetsFolder;
    QString m_mapsFolder;
    QString m_stampsFolder;

    bool m_modified;

    static Project *s_currentProject;
};

inline const QString &Project::fileName() const
{
    return m_fileName;
}

inline const QString &Project::tilesetsFolder() const
{
    return m_tilesetsFolder;
}

inline const QString &Project::mapsFolder() const
{
    return m_mapsFolder;
}

inline const QString &Project::stampsFolder() const
{
    return m_stampsFolder;
}

} // namespace Internal
} // namespace Tiled

#endif // TILED_INTERNAL_PROJECT_H
