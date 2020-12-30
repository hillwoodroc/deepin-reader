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
#include "ScaleWidget.h"

#include <QHBoxLayout>
#include <QLineEdit>

ScaleWidget::ScaleWidget(DWidget *parent)
    : DWidget(parent)
{
    initWidget();
}

ScaleWidget::~ScaleWidget()
{

}

void ScaleWidget::initWidget()
{
    QHBoxLayout *m_layout = new QHBoxLayout();
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(10);
    setLayout(m_layout);

    m_lineEdit = new DLineEdit(this);
    m_lineEdit->lineEdit()->setObjectName("scaleEdit");
    QFont font = m_lineEdit->font();
    font.setPixelSize(14);
    m_lineEdit->setFont(font);
    m_lineEdit->setFixedSize(120, 36);

    DIconButton *arrowBtn = new DIconButton(QStyle::SP_ArrowDown, m_lineEdit);
    arrowBtn->setObjectName("editArrowBtn");
    arrowBtn->setFixedSize(32, 32);
    arrowBtn->move(m_lineEdit->width() - arrowBtn->width() - 2, 2);
    m_lineEdit->lineEdit()->setTextMargins(0, 0, arrowBtn->width(), 0);
    m_lineEdit->setClearButtonEnabled(false);

    connect(m_lineEdit, SIGNAL(returnPressed()), SLOT(onReturnPressed()));
    connect(m_lineEdit, SIGNAL(editingFinished()), SLOT(onEditFinished()));
    connect(arrowBtn, SIGNAL(clicked()), SLOT(onArrowBtnlicked()));

    DIconButton *pPreBtn = new DIconButton(DStyle::SP_DecreaseElement);
    pPreBtn->setObjectName("SP_DecreaseElement");
    DStyle::setFrameRadius(pPreBtn, 12);
    pPreBtn->setFixedSize(24, 24);
    connect(pPreBtn, SIGNAL(clicked()), SLOT(onPrevScale()));

    DIconButton *pNextBtn = new DIconButton(this/*DStyle::SP_IncreaseElement*/);
    pNextBtn->setIcon(QIcon::fromTheme("dr_button_add"));
    pNextBtn->setObjectName("SP_IncreaseElement");
    DStyle::setFrameRadius(pNextBtn, 12);
    pNextBtn->setFixedSize(24, 24);
    connect(pNextBtn, SIGNAL(clicked()), SLOT(onNextScale()));

    m_layout->addWidget(pPreBtn);
    m_layout->addWidget(m_lineEdit);
    m_layout->addWidget(pNextBtn);
}

void ScaleWidget::onPrevScale()
{
    if (m_sheet.isNull())
        return;

    m_sheet->zoomout();
}

void ScaleWidget::onNextScale()
{
    if (m_sheet.isNull())
        return;

    m_sheet->zoomin();
}

void ScaleWidget::onReturnPressed()
{
    if (m_sheet.isNull())
        return;

    qreal value = m_lineEdit->text().replace("%", "").toDouble() / 100.00;
    value = qBound(0.1, value, m_sheet->maxScaleFactor());
    m_sheet->setScaleFactor(value);
}

void ScaleWidget::onArrowBtnlicked()
{
    m_lineEdit->lineEdit()->setFocus(Qt::MouseFocusReason);

    ScaleMenu scaleMenu;
    QPoint point = m_lineEdit->mapToGlobal(QPoint(0, m_lineEdit->height() + 2));
    scaleMenu.readCurDocParam(m_sheet.data());
    scaleMenu.exec(point);
}

void ScaleWidget::onEditFinished()
{
    if (nullptr == m_sheet)
        return;

    QString text = QString::number(QString::number(m_sheet->operation().scaleFactor * 100, 'f', 2).toDouble()) + "%";
    m_lineEdit->setText(text);
}

void ScaleWidget::setSheet(DocSheet *sheet)
{
    m_sheet = sheet;
    if (nullptr == sheet)
        return;
    else if (!sheet->isUnLocked()) {
        clear();
        return;
    }

    m_lineEdit->clear();
    QString text = QString::number(QString::number(m_sheet->operation().scaleFactor * 100, 'f', 2).toDouble()) + "%";
    m_lineEdit->setText(text);
    m_lineEdit->lineEdit()->setCursorPosition(0);
}

void ScaleWidget::clear()
{
    m_lineEdit->clear();
}
