#include "mapwidget.h"

mapWidget::mapWidget(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(768, 768);

}

void mapWidget::paintEvent(QPaintEvent *){
    QPainter painter(this);
    for(Tile t : vec_tile){
        painter.drawImage(t.pos.x(), t.pos.y(), t.image);
    }
    // drawCircle(painter);
}

void mapWidget::drawMap(double lonX, double latY, double radius){
    this->radius = radius;
    int zoom = getZoom(latY);
    std::tuple<int, double> tileX = this->lon2tileX(lonX, zoom);
    std::tuple<int, double> tileY = this->lat2tileY(latY, zoom);
    // qDebug() << std::get<1>(tileX) << std::get<1>(tileY);

    double offsetX = 1 - std::get<1>(tileX), offsetY = 1 - std::get<1>(tileY);

    vec_tile.clear();
    for(int dx = -2; dx <= 2; dx++){
        for(int dy = -2; dy <= 2; dy++){
            this->cur_file = this->file_path + QString("/%1/%2/%3.png").arg(zoom).arg(std::get<0>(tileX) + dx).arg(std::get<0>(tileY) + dy);
            Tile t;
            QImage image(this->cur_file);
            t.pos.setX((dx + 1) * 256 + std::floor(256.0 * offsetX) - 128 + dx);
            t.pos.setY((dy + 1) * 256 + std::floor(256.0 * offsetY) - 128 + dy);
            t.image = image;
            vec_tile.append(t);
            update();
        }
    }
}

int mapWidget::getZoom(double lat){
    const double Earth = 40075.0;
    double correct = std::cos(std::abs(lat) * M_PI / 180.0);
    correct = std::max(correct, 0.1);
    double correctRadius = this->radius / correct;
    int zoom = static_cast<int>(std::round(std::log2(Earth / correctRadius)));
    // qDebug() << std::log2(Earth / correctRadius);
    return std::min(std::max(zoom, 0), 10);
}

std::tuple<int, double> mapWidget::lon2tileX(double lon, int z){
    double val = (lon + 180.0) / 360 * (1 << z);
    int tileX = (int)(std::floor(val));
    return std::make_tuple(tileX, val - tileX);
}

std::tuple<int, double> mapWidget::lat2tileY(double lat, int z){
    double latrad = lat * M_PI / 180.0;
    double val = (1.0 - std::asinh(tan(latrad)) / M_PI) / 2.0 * (1 << z);
    int tileY = (int)(std::floor(val));
    return std::make_tuple(tileY, val - tileY);
}

// void mapWidget::drawCircle(QPainter &painter){
//     int pointCount = 100;
//     QPolygonF pol;
//     for(int i = 0; i < pointCount; i++){
//         double angle = 2 * M_PI * i / pointCount;
//         QPointF point;

//         pol << point;
//     }
//     painter.drawPolygon(pol);

//     // painter.drawEllipse(384, 384, 100, 100);

// }
