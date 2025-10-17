#include <iostream>
#include <vector>
#include <string>
using namespace std;
class solution {
  public:
    int titleToNumber(string columnTitle) {
        int result = 0;
        for (char c : columnTitle) {
            result = result * 26 + (c - 'A' + 1);
        }
        return result;
    }
};
int main() {
    solution sol;
    string columnTitle;
    cout << "columnTitle = ";
    cin >> columnTitle;
    int result = sol.titleToNumber(columnTitle);
    cout << "Output: " << result << endl;
    return 0;
}