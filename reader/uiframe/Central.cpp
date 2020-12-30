/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     zhangsong<zhangsong@uniontech.com>
*
* Maintainer: zhangsong<zhangsong@uniontech.com>
*
* Central(NaviPage ViewPage)
*
* CentralNavPage(openfile)
*
* CentralDocPage(DocTabbar DocSheets)
*
* DocSheet(SheetSidebar SheetBrowser document)
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Central.h"
#include "CentralNavPage.h"
#include "CentralDocPage.h"
#include "TitleMenu.h"
#include "TitleWidget.h"
#include "MainWindow.h"
#include "ShortCutShow.h"

#include <DMessageManager>
#include <DFileDialog>

#include <QStackedLayout>
#include <QMimeData>
#include <QTimer>

Central::Central(QWidget *parent)
    : BaseWidget(parent)
{
    setAcceptDrops(true);

    m_widget = new TitleWidget(parent);

    m_navPage = new CentralNavPage(this);
    connect(m_navPage, SIGNAL(sigNeedOpenFilesExec()), SLOT(onOpenFilesExec()));

    m_layout = new QStackedLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    m_layout->addWidget(m_navPage);
    setLayout(m_layout);

    connect(this, &Central::signalAddFile, this, &Central::onAddFile);
}

Central::~Central()
{
}

TitleWidget *Central::titleWidget()
{
    return m_widget;
}

CentralDocPage *Central::docPage()
{
    if (nullptr == m_docPage) {
        m_docPage = new CentralDocPage(this);
        m_layout->addWidget(m_docPage);
        connect(m_docPage, SIGNAL(sigCurSheetChanged(DocSheet *)), m_menu, SLOT(onCurSheetChanged(DocSheet *)));
        connect(m_docPage, SIGNAL(sigCurSheetChanged(DocSheet *)), m_widget, SLOT(onCurSheetChanged(DocSheet *)));
        connect(m_docPage, SIGNAL(sigFindOperation(const int &)), m_widget, SLOT(onFindOperation(const int &)));
        connect(m_docPage, SIGNAL(sigNeedShowTips(const QString &, int)), this, SLOT(onShowTips(const QString &, int)));
        connect(m_docPage, SIGNAL(sigNeedClose()), this, SIGNAL(sigNeedClose()));
        connect(m_docPage, SIGNAL(sigSheetCountChanged(int)), this, SLOT(onSheetCountChanged(int)));
        connect(m_docPage, SIGNAL(sigNeedOpenFilesExec()), SLOT(onOpenFilesExec()));
        connect(m_docPage, SIGNAL(sigNeedActivateWindow()), this, SLOT(onNeedActivateWindow()));
    }

    return m_docPage;
}

void Central::setMenu(TitleMenu *menu)
{
    m_menu = menu;
    connect(m_menu, SIGNAL(sigActionTriggered(QString)), this, SLOT(onMenuTriggered(QString)));
}

void Central::zoomIn()
{
    if (m_docPage) {
        m_docPage->zoomIn();
    }
}

void Central::zoomOut()
{
    if (m_docPage) {
        m_docPage->zoomOut();
    }
}

void Central::addFilesWithDialog()
{
    DFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setFileMode(DFileDialog::ExistingFiles);
    dialog.setNameFilter(tr("Documents") + " (*.pdf *.djvu *.docx)");
    dialog.setDirectory(QDir::homePath());

    if (QDialog::Accepted != dialog.exec()) {
        return;
    }

    QStringList filePathList = dialog.selectedFiles();

    if (filePathList.count() <= 0) {
        return;
    }

    QWidget *topLevelwidget = this->topLevelWidget();
    topLevelwidget->setProperty("checkLoadPdfStatus", true);
    foreach (QString filePath, filePathList) {
        if (topLevelwidget->property("windowClosed").toBool())
            break;

        if (!MainWindow::activateSheetIfExist(filePath))
            emit signalAddFile(filePath);
    }
    topLevelwidget->setProperty("checkLoadPdfStatus", false);
}

