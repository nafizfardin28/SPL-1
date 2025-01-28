#include <iostream>
#include <vector>
using namespace std;

class Matrix {
private:
    vector<vector<int>> mat;
    int rows, cols;

public:
    Matrix(int r, int c) : rows(r), cols(c) {
        mat.resize(r, vector<int>(c, 0));
    }

    void inputMatrix() {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                cin >> mat[i][j];
            }
        }
    }

    Matrix transpose() const {
        Matrix transposed(cols, rows);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                transposed.mat[j][i] = mat[i][j];
            }
        }
        return transposed;
    }

    void displayMatrix() const {
        for (const auto& row : mat) {
            for (const auto& elem : row) {
                cout << elem << " ";
            }
            cout << endl;
        }
    }
};

int main() {
    int rows, cols;
    cin >> rows >> cols;

    Matrix mat(rows, cols);
    mat.inputMatrix();

    Matrix transposed = mat.transpose();
    transposed.displayMatrix();

    return 0;
}
