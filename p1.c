/**
 * Autores:
 * Mario Rial Romero        LOGIN: m.rial.romero
 * Miguel Blanco Godon      LOGIN: miguel.blanco.godon
 **/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
/*****************************************************************************/
#define tamanho 5
/*****************************************************************************/
void inicializar_semilla(void);
void aleatorio(int v [], int n);
int sumaSubMax1(int v[], int t);
int sumaSubMax2(int v[], int t);
void listar_vector(int v[], int n);
double microsegundos(void);
double medirTempos(int n, int selector, int *repetir);
void test1(void);
void test2(void);
void test3(void);
/*****************************************************************************/
int main(void){
  inicializar_semilla();
  test1();
  test2();
  test3();
  return 0;
}
/*****************************************************************************/
void inicializar_semilla(void){
  srand(time(NULL));
/* se establece la semilla de una nueva serie de enteros pseudo-aleatorios */
}
void aleatorio(int v [], int n){
  int i, m=2*n+1;
  for (i=0; i < n; i++)
  v[i] = (rand() % m) - n;
/* se generan números pseudoaleatorio entre -n y +n */
}
/*****************************************************************************/
int sumaSubMax1(int v[], int t){
	int sumaMax = 0, estaSuma, i, j;
	for (i = 0; i < t; i++){
		estaSuma = 0;
		for (j = i; j < t; j++){
			estaSuma = estaSuma + v[j];
			if (estaSuma > sumaMax) sumaMax = estaSuma;
		}
	}
	return sumaMax;
}
/*****************************************************************************/
int sumaSubMax2(int v[], int t){
	int estaSuma = 0, sumaMax = 0, j;
	for (j = 0; j < t; j++){
		estaSuma = estaSuma + v[j];
		if (estaSuma > sumaMax){
			sumaMax = estaSuma;
		} else if (estaSuma < 0) estaSuma = 0;
	}
	return sumaMax;
}
/*****************************************************************************/
void listar_vector(int v[], int n){
  int i;
  printf("[");
  for (i = 0; i < n; i++){
    printf("%3d", v[i]);
  }
  printf(" ]");
}
/******************************************************************************/
double microsegundos(void) { /* obtiene la hora del sistema en microsegundos */
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}
/*****************************************************************************/
double medirTempos(int n, int selector, int *repetir){
    int v[n], i = 0, k = 1000;
    double tantes, tdespois, tempo, tmp1, tmp2;
    aleatorio(v, n);
    tantes = microsegundos();
    if (selector == 0){
       sumaSubMax1(v,n); 
    } else {
        sumaSubMax2(v,n);
    }
    tdespois = microsegundos();
    tempo = tdespois - tantes;
    if (tempo < 500.0) {   /* " Umbral de confianza " */
        (*repetir) = 1;
        tantes = microsegundos();
        do {
            aleatorio(v, n);
            if (selector == 0){
                sumaSubMax1(v,n); 
            } else {
                sumaSubMax2(v,n);
            }
            i++;
        } while (i < k);
        tdespois = microsegundos();
        tmp1 = tdespois - tantes;
        tantes = microsegundos();
        i = 0;
        do { /* Aqui restamos tamen o tempo das comparacions. */
            aleatorio(v,n);
            if (selector == 0) ;
            else {;}
            i++;
        }while (i < k);
        tdespois = microsegundos();
        tmp2 = tdespois - tantes;
        tempo = (tmp1 - tmp2)/k;
    }return tempo;
}
/*****************************************************************************/
void test1(void){
  int v[][tamanho] = {{-9, 2, -5, -4, 6}, {4, 0, 9, 2, 5},
   {-2, -1, -9, -7, -1}, {9, -2, 1, -7, -8}, {15, -2, -5, -4, 16}, 
   {7, -5, 6, 7, -7}};
  int a, b, i;
  printf("%40s%s\t%s\n","","sumaSubMax1","sumaSubMax2" );
  for (i = 0; i < 6; i++){
    a = sumaSubMax1(v[i], tamanho);
    b = sumaSubMax2(v[i], tamanho);
    listar_vector(v[i], tamanho);
    printf("%30d%16d\n", a, b);
  }
}
/*****************************************************************************/
void test2(void){
  int n = 9, v[n], a, b, i;
  printf("\n");
  printf("%40s%s\t%s\n","","sumaSubMax1","sumaSubMax2" );
  for (i = 0; i < 10; i++){
    aleatorio(v,n);
    listar_vector(v, n);
    a = sumaSubMax1(v, n);
    b = sumaSubMax2(v, n);
    printf("%18d%16d\n", a, b);
  }
}
/*****************************************************************************/
void test3(void){
    int n;
    double t;
    int repetir = 0;
    printf("\n");
    printf("SumaSubMax1 \n");
    printf("%10s%20s%15s%15s%15s\n","n", "t(n)", "t(n)/n^1.8", "t(n)/n^2", 
    "t(n)/n^2.2");
    for (n = 500; n <= 32000; n *= 2){
        t = medirTempos(n,0,&repetir);
        if (repetir == 0){
          printf("%10d%20f%15f%15f%15f\n",n, t, t / pow(n, 1.8), t / pow(n,2),
           t / pow(n, 2.2));
        } else printf("%s%7d%20f%15f%15f%15f\n", "(*)",n, t, t / pow(n, 1.8),
         t / pow(n,2), t / pow(n, 2.2));
    }
    printf("\n");
    printf("SumaSubMax2 \n");
    printf("%10s%20s%15s%15s%15s\n","n", "t(n)", "t(n)/n^0.8", "t(n)/n",
     "t(n)/n^1.2");
    for (n = 500; n <= 32000; n *= 2){
        t = medirTempos(n,1, &repetir);
        if (repetir == 0){
          printf("%10d%20f%15f%15f%15f\n",n, t, t / pow(n, 0.8), t / pow(n,1),
           t / pow(n, 1.2));
        } else printf("%s%7d%20f%15f%15f%15f\n", "(*)",n, t, t / pow(n, 0.8),
         t / pow(n,1), t / pow(n, 1.2));
    } 
    printf("\nNo noso caso particular unha cota mais axustada e: \n");
    printf("SumaSubMax2 \n");
    printf("%10s%20s%15s%15s%15s\n","n", "t(n)", "t(n)/n^0.75", "t(n)/n^0.95",
     "t(n)/n^1.15");
    for (n = 500; n <= 32000; n *= 2){
        t = medirTempos(n,1, &repetir);
        if (repetir == 0){
          printf("%10d%20f%15f%15f%15f\n",n, t, t / pow(n, 0.75),
 			t / pow(n,0.95), t / pow(n, 1.15));
        } else printf("%s%7d%20f%15f%15f%15f\n", "(*)",n, t, t / pow(n, 0.75),
         t / pow(n,0.95), t / pow(n, 1.15));
    }
}
/*****************************************************************************/
