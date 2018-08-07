/*
 * newsfeed.h
 * Copyright 2018, Thorbjørn Lindeijer <bjorn@lindeijer.nl>
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

#pragma once

#include <QDateTime>
#include <QObject>
#include <QString>
#include <QUrl>
#include <QVector>

class QNetworkAccessManager;
class QNetworkReply;

namespace Tiled {
namespace Internal {

class NewsItem
{
public:
    QString title;
    QUrl link;
    QDateTime pubDate;
};

class NewsFeed : public QObject
{
    Q_OBJECT

private:
    NewsFeed();

public:
    static NewsFeed &instance();

    void refresh();

    void markAllRead();
    void markRead(const NewsItem &item);
    int unreadCount() const;
    QDateTime lastRead() const;

    const QVector<NewsItem> &newsItems() const;

signals:
    void refreshed();

private slots:
    void finished(QNetworkReply *reply);

private:
    void setLastRead(const QDateTime &dateTime);

    QNetworkAccessManager *mNetworkAccessManager;
    QVector<NewsItem> mNewsItems;
    QDateTime mLastRead;
};


inline QDateTime NewsFeed::lastRead() const
{
    return mLastRead;
}

inline const QVector<NewsItem> &NewsFeed::newsItems() const
{
    return mNewsItems;
}

} // namespace Internal
} // namespace Tiled
