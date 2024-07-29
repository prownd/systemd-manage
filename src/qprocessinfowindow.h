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

#ifndef QPROCESSINFOWINDOW_H
#define QPROCESSINFOWINDOW_H

#include <QWidget>
#include <QCheckBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QToolButton>
#include <QTreeWidget>
#include <QLabel>

class QProcessInfoWidgetItem : public QTreeWidgetItem
{
public:
    QProcessInfoWidgetItem(QTreeWidget* parent = 0) : QTreeWidgetItem(parent)
    {
    }

private:
    bool operator< (const QTreeWidgetItem& other) const
    {
        int column = treeWidget()->sortColumn();
        if (column == 0) {
            return text(column).toLong() < other.text(column).toLong();
        }
        return text(column) < other.text(column);
    }
};

class QProcessInfoWidget : public QWidget
{
    Q_OBJECT
    public:
        explicit QProcessInfoWidget(QWidget* parent = 0);
       ~QProcessInfoWidget();

        int selectedPid() const;
        int selectedPPid() const;
        QString selectedUsername() const;
        QString selectedName() const;
        QString selectedTime() const;
        QString selectedCommandLine() const;
        void createProcessInfoView();
        QString addTextSpaceMargin(const QString);

    signals:
        void pidSelected(int pid);

    public slots:
    protected slots:
        void refreshList();
        void refreshView();
        void handleDoubleClicked();

    protected:
    private:
        QGridLayout *   m_mainGridLayout;
        QLabel *        m_programNameLabel;
        QLabel *        m_userNameLabel;
        QLineEdit *     m_programNameLineEdit;
        QLineEdit *     m_userNameLineEdit;
        QToolButton *   m_refreshToolButton;
        QTreeWidget *   m_processTreeWidget;
};

#endif // QPROCESSINFOWINDOW_H
