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
 * @file configuration.cpp
 *
 * @brief Read and write window properties (window x position and y position)
 *
 * @author Tony Wang <634599706@qq.com>
 *
 * @date 2021/02/24
 *
 */



#include "configuration.h"

Configuration::Configuration()
{
    m_fileName = "win.ini";
}

void Configuration::loadWindowConfigFile()
{
    QString root = "mainwindow/";
    QSettings settings(m_fileName, QSettings::IniFormat);
    setToolBar_x_pos(settings.value(root + "x_pos").toInt());
    setToolBar_y_pos(settings.value(root + "y_pos").toInt());

    root = "greetingwindow/";
    setShowGreetingWin(settings.value(root + "show").toBool());
}

void Configuration::setWindowPos(const int &x_pos, const int &y_pos)
{
    QString root = "mainwindow";
    QSettings settings(m_fileName, QSettings::IniFormat);
    settings.beginGroup(root);
    settings.setValue("x_pos", x_pos);
    settings.setValue("y_pos", y_pos);
    settings.endGroup();
}

/* Greeting Window is not added to this project yet */
void Configuration::setShowGreetingWindow(const uint8_t &status)
{
    QString root = "greetingwindow";
    QSettings settings(m_fileName, QSettings::IniFormat);
    settings.beginGroup(root);
    settings.setValue("show", status);
    settings.endGroup();
}

QString Configuration::fileName() const
{
    return m_fileName;
}

void Configuration::setFileName(const QString &fileName)
{
    m_fileName = fileName;
}

int Configuration::toolBar_x_pos() const
{
    return m_toolBar_x_pos;
}

void Configuration::setToolBar_x_pos(int toolBar_x_pos)
{
    m_toolBar_x_pos = toolBar_x_pos;
}

int Configuration::toolBar_y_pos() const
{
    return m_toolBar_y_pos;
}

void Configuration::setToolBar_y_pos(int toolBar_y_pos)
{
    m_toolBar_y_pos = toolBar_y_pos;
}

bool Configuration::showGreetingWin() const
{
    return m_showGreetingWin;
}

void Configuration::setShowGreetingWin(const bool &showGreetingWin)
{
    m_showGreetingWin = showGreetingWin;
}
