#ifndef StatisticMultiset_hpp
#define StatisticMultiset_hpp
#include <set> 
#include <list>
#include <vector>
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
        
        int GetCountAbove(float threshold) const;
        int GetCountUnder(float threshold) const;
        
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
        mutable T cachedMaxValue;
        mutable T cachedMinValue;
        mutable float cachedAvgValue;
        mutable bool changeFlag;
        mutable std::pair<float,int> cachedCountAbove;
        mutable std::pair<float,int> cachedCountUnder;
        void RecalculateAvg() const; //
        void RecalculateAll() const; //
        void RecalculateCountAbove(float t) const; //
        void RecalculateCountUnder(float t) const; //
        void setFlagToFalse() const; //
};


//===CONSTRUCTORS===
template <typename T>
StatisticMultiset<T>::StatisticMultiset()
{
    cachedMaxValue = 0;
    cachedMinValue = 0;
    cachedAvgValue = 0;
    cachedCountAbove = std::make_pair(0, 0);
    cachedCountUnder = std::make_pair(0, 0);
    changeFlag = false;
}

template <typename T>
StatisticMultiset<T>::StatisticMultiset(const std::vector<T> &vec)
{
    AddNum(vec);
    changeFlag = true;
}

template <typename T>
StatisticMultiset<T>::StatisticMultiset(const std::set<T> &dset)
{
    AddNum(dset);
    changeFlag = true;
}

template <typename T>
StatisticMultiset<T>::StatisticMultiset(const std::multiset<T> &mset)
{
    AddNum(mset);
    changeFlag = true;
}

template <typename T>
StatisticMultiset<T>::StatisticMultiset(const std::list<T> &dlist)
{
    AddNum(dlist);
    changeFlag = true;
}

//===GETTERS===
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
    cachedCountAbove = std::make_pair(t,cnt);
}

template <typename T>
void StatisticMultiset<T>::RecalculateCountUnder(float t) const
{
    auto it = data.lower_bound(t);
    int cnt = static_cast<int>(std::distance(data.begin(), it));
    cachedCountUnder = std::make_pair(t, cnt);
}

template <typename T>
void StatisticMultiset<T>::RecalculateAll() const
{
    cachedMaxValue = *std::max_element(data.begin(), data.end());
    cachedMinValue = *data.begin();
    RecalculateAvg();
    RecalculateCountUnder(cachedCountUnder.first);
    RecalculateCountAbove(cachedCountAbove.first);
}

template <typename T>
void StatisticMultiset<T>::setFlagToFalse() const
{
    changeFlag = false;
    RecalculateAll();
}

template <typename T>
T StatisticMultiset<T>::GetMaxVal() const
{
    if(!changeFlag)
        return cachedMaxValue;
    
    setFlagToFalse();
    return cachedMaxValue;
}

template <typename T>
T StatisticMultiset<T>::GetMinVal() const
{
    if(!changeFlag)
        return cachedMinValue;
    
    setFlagToFalse();
    return cachedMinValue;
}

template <typename T>
float StatisticMultiset<T>::GetAvgVal() const
{
    if(!changeFlag)
        return cachedAvgValue;
    
    setFlagToFalse();
    return cachedAvgValue;
}

template <typename T>
int StatisticMultiset<T>::GetCountAbove(float threshold) const
{
    if(!changeFlag && threshold == cachedCountAbove.first)
        return cachedCountAbove.second;
    
    cachedCountAbove.first = threshold;
    setFlagToFalse();
    return cachedCountAbove.second;
}

template<typename T>
int StatisticMultiset<T>::GetCountUnder(float threshold) const
{
    if(!changeFlag && threshold == cachedCountUnder.first)
        return cachedCountUnder.second;
    
    cachedCountUnder.first = threshold;
    setFlagToFalse();
    return cachedCountUnder.second;
}

//===ADDNUMS===
template <typename T>
void StatisticMultiset<T>::AddNum(T num)
{
    data.insert(num);
    changeFlag = true;
}

template <typename T>
void StatisticMultiset<T>::AddNum(const std::vector<T> &vec)
{
    for(const auto &it: vec)
        data.insert(it);
    changeFlag = true;
}

template <typename T>
void StatisticMultiset<T>::AddNum(const std::set<T> &dset)
{
    for(const auto &it: dset)
        data.insert(it);
    changeFlag = true;
}

template <typename T>
void StatisticMultiset<T>::AddNum(const std::list<T> &dlist)
{
    for(const auto &it: dlist)
        data.insert(it);
    changeFlag = true;
}

template <typename T>
void StatisticMultiset<T>::AddNum(const std::multiset<T> &mset)
{
    for(const auto &it: mset)
        data.insert(it);
    changeFlag = true;
}

template <typename T>
void StatisticMultiset<T>::AddNum(const StatisticMultiset &statset)
{
    for(const auto &it: statset.data)
        data.insert(it);
    changeFlag = true;
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
    this->changeFlag = true;
    return *this;
}

#endif /* StatisticMultiset_hpp */
