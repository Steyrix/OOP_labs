#ifndef StatisticMultiset_hpp
#define StatisticMultiset_hpp
#include <set> 
#include <list>
#include <vector>
#include <iostream>

template <typename T>
class StatisticMultiset;

template <typename T>
void AddNum(StatisticMultiset<T> const &statset);

template <class T>
class StatisticMultiset
{
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
    public:
        StatisticMultiset<T>(); //
        StatisticMultiset<T>(std::vector<T> const &vec); //
        StatisticMultiset<T>(std::set<T> const &dset); //
        StatisticMultiset<T>(std::multiset<T> const &mset); //
        StatisticMultiset<T>(std::list<T> const &dlist); //
        StatisticMultiset<T>(std::string const &fileName); //
    
        T GetMaxVal() const; //
        T GetMinVal() const; //
        float GetAvgVal() const; //
    
        int GetCountAbove(float threshold) const;
        int GetCountUnder(float threshold) const;
    
        void AddNum (T num); //
        void AddNum (std::vector<T> const &vec); //
        void AddNum (std::set<T> const &dset); //
        void AddNum (std::multiset<T> const &mset); //
        void AddNum (std::list<T> const &dlist); //
        void AddNumFromFile (std::string const &fileName); //
        void AddNum(StatisticMultiset const &statset); //
    
        void Show() const; //
    
        StatisticMultiset<T>& operator=(StatisticMultiset<T> const &otherset); //
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
StatisticMultiset<T>::StatisticMultiset(std::vector<T> const &vec)
{
    AddNum(vec);
    changeFlag = true;
}

template <typename T>
StatisticMultiset<T>::StatisticMultiset(std::set<T> const &dset)
{
    AddNum(dset);
    changeFlag = true;
}

template <typename T>
StatisticMultiset<T>::StatisticMultiset(std::multiset<T> const &mset)
{
    AddNum(mset);
    changeFlag = true;
}

template <typename T>
StatisticMultiset<T>::StatisticMultiset(std::list<T> const &dlist)
{
    AddNum(dlist);
    changeFlag = true;
}

//===GETTERS===
template <typename T>
void StatisticMultiset<T>::RecalculateAvg() const
{
    cachedAvgValue = 0;
    for(auto it: data)
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
void StatisticMultiset<T>::AddNum(std::vector<T> const &vec)
{
    for(auto it: vec)
        data.insert(it);
    changeFlag = true;
}

template <typename T>
void StatisticMultiset<T>::AddNum(std::set<T> const &dset)
{
    for(auto it: dset)
        data.insert(it);
    changeFlag = true;
}

template <typename T>
void StatisticMultiset<T>::AddNum(std::list<T> const &dlist)
{
    for(auto it: dlist)
        data.insert(it);
    changeFlag = true;
}

template <typename T>
void StatisticMultiset<T>::AddNum(std::multiset<T> const &mset)
{
    for(auto it: mset)
        data.insert(it);
    changeFlag = true;
}

template <typename T>
void StatisticMultiset<T>::AddNum(StatisticMultiset const &statset)
{
    for(auto it: statset.data)
        data.insert(it);
    changeFlag = true;
}

//===OTHERFUNCTIONS===
template <typename T>
void StatisticMultiset<T>::Show() const
{
    for(auto it: data)
        std::cout << it << std::endl;
}

template <typename T>
StatisticMultiset<T>& StatisticMultiset<T>::operator=(StatisticMultiset const &otherset)
{
    this->data.clear();
    this->AddNum(otherset);
    this->changeFlag = true;
    return *this;
}

#endif /* StatisticMultiset_hpp */
