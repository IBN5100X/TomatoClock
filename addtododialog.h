#ifndef ADDTODODIALOG_H
#define ADDTODODIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

class AddTodoDialog : public QDialog
{
    Q_OBJECT

public:
    AddTodoDialog(QWidget* parent = nullptr);

    QString getTodoName() ;
    int getDuration() ;
    QString getNote() ;
private:
    QLineEdit* nameEdit;
    QSpinBox* durationSpin;
    QTextEdit* noteEdit;
    QPushButton* okButton;
    QPushButton* cancelButton;

};

#endif // ADDTODODIALOG_H
