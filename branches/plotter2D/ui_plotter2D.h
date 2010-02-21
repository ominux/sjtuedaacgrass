/********************************************************************************
** Form generated from reading UI file 'plotter2D.ui'
**
** Created: Sun Feb 21 16:12:32 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLOTTER2D_H
#define UI_PLOTTER2D_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QSplitter>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FormPlotter2D
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_2;
    QSplitter *splitter;
    QGroupBox *groupBox_magnitude;
    QGridLayout *gridLayout;
    QWidget *widget_renderArea_magnitude;
    QGroupBox *groupBox_phase;
    QGridLayout *gridLayout_2;
    QWidget *widget_renderArea_phase;
    QWidget *tab_2;

    void setupUi(QWidget *FormPlotter2D)
    {
        if (FormPlotter2D->objectName().isEmpty())
            FormPlotter2D->setObjectName(QString::fromUtf8("FormPlotter2D"));
        FormPlotter2D->resize(482, 500);
        FormPlotter2D->setMinimumSize(QSize(480, 500));
        verticalLayout = new QVBoxLayout(FormPlotter2D);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabWidget = new QTabWidget(FormPlotter2D);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setTabsClosable(false);
        tabWidget->setMovable(true);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_2 = new QVBoxLayout(tab);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        splitter = new QSplitter(tab);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Vertical);
        groupBox_magnitude = new QGroupBox(splitter);
        groupBox_magnitude->setObjectName(QString::fromUtf8("groupBox_magnitude"));
        groupBox_magnitude->setFlat(false);
        gridLayout = new QGridLayout(groupBox_magnitude);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        widget_renderArea_magnitude = new QWidget(groupBox_magnitude);
        widget_renderArea_magnitude->setObjectName(QString::fromUtf8("widget_renderArea_magnitude"));
        widget_renderArea_magnitude->setMinimumSize(QSize(0, 0));

        gridLayout->addWidget(widget_renderArea_magnitude, 0, 0, 1, 1);

        splitter->addWidget(groupBox_magnitude);
        groupBox_phase = new QGroupBox(splitter);
        groupBox_phase->setObjectName(QString::fromUtf8("groupBox_phase"));
        gridLayout_2 = new QGridLayout(groupBox_phase);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        widget_renderArea_phase = new QWidget(groupBox_phase);
        widget_renderArea_phase->setObjectName(QString::fromUtf8("widget_renderArea_phase"));
        widget_renderArea_phase->setMinimumSize(QSize(0, 0));

        gridLayout_2->addWidget(widget_renderArea_phase, 0, 0, 1, 1);

        splitter->addWidget(groupBox_phase);

        verticalLayout_2->addWidget(splitter);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tabWidget->addTab(tab_2, QString());

        verticalLayout->addWidget(tabWidget);


        retranslateUi(FormPlotter2D);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(FormPlotter2D);
    } // setupUi

    void retranslateUi(QWidget *FormPlotter2D)
    {
        FormPlotter2D->setWindowTitle(QApplication::translate("FormPlotter2D", "GRASS Explore (2D)", 0, QApplication::UnicodeUTF8));
        groupBox_magnitude->setTitle(QApplication::translate("FormPlotter2D", "Magnitude", 0, QApplication::UnicodeUTF8));
        groupBox_phase->setTitle(QApplication::translate("FormPlotter2D", "Phase", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("FormPlotter2D", "Tab 1", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("FormPlotter2D", "Tab 2", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FormPlotter2D: public Ui_FormPlotter2D {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLOTTER2D_H
