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

#include "tomahawksettings.h"

#ifndef TOMAHAWK_HEADLESS
    #include <QDesktopServices>
    #include "settingsdialog.h"
#endif

#include <QDir>
#include <QDebug>

#define VERSION 2

TomahawkSettings* TomahawkSettings::s_instance = 0;


TomahawkSettings*
TomahawkSettings::instance()
{
    return s_instance;
}


TomahawkSettings::TomahawkSettings( QObject* parent )
    : QSettings( parent )
{
    s_instance = this;

    if( !contains( "configversion") )
    {
        setValue( "configversion", VERSION );
    }
    else if( value( "configversion" ).toUInt() != VERSION )
    {
        qDebug() << "Config version outdated, old:" << value( "configversion" ).toUInt()
                 << "new:" << VERSION
                 << "Doing upgrade, if any...";

        // insert upgrade code here as required
        setValue( "configversion", VERSION );
        if( contains( "script/resolvers") ) {
            setValue( "script/loadedresolvers", value( "script/resolvers" ) );
        }
    }
}


TomahawkSettings::~TomahawkSettings()
{
    s_instance = 0;
}


QStringList
TomahawkSettings::scannerPaths()
{
    //FIXME: After enough time, remove this hack (and make const)
    #ifndef TOMAHAWK_HEADLESS
    if( value( "scannerpaths" ).isNull() )
        setValue( "scannerpaths", value( "scannerpath" ) );
    return value( "scannerpaths", QDesktopServices::storageLocation( QDesktopServices::MusicLocation ) ).toStringList();
    #else
    if( value( "scannerpaths" ).isNull() )
        setValue( "scannerpaths", value( "scannerpath" ) );
    return value( "scannerpaths", "" ).toStringList();
    #endif
}


void
TomahawkSettings::setScannerPaths( const QStringList& paths )
{
    setValue( "scannerpaths", paths );
}


bool
TomahawkSettings::hasScannerPaths() const
{
    //FIXME: After enough time, remove this hack
    return contains( "scannerpaths" ) || contains( "scannerpath" );
}


bool
TomahawkSettings::watchForChanges() const
{
    return value( "watchForChanges", true ).toBool();
}


void
TomahawkSettings::setWatchForChanges( bool watch )
{
    setValue( "watchForChanges", watch );
}


void
TomahawkSettings::setAcceptedLegalWarning( bool accept )
{
    setValue( "acceptedLegalWarning", accept );
}

bool
TomahawkSettings::acceptedLegalWarning() const
{
    return value( "acceptedLegalWarning", false ).toBool();
}

bool
TomahawkSettings::httpEnabled() const
{
    return value( "network/http", true ).toBool();
}


void
TomahawkSettings::setHttpEnabled( bool enable )
{
    setValue( "network/http", enable );
}


QString
TomahawkSettings::proxyHost() const
{
    return value( "network/proxy/host", QString() ).toString();
}


void
TomahawkSettings::setProxyHost( const QString& host )
{
    setValue( "network/proxy/host", host );
}


qulonglong
TomahawkSettings::proxyPort() const
{
    return value( "network/proxy/port", 1080 ).toULongLong();
}


void
TomahawkSettings::setProxyPort( const qulonglong port )
{
    setValue( "network/proxy/port", port );
}


QString
TomahawkSettings::proxyUsername() const
{
    return value( "network/proxy/username", QString() ).toString();
}


void
TomahawkSettings::setProxyUsername( const QString& username )
{
    setValue( "network/proxy/username", username );
}


QString
TomahawkSettings::proxyPassword() const
{
    return value( "network/proxy/password", QString() ).toString();
}


void
TomahawkSettings::setProxyPassword( const QString& password )
{
    setValue( "network/proxy/password", password );
}


int
TomahawkSettings::proxyType() const
{
    return value( "network/proxy/type", 0 ).toInt();
}


void
TomahawkSettings::setProxyType( const int type )
{
    setValue( "network/proxy/type", type );
}


QStringList
TomahawkSettings::aclEntries() const
{
    return value( "acl/entries", QStringList() ).toStringList();
}

void
TomahawkSettings::setAclEntries( const QStringList &entries )
{
    setValue( "acl/entries", entries );
}


QByteArray
TomahawkSettings::mainWindowGeometry() const
{
    return value( "ui/mainwindow/geometry" ).toByteArray();
}


void
TomahawkSettings::setMainWindowGeometry( const QByteArray& geom )
{
    setValue( "ui/mainwindow/geometry", geom );
}


QByteArray
TomahawkSettings::mainWindowState() const
{
    return value( "ui/mainwindow/state" ).toByteArray();
}


