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
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef SEARCHRESDELEGATE_H
#define SEARCHRESDELEGATE_H

#include <DStyledItemDelegate>

DWIDGET_USE_NAMESPACE
/**
 * @brief The SearchResDelegate class
 * 搜索代理
 */
class SearchResDelegate : public DStyledItemDelegate
{
public:
    explicit SearchResDelegate(QAbstractItemView *parent = nullptr);

protected:
    /**
     * @brief paint
     * 绘制事件
     * @param painter
     * @param option
     * @param index
     */
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    /**
     * @brief sizeHint
     * 节点大小
     * @param option
     * @param index
     * @return
     */
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    QAbstractItemView *m_parent = nullptr;
};

#endif // SEARCHRESDELEGATE_H
