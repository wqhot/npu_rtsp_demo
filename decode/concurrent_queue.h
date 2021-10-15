#ifndef COMMONLIB_CONCURRENT_QUEUE_H
#define COMMONLIB_CONCURRENT_QUEUE_H

#include <list>
#include <mutex>
#include <condition_variable>
#include <iostream>

using namespace std;
template <typename T>
class ConcurrentQueue {
public:
    ConcurrentQueue()
    {
        //			LogT << " construct" << endl;
    }
    ~ConcurrentQueue()
    {
        //			LogT << " disconstruct" << endl;
    }

    T front()
    {
        //uniqueLock(mtx);
        std::unique_lock<std::mutex> lock(mtx);
        T data = nullptr;
        while (!notified) {
            cv.wait(lock);
        }
        if (!cList.empty()) {
            data = cList.front();
            cList.pop_front();
        }
        return data;
    }

    T first()
    {
        //uniqueLock(mtx);
        std::unique_lock<std::mutex> lock(mtx);
        T data = nullptr;
        if (!cList.empty()) {
            data = cList.front();
            cList.pop_front();
        }
        return data;
    }

    void pushBack(T data)
    {
        std::unique_lock<std::mutex> lock(mtx);
        cList.push_back(data);
        notified = true;
        cv.notify_all();
    }
    void clear()
    {
        std::unique_lock<std::mutex> lock(mtx);
        notified = true;
        cList.clear();
        cv.notify_all();
    }

    void setWaitFlag(bool waitFlag)
    {
        std::unique_lock<std::mutex> lock(mtx);
        if (waitFlag) {
            notified = false;
        } else {
            notified = true;
        }
        cv.notify_all();
    }

    void notify()
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.notify_all();
    }

    int size(){
        std::unique_lock<std::mutex> lock(mtx);
        cList.size();
    }

private:
    bool notified = false;	// 通知信号
    std::mutex mtx;
    std::condition_variable cv;
    std::list<T> cList;
};

#endif