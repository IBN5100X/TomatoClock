#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QFrame>
#include <QChart>
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QChartView>
#include <QMessageBox>

#include "focuswindow.h"
#include "addtododialog.h"
#include "todoitemwidget.h"

struct TodoItem {
    QString name;
    QString note;
    int duration;
    bool isCompleted;

    TodoItem() : duration(25), isCompleted(false) {}
    TodoItem(const QString& n, const QString& nt, int d)
        : name(n), note(nt), duration(d), isCompleted(false) {}
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void startQuickTimer();
    void addTodo();
    void onTodoStartClicked(int index);  // 待办开始按钮点击
    void onTodoDeleteClicked(int index); // 待办删除按钮点击
    void onFocusFinished(int focusedSeconds);
    void onFocusStopped(int focusedSeconds);

private:
    QListWidget* todoList;
    QPushButton* addTodoButton;
    QSpinBox* quickTimeSpin;
    QPushButton* quickStartButton;
    QLabel* totalFocusLabel;
    QChartView* chartView;

    QList<TodoItem> todos;
    int totalFocusMinutes;
    int currentTodoIndex;

    FocusWindow* focusWindow;

    void updateTotalFocusDisplay();
    void updateTodoList();
    void updateChart();
};

#endif // MAINWINDOW_H
