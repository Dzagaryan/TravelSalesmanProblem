
#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <set>
using namespace std;
const double inf = numeric_limits<double>::max();
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


void Display2DMatrix(int size, vector<vector<double>> Matrix) {
    for (auto& row : Matrix) {
        for (auto& el : row) {
            cout << el << " ";
        }
        cout << endl;
    }
}


bool Search(int el, vector<int>& mas) {
    for (auto& elem : mas) {
        if (elem == el) return true;
    }
    return false;
}

double ReductionRows(int size, vector<vector<double>>& Matrix, vector<int>& TabuRow, vector<int>& TabuColumn ) {
    vector<int> AllowRow{}, AllowColumn{};
    for (int i = 0; i < size; i++) {
        if (!Search(i, TabuRow)) AllowRow.push_back(i);
        if (!Search(i, TabuColumn)) AllowColumn.push_back(i);
    }

    double min_el = inf;
    double suma = 0;
    for (int i : AllowRow) {
        min_el = inf;
        for (int j : AllowColumn) {
            if (i != j) min_el = min(min_el, Matrix[i][j]);
        }
        for (int j : AllowColumn) {
            if (i != j) Matrix[i][j]-= min_el;
        }
        suma += min_el;
    }
    
    return suma;
}

double ReductionColumn(int size, vector<vector<double>>& Matrix, vector<int>& TabuRow, vector<int>& TabuColumn) {
    vector<int> AllowRow{}, AllowColumn{};
    for (int i = 0; i < size; i++) {
        if (!Search(i, TabuRow)) AllowRow.push_back(i);
        if (!Search(i, TabuColumn)) AllowColumn.push_back(i);
    }

    double min_el = inf;
    double suma = 0;
    for (int j : AllowColumn) {
        min_el = inf;
        for (int i : AllowRow) {
            if (i != j) min_el = min(min_el, Matrix[i][j]);
        }
        for (int i : AllowRow) {
            if (i != j) Matrix[i][j] -= min_el;
        }
        suma += min_el;
    }

    return suma;
}

double ReductionRowAndColumn(int size, vector<vector<double>>& Matrix, vector<int>& TabuRow, vector<int>& TabuColumn) {
    return ReductionRows(size, Matrix, TabuRow, TabuColumn) + ReductionColumn(size, Matrix, TabuRow, TabuColumn);
}


pair<double, double> PenaltySearch(int size, pair<int, int> Coordinates, vector<vector<double>>& Matrix, vector<int>& TabuRow, vector<int>& TabuColumn) {
    vector<int> AllowRow{}, AllowColumn{};
    for (int i = 0; i < size; i++) {
        if (!Search(i, TabuRow)) AllowRow.push_back(i);
        if (!Search(i, TabuColumn)) AllowColumn.push_back(i);
    }
    double MinXel = inf, MinYel = inf;
    for (int i : AllowRow) {
        if (i != Coordinates.first) MinYel = min(MinYel, Matrix[i][Coordinates.second]);
    }
    for (int j : AllowColumn) {
        if (j != Coordinates.second) MinXel = min(MinXel, Matrix[Coordinates.first][j]);
    }
    return  { MinXel, MinYel };
}



