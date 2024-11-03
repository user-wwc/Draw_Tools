#ifndef MYLINE_H
#define MYLINE_H

#include <QPoint>
#include <QColor>

class myLine {
public:
    QPoint startPnt;  // 线条起点
    QPoint endPnt;    // 线条终点
    QColor penColor;  // 线条颜色
    int penWidth;     // 线条宽度
};

#endif // MYLINE_H
