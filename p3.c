/****************************************************************************/
/*
 * AUTORES:
 * Mario Rial Romero  (m.rial.romero).
 * Miguel Blanco Godon  (miguel.blanco.godon).
 */ 
/****************************************************************************/
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>
/****************************************************************************/
#define TAM 256000 /* Tamanho maximo dos monticulos. */
#define MIN_VECTOR 500 /* Minimo numero de elementos dos vectores. */
typedef struct {
	int vector[TAM];
	int ultimo;
} monticulo;
/****************************************************************************/
void inicializar_semilla(void);
void aleatorio(int v [], int n);
void ascendente (int v [], int n);
void descendente (int v [], int n);
double microsegundos(void);
double medir_tempos (void (*init_vector) (int v[], int n),
					 int tamano, int* repetir);
double medir_creacion (void (*init_vector) (int v[], int n), int tamano,
					   int *repetir);
int ordenado (int v [], int n); 
void intercambiar (int* a, int* b);
void inicializar_Monticulo (monticulo *m);
int monticulo_vacio (const monticulo *m);
void flotar (monticulo *m, int i);
void insertar (int x, monticulo *m);
void hundir (monticulo *m, int i);
void crearMonticulo (int v [], int n, monticulo *m);
int consultarMenor (const monticulo *m);
void quitarMenor (monticulo *m);
void ordMonticulo (int v [], int n);
void test(void);
void medir_comp_crearMonticulos(void);
void medir_comp_ordMonticulos(void);
/****************************************************************************/
int main (void) {
	inicializar_semilla();
	test(); /* Valida que o algoritmo funcione correctamente. */
	/* Mide os tempos de execucion de crearMonticulo. */
	medir_comp_crearMonticulos(); 
	medir_comp_crearMonticulos();
	/* Mide os tempos de execucion de ordMonticulo. */
	medir_comp_ordMonticulos(); 
	return 0;
}
/*****************************************************************************/
void inicializar_semilla(void) {
    srand(time(NULL));
}
/*****************************************************************************/
void aleatorio(int v [], int n) {  
    /* se generan numeros pseudoaleatorio entre -n e n*/
    int i , m = 2 * n + 1;
    for (i = 0; i < n; i++)
        v[i] = (rand() % m) -n;
}
/*****************************************************************************/
void ascendente (int v [], int n) {
	/* Xerase un vector ascendente entre 0 e n-1.*/
    int i;
    for (i = 0; i < n; i++)
        v[i] = i;
}
/*****************************************************************************/
void descendente (int v [], int n) {
	/* Xerase un vector descendente entre n-1 e 0.*/
    int i;
    n = n - 1;
    for (i = n; i > -1; i--)
        v[n- i] = i;
}
/*****************************************************************************/
double microsegundos(void) { 
    /* obtiene la hora del sistema en microsegundos */
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}
/*****************************************************************************/
double medir_tempos (void (*init_vector) (int v[], int n),
					 int tamano, int* repetir) {
	double t_final = 0, t_antes = 0, t_despois = 0, tr_antes = 0, tr_desp = 0;
	int k = 1000, i = 0, v[tamano];
	*repetir = 0;
	(*init_vector) (v, tamano);
	t_antes = microsegundos();
	ordMonticulo(v, tamano);
	t_despois = microsegundos();
	t_final = t_despois - t_antes;
	if (t_final < 500) { /* Umbral de confianza. */
		*repetir = 1;
		t_antes = microsegundos();
		do {
			(*init_vector) (v, tamano);
			ordMonticulo(v, tamano);
		} while (i++ < k);
		t_despois = microsegundos();
		tr_antes = t_despois - t_antes;
		i = 0;
		t_antes = microsegundos();
		do  {
			(*init_vector) (v, tamano);
			i++;
		} while (i < k);
		t_despois = microsegundos();
		tr_desp = t_despois - t_antes;
		t_final = (tr_antes - tr_desp) / k;
	}
	return t_final;
}
/*****************************************************************************/
double medir_creacion (void (*init_vector) (int v[], int n), int tamano, 
					   int *repetir) {
	double t_final = 0 , t_antes = 0, t_despois = 0, t1 = 0, t2 = 0;
	int k = 1000, i = 0, v[tamano];
	monticulo *M = malloc(sizeof(monticulo));
	if (M != NULL) {
		*repetir = 0;
		(*init_vector) (v, tamano);
		t_antes = microsegundos();
		crearMonticulo(v, tamano, M);
		t_despois = microsegundos();
		t_final = t_despois - t_antes;
		if (t_final < 500) { /* Umbral de confianza. */
			*repetir = 1;
			t_antes = microsegundos();
			do {
				(*init_vector) (v, tamano);
				crearMonticulo(v, tamano, M);
			} while (i++ < k);
			t_despois = microsegundos();
			t1 = t_despois - t_antes;
			i = 0;
			t_antes = microsegundos();
			do {
				(*init_vector) (v, tamano);
				i++;
			} while (i< k);
			t_despois = microsegundos();
			t2 = t_despois - t_antes;
			t_final = (t1 - t2) / k;
		}
		free(M);
	} else {
		errno = ENOMEM; /* Cannot allocate memory. */
		perror("Imposible crear monticulo");
	}
	return t_final;
}
/*****************************************************************************/
int ordenado (int v [], int n) {
	/* Devolve 1 se o vector esta ordenado ascendentemente. */
    int i, resultado = 1;
    for (i = 0; i < n - 1; i++) {
        if (v[i + 1] < v [i]){
            resultado = 0;
        }
    }
    return resultado;
}
/****************************************************************************/
void intercambiar (int* a, int* b) {
	/* Intercambia dous valores enteiros recibidos por referencia. */
	int aux = *a;
	*a = *b;
	*b = aux;
}
/****************************************************************************/
void inicializar_Monticulo (monticulo *m) {
	/* Marca o monticulo como baleiro. */
	m->ultimo = -1;
}
/****************************************************************************/
int monticulo_vacio (const monticulo *m) {
	/* Comproba se o monticulo esta baleiro. */
	int x = 1;
	if (m->ultimo == -1) {
		x = 0;
	}
	return x;
}
/****************************************************************************/
void flotar (monticulo *m, int i) {
	/* Sube un elemento ata a posicion que lle corresponde. */
	while ((i > 0) && (m->vector[i / 2] > m->vector[i])) {
		intercambiar(&(m->vector[i / 2]), &(m->vector[i]));
		i = i / 2;
	}
}
/****************************************************************************/
void insertar (int x, monticulo *m) {
	/* Inserta un elemento no monticulo. */
	if (m->ultimo == TAM) {
		errno = 11; /* Resource temporarily unavailable. */
		perror("Monticulo completo: ");
	} else {
		m->ultimo += 1;
		m->vector[m->ultimo] = x;
		flotar(m, m->ultimo);
	}
}
/****************************************************************************/
void hundir (monticulo *m, int i) {
	/* Baixa un elemento ata a posicion que lle corresponde. */
	int HijoIzq, HijoDer, j;
	do {
		HijoIzq = 2 * i + 1;
		HijoDer = 2 * i + 2;
		j = i;
		if ((HijoDer <= m->ultimo) && (m->vector[HijoDer] < m->vector[i])) {
			i = HijoDer;
		}
		if ((HijoIzq <= m->ultimo) && (m->vector[HijoIzq] < m->vector[i])) {
			i = HijoIzq;
		}
		intercambiar(&(m->vector[j]), &(m->vector[i]));
	} while (i != j);
}
/****************************************************************************/
void quitarMenor (monticulo* m) {
	/* Quita o menor (a raiz) do monticulo. */
	if (monticulo_vacio(m) == 0) {
		errno = 1;
		perror("Monticulo vacio");
	} else {
		m->vector[0] = m->vector[m->ultimo];
		m->ultimo -= 1;
		if (m->ultimo >= 0) {
			hundir (m, 0);
		}
	}
}
/****************************************************************************/
int consultarMenor (const monticulo *m) {
	/* Devolve o menor (a raiz) do monticulo. */
	int x;
	if (monticulo_vacio(m) == 0) {
		errno = 1;
		perror("Monticulo vacio");
	} else {
		x = m->vector[0];
	}
	return x;
}
/****************************************************************************/
void crearMonticulo (int v [], int n, monticulo *m) {
	/* Copia un vector insertandoo nun monticulo. */
	int i;
	inicializar_Monticulo(m);
	for (i = 0; i < n; i++) {
		insertar(v[i], m);
	}
	for (i = m->ultimo / 2; i >= 0; i--) {
		hundir(m, i);
	}
}
/****************************************************************************/
void ordMonticulo (int v [], int n) {
	/* Ordena un monticulo de xeito ascendente. */
	int i;
	monticulo* M = malloc(sizeof(monticulo));
	if (M != NULL) {
		crearMonticulo(v, n ,M);
		for (i = 0; i < n; i++) {
			v[i] = consultarMenor(M);
			quitarMenor(M);
		}
		free(M);
	} else {
		errno = ENOMEM; /* Cannot allocate memory. */
		perror("Imposible crear monticulo");
	}
}
/****************************************************************************/
void test(void) {
	/* Verifica a eficacia das funcions anteriores. */
	int i, j, n = 10, n2 = 20, v[n], w[n], a[n2];
	ascendente(v, n);
	descendente(w, n);
	printf("Vector ascendente:\n ");
	for (i = 0; i < n; i++) {
		printf("%d ", v[i]);
	}
	printf("\nOrdenado? %d\n", ordenado(v, n));
	ordMonticulo(v, n);
	for (i = 0; i < n; i++) {
		printf("%d ", v[i]);
	}
	printf("\nOrdenado? %d\n", ordenado(v, n));
	printf("Vector descendente:\n ");
	for (i = 0; i < n; i++) {
		printf("%d ", w[i]);
	}
	printf("\nOrdenado? %d\n", ordenado(w, n));
	ordMonticulo(w, n);
	for (i = 0; i < n; i++) {
		printf("%d ", w[i]);
	}
	printf("\nOrdenado? %d\n", ordenado(w, n));
	
	for (i = 0; i < 10; i++) {
		printf("Vector %d:\n", i + 1);
		aleatorio(a, n2);
		for (j = 0; j < n2; j++) {
			printf("%d ", a[j]);
		} printf("\nOrdenado? %d\n", ordenado(a, n2));
		ordMonticulo(a, n2);
		for (j = 0; j < n2; j++) {
			printf("%d ", a[j]);
		} printf("\nOrdenado? %d\n", ordenado(a, n2));
	}
}
/****************************************************************************/
void medir_comp_crearMonticulos(void) {
	/* Comprobase que a creacion de monticulos sexa O(n) */
	double t;
	int n, repetir;
	printf("\nProcedemento de creación de monticulos de vectores %s"
			,"aleatorios: \n");
	printf("%10s%20s%15s%15s%15s\n","n", "t(n)", 
            "t(n)/n^0.8", "t(n)/n", "t(n)/n^1.2");
	for (n = MIN_VECTOR; n <= TAM; n*=2) {
		t = medir_creacion(aleatorio, n, &repetir);
        if (repetir == 0){
          printf("%10d%20.8f%15.8f%15.8f%15.8f\n",n, t, t / pow(n, 0.8),
                t / pow(n,1), t / pow(n, 1.2));
        } else printf("%s%7d%20.8f%15.8f%15.8f%15.8f\n", "(*)",n, t,
                        t / pow(n, 0.8), t / pow(n,1), t / pow(n, 1.2));
	}
}
/****************************************************************************/
void medir_comp_ordMonticulos(void) {
	double t;
	int n, repetir;
	printf("\nOrdenacion de monticulos vectores aleatorios: \n");
	printf("%10s%20s%15s%15s%15s\n","n", "t(n)", 
            "t(n)/n", "t(n)/(n*ln n)", "t(n)/n^1.3");
	for (n = MIN_VECTOR; n <= TAM; n*=2) {
		t = medir_tempos(aleatorio, n, &repetir);
        if (repetir == 0){
          printf("%10d%20.8f%15.8f%15.8f%15.8f\n",n, t, t / pow(n, 1),
                t / (n*log(n)), t / pow(n, 1.3));
        } else printf("%s%7d%20.8f%15.8f%15.8f%15.8f\n", "(*)",n, t,
                        t / pow(n, 1), t / (n*log(n)), t / pow(n, 1.3));
	}
	printf("\nOrdenacion de monticulos vectores ordenados: \n");
	printf("%10s%20s%15s%15s%15s\n","n", "t(n)", 
            "t(n)/n", "t(n)/(n*ln n)", "t(n)/n^1.3");
	for (n = MIN_VECTOR; n <= TAM; n*=2) {
		t = medir_tempos(ascendente, n, &repetir);
        if (repetir == 0){
          printf("%10d%20.8f%15.8f%15.8f%15.8f\n",n, t, t / pow(n, 1),
                t / (n*log(n)), t / pow(n, 1.3));
        } else printf("%s%7d%20.8f%15.8f%15.8f%15.8f\n", "(*)",n, t,
                        t / pow(n, 1), t / (n*log(n)), t / pow(n, 1.3));
	}
	printf("\nOrdenacion de monticulos vectores descendentes: \n");
	printf("%10s%20s%15s%15s%15s\n","n", "t(n)", 
            "t(n)/n", "t(n)/(n*ln n)", "t(n)/n^1.3");
	for (n = MIN_VECTOR; n <= TAM; n*=2) {
		t = medir_tempos(descendente, n, &repetir);
        if (repetir == 0){
          printf("%10d%20.8f%15.8f%15.8f%15.8f\n",n, t, t / pow(n, 1),
                t / (n*log(n)), t / pow(n, 1.3));
        } else printf("%s%7d%20.8f%15.8f%15.8f%15.8f\n", "(*)",n, t,
                        t / pow(n, 1), t /(n*log(n)), t / pow(n, 1.3));
	}
}
