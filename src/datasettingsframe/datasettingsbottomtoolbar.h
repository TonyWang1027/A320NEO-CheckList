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
 * @file datasettingsbottomtoolbar.h
 *
 * @author Tony Wang <634599706@qq.com>
 *
 * @date 2021/02/24
 *
 */



#ifndef DATASETTINGSBOTTOMTOOLBAR_H
#define DATASETTINGSBOTTOMTOOLBAR_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QString>
#include <QSizePolicy>

class DataSettingsBottomToolBar : public QWidget
{
    Q_OBJECT
public:
    explicit DataSettingsBottomToolBar(QWidget *parent = nullptr);
    virtual ~DataSettingsBottomToolBar();

    const QString getUserDataFromLE();
    void cleanLineEdit();

private:
    QLineEdit *userDataLineEdit;
    QHBoxLayout *lay;

    void setupUI();
};

#endif // DATASETTINGSBOTTOMTOOLBAR_H
