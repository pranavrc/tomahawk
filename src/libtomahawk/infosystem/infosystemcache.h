/* === This file is part of Tomahawk Player - <http://tomahawk-player.org> ===
 *
 *   Copyright 2010-2011, Christian Muehlhaeuser <muesli@tomahawk-player.org>
 *
 *   Tomahawk is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Tomahawk is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Tomahawk. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TOMAHAWK_INFOSYSTEMCACHE_H
#define TOMAHAWK_INFOSYSTEMCACHE_H

#include <QDateTime>
#include <QObject>
#include <QtDebug>

#include "infosystem.h"

namespace Tomahawk
{

namespace InfoSystem
{

class InfoSystemCache : public QObject
{
Q_OBJECT

public:
    InfoSystemCache( QObject *parent = 0 );

    virtual ~InfoSystemCache();

signals:
    void notInCache( Tomahawk::InfoSystem::InfoCriteriaHash criteria, QString caller, Tomahawk::InfoSystem::InfoType type, QVariant input, Tomahawk::InfoSystem::InfoCustomData customData );
    void info( QString caller, Tomahawk::InfoSystem::InfoType type, QVariant input, QVariant output, Tomahawk::InfoSystem::InfoCustomData customData );

public slots:
    void getCachedInfoSlot( Tomahawk::InfoSystem::InfoCriteriaHash criteria, qint64 newMaxAge, QString caller, Tomahawk::InfoSystem::InfoType type, QVariant input, Tomahawk::InfoSystem::InfoCustomData customData );
    void updateCacheSlot( Tomahawk::InfoSystem::InfoCriteriaHash criteria, qint64 maxAge, Tomahawk::InfoSystem::InfoType type, QVariant output );

private slots:
    void loadCache( Tomahawk::InfoSystem::InfoType type, const QString &cacheFile );
    void saveCache( Tomahawk::InfoSystem::InfoType type, const QString &cacheDir );

private:
    QHash< InfoType, QHash< InfoCriteriaHash, QVariant > > m_dataCache;
    QHash< InfoType, QHash< InfoCriteriaHash, QDateTime > > m_insertTimeCache;
    QHash< InfoType, QHash< InfoCriteriaHash, QDateTime > > m_maxTimeCache;
    QSet< InfoType > m_dirtySet;
    int m_cacheRemainingToLoad;
};

} //namespace InfoSystem

} //namespace Tomahawk

#endif //TOMAHAWK_INFOSYSTEMCACHE_H
