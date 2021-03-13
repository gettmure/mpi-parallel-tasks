#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <mpi/mpi.h>

using namespace std;

const double EPS = 1E-9;

const unsigned char MATRIX_SIZE = 2;
const char MIN_MATRIX_VALUE = -100;
const char MAX_MATRIX_VALUE = 100;

const char DESTINATION_THREAD = 0;

double getDeterminant(vector<vector<double>>& matrix) {
    double det = 1;

    for (int i = 0; i < MATRIX_SIZE; i++) {
        int k = i;

        for (int j = i + 1; j<  MATRIX_SIZE; j++) {
            if (abs(matrix[j][i]) > abs(matrix[k][i])) {
                k = j;
            }
        }

        if (abs(matrix[k][i]) < EPS) {
            det = 0;
            break;
        }

        swap (matrix[i], matrix[k]);

        if (i != k) {
            det = -det;
        }

        det *= matrix[i][i];

        for (int j = i + 1; j < MATRIX_SIZE; j++) {
            matrix[i][j] /= matrix[i][i];
        }

        for (int j = 0; j < MATRIX_SIZE; j++) {
            if (j != i && abs (matrix[j][i]) > EPS) {
                for (int k = i + 1; k < MATRIX_SIZE; ++k) {
                    matrix[j][k] -= matrix[i][k] * matrix[j][i];
                }
            }
        }
    }

    return det;
}

double generateMatrixValue() {
    double randomValue = (double) rand() / RAND_MAX;

    return MIN_MATRIX_VALUE + randomValue * (MAX_MATRIX_VALUE - MIN_MATRIX_VALUE);
}

void initMatrix(vector<vector<double>>& matrix) {
    srand(time(nullptr));

    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrix[i][j] = generateMatrixValue();
        }
    }
}

void printMatrix(vector<vector<double>> matrix) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            cout << matrix[i][j] << ' ';
        }

        cout << endl;
    }

    cout << endl;
}

int main(int argc, char **argv) {
    vector<vector<double>> matrix (MATRIX_SIZE, vector<double> (MATRIX_SIZE));
    int det = 0;

    int threadsCount, threadRank;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &threadsCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &threadRank);

    if (threadRank == 0) {
        if (threadsCount > 1) {
            int receiverElement;

            for (int i = 1; i < threadsCount; i++ ) {
                MPI_Recv(
                        &receiverElement,
                        1,
                        MPI_INT,
                        MPI_ANY_SOURCE,
                        MPI_ANY_TAG,
                        MPI_COMM_WORLD,
                        &status
                );

                det += receiverElement;
            }
            det += receiverElement;
            cout << det;
        }
    } else {
        int element = 1;
        MPI_Send(&element, 1, MPI_INT, DESTINATION_THREAD, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
