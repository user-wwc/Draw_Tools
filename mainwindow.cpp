#include "mainwindow.h"
#include "drawingwidget.h"
#include "editablelabel.h"
#include <QMenuBar>
#include <QFileDialog>
#include <QLabel>
#include <QColorDialog>
#include <QActionGroup>
#include<QDebug>
#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), drawingWidget(new DrawingWidget(this)) {
 setWindowIcon(QIcon(":/drawicon.ico"));
    setCentralWidget(drawingWidget);  // 设置绘图区域为中心窗口

    setupMenu();  // 初始化菜单
}

void MainWindow::setupMenu() {
    QMenuBar *menuBar = new QMenuBar(this);

    QMenu *fileMenu = new QMenu("文件", this);
    QAction *openAction = new QAction("打开", this);
    QAction *saveAction = new QAction("保存", this);
    QAction *newLabelAction =new QAction("新建标签", this);



    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(newLabelAction);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    connect(newLabelAction, &QAction::triggered, this, &MainWindow::createNewLabel);

    QMenu *editMenu = new QMenu("编辑", this);
    QAction *clearAction = new QAction("清空", this);
    editMenu->addAction(clearAction);
    connect(clearAction, &QAction::triggered, this,&MainWindow::clearCanvas);

    QMenu *penMenu = new QMenu("画笔", this);
    QAction *colorAction = new QAction("颜色", this);
    penMenu->addAction(colorAction);
    connect(colorAction, &QAction::triggered, this, &MainWindow::changePenColor);
//笔画
    QMenu *penSizeMenu = new QMenu("画笔大小", this);
    QActionGroup *penSizeGroup = new QActionGroup(this);
    QAction *smallPen = new QAction("小", penSizeGroup);
    QAction *mediumPen = new QAction("中", penSizeGroup);
    QAction *largePen = new QAction("大", penSizeGroup);
    penSizeMenu->addActions(penSizeGroup->actions());
    connect(smallPen, &QAction::triggered, [=]() {changePenWidth(3); });
    connect(mediumPen, &QAction::triggered, [=]() {changePenWidth(6); });
    connect(largePen, &QAction::triggered, [=]() { changePenWidth(10); });

    //图形
    QMenu* shapeMenu=new QMenu("绘制图形",this);
    QAction* shapeNone=new QAction("画笔",this);
    QAction* shapeLine=new QAction("直线",this);
    QAction* shapeRect=new QAction("矩形",this);
    QAction* shapeEllipse=new QAction("椭圆",this);
    shapeMenu->addAction(shapeNone);
    shapeMenu->addAction(shapeLine);
    shapeMenu->addAction(shapeRect);
    shapeMenu->addAction(shapeEllipse);
    connect(shapeNone, &QAction::triggered, this, [=]() { drawingWidget->setShape(DrawingWidget::Shape::None); });
    connect(shapeLine, &QAction::triggered, this, [=]() { drawingWidget->setShape(DrawingWidget::Shape::Line); });
    connect(shapeRect, &QAction::triggered, this, [=]() { drawingWidget->setShape(DrawingWidget::Shape::Rectangle); });
    connect(shapeEllipse, &QAction::triggered, this, [=]() { drawingWidget->setShape(DrawingWidget::Shape::Ellipse); });

    QMenu* ToolEraser=new QMenu("橡皮擦",this);
    QAction *smallEraser = new QAction("小", this);
    QAction *mediumEraser = new QAction("中", this);
    QAction *largeEraser = new QAction("大", this);
    ToolEraser->addAction(smallEraser);
    ToolEraser->addAction(mediumEraser);
    ToolEraser->addAction(largeEraser);
    connect(smallEraser, &QAction::triggered, [=]() { drawingWidget->setShape(DrawingWidget::Shape::Eraser),changeEraserWidth(10); });
    connect(mediumEraser, &QAction::triggered, [=]() { drawingWidget->setShape(DrawingWidget::Shape::Eraser),changeEraserWidth(30); });
    connect(largeEraser, &QAction::triggered, [=]() { drawingWidget->setShape(DrawingWidget::Shape::Eraser),changeEraserWidth(70); });




    QMenu* filter=new QMenu("滤镜",this);
    QAction* filGrey=new QAction("灰色",this);
    QAction* filGreen=new QAction("绿色",this);
    QAction* filRed=new QAction("红色",this);
    QAction* bigmosaic=new QAction("马赛克",this);
    filter->addAction(bigmosaic);
    filter->addAction(filGrey);
    filter->addAction(filGreen);
    filter->addAction(filRed);
    connect(filGrey, &QAction::triggered, this, [=]() { drawingWidget->applyGrayscaleFilter(DrawingWidget::retColor::Grey); });
    connect(filGreen, &QAction::triggered, this, [=]() { drawingWidget->applyGrayscaleFilter(DrawingWidget::retColor::Green); });
    connect(filRed, &QAction::triggered, this, [=]() { drawingWidget->applyGrayscaleFilter(DrawingWidget::retColor::Red); });
    connect(bigmosaic, &QAction::triggered, this, [=]() { drawingWidget->DrawingWidget::applyMosaicFilter(10); });

    QMenu* rotateMenu = new QMenu("旋转", this);
    QAction* rotate90Action = new QAction("顺时针90度", this);
    QAction* rotate180Action = new QAction("180度", this);
    QAction* rotate270Action = new QAction("逆时针90度", this);
    rotateMenu->addAction(rotate90Action);
    rotateMenu->addAction(rotate180Action);
    rotateMenu->addAction(rotate270Action);
    connect(rotate90Action, &QAction::triggered, this, [=]() { drawingWidget->rotateImage(90); });
    connect(rotate180Action, &QAction::triggered, this, [=]() { drawingWidget->rotateImage(180); });
    connect(rotate270Action, &QAction::triggered, this, [=]() { drawingWidget->rotateImage(270); });




    menuBar->addMenu(fileMenu);
    menuBar->addMenu(editMenu);
    menuBar->addMenu(penMenu);
    menuBar->addMenu(penSizeMenu);
    menuBar->addMenu(shapeMenu);
    menuBar->addMenu(ToolEraser);
    menuBar->addMenu(filter);
    menuBar->addMenu(rotateMenu);
    setMenuBar(menuBar);
}

void MainWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开文件"), "", tr("图像文件 (*.jpg *.jpeg *png);;所有文件 (*)"));
    if (!fileName.isEmpty()) {
        DrawingWidget* drawingWidget = qobject_cast<DrawingWidget*>(centralWidget());
        if (drawingWidget) {
            drawingWidget->loadImage(fileName);
        }
    }
}


void MainWindow::saveFile() {
    QString fileName = QFileDialog::getSaveFileName(this, "保存文件", "", "图像文件 (*.jpg *.jpeg)");
    if (!fileName.isEmpty()) {
        drawingWidget->saveImage(fileName);  // 将操作传递给绘图类
    }
}

void MainWindow::clearCanvas() {
    drawingWidget->clear();  // 清空画布
    for (EditableLabel* label : labels) {
        label->close();  // 关闭并删除标签
    }
    labels.clear();
}


void MainWindow::changePenColor() {
    QColor color = QColorDialog::getColor(Qt::black, this, "选择画笔颜色");
    if (color.isValid()) {
        drawingWidget->setPenColor(color);  // 将画笔颜色设置传递给绘图类
    }
}

void MainWindow::changePenWidth(int width) {
    drawingWidget->setPenWidth(width);  // 将画笔宽度传递给绘图类
}
void MainWindow::changeEraserWidth(int width) {
    drawingWidget->setEraserWidth(width);  // 将画笔宽度传递给绘图类
}
void MainWindow::createNewLabel() {
    qDebug() << "Creating a new label...";
    EditableLabel *newLabel = new EditableLabel(drawingWidget); // 创建新的 EditableLabel
    newLabel->setAttribute(Qt::WA_DeleteOnClose); // 确保关闭时删除
    newLabel->move(10, 10 + (this->findChildren<EditableLabel*>().size() * 40)); // 设置新标签的位置
    drawingWidget->addLabel(newLabel);
}
