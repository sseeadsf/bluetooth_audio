/********************************************************************************
** Form generated from reading UI file 'progress.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROGRESS_H
#define UI_PROGRESS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Progress
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QProgressBar *progressBar;
    QLabel *statusLabel;
    QPushButton *cancelButton;

    void setupUi(QDialog *Progress)
    {
        if (Progress->objectName().isEmpty())
            Progress->setObjectName(QStringLiteral("Progress"));
        Progress->resize(408, 115);
        verticalLayout = new QVBoxLayout(Progress);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        titleLabel = new QLabel(Progress);
        titleLabel->setObjectName(QStringLiteral("titleLabel"));

        verticalLayout->addWidget(titleLabel);

        progressBar = new QProgressBar(Progress);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setMinimumSize(QSize(390, 0));
        progressBar->setValue(24);

        verticalLayout->addWidget(progressBar);

        statusLabel = new QLabel(Progress);
        statusLabel->setObjectName(QStringLiteral("statusLabel"));

        verticalLayout->addWidget(statusLabel);

        cancelButton = new QPushButton(Progress);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));

        verticalLayout->addWidget(cancelButton);


        retranslateUi(Progress);

        QMetaObject::connectSlotsByName(Progress);
    } // setupUi

    void retranslateUi(QDialog *Progress)
    {
        Progress->setWindowTitle(QApplication::translate("Progress", "Transfer Progress", Q_NULLPTR));
        titleLabel->setText(QApplication::translate("Progress", "Filename", Q_NULLPTR));
        statusLabel->setText(QApplication::translate("Progress", "Status", Q_NULLPTR));
        cancelButton->setText(QApplication::translate("Progress", "Cancel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Progress: public Ui_Progress {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROGRESS_H
