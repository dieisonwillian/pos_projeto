#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


#define TAG_TASK 1
#define TAG_RESULT 2

int numero_primo(int start, int end){
    int total = 0;
    for (int i = start; i <= end; i++) {
        int prime = 1;
        for (int j = 2; j < i; j++) {
            if (i % j == 0) {
                prime = 0;
                break;
            }
        }
        total += prime;
    }
    return total;
}


void emissor(int rank, int total_workers, int n) {
    int chunk_size = n / total_workers;
    int start = 2;
    for (int i = 1; i <= total_workers; i++) {
        int end = ( i == total_workers) ? n : start + chunk_size - 1;
        int task[2] = {start, end};
        MPI_Send(task, 2, MPI_INT, i, TAG_TASK, MPI_COMM_WORLD);
        printf("emissor (rank %d): Envia tarefa para o trabalhador %d\n", rank, i);
        start = end + 1;
    }
}

void worker(int rank) {
    int task[2];
    MPI_Recv(task, 2, MPI_INT, 0, TAG_TASK, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Trabalhador %d (rank %d): Recebeu tarefa: start=%d, end=%d\n", rank, rank, task[0], task[1]);

    double start_time = MPI_Wtime();
    int local_count = numero_primo(task[0], task[1]);
    double end_time = MPI_Wtime();

    double elapsed_time = end_time - start_time;

    MPI_Send(&local_count, 1, MPI_INT, 0, TAG_RESULT, MPI_COMM_WORLD);
    MPI_Send(&elapsed_time, 1, MPI_DOUBLE, 0, TAG_RESULT, MPI_COMM_WORLD);

    printf("Trabalhador %d (rank %d): Envia resultado para o coletor\n", rank, rank);
}

void coletor(int rank, int total_workers) {
    int global_count = 0;
    double max_time = 0;
    int prime_count = 0;

printf("Coletor (rank %d): Numeros primos:\n", rank);

    for (int i = 1; i <= total_workers; i++) {
        int local_count;
        MPI_Recv(&local_count, 1, MPI_INT, i, TAG_RESULT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&elapsed_time, 1, MPI_DOUBLE, i, TAG_RESULT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Coletor (rank %d): Recebe resultado do trabalhador %d: %d\n", rank, i, local_count);
        global_count +=  local_count;
        if (elapsed_time > max_time) {
            max_time = elapsed_time;
        }
        
        prime_count += local_count;

    }

    printf("Coletor (rank %d): Total Numero Primo: %d\n", rank, global_count);
    printf("Collector (rank %d): Max elapsed time among workers: %.6f seconds\n", rank, max_time);
}


int main(int argc, char *argv[]) {
    int size, rank;
    int n = 10000000; //Número limite para numeros primos

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    if (rank == 0) {
        if (size < 2) {
            fprintf(stderr, "Este programa requer pelo menos 2 processos MPI.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        emissor(rank, size - 1, n);
    } else {
        worker(rank);
        if (rank == size - 1) {
            coletor(rank, size - 1);
        }
    }

    end_time = MPI_Wtime();

    if (rank == 0) {
        printf("Tempo total de execução: %.6f segundos\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}