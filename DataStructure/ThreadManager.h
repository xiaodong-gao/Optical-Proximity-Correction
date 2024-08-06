#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <QThread>
#include <atomic>
#include <QDebug>
#include "DataStructure/MSystemInfo.h"

class ThreadManager : public QThread{
    Q_OBJECT
public:
    explicit ThreadManager(QObject *parent = nullptr);
    ~ThreadManager();

    ThreadManager(const ThreadManager & rhs) = delete;
    ThreadManager& operator=(const ThreadManager & rhs) = delete;

    void start();
    void stop();
    bool isRunning() const ;
    WorkDoneResult get_work_done_result() const;

signals:
    void send_work_done_to_image_process_widget(const WorkDoneResult &work_done_result);
    void send_work_status_to_optial_tool_dlg(int widget_index, bool status);
protected:
    void run() override;
private:
    std::atomic<bool> m_running;
    std::atomic<bool> m_stop;
    WorkDoneResult work_done_result;

};

#endif // THREADMANAGER_H
