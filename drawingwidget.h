#ifndef DRAWINGWIDGET_H
#define DRAWINGWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPoint>
#include <QColor>
#include "editablelabel.h"

class DrawingWidget : public QWidget {
    Q_OBJECT
public:
    enum class Shape {
        None,       // 不绘制
        Line,       // 绘制线条
        Rectangle,  // 绘制矩形
        Ellipse,     // 绘制椭圆
        Eraser
    };
    enum class retColor {
        None,       // 不绘制
        Grey,
        Green,
        Red
    };
public:
    explicit DrawingWidget(QWidget* parent = nullptr);
    void loadImage(const QString& filePath);  // 加载图片
    void saveImage(const QString& filePath);  // 保存图片
    void clear();  // 清空画布
    void setPenColor(const QColor &color);  // 设置画笔颜色
    void setPenWidth(int width);  // 设置画笔宽度
    void setEraserWidth(int width);
    void expandCanvasIfNeeded(const QPoint& point);
    void setShape(Shape shape) { currentShape = shape; }
    void applyGrayscaleFilter(retColor color);//滤镜
    void applyMosaicFilter(int mosaicSize);
    void rotateImage(int degrees);  // 旋转方法
    void addLabel(EditableLabel* label);
protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event)override;
private:
    QImage canvas;         // 画布
    QImage loadedImage;    // 已加载的图片
    QPoint lastPoint;      // 鼠标最后的位置
    QPoint currentPoint;
    bool isDrawing;        // 判断是否正在绘图
    QColor penColor;       // 当前画笔颜色
    int penWidth;          // 当前画笔宽度
    float scaleFactor;
    Shape currentShape = Shape::None;//设置图形枚举值
    const int maxCanvasWidth = 2000;  // 最大画布宽度
    const int maxCanvasHeight = 2000; // 最大画布高度
    float maxScaleFactor = 2.0f;
     float miniScaleFactor = 0.5f;
    QImage originalImage ;

     int EraserWidth;

    QList<EditableLabel*> labels;

};

#endif // DRAWINGWIDGET_H
