#include "MThreadManager.h"

MThreadManager::MThreadManager(size_t numThreads,QObject *parent):
    QObject(parent),
    running(false),
    numThreads(numThreads)
{

}

MThreadManager::~MThreadManager(){
     stop();
}

void MThreadManager::start(){
    stop();  // 确保之前的线程都已停止

    running = true;
    for (size_t i = 0; i < numThreads; ++i) {
        threads.emplace_back([this]() {
            std::function<void()> task;
            while (running) {
                {
                    std::unique_lock<std::mutex> lock(mtx);
                    cv.wait(lock, [this] { return !running || !tasks.empty(); });
                    if (!running && tasks.empty()) return;
                    task = std::move(tasks.front());
                    tasks.pop();
                }
                task();
            }
        });
    }
}

void MThreadManager::stop(){
    {
        std::unique_lock<std::mutex> lock(mtx);
        running = false;
    }
    cv.notify_all();
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    threads.clear();
}

bool MThreadManager::isRunning() const{
    return running;
}

void MThreadManager::addTask(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(mtx);
        tasks.push(std::move(task));
    }
    cv.notify_one();
}

