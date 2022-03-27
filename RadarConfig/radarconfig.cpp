#include "radarconfig.h"

#include <QFile>
#include <QDir>
#include <QSettings>

RadarConfig::RadarConfig* RadarConfig::RadarConfig::instance{nullptr};
QStringList RadarConfig::RadarConfig::nonVolatileKeys =
        QStringList()<<NON_VOLATILE_PPI_DISPLAY_SHOW_RING
                    <<NON_VOLATILE_PPI_DISPLAY_SHOW_COMPASS
                   <<NON_VOLATILE_PPI_DISPLAY_SHOW_HEADING_MARKER
                  <<NON_VOLATILE_PPI_DISPLAY_HEADING_UP
                  <<NON_VOLATILE_PPI_DISPLAY_LAST_SCALE
                 <<NON_VOLATILE_PPI_DISPLAY_SHOW_GZ
                <<NON_VOLATILE_PPI_DISPLAY_SHOW_ARPA
               <<NON_VOLATILE_RADAR_NET_IP_DATA
              <<NON_VOLATILE_RADAR_NET_IP_REPORT
             <<NON_VOLATILE_RADAR_NET_IP_CMD
            <<NON_VOLATILE_RADAR_NET_PORT_DATA
           <<NON_VOLATILE_RADAR_NET_PORT_REPORT
          <<NON_VOLATILE_RADAR_NET_PORT_CMD
         <<NON_VOLATILE_RADAR_TRAIL_ENABLE
        <<NON_VOLATILE_RADAR_TRAIL_TIME
       <<NON_VOLATILE_ARPA_PARAMS_MIN_CONTOUR_LEN
      <<NON_VOLATILE_ARPA_PARAMS_SEARCH_RADIUS1
     <<NON_VOLATILE_ARPA_PARAMS_SEARCH_RADIUS2
    <<NON_VOLATILE_ARPA_PARAMS_MAX_TARGET_SIZE
   <<NON_VOLATILE_ARPA_CONTROL_CREATE_ARPA_BY_CLICK
  <<NON_VOLATILE_ARPA_NET_IP_SEND
 <<NON_VOLATILE_ARPA_NET_PORT_SEND
<<NON_VOLATILE_GZ_ENABLE_ALARM
<<NON_VOLATILE_GZ_MODE
<<NON_VOLATILE_GZ_TIMEOUT
<<NON_VOLATILE_GZ_NOTIF_THRESHOLD
<<NON_VOLATILE_NAV_DATA_LAST_HEADING
<<NON_VOLATILE_NAV_DATA_LAST_LATITUDE
<<NON_VOLATILE_NAV_DATA_LAST_LONGITUDE
<<NON_VOLATILE_NAV_CONTROL_GPS_AUTO
<<NON_VOLATILE_NAV_CONTROL_HEADING_AUTO
                      ;

RadarConfig::RadarConfig::RadarConfig(QString path):filePath(path)
{
    QFile file(filePath);
    if(filePath.isEmpty() || !file.exists())
    {
        filePath = QDir::homePath()+QDir::separator()+".radar.conf";
        file.setFileName(filePath);
    }

    if(!file.exists()) initConfig();
    else loadConfig();
}

bool RadarConfig::RadarConfig::setConfig(const QString &key, const QVariant &value)
{
    bool valid = false;
    if(volatileVar.contains(key))
    {
        valid = true;
        volatileVar.insert(key,value);
    }
    else if(nonVolatileVar.contains(key))
    {
        valid = true;
        nonVolatileVar.insert(key,value);
    }

    return valid;
}

