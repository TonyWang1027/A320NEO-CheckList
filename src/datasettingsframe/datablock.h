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
 * @file datablock.h
 *
 * @author Tony Wang <634599706@qq.com>
 *
 * @date 2021/02/24
 *
 */



#ifndef DATABLOCK_H
#define DATABLOCK_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QColor>
#include <QDebug>

#define BLOCKHEIGHT 30

class DataBlock : public QWidget
{
    Q_OBJECT
public:
    explicit DataBlock(const int dataId = -1, const QString& dataName = nullptr, const QString& userData = nullptr);
    virtual ~DataBlock();

    QString getDataName() const;
    void setDataName(const QString &value);

    QString getUserData() const;
    void setUserData(const QString &value);

    int getDataId() const;
    void setDataId(int value);

private:
    QString dataName;
    QString userData;
    int dataId;
    bool isPressed;
    bool isHovered;

    QLabel *dataNameLabel;
    QLabel *userDataLabel;
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
    void mousePressed_signal(int);

};

#endif // DATABLOCK_H
