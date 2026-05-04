#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), totalFocusMinutes(0), currentTodoIndex(-1)
{
    setWindowTitle("番茄钟");
    setFixedSize(1200, 700);
    setStyleSheet("QMainWindow { background-color: #f0f0f0; }");

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setSpacing(30);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // ========== 左侧：待办事项区域 ==========
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->setSpacing(15);

    // 待办标题 + 添加按钮
    QHBoxLayout *todoHeaderLayout = new QHBoxLayout();
    QLabel *todoTitle = new QLabel("待办事项");
    QFont titleFont = todoTitle->font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    todoTitle->setFont(titleFont);

    addTodoButton = new QPushButton("+ 添加待办");
    addTodoButton->setFixedSize(100, 35);
    addTodoButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; border-radius: 17px; font-size: 14px; }");

    todoHeaderLayout->addWidget(todoTitle);
    todoHeaderLayout->addStretch();
    todoHeaderLayout->addWidget(addTodoButton);
    leftLayout->addLayout(todoHeaderLayout);

    // 添加待办列表
    todoList = new QListWidget();
    todoList->setStyleSheet(
        "QListWidget {"
        "   background-color: white;"
        "   border: none;"
        "   border-radius: 15px;"
        "   padding: 5px;"
        "}"
        "QListWidget::item {"
        "   border-bottom: 1px solid #eee;"
        "}"
        "QListWidget::item:last-child {"
        "   border-bottom: none;"
        "}"
        );
    todoList->setSpacing(0);
    leftLayout->addWidget(todoList, 1);

    // 添加快速计时区域
    QFrame *quickFrame = new QFrame();
    quickFrame->setStyleSheet(
        "QFrame {"
        "   background-color: white;"
        "   border-radius: 15px;"
        "   padding: 20px;"
        "}"
        );

    QVBoxLayout *quickLayout = new QVBoxLayout(quickFrame);
    QLabel *quickLabel = new QLabel("快速开始专注");
    quickLabel->setFont(QFont("", 14, QFont::Bold));

    QHBoxLayout *quickTimeLayout = new QHBoxLayout();
    quickTimeSpin = new QSpinBox();
    quickTimeSpin->setRange(1, 120);
    quickTimeSpin->setValue(25);
    quickTimeSpin->setSuffix(" 分钟");
    quickTimeSpin->setFixedHeight(35);
    quickTimeSpin->setStyleSheet("font-size: 14px;");

    quickStartButton = new QPushButton("开始专注");
    quickStartButton->setFixedSize(100, 35);
    quickStartButton->setStyleSheet("QPushButton { background-color: #2196F3; color: white; border-radius: 17px; font-size: 14px; }");

    quickTimeLayout->addWidget(quickTimeSpin);
    quickTimeLayout->addWidget(quickStartButton);

    quickLayout->addWidget(quickLabel);
    quickLayout->addLayout(quickTimeLayout);

    leftLayout->addWidget(quickFrame);

    mainLayout->addLayout(leftLayout, 1); // 左侧占1份宽度

    // ========== 右侧：今日专注统计区域 ==========
    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(15);

    // 今日专注标题和总时长
    QHBoxLayout* titleLayout = new QHBoxLayout();
    QLabel* focusTitle = new QLabel("今日专注");
    focusTitle->setFont(titleFont);

    totalFocusLabel = new QLabel("今日专注: 0h0m");
    totalFocusLabel->setFont(QFont("", 14));

    titleLayout->addWidget(focusTitle);
    titleLayout->addStretch();
    titleLayout->addWidget(totalFocusLabel);

    rightLayout->addLayout(titleLayout);

    // 图表（圆角白色背景）
    QFrame *chartFrame = new QFrame();
    chartFrame->setStyleSheet(
        "QFrame {"
        "   background-color: white;"
        "   border-radius: 15px;"
        "   padding: 10px;"
        "}"
        );

    QVBoxLayout *chartLayout = new QVBoxLayout(chartFrame);

    QChart* chart = new QChart();
    chart->setTitle("今日专注时长分布");
    chart->setTitleFont(QFont("", 12));
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(false);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setStyleSheet("border: none;");

    chartLayout->addWidget(chartView);
    rightLayout->addWidget(chartFrame, 1);

    mainLayout->addLayout(rightLayout, 1); // 右侧占1份宽度

    //连接信号和槽
    connect(addTodoButton, &QPushButton::clicked, this, &MainWindow::addTodo);
    connect(quickStartButton, &QPushButton::clicked, this, &MainWindow::startQuickTimer);

    focusWindow = new FocusWindow();
    connect(focusWindow, &FocusWindow::countdownFinished, this, &MainWindow::onFocusFinished);
    connect(focusWindow, &FocusWindow::countdownStopped, this, &MainWindow::onFocusStopped);

    // 初始化图表
    updateChart();
}

