#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mapwidget.h"

#include <QFileDialog>
#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString file_path;
    void clearLayout();

private slots:
    void on_pushButton_clicked();
    void on_saveButton_clicked();

private:
    Ui::MainWindow *ui;
    mapWidget *map_widget;
};
#endif // MAINWINDOW_H
