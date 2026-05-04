#include "todoitemwidget.h"

TodoItemWidget::TodoItemWidget(int index, const QString& name, const QString& note, int duration, bool isCompleted, QWidget *parent)
    : QWidget(parent), index(index)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(10, 8, 10, 8);
    layout->setSpacing(10);

    // 待办名称 + 时长
    QString nameText = QString("%1 (%2分钟)").arg(name).arg(duration);
    nameLabel = new QLabel(nameText);
    nameLabel->setStyleSheet("font-size: 14px;");

    // 备注
    noteLabel = new QLabel(note);
    noteLabel->setStyleSheet("font-size: 12px; color: #999;");
    noteLabel->setWordWrap(true); // 自动换行

    // 左侧垂直布局：名称 + 备注
    QVBoxLayout* textLayout = new QVBoxLayout();
    textLayout->setSpacing(2);
    textLayout->addWidget(nameLabel);
    if (!note.isEmpty()) {
        textLayout->addWidget(noteLabel);
    }

    layout->addLayout(textLayout, 1);

    // 开始按钮
    startButton = new QPushButton("开始");
    startButton->setFixedSize(60, 28);
    startButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; border-radius: 14px; font-size: 12px; }");
    layout->addWidget(startButton);
    // 删除按钮
    deleteButton = new QPushButton("删除");
    deleteButton->setFixedSize(60, 28);
    deleteButton->setStyleSheet("QPushButton { background-color: #f44336; color: white; border-radius: 14px; font-size: 12px; }");
    layout->addWidget(deleteButton);

    // 设置完成状态
    setCompleted(isCompleted);

    // 连接
    connect(startButton, &QPushButton::clicked, this, &TodoItemWidget::onStartClicked);
    connect(deleteButton, &QPushButton::clicked, this, &TodoItemWidget::onDeleteClicked);
}

void TodoItemWidget::setCompleted(bool completed)
{
    QFont font = nameLabel->font();
    font.setStrikeOut(completed);
    nameLabel->setFont(font);

    if (completed) {
        nameLabel->setStyleSheet("font-size: 14px; color: #999;");
        noteLabel->setStyleSheet("font-size: 12px; color: #ccc;");
        startButton->setEnabled(false);
        startButton->setStyleSheet("QPushButton { background-color: #ccc; color: white; border-radius: 14px; font-size: 12px; }");
    }
}

void TodoItemWidget::onStartClicked()
{
    emit startClicked(index);
}

void TodoItemWidget::onDeleteClicked()
{
    emit deleteClicked(index);
}
