#ifndef TODOITEMWIDGET_H
#define TODOITEMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>

class TodoItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TodoItemWidget(int index, const QString& name, const QString& note, int duration, bool isCompleted, QWidget *parent = nullptr);

    // 设置待办为完成状态
    void setCompleted(bool completed);

signals:
    void startClicked(int index);
    void deleteClicked(int index);

private slots:
    void onStartClicked();
    void onDeleteClicked();

private:
    int index;
    QLabel *nameLabel;
    QLabel *noteLabel;
    QPushButton *startButton;
    QPushButton *deleteButton;
};

#endif // TODOITEMWIDGET_H
