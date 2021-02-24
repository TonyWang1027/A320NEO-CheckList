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
 * @file configuration.h
 *
 * @author Tony Wang <634599706@qq.com>
 *
 * @date 2021/02/24
 *
 */



#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QString>
#include <QSettings>

class Configuration
{
public:
    Configuration();
    virtual ~Configuration() { }

    void loadWindowConfigFile();
    void setWindowPos(const int &x_pos, const int &y_pos);
    void setShowGreetingWindow(const uint8_t &status);

    /* Getters & Setters */
    QString fileName() const;
    void setFileName(const QString &fileName);

    int toolBar_x_pos() const;
    void setToolBar_x_pos(int toolBar_x_pos);

    int toolBar_y_pos() const;
    void setToolBar_y_pos(int toolBar_y_pos);

    bool showGreetingWin() const;
    void setShowGreetingWin(const bool &showGreetingWin);

private:
    int m_toolBar_x_pos;
    int m_toolBar_y_pos;
    bool m_showGreetingWin;
    QString m_fileName;

};

#endif // CONFIGURATION_H
