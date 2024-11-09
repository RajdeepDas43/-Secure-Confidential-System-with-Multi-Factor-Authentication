#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

class TaskQueue {
private:
    std::queue<std::function<void()>> tasks;
    std::mutex mtx;
    std::condition_variable cv;

public:
    void pushTask(const std::function<void()> &task) {
        std::unique_lock<std::mutex> lock(mtx);
        tasks.push(task);
        cv.notify_one();
    }

    std::function<void()> popTask() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]() { return !tasks.empty(); });
        std::function<void()> task = tasks.front();
        tasks.pop();
        return task;
    }
};

void worker(TaskQueue &taskQueue) {
    while (true) {
        std::function<void()> task = taskQueue.popTask();
        task();  // Execute the task
    }
}
