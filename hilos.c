#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>

#define N 1000000000 // 1'000,000'000 Número de iteraciones
#define NTHREADS 4

double areas[NTHREADS] = {0};

double f(double y) {
    return sqrt(1 - y*y); // Función sqrt(1 - y^2)
}

double calcularPi() {
    double area = 0;
    double base, altura;

    double delta = 1.0 / N; // Tamaño de cada subintervalo

    for(int i = 0; i < N; i++) {
        base = f(i * delta); // Extremo izquierdo del trapecio
        base += f((i + 1) * delta); // Extremo derecho del trapecio
        altura = delta; // Altura del trapecio

        area = area + (base * altura) / 2; // Área del trapecio
    }

    return area * 4; // Multiplicar por 4 para obtener el área del cuarto de círculo
}

void *funcion_de_hilos(void *args) {
    int n = *((int *)args);

    double area = 0;
    double base, altura;

    double delta = 1.0 / N; // Tamaño de cada subintervalo

    // n = [0,1,2,3...<NTHREADS] -> id del proceso
    // N = 1,000,000 -> Iteraciones
    // NTHREADS = 4 -> Numero de procesos

    for(int i = n; i < N; i += NTHREADS) {
        base = f(i * delta); // Extremo izquierdo del trapecio
        base += f((i + 1) * delta); // Extremo derecho del trapecio
        altura = delta; // Altura del trapecio

        area = area + (base * altura) / 2; // Área del trapecio
    }

      // Guardar en el array areas lo que salga al final
    areas[n] = area;
}

int main() {
    struct timeval start, end;
    double tiempo;

    gettimeofday(&start, NULL); // Tiempo inicial

    int ids[NTHREADS] = {0};
    pthread_t tid[NTHREADS];

    int rc;
    // Crear hilos
    for (int t = 0; t < NTHREADS; t++) {
        ids[t] = t;
        rc = pthread_create(&tid[t], NULL, funcion_de_hilos, &ids[t]);
        if (rc) {
            printf("Error: No se pudo crear el hilo %d\n", t);
            exit(-1);
        }
    }

    // Esperar a que todos los hilos terminen
    for (long t = 0; t < NTHREADS; t++) {
        pthread_join(tid[t], NULL);
    }

    double area = 0;
    for (int i = 0; i < NTHREADS; i++) {
        area += areas[i];
    }

    double pi = area * 4; 

    gettimeofday(&end, NULL); // Tiempo final

    tiempo = (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec) / 1000000; // Cálculo del tiempo transcurrido

    printf("Valor aproximado de PI: %.10f\n", pi);
    printf("Tiempo transcurrido: %.5f segundos\n", tiempo);
    
    return 0;
}