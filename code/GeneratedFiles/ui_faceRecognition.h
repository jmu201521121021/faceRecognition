/********************************************************************************
** Form generated from reading UI file 'faceRecognition.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FACERECOGNITION_H
#define UI_FACERECOGNITION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_faceRecognitionClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *faceRecognitionClass)
    {
        if (faceRecognitionClass->objectName().isEmpty())
            faceRecognitionClass->setObjectName(QStringLiteral("faceRecognitionClass"));
        faceRecognitionClass->resize(600, 400);
        menuBar = new QMenuBar(faceRecognitionClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        faceRecognitionClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(faceRecognitionClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        faceRecognitionClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(faceRecognitionClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        faceRecognitionClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(faceRecognitionClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        faceRecognitionClass->setStatusBar(statusBar);

        retranslateUi(faceRecognitionClass);

        QMetaObject::connectSlotsByName(faceRecognitionClass);
    } // setupUi

    void retranslateUi(QMainWindow *faceRecognitionClass)
    {
        faceRecognitionClass->setWindowTitle(QApplication::translate("faceRecognitionClass", "faceRecognition", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class faceRecognitionClass: public Ui_faceRecognitionClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FACERECOGNITION_H
