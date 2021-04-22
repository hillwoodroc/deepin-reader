﻿/*
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
#ifndef MainWindow_H
#define MainWindow_H

#include <DMainWindow>

class Central;
class DocSheet;
class TitleMenu;
class QPropertyAnimation;
class MainWindow : public Dtk::Widget::DMainWindow
{
    Q_OBJECT
    Q_DISABLE_COPY(MainWindow)
    Q_PROPERTY(QList<QWidget *> orderWidgets WRITE updateOrderWidgets)

public:
    /**
     * @brief MainWindow
     * 根据文件路径列表会直接打开对应文件
     * @param filePathList 文件路径列表
     * @param parent
     */
    explicit MainWindow(QStringList filePathList, DMainWindow *parent = nullptr);

    /**
     * @brief MainWindow
     * 直接显示传入已经存在的sheet
     * @param sheet
     * @param parent
     */
    explicit MainWindow(DocSheet *sheet, DMainWindow *parent = nullptr);

    ~MainWindow() override;

    /**
     * @brief addSheet
     * 添加新文档窗口
     * @param sheet
     */
    void addSheet(DocSheet *sheet);

    /**
     * @brief hasSheet
     * 是否有这个文档
     * @param sheet
     * @return
     */
    bool hasSheet(DocSheet *sheet) ;

    /**
     * @brief activateSheet
     * 活动文档显示到最前面
     * @param sheet 哪个文档
     */
    void activateSheet(DocSheet *sheet);

    /**
     * @brief setDocTabBarWidget
     * 全屏时设置tabbar
     * @param widget tabbar
     */
    void setDocTabBarWidget(QWidget *widget);

    /**
     * @brief 调整标题控件大小
     */
    void resizeFullTitleWidget();

    /**
     * @brief 更新TAB控件顺序
     * @param orderlst
     */
    void updateOrderWidgets(const QList<QWidget *> &orderlst);

    /**
     * @brief handleClose
     * 进行关闭，成功会关闭并释放自己
     * @param needToBeSaved
     * @param needSavetip
     * @return
     */
    bool handleClose(bool needToBeSaved, bool needSavetip = true);

public:
    /**
     * @brief 当前窗口打开文件
     * @param filePath 文件路径
     */
    void addFile(const QString &filePath);

protected:
    void closeEvent(QCloseEvent *event) override;

    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    /**
     * @brief initBase
     * 初始化基础项
     */
    void initBase();

    /**
     * @brief initUI
     * 初始化UI
     */
    void initUI();

    /**
     * @brief showDefaultSize
     * 显示默认大小
     */
    void showDefaultSize();

private slots:
    /**
     * @brief onDelayInit
     * 延时初始化
     */
    void onDelayInit();

    /**
     * @brief onUpdateTitleLabelRect
     * 更新提示标签大小
     */
    void onUpdateTitleLabelRect();

    /**
     * @brief 全屏
     */
    void onMainWindowFull();

    /**
     * @brief 退出全屏
     */
    void onMainWindowExitFull();

    /**
     * @brief 标题悬浮动画结束
     */
    void onTitleAniFinished();

    /**
     * @brief 虚拟键盘激活状态
     * @param visible 虚拟键盘是否可见
     */
    void onImActiveChanged(bool visible);

    /**
     * @brief 重置窗口大小
     */
    void onDelayResizeHeight();

private:
    QWidget *m_FullTitleWidget = nullptr;
    QWidget *m_docTabWidget = nullptr;
    QPropertyAnimation *m_TitleAnimation = nullptr;
    TitleMenu *m_menu = nullptr;
    Central *m_central = nullptr;
    bool needToBeSaved = true;
    int m_lastWindowState = Qt::WindowNoState;
    QStringList m_initFilePathList;

public:
    static MainWindow *windowContainSheet(DocSheet *sheet);
    static bool allowCreateWindow();
    static bool activateSheetIfExist(const QString &filePath);
    static MainWindow *createWindow(QStringList filePathList = QStringList());
    static MainWindow *createWindow(DocSheet *sheet);
    static QList<MainWindow *> m_list;
    QTimer *m_showMenuTimer = nullptr;
};

#endif // MainWindow_H
