#include "settings.h"
#include "utils/pathutil.h"
#include <QApplication>

Settings::Settings()
    : settings_(QString::fromStdString(PathUtil::getInstallFilePath("config.ini")), QSettings::IniFormat)
{
}

bool Settings::contains(const QString& key) const
{
    return settings_.contains(key);
}

void Settings::initialize()
{
    // if (!contains(StandardConfig::kStatusTempRefreshInterval.data())) {
    //     put(StandardConfig::kStatusTempRefreshInterval, 1000);
    // }

    // if (!contains(StandardConfig::kStatusMotionRefreshInterval.data())) {
    //     put(StandardConfig::kStatusMotionRefreshInterval, 1000);
    // }
}