#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "editablelabel.h"
#include <QList>

class DrawingWidget;  // 前向声明绘图类

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void openFile();  // 打开文件
    void saveFile();  // 保存文件
    void clearCanvas();  // 清空画布
    void changePenColor();  // 改变画笔颜色
    void changePenWidth(int width);  // 改变画笔宽度
    void changeEraserWidth(int width);
     void createNewLabel();

private:
    DrawingWidget *drawingWidget;  // 绘图区域
    void setupMenu();  // 菜单初始化
    QList<EditableLabel*> labels;

};

#endif // MAINWINDOW_H
