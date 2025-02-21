#include "mainwindow.h"
#include "runtime.h"
#include "utils/logging.h"
#include <QApplication>
#include <QFile>
#include <QFontDatabase>
#include <QMetaObject>
#include <QThread>
#include <QTranslator>

int qInitResources_assets();
int qCleanupResources_assets();

namespace
{
void initDefaultStyle()
{
    QFile style(":/assets/styles/main.qss");
    if (!style.open(QFile::ReadOnly))
    {
        Logging::error("Failed to load default style!");
        return;
    }
    qApp->setStyleSheet(style.readAll());
    style.close();
}
struct AssetsInitializer
{
    AssetsInitializer()
    {
        qInitResources_assets();
        QFontDatabase::addApplicationFont(":/assets/fonts/uifont.ttf");
        initDefaultStyle();
    }
    ~AssetsInitializer() { qCleanupResources_assets(); }
};

void assetsLazyLoad() { static AssetsInitializer ins; }
} // namespace

int main(int argc, char *argv[])
{
    QElapsedTimer initialize_elapsed;
    initialize_elapsed.start();
    Logging::info("Application Started! tid: {}", QThread::currentThreadId());

    QApplication app(argc, argv);

    assetsLazyLoad();
    Runtime::initialize();

    QTranslator translator;
    if (!translator.load("zh_CN.qm", qApp->applicationDirPath() + "/translations"))
    {
        Logging::error("Failed to load chinese language file! {}/zh_CN.qm", qApp->applicationDirPath());
    }
    else
    {
        qApp->installTranslator(&translator);
    }

    QTranslator qt_translator;
    if (!qt_translator.load("qt_zh_CN.qm", qApp->applicationDirPath() + "/translations"))
    {
        Logging::error("Failed to load chinese language file! {}/qt_zh_CN.qm", qApp->applicationDirPath());
    }
    else
    {
        qApp->installTranslator(&qt_translator);
    }

    Logging::info("Application initialize elapsed: {} ms", initialize_elapsed.elapsed());
    MainWindow mainWindow;
    mainWindow.show();
    int ret = app.exec();
    Logging::info("Application Stopped! tid: {}", QThread::currentThreadId());
    return ret;
}