void
TomahawkSettings::setMainWindowState( const QByteArray& state )
{
    setValue( "ui/mainwindow/state", state );
}


QByteArray
TomahawkSettings::mainWindowSplitterState() const
{
    return value( "ui/mainwindow/splitterState" ).toByteArray();
}


void
TomahawkSettings::setMainWindowSplitterState( const QByteArray& state )
{
    setValue( "ui/mainwindow/splitterState", state );
}


QByteArray
TomahawkSettings::playlistColumnSizes( const QString& playlistid ) const
{
    return value( QString( "ui/playlist/%1/columnSizes" ).arg( playlistid ) ).toByteArray();
}


void
TomahawkSettings::setPlaylistColumnSizes( const QString& playlistid, const QByteArray& state )
{
    setValue( QString( "ui/playlist/%1/columnSizes" ).arg( playlistid ), state );
}


QList<Tomahawk::playlist_ptr>
TomahawkSettings::recentlyPlayedPlaylists() const
{
    QStringList playlist_guids = value( "playlists/recentlyPlayed" ).toStringList();

    QList<Tomahawk::playlist_ptr> playlists;
    foreach( const QString& guid, playlist_guids )
    {
        Tomahawk::playlist_ptr pl = Tomahawk::Playlist::load( guid );
        if ( !pl.isNull() )
            playlists << pl;
    }

    return playlists;
}


void
TomahawkSettings::appendRecentlyPlayedPlaylist( const Tomahawk::playlist_ptr& playlist )
{
    QStringList playlist_guids = value( "playlists/recentlyPlayed" ).toStringList();

    playlist_guids.removeAll( playlist->guid() );
    playlist_guids.append( playlist->guid() );

    setValue( "playlists/recentlyPlayed", playlist_guids );
}


bool
TomahawkSettings::jabberAutoConnect() const
{
    return value( "jabber/autoconnect", true ).toBool();
}


void
TomahawkSettings::setJabberAutoConnect( bool autoconnect )
{
    setValue( "jabber/autoconnect", autoconnect );
}


unsigned int
TomahawkSettings::jabberPort() const
{
    return value( "jabber/port", 5222 ).toUInt();
}


void
TomahawkSettings::setJabberPort( int port )
{
    if ( port < 0 )
      return;
    setValue( "jabber/port", port );
}


QString
TomahawkSettings::jabberServer() const
{
    return value( "jabber/server" ).toString();
}


void
TomahawkSettings::setJabberServer( const QString& server )
{
    setValue( "jabber/server", server );
}


QString
TomahawkSettings::jabberUsername() const
{
    return value( "jabber/username" ).toString();
}


void
TomahawkSettings::setJabberUsername( const QString& username )
{
    setValue( "jabber/username", username );
}


QString
TomahawkSettings::jabberPassword() const
{
    return value( "jabber/password" ).toString();
}


void
TomahawkSettings::setJabberPassword( const QString& pw )
{
    setValue( "jabber/password", pw );
}


TomahawkSettings::ExternalAddressMode
TomahawkSettings::externalAddressMode() const
{
    return (TomahawkSettings::ExternalAddressMode) value( "network/external-address-mode", TomahawkSettings::Upnp ).toInt();
}


void
TomahawkSettings::setExternalAddressMode( ExternalAddressMode externalAddressMode )
{
    setValue( "network/external-address-mode", externalAddressMode );
}

bool TomahawkSettings::preferStaticHostPort() const
{
    return value( "network/prefer-static-host-and-port" ).toBool();
}

void TomahawkSettings::setPreferStaticHostPort( bool prefer )
{
    setValue( "network/prefer-static-host-and-port", prefer );
}

QString
TomahawkSettings::externalHostname() const
{
    return value( "network/external-hostname" ).toString();
}

void
TomahawkSettings::setExternalHostname(const QString& externalHostname)
{
    setValue( "network/external-hostname", externalHostname );
}

int
TomahawkSettings::defaultPort() const
{
    return 50210;
}

int
TomahawkSettings::externalPort() const
{
    return value( "network/external-port", 50210 ).toInt();
}

void
TomahawkSettings::setExternalPort(int externalPort)
{
    if ( externalPort == 0 )
        setValue( "network/external-port", 50210);
    else
        setValue( "network/external-port", externalPort);
}


QString
TomahawkSettings::lastFmPassword() const
{
    return value( "lastfm/password" ).toString();
}


void
TomahawkSettings::setLastFmPassword( const QString& password )
{
    setValue( "lastfm/password", password );
}


QByteArray
TomahawkSettings::lastFmSessionKey() const
{
    return value( "lastfm/session" ).toByteArray();
}


