#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QVector>
#include <QDebug>
#include <QPainter>

class mapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit mapWidget(QWidget *parent = nullptr);
    QString file_path, cur_file;
    std::tuple<int, double> lon2tileX(double, int);
    std::tuple<int, double> lat2tileY(double, int);
    void drawMap(double, double, double);

private:
    struct Tile{
        QImage image;
        QPoint pos;
    };
    QVector<Tile> vec_tile;
    int getZoom(double);
    void drawCircle(QPainter &);
    double radius;

protected:
    void paintEvent(QPaintEvent *);
};

#endif // MAPWIDGET_H
