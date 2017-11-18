#ifndef StatisticMultiset_hpp
#define StatisticMultiset_hpp
#include <map>
#include <set> 
#include <list>
#include <vector>
#include <fstream>
#include <iostream>
#include "myException.hpp"

template <class T>
class StatisticMultiset
{
    public:
        StatisticMultiset<T>();
        StatisticMultiset<T>(const std::vector<T> &vec);
        StatisticMultiset<T>(const std::set<T> &dset);
        StatisticMultiset<T>(const std::multiset<T> &mset);
        StatisticMultiset<T>(const std::list<T> &dlist);
        StatisticMultiset<T>(const std::string &fileName);
    
        T GetMaxVal() const;
        T GetMinVal() const;
        float GetAvgVal() const;
        
        int GetCountAbove(float threshold) const;
        int GetCountUnder(float threshold) const;
        
        void AddNum (T num);
        void AddNumFromFile (const std::string &fileName);
        void AddNum(const StatisticMultiset &statset);
        template <class Iter>
        void AddNum(Iter first, Iter last);
        void Show() const;
        
        StatisticMultiset<T>& operator=(const StatisticMultiset<T> &otherset);
    
    private:
        std::multiset<T> data;
    
        mutable std::map<float, int> cacheAbove;
        mutable std::map<float, int> cacheUnder;
        mutable float cachedAvgValue;
    
        void RecalculateAvg() const;
        void RecalculateCountAbove(float t) const;
        void RecalculateCountUnder(float t) const; 
        void RecalculateAll() const;
};


//===CONSTRUCTORS===
template <typename T>
StatisticMultiset<T>::StatisticMultiset()
{
    cachedAvgValue = 0;
}

template <typename T>
StatisticMultiset<T>::StatisticMultiset(const std::vector<T> &vec)
{
    AddNum(vec.begin(), vec.end());
}

template <typename T>
StatisticMultiset<T>::StatisticMultiset(const std::set<T> &dset)
{
    AddNum(dset.begin(), dset.end());
}

template <typename T>
StatisticMultiset<T>::StatisticMultiset(const std::multiset<T> &mset)
{
    AddNum(mset.begin(), mset.end());
}

template <typename T>
StatisticMultiset<T>::StatisticMultiset(const std::list<T> &dlist)
{
    AddNum(dlist.begin(), dlist.end());
}

//===GETTERS AND RECALCULATES CACHES===
template <typename T>
void StatisticMultiset<T>::RecalculateAvg() const
{
    cachedAvgValue = 0;
    for(const auto &it: data)
        cachedAvgValue += it;
    cachedAvgValue /= data.size();
}

template <typename T>
void StatisticMultiset<T>::RecalculateCountAbove(float t) const
{
    auto it = data.upper_bound(t);
    int cnt = static_cast<int>(std::distance(it, data.end()));
    cacheAbove[t] = cnt;
}

template <typename T>
void StatisticMultiset<T>::RecalculateCountUnder(float t) const
{
    auto it = data.lower_bound(t);
    int cnt = static_cast<int>(std::distance(data.begin(), it));
    cacheUnder[t] = cnt;
}

template <typename T>
void StatisticMultiset<T>::RecalculateAll() const
{
    for( auto &it : cacheAbove)
        RecalculateCountAbove(it.first);
    for( auto &it : cacheUnder)
        RecalculateCountUnder(it.first);
    
    RecalculateAvg();
}

template <typename T>
T StatisticMultiset<T>::GetMaxVal() const
{
    return *(std::prev(data.end()));
}

template <typename T>
T StatisticMultiset<T>::GetMinVal() const
{
    return *(data.begin());
}

template <typename T>
float StatisticMultiset<T>::GetAvgVal() const
{
    return cachedAvgValue;
}

template <typename T>
int StatisticMultiset<T>::GetCountAbove(float threshold) const
{
    auto it = cacheAbove.find(threshold);
    if(it == cacheAbove.end()){
        RecalculateCountAbove(threshold);
        return cacheAbove.find(threshold)->second;
    }
    else return it->second;
}

template<typename T>
int StatisticMultiset<T>::GetCountUnder(float threshold) const
{
    auto it = cacheUnder.find(threshold);
    if(it == cacheUnder.end()){
        RecalculateCountUnder(threshold);
        return cacheUnder.find(threshold)->second;
    }
    else return it->second;
}

//===ADDNUMS===
template <typename T>
void StatisticMultiset<T>::AddNum(T num)
{
    data.insert(num);
    RecalculateAll();
}

template <typename T>
void StatisticMultiset<T>::AddNum(const StatisticMultiset &statset)
{
    if(statset.data.begin() == this->data.begin())
        throw CopyingFromItself();
    for(const auto &it: statset.data)
        data.insert(it);
    RecalculateAll();
}

template <typename T>
template <class Iter>
void StatisticMultiset<T>::AddNum(Iter first, Iter last)
{
    for(;first!=last;first++)
        AddNum(*first);
}

template <typename T>
void StatisticMultiset<T>::AddNumFromFile(const std::string &fileName)
{
    std::ifstream fin;
    
    fin.open(fileName);
    if(!fin.is_open())
        std::cerr << "Unable ot open file" << std::endl;
    
    T num;
    while(fin >> num)
        AddNum(num);
}

//===OTHERFUNCTIONS===
template <typename T>
void StatisticMultiset<T>::Show() const
{
    for(const auto &it: data)
        std::cout << it << std::endl;
}

template <typename T>
StatisticMultiset<T>& StatisticMultiset<T>::operator=(const StatisticMultiset &otherset)
{
    this->data.clear();
    this->AddNum(otherset);
    return *this;
}

#endif /* StatisticMultiset_hpp */