void
TomahawkSettings::setLastFmSessionKey( const QByteArray& key )
{
    setValue( "lastfm/session", key );
}


QString
TomahawkSettings::lastFmUsername() const
{
    return value( "lastfm/username" ).toString();
}


void
TomahawkSettings::setLastFmUsername( const QString& username )
{
    setValue( "lastfm/username", username );
}

QString
TomahawkSettings::twitterScreenName() const
{
    return value( "twitter/ScreenName" ).toString();
}

void
TomahawkSettings::setTwitterScreenName( const QString& screenName )
{
    setValue( "twitter/ScreenName", screenName );
}

QString
TomahawkSettings::twitterOAuthToken() const
{
    return value( "twitter/OAuthToken" ).toString();
}

void
TomahawkSettings::setTwitterOAuthToken( const QString& oauthtoken )
{
    setValue( "twitter/OAuthToken", oauthtoken );
}

QString
TomahawkSettings::twitterOAuthTokenSecret() const
{
    return value( "twitter/OAuthTokenSecret" ).toString();
}

void
TomahawkSettings::setTwitterOAuthTokenSecret( const QString& oauthtokensecret )
{
    setValue( "twitter/OAuthTokenSecret", oauthtokensecret );
}

qint64
TomahawkSettings::twitterCachedFriendsSinceId() const
{
    return value( "twitter/CachedFriendsSinceID", 0 ).toLongLong();
}

void
TomahawkSettings::setTwitterCachedFriendsSinceId( qint64 cachedId )
{
    setValue( "twitter/CachedFriendsSinceID", cachedId );
}

qint64
TomahawkSettings::twitterCachedMentionsSinceId() const
{
    return value( "twitter/CachedMentionsSinceID", 0 ).toLongLong();
}

void
TomahawkSettings::setTwitterCachedMentionsSinceId( qint64 cachedId )
{
    setValue( "twitter/CachedMentionsSinceID", cachedId );
}

qint64
TomahawkSettings::twitterCachedDirectMessagesSinceId() const
{
    return value( "twitter/CachedDirectMessagesSinceID", 0 ).toLongLong();
}

void
TomahawkSettings::setTwitterCachedDirectMessagesSinceId( qint64 cachedId )
{
    setValue( "twitter/CachedDirectMessagesSinceID", cachedId );
}

QHash<QString, QVariant>
TomahawkSettings::twitterCachedPeers() const
{
    return value( "twitter/CachedPeers", QHash<QString, QVariant>() ).toHash();
}

void
TomahawkSettings::setTwitterCachedPeers( const QHash<QString, QVariant> &cachedPeers )
{
    setValue( "twitter/CachedPeers", cachedPeers );
}

bool
TomahawkSettings::scrobblingEnabled() const
{
    return value( "lastfm/enablescrobbling", false ).toBool();
}


void
TomahawkSettings::setScrobblingEnabled( bool enable )
{
    setValue( "lastfm/enablescrobbling", enable );
}


QString
TomahawkSettings::xmppBotServer() const
{
    return value( "xmppBot/server", QString() ).toString();
}


void
TomahawkSettings::setXmppBotServer( const QString& server )
{
    setValue( "xmppBot/server", server );
}


QString
TomahawkSettings::xmppBotJid() const
{
    return value( "xmppBot/jid", QString() ).toString();
}


void
TomahawkSettings::setXmppBotJid( const QString& component )
{
    setValue( "xmppBot/jid", component );
}


QString
TomahawkSettings::xmppBotPassword() const
{
    return value( "xmppBot/password", QString() ).toString();
}


void
TomahawkSettings::setXmppBotPassword( const QString& password )
{
    setValue( "xmppBot/password", password );
}


int
TomahawkSettings::xmppBotPort() const
{
    return value( "xmppBot/port", -1 ).toInt();
}


void
TomahawkSettings::setXmppBotPort( const int port )
{
    setValue( "xmppBot/port", port );
}

void
TomahawkSettings::addScriptResolver(const QString& resolver)
{
    setValue( "script/resolvers", allScriptResolvers() << resolver );
}

QStringList
TomahawkSettings::allScriptResolvers() const
{
    return value( "script/resolvers" ).toStringList();
}

void
TomahawkSettings::setAllScriptResolvers( const QStringList& resolver )
{
    setValue( "script/resolvers", resolver );
}


QStringList
TomahawkSettings::enabledScriptResolvers() const
{
    return value( "script/loadedresolvers" ).toStringList();
}

void
TomahawkSettings::setEnabledScriptResolvers( const QStringList& resolvers )
{
    setValue( "script/loadedresolvers", resolvers );
}
