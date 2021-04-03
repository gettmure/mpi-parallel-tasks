#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <mpi/mpi.h>

using namespace std;

const int MATRIX_SIZE = 3;

void multiplyMatrixRec(int A[][MATRIX_SIZE], int B[][MATRIX_SIZE], int C[][MATRIX_SIZE])
{
    static int i = 0, j = 0, k = 0;

    if (i >= MATRIX_SIZE) return;

    if (j < MATRIX_SIZE)
    {
        if (k < MATRIX_SIZE)
        {
            C[i][j] += A[i][k] * B[k][j];
            k++;

            multiplyMatrixRec(A, B, C);
        }

        k = 0;
        j++;
        multiplyMatrixRec(A, B, C);
    }

    j = 0;
    i++;
    multiplyMatrixRec(A, B, C);
}

void multiplyMatrix(int A[][MATRIX_SIZE], int B[][MATRIX_SIZE])
{
    int C[MATRIX_SIZE][MATRIX_SIZE] = {0};

    multiplyMatrixRec(A, B, C);

    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d  ", C[i][j]);
        }

        printf("\n");
    }
}

int main(int argc, char **argv) {
    int threadsCount, threadRank;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &threadsCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &threadRank);

    int a[MATRIX_SIZE][MATRIX_SIZE] = {{1, 1, 1}, {2, 2, 2}, {3, 3, 3}};
    int b[MATRIX_SIZE][MATRIX_SIZE] = {{1, 1, 1}, {2, 2, 2}, {3, 3, 3}};

    multiplyMatrix(a, b);

//    if (threadRank == 0) {
//        for (int i = 1; i < threadsCount; i++ ) {
//            MPI_Recv(
//                    &reciever,
//                    1,
//                    MPI_INT,
//                    MPI_ANY_SOURCE,
//                    MPI_ANY_TAG,
//                    MPI_COMM_WORLD,
//                    &status
//            );
//        }
//
//        cout << reciever << endl;
//    } else {
//        MPI_Send(&threadRank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
//    }

    MPI_Finalize();

    return 0;
}