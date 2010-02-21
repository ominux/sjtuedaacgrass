/********************************************************************************
** Form generated from reading UI file 'plotter2D.ui'
**
** Created: Sun Feb 21 23:32:49 2010
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
    QWidget *tab_1;
    QVBoxLayout *verticalLayout_tab_1;
    QSplitter *splitter;
    QGroupBox *groupBox_magnitude;
    QGroupBox *groupBox_phase;
    QWidget *tab_2;

    void setupUi(QWidget *FormPlotter2D)
    {
        if (FormPlotter2D->objectName().isEmpty())
            FormPlotter2D->setObjectName(QString::fromUtf8("FormPlotter2D"));
        FormPlotter2D->resize(482, 506);
        FormPlotter2D->setMinimumSize(QSize(480, 500));
        verticalLayout = new QVBoxLayout(FormPlotter2D);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabWidget = new QTabWidget(FormPlotter2D);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy);
        tabWidget->setTabsClosable(false);
        tabWidget->setMovable(true);
        tab_1 = new QWidget();
        tab_1->setObjectName(QString::fromUtf8("tab_1"));
        verticalLayout_tab_1 = new QVBoxLayout(tab_1);
        verticalLayout_tab_1->setObjectName(QString::fromUtf8("verticalLayout_tab_1"));
        splitter = new QSplitter(tab_1);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Vertical);
        groupBox_magnitude = new QGroupBox(splitter);
        groupBox_magnitude->setObjectName(QString::fromUtf8("groupBox_magnitude"));
        groupBox_magnitude->setFlat(false);
        splitter->addWidget(groupBox_magnitude);
        groupBox_phase = new QGroupBox(splitter);
        groupBox_phase->setObjectName(QString::fromUtf8("groupBox_phase"));
        splitter->addWidget(groupBox_phase);

        verticalLayout_tab_1->addWidget(splitter);

        tabWidget->addTab(tab_1, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tab_2->sizePolicy().hasHeightForWidth());
        tab_2->setSizePolicy(sizePolicy1);
        tabWidget->addTab(tab_2, QString());

        verticalLayout->addWidget(tabWidget);


        retranslateUi(FormPlotter2D);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(FormPlotter2D);
    } // setupUi

    void retranslateUi(QWidget *FormPlotter2D)
    {
        FormPlotter2D->setWindowTitle(QApplication::translate("FormPlotter2D", "GRASS Explore (2D)", 0, QApplication::UnicodeUTF8));
        groupBox_magnitude->setTitle(QApplication::translate("FormPlotter2D", "Magnitude", 0, QApplication::UnicodeUTF8));
        groupBox_phase->setTitle(QApplication::translate("FormPlotter2D", "Phase", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_1), QApplication::translate("FormPlotter2D", "Tab 1", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("FormPlotter2D", "Tab 2", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FormPlotter2D: public Ui_FormPlotter2D {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLOTTER2D_H
