#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

vector<int> twoSum(vector<int>& nums, int target) {
    unordered_map<int,int> mapa;
    for (int i=0;i<nums.size();i++) {
        int complemento = target - nums[i];
        if(mapa.count(complemento)) 
            return {mapa[complemento], i};
        mapa[nums[i]] = i;
    }
    return {};
}
