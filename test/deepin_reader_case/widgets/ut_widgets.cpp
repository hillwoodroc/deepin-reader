/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     leiyu <leiyu@uniontech.com>
*
* Maintainer: leiyu <leiyu@uniontech.com>
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
#include "ut_widgets.h"
#include "widgets/AttrScrollWidget.h"
#include "widgets/BookMarkButton.h"
#include "widgets/CustomClickLabel.h"
#include "widgets/CustemPushButton.h"
#include "widgets/PrintManager.h"
#include "widgets/RoundColorWidget.h"
#include "widgets/SaveDialog.h"
#include "widgets/ScaleWidget.h"
#include "widgets/ShortCutShow.h"
#include "widgets/SlidePlayWidget.h"
#include "widgets/SlideWidget.h"
#include "widgets/SpinnerWidget.h"
#include "widgets/TipsWidget.h"
#include "widgets/WordWrapLabel.h"

#define private public
#define protected public

#include "widgets/ScaleMenu.h"
#include "widgets/CustomMenu.h"
#include "widgets/FindWidget.h"
#include "widgets/CustomWidget.h"
#include "widgets/EncryptionPage.h"
#include "widgets/ColorWidgetAction.h"
#include "widgets/FileAttrWidget.h"
#include "widgets/HandleMenu.h"

#undef private
#undef protected

Ut_Widgets::Ut_Widgets()
{
}

void Ut_Widgets::SetUp()
{
}

void Ut_Widgets::TearDown()
{
}

