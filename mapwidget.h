#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <cmath>
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
    std::tuple<int, double> lon2tileX(double);
    std::tuple<int, double> lat2tileY(double);
    void drawMap(double, double, double);

private:
    struct Tile{
        QImage image;
        QPoint pos;
    };
    QVector<Tile> vec_tile;
    int getZoom();
    int zoom;
    void drawCircle(QPainter &);
    double lonX, latY, radius;
    std::tuple<int, double> tileX, tileY;

protected:
    void paintEvent(QPaintEvent *);
};

#endif // MAPWIDGET_H
