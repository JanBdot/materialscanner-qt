#pragma once

#include <QtWidgets/QWidget>
#include "ui_materialscanner.h"

class materialscanner : public QWidget
{
    Q_OBJECT

public:
    materialscanner(QWidget *parent = Q_NULLPTR);

private:
    Ui::materialscannerClass ui;
};
