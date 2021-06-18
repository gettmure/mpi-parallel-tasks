#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <mpi/mpi.h>

using namespace std;

const double EPS = 1E-9;

const unsigned char N = 3;
const char MIN_MATRIX_VALUE = -100;
const char MAX_MATRIX_VALUE = 100;

const char DESTINATION_THREAD = 0;

// Function to get cofactor of mat[p][q] in temp[][]. n is
// current dimension of mat[][]
void getCofactor(int mat[N][N], int temp[N][N], int p,
                 int q, int n)
{
    int i = 0, j = 0;

    // Looping for each element of the matrix
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            //  Copying into temporary matrix only those
            //  element which are not in given row and
            //  column
            if (row != p && col != q)
            {
                temp[i][j++] = mat[row][col];

                // Row is filled, so increase row index and
                // reset col index
                if (j == n - 1)
                {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

int determinantOfMatrix(int mat[N][N], int n, int argc, char **argv)
{
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
            }
        }
    } else {
        int D = 0;
        if (n == 1) {
            return mat[0][0];
        }

        int temp[N][N];
        int sign = 1;

        for (int f = 0; f < n; f++) {
            getCofactor(mat, temp, 0, f, n);
            D += sign * mat[0][f]
                 * determinantOfMatrix(temp, n - 1, argc, argv);

            sign = -sign;
        }

        return D;
    }

    MPI_Finalize();

}

/* function for displaying the matrix */
void display(int mat[N][N], int row, int col)
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
            printf("  %d", mat[i][j]);
        printf("n");
    }
}

int main(int argc, char **argv) {
    int mat[N][N] = { { 1, 20, -18 },
                      { 3, 8, 21 },
                      { 9, 34, 1 },};

    return 0;
}
