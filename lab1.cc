// #include <assert.h>
// #include <stdio.h>
// #include <math.h>

// int main(int argc, char** argv) {
// 	if (argc != 3) {
// 		fprintf(stderr, "must provide exactly 2 arguments!\n");
// 		return 1;
// 	}
// 	unsigned long long r = atoll(argv[1]);
// 	unsigned long long k = atoll(argv[2]);
// 	unsigned long long pixels = 0;
// 	for (unsigned long long x = 0; x < r; x++) {
// 		unsigned long long y = ceil(sqrtl(r*r - x*x));
// 		pixels += y;
// 		pixels %= k;
// 	}
// 	printf("%llu\n", (4 * pixels) % k);
// }
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // 取回此Group裡面的Rank ID
    MPI_Comm_size(MPI_COMM_WORLD, &size); // 賦予size 此Group process個數


    unsigned long long r = atoll(argv[1]);
    unsigned long long k = atoll(argv[2]);
    unsigned long long pixels = 0;

    // Calculate the range of 'x' values to process for each process
    unsigned long long start_x = rank * (r / size);
    unsigned long long end_x = (rank == size - 1) ? r : (rank + 1) * (r / size);

    for (unsigned long x = start_x; x < end_x; x++) {
        unsigned long long y = ceil(sqrtl(r * r - x * x));
        pixels += y;
        if(x % 30 == 0){
            pixels %= k;
        }
    }

    // Collect the 'pixels' values from all processes
    unsigned long long global_pixels = 0;
    MPI_Reduce(&pixels, &global_pixels, 1, MPI_UNSIGNED_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("%llu\n", (4 * global_pixels) % k);
    }

    MPI_Finalize();

    return 0;
}
