#include "DancingLinkX.hpp"
#include <ctime>
#include <chrono>

struct triple {int x, y, n;};
std::vector<triple> ope;
int square[9][9];

void sudokuShow(const std::vector<int>& dlxAns) {
    for (auto i : dlxAns) square[ope[i].x][ope[i].y] = ope[i].n+1;
    for (auto & i : square) {
        for (int j : i) std::cout << j << ' ';
        std::cout << '\n';
    }
}

int main() {
    int rowCnt = 0;
    std::string buff; //store the sudoku problem
    std::cin >> buff;
    auto beg = std::chrono::system_clock::now();
    bool** sta = new bool* [729];
    for (int i = 0; i < 729; ++i) sta[i] = new bool [324];  //81 162 243 324
    for (int i = 0; i < 9; ++i) for (int j = 0; j < 9; ++j) {
        if (buff[i*9+j] == '0') {
            for (int k = 0; k < 9; ++k) {
                sta[rowCnt][i*9+j] = sta[rowCnt][81+k*9+i] = sta[rowCnt][162+k*9+j] = sta[rowCnt][243+k*9+i/3*3+j/3] = true, ++rowCnt;
                ope.push_back((triple){i, j, k});
            }
        }
        else {
            int k = int(buff[i*9+j]-'0')-1;
            sta[rowCnt][i*9+j] = sta[rowCnt][81+k*9+i] = sta[rowCnt][162+k*9+j] = sta[rowCnt][243+k*9+i/3*3+j/3] = true, ++rowCnt;
            ope.push_back((triple){i, j, k});
            square[i][j] = k+1;
        }
    }
    DancingLinkMatrix dlx(324, rowCnt, sta);
    if (dlx.dfs()) sudokuShow(dlx.ans);
    else std::cout << "404 Not Found!\n";
    auto end = std::chrono::system_clock::now();
    auto d = std::chrono::duration_cast<std::chrono::nanoseconds>(end-beg);
    std::cout << "total time: " << d.count() << '\n';
    delete [] sta;
    return 0;
}
