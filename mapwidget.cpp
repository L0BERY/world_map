#include "mapwidget.h"

mapWidget::mapWidget(QWidget *parent)
    : QWidget{parent}{
    setFixedSize(768, 768);

}

void mapWidget::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.setPen(QPen(Qt::red, 2));
    for(Tile t : vec_tile){
        painter.drawImage(t.pos.x(), t.pos.y(), t.image);
    }
    drawCircle(painter);
    painter.drawEllipse(384, 384, 1, 1);
}

void mapWidget::drawMap(double lonX, double latY, double radius){
    this->radius = radius;
    this->lonX = lonX;
    this->latY = latY;
    this->zoom = getZoom();
    this->tileX = this->lon2tileX(lonX);
    this->tileY = this->lat2tileY(latY);

    double offsetX = 1 - std::get<1>(this->tileX), offsetY = 1 - std::get<1>(this->tileY);

    vec_tile.clear();
    for(int dx = -2; dx <= 2; dx++){
        for(int dy = -2; dy <= 2; dy++){

            int currentTileX = std::get<0>(this->tileX) + dx;
            if(currentTileX < 0) {
                currentTileX = std::get<0>(this->tileX) + (1 << zoom) + dx;
            }
            else if(currentTileX > (1 << zoom) - 1){
                currentTileX = std::get<0>(this->tileX) - (1 << zoom) + dx;
            }

            this->cur_file = this->file_path + QString("/%1/%2/%3.png").arg(zoom).arg(currentTileX).arg(std::get<0>(this->tileY) + dy);
            Tile t;
            QImage image(this->cur_file);
            if(image.isNull()){
                qDebug() << "Изображения нет:" <<  currentTileX << std::get<0>(this->tileY) + dy;
                continue;
            }
            t.pos.setX((dx + 1) * 256 + std::floor(256.0 * offsetX) - 128 + dx);
            t.pos.setY((dy + 1) * 256 + std::floor(256.0 * offsetY) - 128 + dy);
            t.image = image;
            vec_tile.append(t);
            update();
        }
    }
}

int mapWidget::getZoom(){
    const double Earth = 40075.0;
    double correct = std::cos(std::abs(this->latY) * M_PI / 180.0);
    correct = std::max(correct, 0.1);
    double correctRadius = this->radius / correct;
    int zoom = static_cast<int>(std::round(std::log2(Earth / correctRadius)));
    return std::min(std::max(zoom, 0), 10);
}

std::tuple<int, double> mapWidget::lon2tileX(double lon){
    double val = (lon + 180.0) / 360 * (1 << this->zoom);
    int tileX = (int)(std::floor(val));
    return std::make_tuple(tileX, val - tileX);
}

std::tuple<int, double> mapWidget::lat2tileY(double lat){
    double latrad = lat * M_PI / 180.0;
    double val = (1.0 - std::asinh(tan(latrad)) / M_PI) / 2.0 * (1 << this->zoom);
    int tileY = (int)(std::floor(val));
    return std::make_tuple(tileY, val - tileY);
}

void mapWidget::drawCircle(QPainter &painter){
    const double radiusEarth = 6378.0;
    const int pointCount = 200;
    QPolygonF pol;
    double radianRadius = this->radius / radiusEarth;
    double lonRad_0 = this->lonX * M_PI / 180.0;
    double latRad_0 = this->latY * M_PI / 180.0;
    for(int i = 0; i < pointCount; i++){
        double angle = 2 * M_PI * i / pointCount;

        double latRad_i = std::asin(
            std::sin(latRad_0) * std::cos(radianRadius) +
            std::cos(latRad_0) * std::sin(radianRadius) * std::cos(angle));;

        double lonRad_i = lonRad_0 + std::atan2(
            std::sin(angle) * std::sin(radianRadius) * std::cos(latRad_0),
            std::cos(radianRadius) - std::sin(latRad_0) * std::sin(latRad_i));

        double lon_i = lonRad_i * 180.0 / M_PI;
        double lat_i = latRad_i * 180.0 / M_PI;

        std::tuple<int, double> tile_X_i = this->lon2tileX(lon_i);
        std::tuple<int, double> tile_Y_i = this->lat2tileY(lat_i);

        double offsetX = 1 - std::get<1>(tile_X_i), offsetY = 1 - std::get<1>(tile_Y_i);

        int X = (std::get<0>(tile_X_i) - std::get<0>(this->tileX) + 2) * 256 -
            std::floor(256.0 * offsetX) - std::floor(256.0 * (std::get<1>(this->tileX))) + 128;

        int Y = (std::get<0>(tile_Y_i) - std::get<0>(this->tileY) + 2) * 256 -
            std::floor(256.0 * offsetY) - std::floor(256.0 * (std::get<1>(this->tileY))) + 128;

        QPointF point(X, Y);
        // painter.drawEllipse(X-1, Y-1, 2, 2);
        pol << point;
    }
    painter.drawPolygon(pol);
}
