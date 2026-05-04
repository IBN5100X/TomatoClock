#ifndef FOCUSWINDOW_H
#define FOCUSWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QPixmap>
#include <QFile>
#include <QUrl>
#include <QMediaPlayer>
#include <QAudioOutput>

class FocusWindow : public QWidget
{
    Q_OBJECT

public:
    FocusWindow(QWidget* parent = nullptr);

    void setTotalSeconds(int seconds);
    void startCountdown();

public:
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void countdownFinished(int focusedSeconds);
    void countdownStopped(int focusedSeconds);

public slots:
    void updateCountdown();
    void switchPause();
    void endEarly();

public:
    QLabel* timeShow;
    QPushButton* pauseButton;
    QPushButton* endButton;
    QTimer* timer;
    QMediaPlayer* bgmPlayer;
    QAudioOutput* audioOutput;

    int totalSeconds;
    int remainingSeconds;
    bool isPaused;

    void updateTimeDisplay();
};

#endif // FOCUSWINDOW_H
