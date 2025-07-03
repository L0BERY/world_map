#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    map_widget = new mapWidget(this);
    ui->layout->addWidget(map_widget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    file_path = ui->file_path->toPlainText();
    map_widget->file_path = this->file_path;
    map_widget->drawMap(ui->x_spin->value(), ui->y_spin->value(), ui->radius->value());
    ui->saveButton->setEnabled(true);

}

void MainWindow::on_saveButton_clicked()
{
    QString file = QFileDialog::getSaveFileName(this, "Сохранить", "map", "PNG (*.png);; JPEG (*.jpg *.jpeg)");
    map_widget->grab().save(file);
}





