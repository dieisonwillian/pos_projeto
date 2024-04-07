#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono;

int prime_number(int n) {
    int total = 0;
    for (int i = 2; i <= n; i++) {
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

int main() {
    int n;
    cout << "Enter a number: ";
    cin >> n;

    auto start = high_resolution_clock::now(); // Inicia a contagem de tempo

    int primeCount = prime_number(n);

    auto stop = high_resolution_clock::now(); // Para a contagem de tempo
    auto duration = duration_cast<microseconds>(stop - start); // Calcula a duração em microssegundos

    cout << "Number of prime numbers up to " << n << ": " << primeCount << endl;
    cout << "Time taken for calculation: " << duration.count() << " microseconds" << endl;

    return 0;
}
