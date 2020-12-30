#include "MainWindow.h"
#include "Application.h"
#include "CentralDocPage.h"
#include "accessible.h"
#include "Utils.h"

#include <DLog>
#include <QCommandLineParser>
#include <DApplicationSettings>
#include <QDesktopWidget>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QAccessible>
#include <QDebug>
#include <QFontDatabase>

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    PERF_PRINT_BEGIN("POINT-01", "");
    // Init DTK.
    Application a(argc, argv);

    if (Utils::isWayland()) {
        qputenv("QT_WAYLAND_SHELL_INTEGRATION", "kwayland-shell");
    }

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOptions({
        {
            {"f", "filePath"},
            QCoreApplication::translate("main", "Document File Path."),
            QCoreApplication::translate("main", "FilePath")
        },
        {   {"t", "thumbnailPath"},
            QCoreApplication::translate("main", "ThumbnailPath Path."),
            QCoreApplication::translate("main", "FilePath")
        },
        {
            "thumbnail",
            QCoreApplication::translate("main", "Generate thumbnail.")
        }
    });

    parser.process(a);

    if (parser.isSet("thumbnail") && parser.isSet("filePath") && parser.isSet("thumbnailPath")) {
        QString filePath = parser.value("filePath");
        QString thumbnailPath = parser.value("thumbnailPath");
        if (filePath.isEmpty() || thumbnailPath.isEmpty())
            return -1;

        if (!CentralDocPage::firstThumbnail(filePath, thumbnailPath))
            return -1;

        return 0;
    }

    QStringList arguments = parser.positionalArguments();
    if (arguments.size() > 0)
        PERF_PRINT_BEGIN("POINT-05", "");

    //=======通知已经打开的进程
    QDBusConnection dbus = QDBusConnection::sessionBus();
    if (!dbus.registerService("com.deepin.Reader")) {
        QDBusInterface notification("com.deepin.Reader", "/com/deepin/Reader", "com.deepin.Reader", QDBusConnection::sessionBus());
        QList<QVariant> args;
        args.append(arguments);
        notification.callWithArgumentList(QDBus::Block, "handleFiles", args).errorMessage();
        return 0;
    }

    dbus.registerObject("/com/deepin/Reader", &a, QDBusConnection::ExportScriptableSlots);

    QDBusConnection::systemBus().connect(GESTURE_SERVICE, GESTURE_PATH, GESTURE_INTERFACE, GESTURE_SIGNAL, &a, SIGNAL(sigTouchPadEventSignal(QString, QString, int)));

    QAccessible::installFactory(accessibleFactory);
    DApplicationSettings savetheme;
    Q_UNUSED(savetheme)

    Dtk::Core::DLogManager::registerConsoleAppender();
    Dtk::Core::DLogManager::registerFileAppender();

    if (!MainWindow::allowCreateWindow())
        return -1;

    MainWindow *w = MainWindow::createWindow(arguments);

    w->show();

    PERF_PRINT_END("POINT-01", "");

    int result = a.exec();

    PERF_PRINT_END("POINT-02", "");
    return result;
}
