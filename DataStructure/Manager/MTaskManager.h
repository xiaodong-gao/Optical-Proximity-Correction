#ifndef MTASKMANAGER_H
#define MTASKMANAGER_H

#include <QObject>
#include <iostream>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>

class MTaskManager : public QObject{
    Q_OBJECT
public:
    explicit MTaskManager(QObject *parent = nullptr);
    ~MTaskManager();

    void addTask(std::function<void()> task);
    std::function<void()> getNextTask();
private:
    std::queue<std::function<void()>> taskQueue;
    std::mutex mtx;
    std::condition_variable cv;
};

#endif // MTASKMANAGER_H
