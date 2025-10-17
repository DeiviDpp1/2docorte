#include <iostream>
#include <vector>
using namespace std;

class Solution{
public:
    vector<vector<int>> generate(int numRows) {
        vector<vector<int>> triangle(numRows);
        for(int row=0; row<numRows; row++){
            triangle[row].resize(row+1);
            triangle[row][0] = triangle[row][row] = 1;
            for(int col=1; col<row; col++){
                triangle[row][col] = triangle[row-1][col-1] + triangle[row-1][col];
            }
        }
        return triangle;
    }
};

main(){
    Solution sol;
    int numRows;
    cout << "numRows = ";
    cin >> numRows;
    if (numRows < 1 || numRows > 30) {
        cout << "numRows debe de estar entre 1 y 30." << endl;
        return 1;
    }

    vector<vector<int>> triangle = sol.generate(numRows);
    
    for(const auto& row : triangle){
        for(int val : row){
            cout << val << " ";
        }
        cout << endl;
    }
    return 0;
}
