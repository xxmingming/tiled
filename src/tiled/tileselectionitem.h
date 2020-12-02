/*
 * tileselectionitem.h
 * Copyright 2008-2009, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
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

#include <QGraphicsObject>

namespace Tiled {

class Layer;

class ChangeEvent;
class MapDocument;

/**
 * A graphics item displaying a tile selection.
 */
class TileSelectionItem : public QGraphicsObject
{
    Q_OBJECT

public:
    TileSelectionItem(MapDocument *mapDocument,
                      QGraphicsItem *parent = nullptr);

    void updatePosition();

    // QGraphicsItem
    QRectF boundingRect() const override;

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;

private:
    void documentChanged(const ChangeEvent &change);
    void selectionChanged(const QRegion &newSelection,
                          const QRegion &oldSelection);

    void currentLayerChanged(Layer *layer);

    void updateBoundingRect();

    MapDocument *mMapDocument;
    QRectF mBoundingRect;
};

} // namespace Tiled
