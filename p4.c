/*
 * AUTORES:
 * Mario Rial Romero  (m.rial.romero). DNI: 58004451S
 * Miguel Blanco Godon  (miguel.blanco.godon). DNI: 78809752E
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>	
#include <limits.h>
#include <math.h>
#define TAM_MAX 1000

typedef int ** matriz;
void inicializar_semilla(void);
double microsegundos(void);
matriz crearMatriz (int n);
void iniMatriz(matriz m, int n);
void liberarMatriz (matriz m, int n);
void dijkstra (matriz grafo, matriz distancias, int tam);
void imprimir_matriz (matriz m, int tam);
void test (void);
void medir_complexidade_dijkstra (void);
double medirTempos (int tam, int* repetir);


int main (void) {
	test();
	medir_complexidade_dijkstra();
	medir_complexidade_dijkstra();
	medir_complexidade_dijkstra();
	return 0;
}

/*****************************************************************************/
void inicializar_semilla(void) {
    srand(time(NULL));
}
/*****************************************************************************/
double microsegundos(void) { 
    /* obtiene la hora del sistema en microsegundos */
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}
/****************************************************************************/
matriz crearMatriz (int n) {
	int i;
	matriz aux;
	if ((aux = malloc(n * sizeof(int *))) == NULL)
		return NULL;
	for (i = 0; i < n; i++) 
		if ((aux[i] = malloc(n * sizeof(int))) == NULL)
			return NULL;
	return aux;
}
/****************************************************************************/
/* Inicializacion pseudoalatoria [1..TAM_MAX] de un grafo completo
no dirigido con n nodos, representado por su matriz de adyacencia. */
void iniMatriz(matriz m, int n) {
	int i, j;
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++) 
			m[i][j] = rand() % TAM_MAX + 1;
	for (i = 0; i < n; i++) 
		for (j = 0; j <= i; j++)
			if (i == j)
				m[i][j] = 0;
			else 
				m[i][j] = m [j][i];
}
/****************************************************************************/
void liberarMatriz (matriz m, int n) {
	int i;
	for (i = 0; i < n; i++)
		free(m[i]);
	free(m);
}
/****************************************************************************/
void dijkstra (matriz grafo, matriz distancias, int tam) {
	int n, i, j, min, v = 0, w;
	int* noVisitados = malloc(tam * sizeof(int)); /* vector booleanos */
	for (n = 0; n < tam; n++) {
		for (i = 0; i < tam; i++) {
			noVisitados[i] = 1;
			distancias[n][i] = grafo[n][i];
		}
		noVisitados[n] = 0;
		j = 0;
		do {
			min = INT_MAX;
			for(i = 0; i < tam; i++) {
				if (noVisitados[i] == 1 && min > distancias[n][i]) {
					min = distancias[n][i];
					v = i;
				}
			}
			noVisitados[v] = 0;
			for (w = 0; w < tam; w++) {
				if (distancias[n][w] > distancias[n][v] + grafo[v][w]) 
					distancias[n][w] = distancias[n][v] + grafo[v][w];
			}
			j++;
		} while (j < tam-1);
	}
	free(noVisitados);
}
/****************************************************************************/
void imprimir_matriz (matriz m, int tam) {
	int i, j;
	for (i = 0; i < tam; i++) {
		for (j = 0; j < tam; j++) 
			printf("%d ", m[i][j]);
		printf("\n");
	}
}
/****************************************************************************/
void test (void) {
	matriz M = crearMatriz(5); 
	matriz D = crearMatriz(5);
	matriz M2 = crearMatriz(4);
	matriz D2 = crearMatriz(4);

	M[0][0] = 0; M[0][1] = 1; M[0][2] = 8; M[0][3] = 4; M[0][4] = 7;
	M[1][0] = 1; M[1][1] = 0; M[1][2] = 2; M[1][3] = 6; M[1][4] = 5;
	M[2][0] = 8; M[2][1] = 2; M[2][2] = 0; M[2][3] = 9; M[2][4] = 5;
	M[3][0] = 4; M[3][1] = 6; M[3][2] = 9; M[3][3] = 0; M[3][4] = 3;
	M[4][0] = 7; M[4][1] = 5; M[4][2] = 5; M[4][3] = 3; M[4][4] = 0;

	M2[0][0] = 0; M2[0][1] = 1; M2[0][2] = 4; M2[0][3] = 7;
	M2[1][0] = 1; M2[1][1] = 0; M2[1][2] = 2; M2[1][3] = 8;
	M2[2][0] = 4; M2[2][1] = 2; M2[2][2] = 0; M2[2][3] = 3;
	M2[3][0] = 7; M2[3][1] = 8; M2[3][2] = 3; M2[3][3] = 0;

	printf("Matriz de adyacencia 1\n");
	imprimir_matriz(M,5);
	dijkstra(M,D,5);
	printf("\nDistancias minimas 1\n");
	imprimir_matriz(D,5);
	liberarMatriz(M,5);
	liberarMatriz(D,5);
	printf("\nMatriz de adyacencia 2\n");
	imprimir_matriz(M2,4);
	dijkstra(M2,D2,4);
	printf("\nDistancias minimas 2\n");
	imprimir_matriz(D2,4);
	liberarMatriz(M2,4);
	liberarMatriz(D2,4);
}
/****************************************************************************/
double medirTempos (int tam, int* repetir) {
	double t_antes, t_despois, tr_antes, tr_despois, t_final,
	t1, t2, k = 1000, i = 0;
	matriz m = crearMatriz(tam);
	matriz d = crearMatriz(tam);
	*repetir = 0; 
	iniMatriz(m,tam);
	t_antes = microsegundos();
	dijkstra(m,d,tam);
	t_despois = microsegundos();
	t_final = t_despois - t_antes;
	if (t_despois-t_antes < 500) {
		*repetir = 1; 
		tr_antes = microsegundos();
		do {
			dijkstra(m,d,tam);
			i++;
		}while (i < k);
		tr_despois = microsegundos();
		t1 = tr_despois - tr_antes;
		i = 0;
		tr_antes = microsegundos();
		do {
			i++;
		} while (i < k);
		tr_despois = microsegundos();
		t2 = tr_despois - tr_antes;
		t_final = (t1 -t2)/k;
	}
	liberarMatriz(m, tam);
	liberarMatriz(d, tam);
	return t_final;
}
/****************************************************************************/
void medir_complexidade_dijkstra (void) {
	/* Comprobase que a creacion de monticulos sexa O(n) */
	double t;
	int n, repetir;
	printf("\nCalculo de camiños minimos: Algoritmo de Dijkstra: \n");
	printf("%10s%20s%15s%15s%15s\n","n", "t(n)", 
            "t(n)/n^2.75", "t(n)/n^3", "t(n)/n^3.25");
	for (n = 25; n <= 800; n*=2) {
		t = medirTempos(n, &repetir);
        if (repetir == 0){
          printf("%10d%20.8f%15.8f%15.8f%15.8f\n",n, t, t / pow(n, 2.75),
                t / pow(n,3), t / pow(n, 3.25));
        } else printf("%s%7d%20.8f%15.8f%15.8f%15.8f\n", "(*)",n, t,
                        t / pow(n, 2.75), t / pow(n,3), t / pow(n, 3.25));
	}
}