QVariant RadarConfig::RadarConfig::getConfig(const QString &key) const
{
    QVariant val;
    if(volatileVar.contains(key)) val = volatileVar.value(key);
    else if(nonVolatileVar.contains(key)) val = nonVolatileVar.value(key);

    return val;
}
/*
bool RadarConfig::RadarConfig::createVar(const QString &key, const QVariant &value, bool nonVolatile)
{
    bool valid = !volatileVar.contains(key) && !nonVolatileVar.contains(key);
    if(valid)
    {
        if(nonVolatile) nonVolatileVar.insert(key,value);
        else volatileVar.insert(key,value);
    }

    return valid;
}
*/
void RadarConfig::RadarConfig::loadConfig()
{
    qDebug()<<Q_FUNC_INFO;
    //volatile
    volatileVar.insert(VOLATILE_GZ_CONFIRMED,true);

    volatileVar.insert(VOLATILE_RADAR_PARAMS_FILTER_DATA_GAIN,0);
    volatileVar.insert(VOLATILE_RADAR_PARAMS_FILTER_DATA_RAIN,0);
    volatileVar.insert(VOLATILE_RADAR_PARAMS_FILTER_DATA_SEA,0);
    volatileVar.insert(VOLATILE_RADAR_PARAMS_FILTER_DATA_MTI,0);
    volatileVar.insert(VOLATILE_RADAR_PARAMS_FILTER_CONTROL_GAIN,true); //auto
    volatileVar.insert(VOLATILE_RADAR_PARAMS_FILTER_CONTROL_RAIN,true); //auto
    volatileVar.insert(VOLATILE_RADAR_PARAMS_FILTER_CONTROL_SEA,true); //auto
    volatileVar.insert(VOLATILE_RADAR_PARAMS_FILTER_CONTROL_MTI,true); //enable
    volatileVar.insert(VOLATILE_RADAR_PARAMS_FILTER_DATA_TARGET_BOOST,0);
    volatileVar.insert(VOLATILE_RADAR_PARAMS_FILTER_DATA_INTERFERENCE,0);
    volatileVar.insert(VOLATILE_RADAR_PARAMS_FILTER_DATA_TARGET_EXPAND,0);
    volatileVar.insert(VOLATILE_RADAR_PARAMS_ALIGN_DATA_BEARING,0);
    volatileVar.insert(VOLATILE_RADAR_PARAMS_ALIGN_DATA_ANTENA_HEIGHT,0);
    volatileVar.insert(VOLATILE_RADAR_PARAMS_SCAN_DATA_SCAN_SPEED,0);
    volatileVar.insert(VOLATILE_RADAR_PARAMS_SCAN_DATA_NOISE_REJECT,0);
    volatileVar.insert(VOLATILE_RADAR_PARAMS_SCAN_DATA_TARGET_SEPARATION,0);
    volatileVar.insert(VOLATILE_RADAR_PARAMS_SCAN_DATA_SIDE_LOBE_SUPPRESSION,0);
    volatileVar.insert(VOLATILE_RADAR_PARAMS_SCAN_DATA_LOCAL_INTERFERENCE,0);
    volatileVar.insert(VOLATILE_RADAR_PARAMS_RANGE_DATA_RANGE,0);
    volatileVar.insert(VOLATILE_RADAR_STATUS,RADAR_OFF);

    /*non volatile*/
    QSettings config(filePath,QSettings::IniFormat);
    foreach (QString key, config.allKeys()) nonVolatileVar.insert(key,config.value(key));

    /*match desired keys*/
    QMapIterator<QString, QVariant>i(nonVolatileVar);

    while (i.hasNext())
    {
        i.next();
        if(!nonVolatileKeys.contains(i.key())) qWarning()<<Q_FUNC_INFO<<"undesired key"<<i.key();
    }
}
void RadarConfig::RadarConfig::initConfig()
{
    qDebug()<<Q_FUNC_INFO;
    //non volatile
    nonVolatileVar.insert(NON_VOLATILE_PPI_DISPLAY_SHOW_RING,false);
    nonVolatileVar.insert(NON_VOLATILE_PPI_DISPLAY_HEADING_UP,false);
    nonVolatileVar.insert(NON_VOLATILE_PPI_DISPLAY_SHOW_COMPASS,true);
    nonVolatileVar.insert(NON_VOLATILE_PPI_DISPLAY_SHOW_HEADING_MARKER,true);
    nonVolatileVar.insert(NON_VOLATILE_PPI_DISPLAY_LAST_SCALE,2000);
    nonVolatileVar.insert(NON_VOLATILE_PPI_DISPLAY_SHOW_GZ,true);
    nonVolatileVar.insert(NON_VOLATILE_PPI_DISPLAY_SHOW_ARPA,true);

    nonVolatileVar.insert(NON_VOLATILE_RADAR_NET_IP_DATA,"127.0.0.1");
    nonVolatileVar.insert(NON_VOLATILE_RADAR_NET_IP_REPORT,"127.0.0.1");
    nonVolatileVar.insert(NON_VOLATILE_RADAR_NET_IP_CMD,"127.0.0.1");
    nonVolatileVar.insert(NON_VOLATILE_RADAR_NET_PORT_DATA,6132);
    nonVolatileVar.insert(NON_VOLATILE_RADAR_NET_PORT_REPORT,6133);
    nonVolatileVar.insert(NON_VOLATILE_RADAR_NET_PORT_CMD,6131);
    nonVolatileVar.insert(NON_VOLATILE_RADAR_TRAIL_ENABLE,true);
    nonVolatileVar.insert(NON_VOLATILE_RADAR_TRAIL_TIME,15); //second

    nonVolatileVar.insert(NON_VOLATILE_ARPA_PARAMS_MIN_CONTOUR_LEN,90);
    nonVolatileVar.insert(NON_VOLATILE_ARPA_PARAMS_SEARCH_RADIUS1,10);
    nonVolatileVar.insert(NON_VOLATILE_ARPA_PARAMS_SEARCH_RADIUS2,50);
    nonVolatileVar.insert(NON_VOLATILE_ARPA_PARAMS_MAX_TARGET_SIZE,50);
    nonVolatileVar.insert(NON_VOLATILE_ARPA_CONTROL_CREATE_ARPA_BY_CLICK,true);
    nonVolatileVar.insert(NON_VOLATILE_ARPA_NET_IP_SEND,"127.0.0.1");
    nonVolatileVar.insert(NON_VOLATILE_ARPA_NET_PORT_SEND,7132);

    nonVolatileVar.insert(NON_VOLATILE_GZ_ENABLE_ALARM,true);
    nonVolatileVar.insert(NON_VOLATILE_GZ_MODE,0); //arc mode
    nonVolatileVar.insert(NON_VOLATILE_GZ_NOTIF_THRESHOLD,10);
    nonVolatileVar.insert(NON_VOLATILE_GZ_TIMEOUT,90);

    nonVolatileVar.insert(NON_VOLATILE_NAV_DATA_LAST_HEADING,0.0);
    nonVolatileVar.insert(NON_VOLATILE_NAV_DATA_LAST_LATITUDE,0.0);
    nonVolatileVar.insert(NON_VOLATILE_NAV_DATA_LAST_LONGITUDE,0.0);
    nonVolatileVar.insert(NON_VOLATILE_NAV_CONTROL_GPS_AUTO,true);
    nonVolatileVar.insert(NON_VOLATILE_NAV_CONTROL_HEADING_AUTO,true);

}

void RadarConfig::RadarConfig::saveConfig() const
{
    qDebug()<<Q_FUNC_INFO<<filePath;

    QSettings config(filePath,QSettings::IniFormat);
    QMapIterator<QString, QVariant>i(nonVolatileVar);

    while (i.hasNext())
    {
        i.next();
        config.setValue(i.key(),i.value());
    }
}

RadarConfig::RadarConfig::~RadarConfig()
{
    saveConfig();
}
RadarConfig::RadarConfig* RadarConfig::RadarConfig::getInstance(const QString& path )
{
    if(instance == nullptr) instance = new RadarConfig(path);
    else
    {
        if(instance->filePath != path && !path.isEmpty()) return nullptr;
    }

    return  instance;
}
