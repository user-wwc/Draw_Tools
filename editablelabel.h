#ifndef EDITABLELABEL_H
#define EDITABLELABEL_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QMouseEvent>

class EditableLabel : public QWidget {
    Q_OBJECT

public:
    explicit EditableLabel(QWidget *parent = nullptr);
    QString text() const;
    void clear();

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override; // 处理双击事件
    bool eventFilter(QObject *obj, QEvent *event) override;   // 添加eventFilter的声明

private slots:
    void editLabel(); // 编辑标签文本

private:
    QLabel *label;       // 用于显示文本的标签
    QLineEdit *lineEdit; // 用于编辑文本的输入框
    QPoint dragStartPosition; // 拖动起始位置
};

#endif
