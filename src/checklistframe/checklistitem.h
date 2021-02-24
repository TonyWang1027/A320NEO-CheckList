/*
* Airbus A320NEO CheckList For MSFS2020
*
* This file is part of Airbus A320NEO CheckList Project
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Airbus A320NEO CheckList For MSFS2020.
* If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file checklistitem.h
 *
 * @author Tony Wang <634599706@qq.com>
 *
 * @date 2021/02/24
 *
 */



#ifndef CHECKLISTITEM_H
#define CHECKLISTITEM_H

#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QString>
#include <QEvent>
#include <QPaintEvent>
#include <QStyleOption>
#include <QPainter>
#include <QBrush>
#include <QColor>
#include <QPen>
#include <QPixmap>

#define ITEMHEIGHT 40   // This height is the check list item bar height

class CheckListItem : public QWidget
{
    Q_OBJECT
public:
    explicit CheckListItem(uint8_t t_editable = 0, bool *t_isChecked = nullptr);
    virtual ~CheckListItem();

    void setContentLabel(const QString& content);
    void setStatusLabel(const QString& status);
    void adjustItemStyle();
    void setItemStyle(const bool &status);

private:
    bool editable;
    bool isHovered;
    bool isPressed;
    bool *isChecked;

    QLabel *contentLabel;
    QLabel *displayIconLabel;
    QLabel *statusLabel;

    QHBoxLayout *lay;

    void setupUI();
    void events();

    /* Overrides */
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void itemStatusChanged_signal();

};

#endif // CHECKLISTITEM_H
