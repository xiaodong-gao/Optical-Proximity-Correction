#include "MTaskManager.h"

MTaskManager::MTaskManager(QObject *parent) :
    QObject(parent)
{

}

MTaskManager::~MTaskManager(){

}

// 添加新任务到任务队列
void MTaskManager::addTask(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(mtx);
        taskQueue.push(std::move(task));
    }
    cv.notify_one();
}

// 获取下一个任务
std::function<void()> MTaskManager::getNextTask() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this] { return !taskQueue.empty(); });
    auto task = std::move(taskQueue.front());
    taskQueue.pop();
    return task;
}
