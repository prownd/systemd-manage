/********************************************************************************
 * Copyright (C) 2024 Han Jinpeng <hanjinpeng127@gmail.com>                     *
 * Website: https://github.com/prownd/systemd-manage                            *
 *                                                                              *
 * This file is part of the systemd-manage project.                             *
 * License: GPLv3                                                               *
 *                                                                              *
 * This program is free software: you can redistribute it and/or modify it      *
 * under the terms of the GNU General Public License as published by the Free   *
 * Software Foundation, either version 3 of the License, or (at your option)    *
 * any later version.                                                           *
 *                                                                              *
 * This program is distributed in the hope that it will be useful, but WITHOUT  *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or        *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for     *
 * more details.                                                                *
 *                                                                              *
 * You should have received a copy of the GNU General Public License along      *
 * with this program. If not, see <https://www.gnu.org/licenses/gpl-3.0.html>.  *
 ********************************************************************************/

#ifndef GLOBALVAR_H
#define GLOBALVAR_H

#include <QString>
QString globalOrganizationName("systemd-manage");
QString globalApplicationName("systemd-manage");

QString globalApplicationWindowtStyleSheet1(R"(
/* QWidget style   */
QWidget {
    alignment: center;
}

/* QTabBar style  */
QTabBar::tab {
    /* color: rgb(84,2,119);    */
    /* background-image: url(:/pics/wood.jpg); */
    /* border: 1px solid rgb(68,66,64);     */
    /* border-bottom-color: rgb(68,66,64); */
    border-top: 2px outset transparent;
    border-bottom: 6px outset transparent;
    border-right: 6px outset transparent;
    border-left: 6px outset transparent;
    background-color: #ebebeb;
    border-top-left-radius: 4px;
    border-top-right-radius: 4px;
    margin-top: 2px;
    height: 45px;
    width: 160px;
}
QTabWidget::tab-bar {
    alignment: center;
}

QTabBar::tab:hover {
    margin-top: 5px;
    background-color: #dedede;
}

QTabBar::tab:!selected {
    margin-top: 5px;
}
QTabBar::tab:selected {
    color: rgb(255,0,128);
    background-color: #c7c7c7;
}

/* QCheckBox style   */
QCheckBox{
    height: 40px;
}
QCheckBox::indicator,QRadioButton::indicator{
    width:20px; height:20px;
}

QCheckBox,QRadioButton{spacing :10px;}

QCheckBox:hover{color:blue;}

QCheckBox:check:hover{color:red;}

QCheckBox:check,QCheckBox:hover{color:red;}

QLineEdit{
    color:rgb(127,0,63);
    background-color:rgb(255,255,241);
    selection-color:white;
    selection-background-color:rgb(191,31,127);
    border:2px groove gray;
    border-radius:10px;
    padding:2px 4px;
    height: 35px;
}

/* QToolButton style */
QToolButton{
    border-top: 0px outset transparent;
    border-bottom: 0px outset transparent;
    border-right: 0px outset transparent;
    border-left: 0px outset transparent;
    border-radius:6px;
    background-color: #ebebeb;
    width: 270px;
    height: 45px;
    margin: 0px 0px;
}

/* hover */
QToolButton:hover{
    background-color: #dedede;
}

/* press */
QToolButton:pressed {
    border-left: 0px outset rgb(93, 95, 97);
    background-color: #cdcdcd;
}

/* checked */
QToolButton:checked {
    /* background-color: #dedede;  */
    /* background-color: #00ff7e;  */
    background-color: #cdcdcd
}

/* hover and checked */
QToolButton:hover:checked {
    background-color: #d3d3d3;
}

/* press and checked */
QToolButton:pressed:checked {
    border-left: 0px outset rgb(93, 95, 97);
    background-color: #c7c7c7;
}

/* QPushButton */
QPushButton {
    border-top: 0px outset transparent;
    border-bottom: 0px outset transparent;
    border-right: 0px outset transparent;
    border-left: 0px outset transparent;
    border-radius:6px;
    background-color: #dbdbdb;
    width: 120px;
    height: 40px;
    margin: 0px 0px;
}

QPushButton:checked {
    background-color: #00ff7e;
}

/* press and checked */
QPushButton:pressed:checked {
    border-left: 0px outset rgb(93, 95, 97);
    background-color: #00ff7e;
}

QPushButton:hover:!checked{
    background-color: #cdcdcd;
}


/* QComboBox style */
QComboBox{
    color:#666666;
    font-size:16px;
    padding: 1px 15px 1px 3px;
    border:1px solid rgba(228,228,228,1);
    border-radius:5px 5px 0px 0px;
    font: 75 13pt "Arial";
}

QComboBox::drop-down {
    subcontrol-origin: padding;
    subcontrol-position: top right;
    width: 50px;
    border:none;
}

QComboBox::down-arrow {
    image: url(:/icons/down-arrow-16.svg);
}

QComboBox QAbstractItemView{
    background:rgba(255,255,255,1);
    border:1px solid rgba(228,228,228,1);
    border-radius:0px 0px 5px 5px;
    font-size:20px;
    outline: 0px;
}

QComboBox QAbstractItemView::item{
    height:36px;
    color:#666666;
    padding-left:9px;
    background-color:#FFFFFF;
}
QComboBox QAbstractItemView::item:hover{
  background-color:#409CE1;
  color:#ffffff;
}
QComboBox QAbstractItemView::item:selected{
  background-color:#409CE1;
  color:#ffffff;
}


/*  QListWidget  */
QListWidget
{
    border:1px solid gray;
    /*background:gray;*/
    color:black;
    /*margin:5px,5px,0px,50px;*/
}

/*
QListWidget::item
{
    padding-top:24px;
    padding-bottom:4px;
}
*/

QListWidget::item:hover
{
    show-decoration-selected:5;
    background:skyblue;
}

QListWidget::item:selected
{
    /*border:0px;*/
    /* background:lightgray; */
    background: rgb(63, 147, 168);
    padding:0px;
    margin:0px;
    color:red;
}

QListWidget::item:selected:!active
{
    border-width:0px;
    background:lightgreen;
}


QTreeWidget{
    /* background-color: #5B677A;    */
    background-color: #FAFAFA;
    /* set the font properties*/
    font-size:17px;
    color: black;
}

QTreeWidget::item{
    margin:13px;
    background: #FAFAFA;
    background-clip: margin;
}

QTreeWidget::branch{
     background:#FAFAFA;
}

QTreeView::item:hover {
    background: rgb(69, 187, 217);
}

QTreeView::item:selected:active{
    background: rgb(63, 147, 168);
}

QTreeView::item:selected:!active {
    background: rgb(63, 147, 168);
}



/*QTableView style*/
QTableView {
    color: black;
    selection-color: red;
    /* selection-background-color:  #bed8f6; */
    selection-background-color:  rgb(69, 187, 217);
    border: 2px outset transparent;
    border-radius: 2px;
    padding: 2px 4px;
    font: 14pt;
}

QHeaderView {
    color: black;
    font: bold 14pt;
    background-color: #dedede;
    border: 0px solid rgb(144, 144, 144);
    border:0px solid rgb(191,191,191);
    border-left-color: rgba(255, 255, 255, 0);
    border-top-color: rgba(255, 255, 255, 0);
    border-radius:0px;
    min-height:29px;
}

QHeaderView::section {
    color: black;
    background-color: #dedede;
    border: 5px solid #f6f7fa;
    border-radius:0px;
}

/* QMenu style sheet  */
QMenu {
    background-color:rgba(255,255,255,1);
    border:none;
}

QMenu::item {
    min-width:140px;
    font-size: 16px;
    color:rgba(51,51,51,1);
    padding:11px 22px;
    border-bottom:1px solid #DBDBDB;
}

QMenu::item:hover{
  background-color:#409CE1;
}

QMenu::item:selected {
    background: #409CE1;
}

)");

#endif // GLOBALVAR_H
