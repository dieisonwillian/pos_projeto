/*
Testes!!

mpirun -clear 6 --machinefile /home/mpihpc/shared/.cluster_hostfile ./projeto
mpic++ -O3 -std=c++1y -Wall projeto01.cpp -o projeto
*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define WORK 1
#define RESULT 2


void emissor(int rank, int total_workers){
    int data = 42;
    for (int i = 1; i <= total_workers; i++){
        if (i != rank){
            MPI_Send(&data, 1, MPI_INT, i, WORK, MPI_COMM_WORLD);
            printf("Emissor (rank %d): envia trabalho para o Worker 1\n", rank, i); 
            }
        }
    }

void worker(int rank) {
    int data;
    MPI_Recv(&data, 1, MPI_INT, 0, WORK, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Worker %d (rank %d): recebe data do emissor\n", rank, rank, data);

    // trabalho que pode executar ex:
    data *= 2;

    MPI_Send(&data, 1, MPI_INT, 3, RESULT, MPI_COMM_WORLD);
    printf("worker %d (rank %d): Envia data para o Coletor\n", rank, rank);
}

void coletor(int rank, int total_workers){
    for (int i = 1; i <= total_workers; i++){
        //if (i != rank){
            int resultado;
            MPI_Recv(&resultado, 1, MPI_INT, i, RESULT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Coletor (rank %d): Recebe resultado do Worker %d: %d\n", rank, i, resultado);
       // }

    }
}

int main(int argc, char *argv[]){
    int size, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size < 2){
        fprintf(stderr, "Estre programa requere pelo menos 2 processos MPI.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    if (rank == 0){
        emissor(rank, size - 1);
    }else {
        worker(rank);
        if (rank == size - 1){
            coletor(rank, size -1);
        }
    }
    MPI_Finalize();
    return 0;
}
/*

if (myrank == EMISSOR_RANK) {
    // Codigo do emissor
}else if (myrank == EMISSOR_RANK+1)
{
    //codigo do coletor
}else{
    //codigo dos trabalhadores
}
*/