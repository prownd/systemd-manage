/********************************************************************************
 * Obtained from https://github.com/epasveer/QProcessInfoWidget                 *
 * My version (epasveer) is based on this person's code.                        *
 *                                                                              *
 * Copyright (c) 2016, Baldur Karlsson                                          *
 * Licensed under BSD 2-Clause License, see LICENSE file.                       *
 * Obtained from https://github.com/baldurk/qprocessinfo                        *
 *                                                                              *
 *                                                                              *
 * This version is modify by Han Jinpeng <hanjinpeng127@gmail.com>              *
 * Website: https://github.com/prownd/systemd-manage                            *
 *                                                                              *
 * Licensed under BSD 2-Clause License or GPLv3                                 *
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
        QString selectedCPUTime() const;
        quint64 selectedMemory() const;
        QString selectedNice() const;
        QString selectedState() const;
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
        //void QTreeWidgetitemPressedSlot(QTreeWidgetItem * pressedItem, int column);
        void QTreeWidgetitemPressedSlot(const QPoint& pos);
        void handleQProcessDetailAction();
        void handleQProcessKillAction();
        void slotTreeWidgetItemDoubleClicked(QTreeWidgetItem* item, int column);

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
