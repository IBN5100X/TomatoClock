#include "addtododialog.h"

AddTodoDialog::AddTodoDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("添加待办事项");
    setFixedSize(350, 250);

    QVBoxLayout* colLayout = new QVBoxLayout(this);

    QHBoxLayout* nameLayout = new QHBoxLayout();
    nameLayout->addWidget(new QLabel("待办名称:"));
    nameEdit = new QLineEdit();
    nameEdit->setPlaceholderText("请输入待办事项名称");
    nameLayout->addWidget(nameEdit);
    colLayout->addLayout(nameLayout);

    QHBoxLayout* durationLayout = new QHBoxLayout();
    durationLayout->addWidget(new QLabel("专注时长(分钟):"));
    durationSpin = new QSpinBox();
    durationSpin->setRange(1, 120);
    durationSpin->setValue(25);
    durationLayout->addWidget(durationSpin);
    colLayout->addLayout(durationLayout);

    colLayout->addWidget(new QLabel("备注:"));
    noteEdit = new QTextEdit();
    noteEdit->setPlaceholderText("在此可以添加一些备注信息");
    colLayout->addWidget(noteEdit);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    okButton = new QPushButton("确定");
    cancelButton = new QPushButton("取消");
    buttonLayout->addStretch(); //弹簧填充
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    colLayout->addLayout(buttonLayout);

    //连接
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

//get函数部分
QString AddTodoDialog::getTodoName(){
    return nameEdit->text();
}

int AddTodoDialog::getDuration(){
    return durationSpin->value();
}

QString AddTodoDialog::getNote(){
    return noteEdit->toPlainText();
}
