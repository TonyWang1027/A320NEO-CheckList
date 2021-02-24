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
 * @file mainwindow.cpp
 *
 * @brief setup main window UI
 *
 * @author Tony Wang <634599706@qq.com>
 *
 * @date 2021/02/24
 *
 */



#include <QDebug>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->mainToolBar = new MainToolBar(this);  // passing mainwindow object
    this->center = new QWidget();
    this->lay = new QVBoxLayout();

    this->setupUI();
}

MainWindow::~MainWindow()
{
    config.setWindowPos(this->pos().x(), this->pos().y());

    delete this->mainToolBar;
    this->mainToolBar = nullptr;

    delete this->lay;
    this->lay = nullptr;

    delete this->center;
    this->center = nullptr;
}

void MainWindow::setupUI()
{
    config.loadWindowConfigFile();

    this->setGeometry(config.toolBar_x_pos(), config.toolBar_y_pos(), 350, 30);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setStyleSheet("background-color:#3C3C4D;");

    this->center->setLayout(this->lay);
    this->setCentralWidget(this->center);

    this->lay->addWidget(this->mainToolBar);
    this->lay->setSpacing(0);
    this->lay->setContentsMargins(0, 0, 0, 0);
}
