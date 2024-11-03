#include "editablelabel.h"
#include <QApplication>
#include <QLabel>
#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif
EditableLabel::EditableLabel(QWidget *parent) : QWidget(parent) {
    label = new QLabel("双击编辑我", this);
    lineEdit = new QLineEdit(this);
    lineEdit->setVisible(false); // 初始状态下隐藏编辑框

    label->setFixedSize(150, 30); // 设置标签的大小
    lineEdit->setFixedSize(200, 30); // 设置输入框的大小
    label->move(10, 10); // 设置标签的位置
    lineEdit->move(label->pos()); // 将输入框与标签位置一致

    // 连接信号
    connect(lineEdit, &QLineEdit::editingFinished, [this]() {
        label->setText(lineEdit->text());
        lineEdit->setVisible(false); // 隐藏输入框
        label->setVisible(true); // 显示标签
    });

    // 安装事件过滤器
    label->installEventFilter(this);
}

// 处理双击事件
void EditableLabel::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        editLabel(); // 进入编辑模式
    }
}

// 处理拖动事件
bool EditableLabel::eventFilter(QObject *obj, QEvent *event) {
    if (obj == label) {
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                dragStartPosition = mouseEvent->pos();
                return true; // 表示事件已处理
            }
        } else if (event->type() == QEvent::MouseMove) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->buttons() & Qt::LeftButton) {
                // 计算移动距离
                if ((mouseEvent->pos() - dragStartPosition).manhattanLength() > QApplication::startDragDistance()) {
                    // 移动标签位置
                    move(pos() + (mouseEvent->pos() - dragStartPosition));
                    lineEdit->move(label->pos()); // 移动输入框与标签一致
                }
                return true; // 表示事件已处理
            }
        } else if (event->type() == QEvent::MouseButtonRelease) {
            return true; // 表示事件已处理
        }
    }
    return QWidget::eventFilter(obj, event); // 继续处理其他事件
}

// 编辑标签文本的实现
void EditableLabel::editLabel() {
    lineEdit->setText(label->text());
    lineEdit->setVisible(true); // 显示输入框
    lineEdit->setFocus(); // 使文本框获得焦点
    label->setVisible(false); // 隐藏标签

    // 确保输入框位置与标签相同
    lineEdit->move(label->pos()); // 设置lineEdit的位置与标签一致
}
QString EditableLabel::text() const {
    return label->text(); // 从 QLabel 中获取文本
}

