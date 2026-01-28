
#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <set>
using namespace std;
const double inf = numeric_limits<double>::max();

// DataRead считывает данные из файла
void DataRead(int& size, vector<vector<double>>& mas) {
    ifstream file("tsp.txt");

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


//GreedyWay ищет стоимость жадного пути
pair<double, vector<int>> GreedyWay(int size, const vector<vector<double>>& Matrix) {
    vector<bool> visited(size, false);
    vector<int> path{ 0 };
    double PathCost = 0, MinEdge = inf;
    int CurrVertex = 0, MinVertex = -1;
    visited[CurrVertex] = true;

    for (int count = 1; count < size; count++) {
        MinEdge = inf;
        MinVertex = -1;
        for (int i = 0; i < size; i++) {
            if (!visited[i] && Matrix[CurrVertex][i] < MinEdge) {
                MinEdge = Matrix[CurrVertex][i];
                MinVertex = i;
            }
        }
        PathCost += MinEdge;
        CurrVertex = MinVertex;
        visited[CurrVertex] = true;
        path.push_back(CurrVertex);
    }

    PathCost += Matrix[CurrVertex][0];
    path.push_back(0);
    return { PathCost, path };
}


// GenerateRemain инициализирует Остаток для функции NaiveAlgorithm
set<int> GenerateRemain(int size) {
    set<int> remains = {};
    for (int i = 1; i < size; i++) {
        remains.insert(i);
    }
    return remains;
}


void NaiveAlgorithm(int CurrVertex, int preVertex, set<int> remains, double LowerBound, double& UpperBound,vector<int>CurrPath,vector<int>& UpperPath, vector<vector<double>> & Matrix) {
    CurrPath.push_back(CurrVertex);
    if (preVertex != -1) LowerBound += Matrix[preVertex][CurrVertex];
    if (LowerBound > UpperBound) return;
    if (remains.empty() && UpperBound >= LowerBound + Matrix[CurrVertex][0]) {
        UpperBound = LowerBound + Matrix[CurrVertex][0];
        UpperPath = CurrPath;
    }
    else if (remains.empty()) return;
    else {
        for (int child : remains) {
            set<int> NewRemains = remains;
            NewRemains.erase(child);
            NaiveAlgorithm(child, CurrVertex, NewRemains, LowerBound, UpperBound, CurrPath, UpperPath,   Matrix);
        }
    }
}


int main()
{
    vector<vector<double>> AdjacencyMatrix{};
    int size = 0;
    DataRead(size, AdjacencyMatrix);

    //проверка считывания
    /*cout << size << endl;
    for (auto row : AdjacencyMatrix) {
        for (auto el : row) {
            cout << el << " ";
        }
        cout << endl;
    }*/
    
    double GreedyCost;
    vector<int> Greedypath;
    pair<double, vector<int>> GreedyRes = GreedyWay(size, AdjacencyMatrix);
    GreedyCost = GreedyRes.first;
    Greedypath = GreedyRes.second;
    double UpperBound = GreedyCost;
    vector<int> path{};
    NaiveAlgorithm(0, -1, GenerateRemain(size), 0, UpperBound, {}, path, AdjacencyMatrix);
    cout << "Optimal cost is "<<UpperBound << endl;
    cout << "Trajectory is ";
    for (auto el : path) {
        cout << el << "->";
    }
    cout << "0";
    return 0;
}

