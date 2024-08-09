#ifndef MTHREADMANAGER_H
#define MTHREADMANAGER_H

#include <QObject>
#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <atomic>
#include <queue>
#include <condition_variable>

class MThreadManager : public QObject{
    Q_OBJECT
public:
    explicit MThreadManager(size_t numThreads,QObject *parent = nullptr);
    ~MThreadManager();
    MThreadManager(const MThreadManager &rhs) = delete;
    MThreadManager& operator=(const MThreadManager &rhs) = delete;

    void addTask(std::function<void()> task);
    void start();
    void stop();
    bool isRunning() const;
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex mtx;
    std::condition_variable cv;
    std::atomic<bool> running;
    size_t numThreads;

};

#endif // MTHREADMANAGER_H
