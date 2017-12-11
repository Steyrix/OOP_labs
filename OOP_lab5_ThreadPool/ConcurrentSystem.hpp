#pragma once
#include "Factorizator.hpp"
#include "ThreadPool.hpp"
#include <fstream>

//================  CONCURRENT QUEUE CLASS =================
//A simple concurrent queue, which has a fixed size (20 items), considering we will
//have enormously big number of data in file to process, so we do not have enough
//memory to read them all at once
template <class T>
class ConcurrentQueue
{
public:
    ConcurrentQueue()= default;
    ~ConcurrentQueue()= default;
    void push(T item)
    {
        std::unique_lock<std::mutex> locker(queueMtx);
        cv.wait(locker, [this]()->bool{return dataQueue.size() < maxSize;});
        dataQueue.push(item);
        std::cout << "Pushed " << item << "\n";
        cv.notify_all();
    }
    T popFront()
    {
        std::unique_lock<std::mutex> locker(queueMtx);
        cv.wait(locker, [this]()->bool{return !dataQueue.empty();});
        std::cout << "Popping item " << dataQueue.front() << "\n";
        T out = dataQueue.front();
        dataQueue.pop();
        cv.notify_all();
        return out;
    }
    bool isEmpty()
    {
        return dataQueue.empty();
    }
private:
    std::queue<T> dataQueue;
    std::mutex queueMtx;
    std::condition_variable cv;
    static const uint8_t maxSize;
};

template <class T>
const uint8_t ConcurrentQueue<T>::maxSize = 20;

//=======================READER CLASS===============================
//Reads from file and pushes values to queue
class Reader
{
public:
    Reader(const std::string &input) : inputFile(input){};
    ~Reader(){};

    template <typename T>
    void runLoop(ConcurrentQueue<T> &numbersQueue)
    {
        std::ifstream fin(inputFile);
        if(!fin.good())
            throw std::runtime_error("Unable to open input file!");

        T value;
        while(fin >> value && !fileDone){
            numbersQueue.push(value);
        }
        std::cout << "File done. Finishing reading..." << std::endl;
        fileDone = true;

        fin.close();
    }
    void setFileDone(bool status)
    {
        fileDone = status;
    }
    bool getFileDone()
    {
        return fileDone;
    }
private:
    bool fileDone;
    std::string inputFile;
};

//================  CONCURRENT FACTORIZATOR CLASS =================
//Main loop processes a file: reads data and push it to the queue, if
//it has space left. Concurrently, thread pool will be processing data
//and finally, processed data will be written to the output file
template <class T>
class ConcurrentProcesser
{
public:
    ConcurrentProcesser(const std::string &input, const std::string &output, size_t poolSize)
            : inputReader(input), outputFile(output), pool(poolSize + 2), pause(false), fileDone(false), disabled(false) {};
    ~ConcurrentProcesser()= default;
    void processFile()
    {

        std::ofstream fout(outputFile);
        if(!fout.good())
            throw std::runtime_error("Unable to open/create output file");

        std::future<void> producer = pool.addTaskToPool([this](){inputReader.runLoop(numbersQueue);});

        std::future<void> pauser = pool.addTaskToPool([this, &fout](){
            std::string comm;
            while(!numbersQueue.isEmpty() || !inputReader.getFileDone())
            {
                std::cin >> comm;
                if(comm == "pause" && !disabled)
                    pauseSystem(fout);

                if(comm == "resume" && !disabled)
                    resumeSystem(fout);

                if(comm == "exit")
                {
                    exitSystem(fout);
                    break;
                }
            }
            std::cout << "Pauser thread is finishing..." << std::endl;
        });

        while(true)
        {
            std::future<void> factors;
            if(!numbersQueue.isEmpty())
            {
                std::unique_lock<std::mutex> locker(mtx);
                pauseCondition.wait(locker, [this](){return pause == false;});

                factors = pool.addTaskToPool([this, &fout](){
                    T val = numbersQueue.popFront();
                    std::string strout = Factorizator::getFactorStringFromVector(Factorizator::factoriseByNaive(val));
                    fout << val << ": " << strout << std::endl;
                });
                factors.get();
            }
            if((numbersQueue.isEmpty() && inputReader.getFileDone()))
            {
                std::cout << "Queue now is empty, finishing..."   << std::endl
                          << "Press any key + ENTER to continue." << std::endl;
                break;
            }
            if(disabled)
            {
                std::cout << "Queue now is empty, finishing..." << std::endl;
                break;
            }
        }

        if(fout.is_open()) fout.close();

    }
    void pauseSystem(std::ofstream &fout)
    {
        {
            std::unique_lock<std::mutex> locker (mtx);
            std::cout << "Pausing..." << std::endl;
            pause = true;
            fout.close();
        }
        pauseCondition.notify_all();
    }
    void resumeSystem(std::ofstream &fout)
    {
        {
            std::unique_lock<std::mutex> locker(mtx);
            std::cout << "Resuming..." << std::endl;
            pause = false;
            fout.open(outputFile, std::ofstream::app);
        }
        pauseCondition.notify_all();
    }
    void exitSystem(std::ofstream &fout)
    {
        std::unique_lock<std::mutex> locker(mtx);
        std::cout << "Exiting..." << std::endl;
        disabled = true;
        pause = false;
        inputReader.setFileDone(true);
    }
private:
    std::string outputFile;
    std::mutex mtx;
    std::condition_variable pauseCondition;
    ConcurrentQueue<T> numbersQueue;
    ThreadPool pool;
    Reader inputReader;
    bool pause;
    bool disabled;
    bool fileDone;
};


