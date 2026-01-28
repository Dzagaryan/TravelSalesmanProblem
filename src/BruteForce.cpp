
#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <set>
#include<algorithm>
using namespace std;
void DataRead(int& size, vector<vector<double>>& mas) {
    ifstream file("D:/VisualStdudio/TravellingSalesmanProblemNaiveAlgorithm/tsp.txt");

    string line;
    getline(file, line);
    stringstream str(line);
    str >> size;

    mas.resize(size, vector<double>(size));
    int i = 0;
    while (getline(file, line)) {
        stringstream str(line);
        for (int j = 0; j < size; ++j) {
            str >> mas[i][j];
        }
        ++i;
    }
    file.close();
}

vector<vector<int>> GeneratePermutation(int size) {
    vector<int> ObjectOfPermutation{};
    vector<vector<int>> res = {};
    for (int i = 1; i < size; i++) {
        ObjectOfPermutation.push_back(i);
    }
    vector<int> test;
    do {
        test = {};
        for (int x : ObjectOfPermutation) {
            test.push_back(x);
        }
        res.push_back(test);
    } while (next_permutation(ObjectOfPermutation.begin(), ObjectOfPermutation.end()));
    return res;
}

pair<double, vector<int>> BruteForceAlgorithm(int size, vector<vector<double>>& Matrix) {
    vector<vector<int>> PermutationList = GeneratePermutation(size);
    double Min_cost = numeric_limits<double>::max();
    double CurrCost;
    vector<int> path{};
    for (auto& permutate : PermutationList) {
        CurrCost = Matrix[0][permutate[0]];
        for (int i = 0; i < size - 2; i++) {
            CurrCost += Matrix[permutate[i]][permutate[i + 1]];
        }
        CurrCost += Matrix[permutate[size - 2]][0];
        if (Min_cost >= CurrCost) {
            Min_cost = CurrCost;
            path = permutate;
        }

        // вывод путей и их стоимостей
        /*for (auto el : permutate) {
            cout << el << " ";
        }
        cout << endl;
        cout << CurrCost;
        cout << endl;
        cout << "---------------";
        cout << endl;*/
    }

    return { Min_cost, path };
}

int main()
{
    vector<vector<double>> AdjacencyMatrix{};
    int size = 0;
    DataRead(size, AdjacencyMatrix);
    pair<double, vector<int>> res = BruteForceAlgorithm(size, AdjacencyMatrix);
    cout << "Optimal cost is " << res.first << endl;
    cout << "Trajectory is " << "0->";
    for (auto el : res.second) {
        cout << el << "->";
    }
    cout << "0";
}