void BranchAndBound(int size, double LowerBound, double& UpperBound, vector<vector<double>> Matrix, vector<int> TabuRow, vector<int> TabuColumn, vector<pair<int, int>> CurrPath, vector<pair<int, int>>& BestPath) {
    // обработка матрицы и обновления lowerbound

    LowerBound += ReductionRowAndColumn(size, Matrix, TabuRow, TabuColumn);
    
    // начало следующей итерации
    if (LowerBound >= UpperBound) return;
    if (CurrPath.size() == size) {
        UpperBound = LowerBound;
        BestPath = CurrPath;
    }               // нужна редакция если нужен будет путь или мб вообще непраильно
    
    //Выбор ребра с самым высокми штрафом
    double maxPenalty = -inf;
    pair<int, int> BestEdjeCoordinates;
    pair<double, double> CurrPenalty;
    vector<int> AllowRow{}, AllowColumn{};

    for (int i = 0; i < size; i++) {
        if (!Search(i, TabuRow)) AllowRow.push_back(i);
        if (!Search(i, TabuColumn)) AllowColumn.push_back(i);
    }

    for (int i : AllowRow) {
        for (int j : AllowColumn) {
            if (Matrix[i][j] == 0) { // нужна проверка на циклы в пути 
                CurrPenalty = PenaltySearch(size, { i,j }, Matrix, TabuRow, TabuColumn);
                if (CurrPenalty.first + CurrPenalty.second > maxPenalty) {
                    maxPenalty = CurrPenalty.first + CurrPenalty.second;
                    BestEdjeCoordinates.first = i;
                    BestEdjeCoordinates.second = j;
                }
            }
        }
    }

    // BestEdjeCoordinates - координаты наиболее выгодного ребра, BestEdjeCPenalty - штрафы по х и у
    // Разбиения

    // 1 берем таки прцедуры удаления и добавления делаются чтобы не вводить новые переменые векторы итд
    double Tabuelement = Matrix[BestEdjeCoordinates.second][BestEdjeCoordinates.first];
    TabuRow.push_back(BestEdjeCoordinates.first);
    TabuColumn.push_back(BestEdjeCoordinates.second);
    // !!!!иногда может получиться так что данного ребра не будет и нужно будет блокать другое ребро я не учитываю !!!!!!
    if (!Search(BestEdjeCoordinates.second, TabuRow) && !Search(BestEdjeCoordinates.first, TabuColumn)) Matrix[BestEdjeCoordinates.second][BestEdjeCoordinates.first] = inf;
    CurrPath.push_back(BestEdjeCoordinates);
    BranchAndBound(size, LowerBound, UpperBound, Matrix, TabuRow, TabuColumn, CurrPath, BestPath);

    TabuRow.pop_back();
    TabuColumn.pop_back();
    Matrix[BestEdjeCoordinates.second][BestEdjeCoordinates.first] = Tabuelement;
    

    //2 не берем
    Matrix[BestEdjeCoordinates.first][BestEdjeCoordinates.second] = inf;
    BranchAndBound(size, LowerBound, UpperBound, Matrix, TabuRow, TabuColumn, CurrPath, BestPath);

}
int main()
{
    vector<vector<double>> AdjacencyMatrix{};
    int size = 0;
    DataRead(size, AdjacencyMatrix);

    for (int i = 0; i < size; i++) {
        AdjacencyMatrix[i][i] = inf;
    }

    double GreedyCost;
    vector<int> Greedypath;
    pair<double, vector<int>> GreedyRes = GreedyWay(size, AdjacencyMatrix);
    GreedyCost = GreedyRes.first;
    Greedypath = GreedyRes.second;

    vector<int> TabuRow{};
    vector<int> TabuColumn{};
    double LowerBound = ReductionRowAndColumn(size, AdjacencyMatrix, TabuRow, TabuColumn);
    double UpperBound = GreedyCost;

    vector<pair<int, int>> BestPath{};
    for (int i = 0; i < size - 1; i++) {
        BestPath.push_back({ Greedypath[i], Greedypath[i + 1] });
    }

    BranchAndBound(size, LowerBound, UpperBound, AdjacencyMatrix, {}, {}, {}, BestPath);
    cout <<  "Optimal cost is " << UpperBound << endl;
    cout << "Best Path is ";
    int last = 0;
    for (int i = 0; i < size; i++) {
        cout << last << "->";
        for (int j = 0; j < size; j++) {
            if (BestPath[i].first == last) {
                last = BestPath[i].second;
                break;
            }
            else if (BestPath[i].second = last) {
                last = BestPath[i].first;
                break;
            }
        }
    }

    /*cout << endl;
    for (auto el : BestPath) {
        cout << el.first << " " << el.second << endl;
    }*/
}


