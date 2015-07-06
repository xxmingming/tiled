/*
 * project.cpp
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

#include "project.h"

#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QSaveFile>

namespace Tiled {
namespace Internal {

Project *Project::s_currentProject;

Project::Project(QObject *parent)
    : QObject(parent)
    , m_modified(false)
{
}

static QString relative(const QDir &dir, const QString &fileName)
{
    QString rel = dir.relativeFilePath(fileName);
    return rel.isEmpty() ? QLatin1String(".") : rel;
}

bool Project::save(const QString &fileName)
{
    QJsonObject project;

    const QDir dir = QFileInfo(fileName).dir();

    if (!m_tilesetsFolder.isEmpty()) {
        project.insert(QLatin1String("tilesetsFolder"),
                       relative(dir, m_tilesetsFolder));
    }

    if (!m_mapsFolder.isEmpty()) {
        project.insert(QLatin1String("mapsFolder"),
                       relative(dir, m_mapsFolder));
    }

    if (!m_stampsFolder.isEmpty()) {
        project.insert(QLatin1String("stampsFolder"),
                       relative(dir, m_stampsFolder));
    }

    QJsonDocument document(project);

    QSaveFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    file.write(document.toJson());
    if (!file.commit())
        return false;

    m_fileName = fileName;
    return true;
}

bool Project::load(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QJsonParseError error;
    QByteArray json = file.readAll();
    QJsonDocument document(QJsonDocument::fromJson(json, &error));
    if (error.error != QJsonParseError::NoError)
        return false;

    const QDir dir = QFileInfo(fileName).dir();

    QJsonObject project = document.object();

    QString tilesetsFolder = project.value(QLatin1String("tilesetsFolder")).toString();
    QString mapsFolder = project.value(QLatin1String("mapsFolder")).toString();
    QString stampsFolder = project.value(QLatin1String("stampsFolder")).toString();

    if (!tilesetsFolder.isEmpty())
        m_tilesetsFolder = dir.absoluteFilePath(tilesetsFolder);

    if (!mapsFolder.isEmpty())
        m_mapsFolder = dir.absoluteFilePath(mapsFolder);

    if (!stampsFolder.isEmpty())
        m_stampsFolder = dir.absoluteFilePath(stampsFolder);

    return true;
}

void Project::setTilesetsFolder(const QString &tilesetsFolder)
{
    m_tilesetsFolder = tilesetsFolder;
    m_modified = true;
}

void Project::setMapsFolder(const QString &mapsFolder)
{
    m_mapsFolder = mapsFolder;
    m_modified = true;
}

void Project::setStampsFolder(const QString &stampsFolder)
{
    m_stampsFolder = stampsFolder;
    m_modified = true;
}

Project *Project::current()
{
    if (!s_currentProject)
        s_currentProject = new Project;
    return s_currentProject;
}

void Project::deleteCurrent()
{
    delete s_currentProject;
}

} // namespace Internal
} // namespace Tiled
