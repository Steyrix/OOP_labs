#ifndef StatisticMultiset_hpp
#define StatisticMultiset_hpp
#include <set> 
#include <list>
#include <vector>
#include <map>
#include <iostream>

template <class T>
class StatisticMultiset
{
    public:
        StatisticMultiset<T>(); //
        StatisticMultiset<T>(const std::vector<T> &vec); //
        StatisticMultiset<T>(const std::set<T> &dset); //
        StatisticMultiset<T>(const std::multiset<T> &mset); //
        StatisticMultiset<T>(const std::list<T> &dlist); //
        StatisticMultiset<T>(const std::string &fileName); //
        
        T GetMaxVal() const; //
        T GetMinVal() const; //
        float GetAvgVal() const; //
        
        int GetCountAbove(float threshold) const; //
        int GetCountUnder(float threshold) const; //
        
        void AddNum (T num); //
        void AddNum (const std::vector<T> &vec); //
        void AddNum (const std::set<T> &dset); //
        void AddNum (const std::multiset<T> &mset); //
        void AddNum (const std::list<T> &dlist); //
        void AddNumFromFile (const std::string &fileName); //
        void AddNum(const StatisticMultiset &statset); //
        
        void Show() const; //
        
        StatisticMultiset<T>& operator=(const StatisticMultiset<T> &otherset); //
    private:
        std::multiset<T> data;
    
        mutable std::map<float, int> cacheAbove;
        mutable std::map<float, int> cacheUnder;
        mutable T cachedMaxValue;
        mutable T cachedMinValue;
        mutable float cachedAvgValue;
    
        void ResetMinMax(T num) const; //
        void RecalculateAvg() const; //
        void RecalculateCountAbove(float t) const; //
        void RecalculateCountUnder(float t) const; //
};


//===CONSTRUCTORS===
template <typename T>
StatisticMultiset<T>::StatisticMultiset()
{
    cachedMaxValue = 0;
    cachedMinValue = 0;
    cachedAvgValue = 0;
}

template <typename T>
StatisticMultiset<T>::StatisticMultiset(const std::vector<T> &vec)
{
    AddNum(vec);
}

template <typename T>
StatisticMultiset<T>::StatisticMultiset(const std::set<T> &dset)
{
    AddNum(dset);
}

template <typename T>
StatisticMultiset<T>::StatisticMultiset(const std::multiset<T> &mset)
{
    AddNum(mset);
}

template <typename T>
StatisticMultiset<T>::StatisticMultiset(const std::list<T> &dlist)
{
    AddNum(dlist);
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
void StatisticMultiset<T>::ResetMinMax(T num) const
{
    if(data.empty())
    {
        cachedMaxValue = num;
        cachedMinValue = num;
    }
    if(num < cachedMinValue)
        cachedMinValue = num;
    if(num > cachedMaxValue)
        cachedMaxValue = num;
}

template <typename T>
T StatisticMultiset<T>::GetMaxVal() const
{
    return cachedMaxValue;
}

template <typename T>
T StatisticMultiset<T>::GetMinVal() const
{
    return cachedMinValue;
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
        RecalculateCountAbove(threshold);
        return cacheUnder.find(threshold)->second;
    }
    else return it->second;
}

//===ADDNUMS===
template <typename T>
void StatisticMultiset<T>::AddNum(T num)
{
    data.insert(num);
    ResetMinMax(num);
    RecalculateAvg();
}

template <typename T>
void StatisticMultiset<T>::AddNum(const std::vector<T> &vec)
{
    for(const auto &it: vec)
    {
        data.insert(it);
        ResetMinMax(it);
    }
    RecalculateAvg();
}

template <typename T>
void StatisticMultiset<T>::AddNum(const std::set<T> &dset)
{
    for(const auto &it: dset)
    {
        data.insert(it);
        ResetMinMax(it);
    }
    RecalculateAvg();
}

template <typename T>
void StatisticMultiset<T>::AddNum(const std::list<T> &dlist)
{
    for(const auto &it: dlist)
    {
        data.insert(it);
        ResetMinMax(it);
    }
    RecalculateAvg();
}

template <typename T>
void StatisticMultiset<T>::AddNum(const std::multiset<T> &mset)
{
    for(const auto &it: mset)
    {
        data.insert(it);
        ResetMinMax(it);
    }
    RecalculateAvg();
}

template <typename T>
void StatisticMultiset<T>::AddNum(const StatisticMultiset &statset)
{
    for(const auto &it: statset.data)
    {
        data.insert(it);
        ResetMinMax(it);
    }
    RecalculateAvg();
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