void Central::onAddFile(const QString &filePath)
{
    docPage()->addFileAsync(filePath);
}

void Central::addSheet(DocSheet *sheet)
{
    docPage()->addSheet(sheet);
}

bool Central::hasSheet(DocSheet *sheet)
{
    return docPage()->hasSheet(sheet);
}

void Central::showSheet(DocSheet *sheet)
{
    docPage()->showSheet(sheet);
}

bool Central::saveAll()
{
    return docPage()->saveAll();
}

void Central::handleShortcut(QString shortcut)
{
    if (shortcut == Dr::key_ctrl_o) {
        addFilesWithDialog();
    } if (shortcut == Dr::key_ctrl_shift_slash) { //  显示快捷键预览
        ShortCutShow show;
        show.setSheet(docPage()->getCurSheet());
        show.show();
    } else
        docPage()->handleShortcut(shortcut);
}

void Central::onSheetCountChanged(int count)
{
    if (count > 0) {
        m_layout->setCurrentIndex(1);
    } else {
        m_layout->setCurrentIndex(0);
        m_navPage->setFocus();
        m_widget->setControlEnabled(false);
    }
}

void Central::onMenuTriggered(const QString &action)
{
    if (action == "New window") {
        if (MainWindow::allowCreateWindow())
            MainWindow::createWindow()->show();
    } else if (action == "New tab") {
        addFilesWithDialog();
    } else if (action == "Save") { //  保存当前显示文件
        docPage()->saveCurrent();
    } else if (action == "Save as") {
        docPage()->saveAsCurrent();
    } else if (action == "Magnifer") {   //  开启放大镜
        docPage()->openMagnifer();
    } else if (action == "Display in file manager") {    //  文件浏览器 显示
        docPage()->openCurFileFolder();
    } else if (action == "Search") {
        docPage()->handleSearch();
    }
}

void Central::onOpenFilesExec()
{
    addFilesWithDialog();
}

void Central::onNeedActivateWindow()
{
    activateWindow();
}

void Central::onShowTips(const QString &text, int iconIndex)
{
    if (m_layout->currentIndex() == 0) {
        if (0 == iconIndex)
            DMessageManager::instance()->sendMessage(m_navPage, QIcon::fromTheme(QString("dr_") + "ok"), text);
        else
            DMessageManager::instance()->sendMessage(m_navPage, QIcon::fromTheme(QString("dr_") + "warning"), text);
    } else {
        if (0 == iconIndex)
            DMessageManager::instance()->sendMessage(this, QIcon::fromTheme(QString("dr_") + "ok"), text);
        else
            DMessageManager::instance()->sendMessage(this, QIcon::fromTheme(QString("dr_") + "warning"), text);
    }
}

void Central::dragEnterEvent(QDragEnterEvent *event)
{
    auto mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        event->accept();
        activateWindow();
    } else if (mimeData->hasFormat("reader/tabbar")) {
        event->accept();
        activateWindow();
    } else
        event->ignore();
}

void Central::dropEvent(QDropEvent *event)
{
    auto mimeData = event->mimeData();
    if (mimeData->hasFormat("deepin_reader/tabbar")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
        activateWindow();

        QString id = mimeData->data("deepin_reader/uuid");
        DocSheet *sheet = DocSheet::getSheet(id);
        if (nullptr != sheet)
            docPage()->onCentralMoveIn(sheet);

    } else if (mimeData->hasUrls()) {
        QWidget *topLevelwidget = topLevelWidget();
        topLevelwidget->setProperty("checkLoadPdfStatus", true);
        for (auto url : mimeData->urls()) {
            if (topLevelwidget->property("windowClosed").toBool())
                break;

            QString filePath = url.toLocalFile();
            if (!MainWindow::activateSheetIfExist(filePath)) {
                emit signalAddFile(filePath);
            }
        }
        topLevelwidget->setProperty("checkLoadPdfStatus", false);
    }
}
