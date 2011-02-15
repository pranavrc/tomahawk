#include "scanmanager.h"
#include "musicscanner.h"

#include <QDebug>
#include <QThread>

ScanManager* ScanManager::s_instance = 0;


ScanManager*
ScanManager::instance()
{
    return s_instance;
}


ScanManager::ScanManager( QObject* parent )
    : QObject( parent )
    , m_scanner( 0 )
    , m_musicScannerThreadController( 0 )
{
    s_instance = this;
}


ScanManager::~ScanManager()
{
    s_instance = 0;
    m_musicScannerThreadController->deleteLater();
    m_musicScannerThreadController = 0;
    m_scanner->deleteLater();
    m_scanner = 0;    
}

void
ScanManager::runManualScan( const QString &path )
{
    qDebug() << Q_FUNC_INFO;
    if ( !m_musicScannerThreadController && !m_scanner ) //still running if these are not zero
    {
        m_musicScannerThreadController = new QThread( this );
        MusicScanner* m_scanner = new MusicScanner( path );
        m_scanner->moveToThread( m_musicScannerThreadController );
        connect( m_scanner, SIGNAL( finished() ), m_scanner, SLOT( deleteLater() ) );
        connect( m_scanner, SIGNAL( destroyed(QObject*) ), this, SLOT( scanDestroyed(QObject*) ) );
        m_musicScannerThreadController->start( QThread::IdlePriority );
        QMetaObject::invokeMethod( m_scanner, "startScan" );
    }
}

void
ScanManager::scannerDestroyed( QObject* scanner )
{
    qDebug() << Q_FUNC_INFO;
    m_scanner = 0;
    m_musicScannerThreadController->deleteLater();
    m_musicScannerThreadController = 0;
}