#include "drawingwidget.h"
#include "editablelabel.h"
#include <QPainter>
#include <QMouseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QPen>
#include <QLabel>
#include <QColor>
#include <QImage>
#include <QIcon>
#include "editablelabel.h"
#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif
DrawingWidget::DrawingWidget(QWidget* parent)
    : QWidget(parent), isDrawing(false), scaleFactor(1.0f),penColor(Qt::black), penWidth(5),currentShape(Shape::None) {
    // 初始化画布大小，和窗口一致
    setMinimumSize(1000,1000);
     setWindowIcon(QIcon(":/drawicon.ico"));
    canvas = QImage(1000, 1000, QImage::Format_ARGB32);
    canvas.fill(Qt::white);  // 初始填充白色

}

void DrawingWidget::loadImage(const QString& filePath) {
    qDebug() << filePath;
    if (!filePath.isEmpty()) {
        QImage image;
        if (image.load(filePath)) {

            // 获取窗口大小
            QSize windowSize = this->size();
            QSize imageSize = image.size();

            // 如果图片大小超过窗口大小，则缩小；如果图片太小，可以放大
            if (imageSize.width() > windowSize.width() || imageSize.height() > windowSize.height()) {
                // 缩放图片保持其宽高比，适应窗口大小
                image = image.scaled(windowSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            }
            originalImage = image;
            loadedImage = image;

            // 清空画布并创建适当大小的QImage画布
            canvas = QImage(windowSize, QImage::Format_ARGB32); // 根据窗口大小创建空白画布
            canvas.fill(Qt::transparent); // 设置画布背景透明或其他颜色

            QPainter painter(&canvas);
            painter.drawImage(QPoint(0, 0), loadedImage);  // 绘制图片到画布

            update();  // 刷新窗口，重新绘制
        } else {
            QMessageBox::warning(this, tr("加载失败"), tr("无法加载图片！"));
        }
    }
}



void DrawingWidget::saveImage(const QString& filePath) {
    if (!filePath.isEmpty()) {
        QImage finalImage(canvas.size(), QImage::Format_ARGB32);
        finalImage.fill(Qt::white);  // 使用白色背景

        QPainter painter(&finalImage);
        painter.setRenderHint(QPainter::Antialiasing, true);

        painter.drawImage(QPoint(0, 0), canvas);

        // 绘制所有的文本框（EditableLabel）
        for (EditableLabel* label : labels) {
            // 获取文本框的位置信息和文本内容
            QPoint position = label->pos();
            QString text =label->text();

            // 使用绘制文字的画笔和字体
            painter.setPen(Qt::black); // 设置文字颜色
            painter.setFont(label->font()); // 使用文本框的字体

            // 绘制文本到画布上的指定位置
            painter.drawText(position, text);
        }

        painter.end(); // 结束绘制

        // 将最终的图像保存到指定的文件路径
        finalImage.save(filePath);  // 将当前画布保存为图像文件

    }
}

void DrawingWidget::clear() {
    canvas.fill(Qt::white);  // 清空画布为白色
    loadedImage = QImage();  // 清除已加载的图片
    for (EditableLabel* label : labels) {
        label->close();  // 关闭并删除标签
    }
    labels.clear();
    update();  // 刷新窗口

}


void DrawingWidget::setPenColor(const QColor &color) {
    penColor = color;  // 更新画笔颜色
}

void DrawingWidget::setPenWidth(int width) {
    penWidth = width;  // 更新画笔宽度
}
void DrawingWidget::setEraserWidth(int width) {
    EraserWidth = width;  // 更新画笔宽度
}
void DrawingWidget::addLabel(EditableLabel* label) {
    labels.append(label);     // 添加到列表中进行管理
    label->setParent(this);   // 设置父控件为 DrawingWidget，使其跟随画布一起显示
    label->show();            // 显示标签
}
void DrawingWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    // 绘制加载的图片
    painter.fillRect(rect(), QColor(135, 206, 235));
    painter.scale(scaleFactor, scaleFactor);
    painter.drawImage(QPoint(0, 0), canvas);
    QPen pen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);


    switch (currentShape) {//窗口实时性
    case Shape::Line:
        painter.drawLine(lastPoint, currentPoint);
        break;
    case Shape::Rectangle:
        painter.drawRect(QRect(lastPoint, currentPoint));
        break;
    case Shape::Ellipse:
        painter.drawEllipse(QRect(lastPoint, currentPoint));
        break;
    default:
        break;
    }

}

void DrawingWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos() / scaleFactor;  // 记录按下时的坐标
        currentPoint = lastPoint;
        isDrawing = true;  // 开始绘图
    }
}

void DrawingWidget::mouseMoveEvent(QMouseEvent* event) {
    if (isDrawing && (event->buttons() & Qt::LeftButton)) {
        QPoint newPoint = event->pos() / scaleFactor; // 根据缩放因子调整坐标
        expandCanvasIfNeeded(newPoint);

        QPainter painter(&canvas);
        QPen pen = (currentShape == Shape::Eraser) ?
                       QPen(Qt::white, EraserWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin) :
                       QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(pen);

        if (currentShape == Shape::None || currentShape == Shape::Eraser) {
            painter.drawLine(lastPoint, newPoint);
        }
        lastPoint = newPoint;  // 更新最后位置
        update();  // 刷新显示
    }
}
void DrawingWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && isDrawing) {
        QPainter painter(&canvas);
        QPoint newPoint = event->pos() / scaleFactor; // 根据缩放因子调整坐标
        expandCanvasIfNeeded(newPoint);
        QPen pen = (currentShape == Shape::Eraser) ?
                       QPen(Qt::white, EraserWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin) :
                       QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(pen);

        switch (currentShape) {//真正绘制到画布保存
        case Shape::Line:
            painter.drawLine(lastPoint, currentPoint);
            break;
        case Shape::Rectangle:
            painter.drawRect(QRect( currentPoint,lastPoint));
            break;
        case Shape::Ellipse:
            painter.drawEllipse(QRect(lastPoint, currentPoint));
            break;
        case Shape::Eraser:
            painter.drawLine(lastPoint, currentPoint);
            break;
        default:
            break;
        }

        // 停止绘图

        isDrawing = false;
        update();
    }

}
void DrawingWidget::wheelEvent(QWheelEvent* event) {
    const int numDegrees = event->angleDelta().y(); // 获取滚轮滚动的角度
    const float scaleStep = 0.1f;  // 每次缩放的步长

    if (numDegrees > 0) {
        // 向上滚动，放大
        float newScaleFactor = scaleFactor + scaleStep; // 计算新的缩放因子
        // 检查新的缩放因子是否会导致画布尺寸超过最大值
        if (canvas.width() * newScaleFactor <= maxCanvasWidth && canvas.height() * newScaleFactor <= maxCanvasHeight) {
            scaleFactor = newScaleFactor; // 更新缩放因子
        }
    } else  {

        // 向下滚动，缩小，但限制缩放最小值为0.1，防止太小
        scaleFactor = std::max(miniScaleFactor, scaleFactor - scaleStep);
        }
      qDebug()<<scaleFactor;
        for (EditableLabel* label : labels) {
            // 计算新的位置和大小
            QPoint newPos = label->pos() * scaleFactor;
            QSize newSize = label->size() * scaleFactor;
            label->move(newPos);
            label->resize(newSize);
        }
    update();  // 更新绘图区域
}

