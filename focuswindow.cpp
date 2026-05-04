#include "focuswindow.h"
#include <QFont>
#include <QPalette>
#include <QMessageBox>
#include <QScreen>
#include <QApplication>

FocusWindow::FocusWindow(QWidget *parent)
    : QWidget(parent), totalSeconds(0), remainingSeconds(0), isPaused(false)
{
    // 设置全屏窗口
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setWindowState(Qt::WindowFullScreen);

    // 强制设置为屏幕大小
    QScreen* screen = QApplication::primaryScreen();
    setGeometry(screen->geometry());

    // 专注界面背景图设置
    QPixmap bgPixmap("mountain.jpg");
    if (!bgPixmap.isNull()) {
        // 图片存在
        bgPixmap = bgPixmap.scaled(this->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        QPalette palette;
        palette.setBrush(QPalette::Window, QBrush(bgPixmap));
        this->setPalette(palette);
    } else {
        // 图片不存在用黑色
        QPalette palette = this->palette();
        palette.setColor(QPalette::Window, Qt::black);
        setPalette(palette);
    }
    setAutoFillBackground(true);

    QVBoxLayout *colLayout = new QVBoxLayout(this);
    colLayout->setContentsMargins(0, 0, 0, 0);
    colLayout->setSpacing(0);

    // 时钟倒计时部分绘制
    timeShow = new QLabel("00:00");
    timeShow->setAlignment(Qt::AlignCenter);//居中对齐

    QFont timeFont = timeShow->font();
    timeFont.setPointSize(300);
    timeFont.setBold(true);

    timeShow->setFont(timeFont);
    timeShow->setStyleSheet("color: white;");

    colLayout->addWidget(timeShow);

    // 按钮区域绘制
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(50);
    buttonLayout->setContentsMargins(0, 0, 0, 100);
    pauseButton = new QPushButton("暂停");
    pauseButton->setFixedSize(200, 60);
    pauseButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-size: 22px; border-radius: 30px; }");

    endButton = new QPushButton("提前结束");
    endButton->setFixedSize(200, 60);
    endButton->setStyleSheet("QPushButton { background-color: #f44336; color: white; font-size: 22px; border-radius: 30px; }");

    buttonLayout->addStretch();
    buttonLayout->addWidget(pauseButton);
    buttonLayout->addWidget(endButton);
    buttonLayout->addStretch();

    colLayout->addLayout(buttonLayout);

    // 定时器
    timer = new QTimer(this);
    timer->setInterval(1000);

    connect(timer, &QTimer::timeout, this, &FocusWindow::updateCountdown);
    connect(pauseButton, &QPushButton::clicked, this, &FocusWindow::switchPause);
    connect(endButton, &QPushButton::clicked, this, &FocusWindow::endEarly);
}

void FocusWindow::setTotalSeconds(int seconds)
{
    totalSeconds = seconds;
    remainingSeconds = seconds;
    updateTimeDisplay();
}

void FocusWindow::startCountdown()
{
    isPaused = false;
    pauseButton->setText("暂停");
    timer->start();
    //播放音乐
    if (QFile::exists("chuanyueshikong.mp3")) {
        bgmPlayer = new QMediaPlayer(this);
        audioOutput = new QAudioOutput(this);
        bgmPlayer->setAudioOutput(audioOutput);
        bgmPlayer->setSource(QUrl::fromLocalFile("chuanyueshikong.mp3"));
        bgmPlayer->setLoops(-1); // 无限循环
        audioOutput->setVolume(0.1); //音量设置
        bgmPlayer->play();
    }
}

void FocusWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "确认退出",  "确定要退出专注模式吗？\n您当前已专注的时间将被记录。",  QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            endEarly();
        }
    }
}

void FocusWindow::updateCountdown()
{
    if (remainingSeconds > 0) {
        remainingSeconds--;
        updateTimeDisplay();
    } else {
        timer->stop();
        QMessageBox::information(this, "专注完成", "恭喜你完成了本次专注！");
        emit countdownFinished(totalSeconds);
        if (bgmPlayer){
            bgmPlayer->stop();
            delete bgmPlayer;
        }
        close();
    }
}

void FocusWindow::switchPause()
{
    if (isPaused) {
        timer->start();
        pauseButton->setText("暂停");
        isPaused = false;
        if (bgmPlayer) bgmPlayer->play();
    } else {
        timer->stop();
        pauseButton->setText("继续");
        isPaused = true;
        if(bgmPlayer) bgmPlayer->pause();
    }
}

void FocusWindow::endEarly()
{
    timer->stop();
    int focusedSeconds;
    focusedSeconds = totalSeconds - remainingSeconds;

    emit countdownStopped(focusedSeconds);
    if (bgmPlayer){
        bgmPlayer->stop();
        delete bgmPlayer;
    }
    close();
}

void FocusWindow::updateTimeDisplay()
{
    int minutes;
    int seconds;

    minutes = remainingSeconds / 60;

    seconds = remainingSeconds % 60;

    QString min;

    if (minutes < 10) {
        min = "0" + QString::number(minutes);
    } else {
        min = QString::number(minutes);
    }

    QString sec;

    if (seconds < 10) {
        sec = "0" + QString::number(seconds);
    } else {
        sec = QString::number(seconds);
    }

    QString timeStr;
    timeStr = min + ":" + sec;

    timeShow->setText(timeStr);
}
