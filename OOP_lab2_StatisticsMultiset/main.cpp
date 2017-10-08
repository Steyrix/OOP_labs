#include "StatisticMultiset.hpp"


int main() {

    std::vector<int> vec;
    vec.push_back(-1);
    vec.push_back(-7);
    vec.push_back(8);
    vec.push_back(19);
    vec.push_back(0);
    
    StatisticMultiset<int> data(vec);
    data.Show();
    
    
    StatisticMultiset<int> s2;
    s2.AddNum(data);
    s2.Show();
    
    StatisticMultiset<int> s3;
    s3.operator=(s2);
    s3.Show();
    
    std::cout << "MAX: " << s3.GetMaxVal() << std::endl;
    std::cout << "MIn: " << s3.GetMinVal() << std::endl;
    std::cout << "Avg: " << s3.GetAvgVal() << std::endl;
    std::cout << "CountAbove:" << s3.GetCountAbove(-7) << std::endl;
    getchar();
    return 0;
}