void DrawingWidget::expandCanvasIfNeeded(const QPoint& point) {
    int canvasWidth = canvas.width();
    int canvasHeight = canvas.height();
    qDebug() << "Current canvas size:" << canvasWidth << "x" << canvasHeight;

    // 检查点是否在当前画布范围内
    if (point.x() < 0 || point.x() >= canvasWidth || point.y() < 0 || point.y() >= canvasHeight) {
        // 计算新画布的尺寸，留出一定空间
        int newWidth = std::max(canvasWidth, point.x() + 100);
        int newHeight = std::max(canvasHeight, point.y() + 100);

        // 限制新画布的尺寸，确保不超过最大值
        newWidth = std::min(newWidth, maxCanvasWidth);
        newHeight = std::min(newHeight, maxCanvasHeight);

        // 创建新的画布
        QImage newCanvas(newWidth, newHeight, QImage::Format_ARGB32);
        newCanvas.fill(Qt::white);  // 初始化为白色

        // 将旧画布内容复制到新画布
        QPainter painter(&newCanvas);
        painter.drawImage(0, 0, canvas);

        // 更新当前画布
        canvas = newCanvas;
    }
}
void DrawingWidget::applyGrayscaleFilter(retColor rcolor) {
    if (originalImage.isNull()) return;

    QImage grayscaleImage = originalImage;

    for (int y = 0; y < grayscaleImage.height(); ++y) {
        for (int x = 0; x < grayscaleImage.width(); ++x) {
            QColor color = grayscaleImage.pixelColor(x, y);//拿到像素点的颜色
            int currentColor;

            // 根据选择的颜色类型应用不同的灰度处理
            switch (rcolor) {
            case retColor::Grey:
                currentColor = qGray(color.rgb()); // Gray=0.3×R+0.59×G+0.11×B
                grayscaleImage.setPixelColor(x, y, QColor(currentColor, currentColor, currentColor));
                break;
            case retColor::Green:
                currentColor = qGreen(color.rgb());
                grayscaleImage.setPixelColor(x, y, QColor(0, currentColor, 0));
                break;
            case retColor::Red:
                currentColor = qRed(color.rgb());
                grayscaleImage.setPixelColor(x, y, QColor(currentColor, 0, 0));
                break;
            default:
                break;
            }
        }
    }
    loadedImage = grayscaleImage;

    // 将图像绘制到画布上
    QPainter painter(&canvas);
    painter.drawImage(QPoint(0, 0), loadedImage);

    update();  // 刷新显示
}

void DrawingWidget::applyMosaicFilter(int mosaicSize) {
    // 确保图像已加载并且马赛克大小有效
    if (loadedImage.isNull() || mosaicSize <= 0) return;

    QImage mosaicImage = canvas.copy(); // 复制当前画布
    int width = mosaicImage.width();
    int height = mosaicImage.height();

    // 遍历画布，为每个马赛克块应用平均颜色
    for (int y = 0; y < height; y += mosaicSize) {
        for (int x = 0; x < width; x += mosaicSize) {
            // 计算每个块的平均颜色
            int red = 0, green = 0, blue = 0, pixelCount = 0;

            // 遍历像素块的像素点
            for (int j = 0; j < mosaicSize && (y + j) < height; ++j) {
                for (int i = 0; i < mosaicSize && (x + i) < width; ++i) {
                    QColor pixelColor = mosaicImage.pixelColor(x + i, y + j);
                    red += pixelColor.red();
                    green += pixelColor.green();
                    blue += pixelColor.blue();
                    ++pixelCount;
                }
            }

            // 计算该块的平均颜色（马赛克）
            if (pixelCount > 0) {  // 确保不会发生除零错误
                red /= pixelCount;
                green /= pixelCount;
                blue /= pixelCount;
            }

            // 将该块的所有像素点设置为平均颜色
            for (int j = 0; j < mosaicSize && (y + j) < height; ++j) {
                for (int i = 0; i < mosaicSize && (x + i) < width; ++i) {
                    mosaicImage.setPixelColor(x + i, y + j, QColor(red, green, blue));
                }
            }
        }
    }
    // 更新加载的图像为马赛克处理后的图像
    loadedImage = mosaicImage;

    // 将图像绘制到画布上
    QPainter painter(&canvas);
    painter.drawImage(QPoint(0, 0), loadedImage);

    update();  // 刷新显示
}

void DrawingWidget::rotateImage(int angle) {
    if (canvas.isNull()) {
        return;  // 如果没有画布内容，则直接返回
    }

    // 使用QTransform来创建一个旋转变换
    QTransform transform;
    transform.translate(canvas.width() / 2, canvas.height() / 2);  // 将旋转中心移动到画布中心
    transform.rotate(angle);  // 旋转指定角度
    transform.translate(-canvas.width() / 2, -canvas.height() / 2);  // 再将旋转中心移回去

    // 创建一个新的画布用于存放旋转后的内容
    QImage rotatedCanvas = canvas.transformed(transform, Qt::SmoothTransformation);

    // 更新画布为旋转后的画布
    canvas = rotatedCanvas;
    for (EditableLabel* label : labels) {
        QPointF labelCenter = label->geometry().center();
        QPointF rotatedPoint = transform.map(labelCenter);
        label->move(rotatedPoint.toPoint() - label->rect().center());
    }
    // 清空窗口并绘制新的画布
    update();  // 刷新显示，调用 paintEvent 重新绘制
}

