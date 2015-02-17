/**
 * This file is part of lvfs-core.
 *
 * Copyright (C) 2011-2014 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * lvfs-core is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * lvfs-core is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with lvfs-core. If not, see <http://www.gnu.org/licenses/>.
 */

#include "lvfs_core_qt_StyledItemDelegate.h"

#include <lvfs-core/models/Qt/INode>
#include <QtGui/QApplication>


namespace LVFS {
namespace Core {
namespace Qt {

StyledItemDelegate::StyledItemDelegate(const Interface::Holder &node, const QSortFilterProxyModel &proxy, QObject *parent) :
    QStyledItemDelegate(parent),
    m_node(node),
    m_proxy(proxy)
{}

void StyledItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    quint64 total;
    quint64 progress;

    if (index.column() == 1 && m_node->as<Qt::INode>()->isLocked(m_proxy.mapToSource(index), progress, total))
        paintProgressInMb(painter, option, total ? progress * 100 / total : 100);
    else
        paintBackgroundLines(painter, option, index);
}

void StyledItemDelegate::paintBackgroundLines(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 opt = option;
    initStyleOption(&opt, index);

    if (index.row() % 2 == 0)
        opt.backgroundBrush = QBrush(::Qt::white);
    else
        opt.backgroundBrush = QBrush(QColor(245, 245, 245));

    if (const QStyleOptionViewItemV3 *v3 = qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option))
        v3->widget->style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter, v3->widget);
    else
        QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter, 0);
}

void StyledItemDelegate::paintProgressInMb(QPainter *painter, const QStyleOptionViewItem &option, int progress) const
{
    if (progress == 100)
    {
        QStyleOptionProgressBarV2 progressBarOption;
        progressBarOption.rect = option.rect;
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 100;
        progressBarOption.progress = 100;
//        progressBarOption.text = Tools::humanReadableTime(entry->timeElapsed());
//        progressBarOption.textAlignment = Qt::AlignCenter;
//        progressBarOption.textVisible = true;

        if (const QStyleOptionViewItemV3 *v3 = qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option))
            v3->widget->style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
        else
            QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
    }
    else
    {
        QStyleOptionProgressBarV2 progressBarOption;
        progressBarOption.rect = option.rect;
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 100;
        progressBarOption.progress = progress;

//        progressBarOption.text =
//                Tools::humanReadableShortSize(entry->done()).
//                append(QString::fromLatin1(" / ")).
//                append(Tools::humanReadableShortSize(entry->total())).
//                append(QString::fromLatin1("  ")).
//                append(Tools::humanReadableTime((entry->total() / entry->done()) * entry->timeElapsed()));
//        progressBarOption.textAlignment = ::Qt::AlignCenter;
//        progressBarOption.textVisible = true;

        if (const QStyleOptionViewItemV3 *v3 = qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option))
            v3->widget->style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
        else
            QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
    }
}

}}}