MainWindow::~MainWindow()
{
    delete focusWindow;
}

void MainWindow::startQuickTimer()
{
    int minutes = quickTimeSpin->value();
    currentTodoIndex = -1;

    focusWindow->setTotalSeconds(minutes * 60);
    focusWindow->show();
    focusWindow->startCountdown();
}

void MainWindow::addTodo()
{
    AddTodoDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString name = dialog.getTodoName();
        if (name.isEmpty()) {
            QMessageBox::warning(this, "输入错误", "待办名称不能为空！");
            return;
        }

        todos.append(TodoItem(name, dialog.getNote(), dialog.getDuration()));
        updateTodoList();
    }
}

void MainWindow::onTodoStartClicked(int index)
{
    if (todos[index].isCompleted) {
        QMessageBox::information(this, "提示", "这个待办事项已经完成了");
        return;
    }

    currentTodoIndex = index;
    focusWindow->setTotalSeconds(todos[index].duration * 60);
    focusWindow->show();
    focusWindow->startCountdown();
}

void MainWindow::onTodoDeleteClicked(int index)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除", "确定要删除这个待办事项吗？", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        todos.removeAt(index);
        updateTodoList();
    }
}

void MainWindow::onFocusFinished(int focusedSeconds)
{
    int minutes = focusedSeconds / 60;
    totalFocusMinutes += minutes;

    updateTotalFocusDisplay();
    updateChart();

    if (currentTodoIndex != -1 && currentTodoIndex < todos.size()) {
        todos[currentTodoIndex].isCompleted = true;
        updateTodoList();
    }
}

void MainWindow::onFocusStopped(int focusedSeconds)
{
    int minutes = focusedSeconds / 60;
    if (minutes > 0) {
        totalFocusMinutes += minutes;
        updateTotalFocusDisplay();
        updateChart();
    }
}

void MainWindow::updateTotalFocusDisplay()
{
    int hours = totalFocusMinutes / 60;
    int minutes = totalFocusMinutes % 60;

    totalFocusLabel->setText(QString("今日专注: %1h%2m").arg(hours).arg(minutes));
}

void MainWindow::updateTodoList()
{
    todoList->clear();

    for (int i = 0; i < todos.size(); i++) {
        const TodoItem& item = todos[i];

        TodoItemWidget *itemWidget = new TodoItemWidget(
            i, item.name, item.note, item.duration, item.isCompleted
            );

        // 连接
        connect(itemWidget, &TodoItemWidget::startClicked, this, &MainWindow::onTodoStartClicked);
        connect(itemWidget, &TodoItemWidget::deleteClicked, this, &MainWindow::onTodoDeleteClicked);

        QListWidgetItem *listItem = new QListWidgetItem(todoList);
        listItem->setSizeHint(itemWidget->sizeHint());
        todoList->setItemWidget(listItem, itemWidget);
    }
}

void MainWindow::updateChart()
{
    QChart *chart = chartView->chart();
    chart->removeAllSeries();

    foreach (QAbstractAxis *axis, chart->axes()) {
        chart->removeAxis(axis);
    }

    // 创建柱状图数据
    QBarSet *set = new QBarSet("专注时长(分钟)");
    set->setColor(QColor(0x4CAF50)); // 绿色柱子

    // 显示最近10次专注的时长
    for (int i = 0; i < 10; i++) {
        if (i == 0) {
            *set << totalFocusMinutes;
        } else {
            *set << 0;
        }
    }

    QBarSeries *series = new QBarSeries();
    series->append(set);
    series->setBarWidth(0.5);
    chart->addSeries(series);

    // 设置坐标轴
    QStringList categories;
    categories << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10";

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setLabelsFont(QFont("", 10));
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, qMax(60, totalFocusMinutes + 10));
    axisY->setLabelsFont(QFont("", 10));
    axisY->setTitleText("分钟");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
}