#ifdef UT_WIDGETS_TEST
TEST(Ut_Widgets, BookMarkButtonTest)
{
    BookMarkButton bkBtn;
    bkBtn.setClickState(true);
    EXPECT_TRUE(bkBtn.clickState());
    bkBtn.repaint();

    QMouseEvent mouseLPevent(QEvent::MouseButtonPress, QPoint(0, 0), Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
    QCoreApplication::sendEvent(&bkBtn, &mouseLPevent);

    QMouseEvent mouseRPevent(QEvent::MouseButtonPress, QPoint(0, 0), Qt::RightButton, Qt::NoButton, Qt::NoModifier);
    QCoreApplication::sendEvent(&bkBtn, &mouseLPevent);

    QMouseEvent mouseLRevent(QEvent::MouseButtonRelease, QPoint(0, 0), Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
    QCoreApplication::sendEvent(&bkBtn, &mouseLRevent);

    QMouseEvent mouseRRevent(QEvent::MouseButtonRelease, QPoint(0, 0), Qt::RightButton, Qt::NoButton, Qt::NoModifier);
    QCoreApplication::sendEvent(&bkBtn, &mouseLRevent);

    QEvent hoverE(QEvent::HoverEnter);
    QCoreApplication::sendEvent(&bkBtn, &hoverE);

    QEvent hoverL(QEvent::HoverLeave);
    QCoreApplication::sendEvent(&bkBtn, &hoverL);
}

TEST(Ut_Widgets, BColorWidgetActionTest)
{
    ColorWidgetAction colorAction;
    EXPECT_TRUE(colorAction.getIndex() >= 0);
    colorAction.slotBtnClicked(0);
    colorAction.slotBtnDefaultClicked();
}

TEST(Ut_Widgets, CustemPushButtonTest)
{
    CustemPushButton cusBtn;
    QKeyEvent keyevent(QEvent::KeyPress, Qt::Key_1, Qt::ControlModifier);
    QCoreApplication::sendEvent(&cusBtn, &keyevent);

    QKeyEvent keyevent1(QEvent::KeyPress, Qt::Key_Enter, Qt::ControlModifier);
    QCoreApplication::sendEvent(&cusBtn, &keyevent1);

    QKeyEvent keyevent2(QEvent::KeyPress, Qt::Key_Return, Qt::ControlModifier);
    QCoreApplication::sendEvent(&cusBtn, &keyevent2);
}

TEST(Ut_Widgets, CustomClickLabelTest)
{
    CustomClickLabel clickLabel;
    QMouseEvent mouseLPevent(QEvent::MouseButtonPress, QPoint(0, 0), Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
    QCoreApplication::sendEvent(&clickLabel, &mouseLPevent);

    QMouseEvent mouseRPevent(QEvent::MouseButtonPress, QPoint(0, 0), Qt::RightButton, Qt::NoButton, Qt::NoModifier);
    QCoreApplication::sendEvent(&clickLabel, &mouseLPevent);
}

TEST(Ut_Widgets, CustomMenuTest)
{
    CustomMenu menu;
    EXPECT_TRUE(menu.createAction("test", nullptr, true));
}

TEST(Ut_Widgets, CustomWidgetTest)
{
    CustomWidget cusWidget;
    cusWidget.adaptWindowSize(1.0);
    cusWidget.updateThumbnail(-1);
    cusWidget.updateThumbnail(0);
    cusWidget.updateThumbnail(10000);
    cusWidget.showMenu();
    cusWidget.initWidget();
    cusWidget.updateWidgetTheme();
}

TEST(Ut_Widgets, EncryptionPageTest)
{
    EncryptionPage encryPage;
    encryPage.setPassowrdFocus();
    encryPage.resetPage();
    encryPage.nextbuttonClicked();
    encryPage.wrongPassWordSlot();
    encryPage.onPasswordChanged();
    encryPage.onUpdateTheme();
}

TEST(Ut_Widgets, FileAttrWidgetTest)
{
    FileAttrWidget filewidget;
    filewidget.setFileAttr(0);
    filewidget.showScreenCenter();
    filewidget.addTitleFrame("");
}

TEST(Ut_Widgets, FindWidgetTest)
{
    FindWidget findWidget;
    //    findWidget.setDocSheet(0);
    //    findWidget.showPosition(0);
    //    findWidget.setSearchEditFocus();
    //    findWidget.setEditAlert(true);
    //    findWidget.setEditAlert(false);
    //    findWidget.findCancel();
    //    findWidget.handleContentChanged();
    //    findWidget.slotFindNextBtnClicked();
    //    findWidget.slotFindPrevBtnClicked();
    //    findWidget.slotEditAborted();
    //    findWidget.slotClearContent();
    //    findWidget.stopSearch();
}

TEST(Ut_Widgets, HandleMenuTest)
{
    HandleMenu handleMenu;
    handleMenu.readCurDocParam(0);
    handleMenu.onHandTool();
    handleMenu.onSelectText();
}

TEST(Ut_Widgets, SaveDialogTest)
{
    SaveDialog saveDialog;
}

TEST(Ut_Widgets, ScaleMenuTest)
{
    ScaleMenu scaleMenu;
    //scaleMenu.readCurDocParam(0);
}

TEST(Ut_Widgets, RoundColorWidgetTest)
{
    RoundColorWidget roundColorWidget(Qt::red);
    roundColorWidget.setSelected(true);
    roundColorWidget.setAllClickNotify(false);
    EXPECT_EQ(roundColorWidget.isSelected(), true);
}

TEST(Ut_Widgets, ShortCutShowTest)
{
    ShortCutShow shortCutDialog;
}

TEST(Ut_Widgets, SlidePlayWidgetTest)
{
    SlidePlayWidget slidePlaywidget;
}

TEST(Ut_Widgets, SpinnerWidgetTest)
{
    SpinnerWidget spinnerWidget;
}

TEST(Ut_Widgets, ScaleWidgetTest)
{
    ScaleWidget scaleWidget;
}

TEST(Ut_Widgets, TipsWidgetTest)
{
    TipsWidget tipsWidget;
}

TEST(Ut_Widgets, WordWrapLabelTest)
{
    WordWrapLabel wordLabel;
}
#endif
