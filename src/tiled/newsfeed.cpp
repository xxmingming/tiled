/*
 * newsfeed.cpp
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

#include "newsfeed.h"

#include "preferences.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSettings>
#include <QXmlStreamReader>

static const char devlogUrl[] = "https://thorbjorn.itch.io/tiled/devlog.rss";

namespace Tiled {
namespace Internal {

NewsFeed::NewsFeed()
    : mNetworkAccessManager(new QNetworkAccessManager(this))
{
    connect(mNetworkAccessManager, &QNetworkAccessManager::finished,
            this, &NewsFeed::finished);

    auto settings = Preferences::instance()->settings();
    mLastRead = settings->value(QLatin1String("Install/NewsFeedLastRead")).toDateTime();

    refresh();
}

NewsFeed &NewsFeed::instance()
{
    static NewsFeed newsFeed;
    return newsFeed;
}

void NewsFeed::refresh()
{
    mNetworkAccessManager->get(QNetworkRequest(QUrl(QLatin1String(devlogUrl))));
}

void NewsFeed::markAllRead()
{
    if (mNewsItems.isEmpty())
        return;

    markRead(mNewsItems.first());
}

void NewsFeed::markRead(const NewsItem &item)
{
    if (mLastRead < item.pubDate)
        setLastRead(item.pubDate);
}

int NewsFeed::unreadCount() const
{
    int count = 0;
    for (const NewsItem &newsItem : mNewsItems)
        if (newsItem.pubDate > mLastRead)
            ++count;
    return count;
}

void NewsFeed::finished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << reply->errorString();
        return;
    }

    QByteArray feed = reply->readAll();
    QXmlStreamReader xml(feed);

    if (!xml.readNextStartElement() || xml.name() != QLatin1String("rss"))
        return;
    if (!xml.readNextStartElement() || xml.name() != QLatin1String("channel"))
        return;

    mNewsItems.clear();

    while (xml.readNextStartElement()) {
        if (xml.name() == QLatin1String("item")) {
            NewsItem newsItem;

            while (xml.readNextStartElement()) {
                if (xml.name() == QLatin1String("title"))
                    newsItem.title = xml.readElementText(QXmlStreamReader::IncludeChildElements);
                else if (xml.name() == QLatin1String("link"))
                    newsItem.link = QUrl(xml.readElementText(QXmlStreamReader::SkipChildElements));
                else if (xml.name() == QLatin1String("pubDate"))
                    newsItem.pubDate = QDateTime::fromString(xml.readElementText(QXmlStreamReader::SkipChildElements), Qt::RFC2822Date);
                else
                    xml.skipCurrentElement();
            }

            mNewsItems.append(newsItem);

            // No need for parsing everything
            if (mNewsItems.size() == 5)
                xml.raiseError();

        } else {
            xml.skipCurrentElement();
        }
    }

    emit refreshed();
}

void NewsFeed::setLastRead(const QDateTime &dateTime)
{
    mLastRead = dateTime;

    auto settings = Preferences::instance()->settings();
    settings->setValue(QLatin1String("Install/NewsFeedLastRead"),
                       mLastRead.toString(Qt::ISODate));
}

} // namespace Internal
} // namespace Tiled
