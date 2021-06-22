#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void inicializar_semilla(void);
void aleatorio(int v [], int n);
void ascendente (int v [], int n);
void descendente (int v [], int n);
int ordenado (int v [], int n);
void ord_ins (int v[], int n);
void ord_shell (int v[], int n);
double microsegundos(void);
double medir_tempos (void (*ini_vect) (int v [], int n)
, void (*alg_ord) (int v [], int n), int tamano, int* repetir);
void test1(void (*f) (int v [], int n), int selector);
void test2 (void);
void test3 (void);
/*****************************************************************************/
int main(void) {
    inicializar_semilla();
    printf("Insercion:\n");
    test1(ord_ins, 0);
    printf("\nShell:\n");
    test1(ord_shell, 1);
    test2();
    test3();
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
    int i;
    for (i = 0; i < n; i++)
        v[i] = i;
}
/*****************************************************************************/
void descendente (int v [], int n) {
    int i;
    n = n - 1;
    for (i = n; i > -1; i--)
        v[n- i] = i;
}
/*****************************************************************************/
int ordenado (int v [], int n) {
    int i, resultado = 1;
    for (i = 0; i < n - 1; i++) {
        if (v[i + 1] < v [i]){
            resultado = 0;
        }
    }
    return resultado;
}
/*****************************************************************************/
void ord_ins (int v [], int n) {
    int i, x, j;
    for (i = 1; i < n; i++){
        x = v[i];
        j = i - 1;
        while ((j >= 0) && (v[j] > x)) {
            v[j + 1] = v[j];
            j = j - 1;
        }
        v[j + 1] = x;
    }
}
/*****************************************************************************/
void ord_shell (int v [], int n) {
    int incremento = n - 1,seguir = 1, i, tmp, j;
    do {
        incremento = incremento / 2;
        for (i = incremento; i < n; i++) {
            tmp = v[i];
            j = i;
            seguir = 0;
            while ((j - incremento >= 0) && (seguir == 0)){
                if (tmp < v[j - incremento]){
                    v[j] = v[j - incremento];
                    j = j - incremento;
                } else {
                    seguir = 1;
                }
            }
            v[j] = tmp;
        }
    } while (incremento != 1);
}
/*****************************************************************************/
void test1(void (*f) (int v [], int n), int selector) {
    int vn = 17, ndes = 10;
    int des[ndes], v[vn], i = 0;
    aleatorio(v, vn); /* Inicializase o vector aleatorio. */
    printf("Inicializacion aleatoria\n[%d", v[0]);
    for (i = 1; i < vn; i++) /* Mostrase o vector. */
        printf(", %d", v[i]);
    printf("]\n");
    printf("ordenado? %d\n", ordenado(v, vn));
    (*f) (v, vn); /* Aplicase o algoritmo de ordenacion. */
    if (selector == 0) { /* Escolle a mensaxe dependendo do codigo. */
        printf("Ordenacion por Insercion\n[%d", v[0]);
    } else {
        printf("Ordenacion por Shell\n[%d", v[0]);
    }
    for (i = 1; i < vn; i++) /* Volvese a mostrar o vector. */
        printf(", %d", v[i]);
    printf("]\n");
    printf("ordenado? %d\n", ordenado(v, 10));
    descendente(des, ndes); /* Inicializase o vector descendente. */
    printf("Inicializacion descendente\n[%d", des[0]);
    for (i = 1; i < ndes; i++) /* Mostrase o vector. */
        printf(", %d", des[i]);
    printf("]\n");
    printf("ordenado? %d\n", ordenado(des, ndes));
    (*f)(des, ndes); /* Aplicase o algoritmo de ordenacion. */
    if (selector == 0) { /* Escolle a mensaxe dependendo do codigo.*/
        printf("Ordenacion por Insercion\n[%d", des[0]);
    } else {
        printf("Ordenacion por Shell\n[%d", des[0]);
    }
    for (i = 1; i < ndes; i++) /* Volvese a mostrar o vector. */
        printf(", %d", des[i]);
    printf("]\n");
    printf("ordenado? %d\n", ordenado(des, 10)); 
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
double medir_tempos (void (*ini_vect) (int v [], int n)
, void (*alg_ord) (int v [], int n), int tamano, int* repetir) {
    double t, t_antes, t_despois, t1, t2;
    int k = 1000, i = 0, v[tamano];
    *repetir = 0;
    (*ini_vect) (v, tamano);
    t_antes = microsegundos();
    (*alg_ord) (v, tamano);
    t_despois = microsegundos();
    t = t_despois - t_antes;
    if (t < 500) {
        *repetir = 1;
        t_antes = microsegundos();
        do {
            (*ini_vect) (v, tamano);
            (*alg_ord) (v, tamano);
            i++;
        } while (i < k);
        t_despois = microsegundos();
        t1 = t_despois - t_antes;
        t_antes = microsegundos();
        i = 0;
        do {
            (*ini_vect) (v, tamano);
            i++;
        } while (i < k);
        t_despois = microsegundos();
        t2 = t_despois - t_antes;
        t = (t1 - t2) / k;
    }
    return t;
}
/*****************************************************************************/
void test2 (void) {
    double t;
    int n, repetir = 0;
    printf("Ordenacion por insercion:\n");
    printf("Ordenacion por insercion con inicializacion ascendente:\n");
    printf("%10s%20s%15s%15s%15s\n","n", "t(n)", 
            "t(n)/n^0.8", "t(n)/n^1", "t(n)/n^1.2");
    for (n = 500; n <= 64000; n *= 2){
        t = medir_tempos(ascendente, ord_ins, n, &repetir);
        if (repetir == 0){
          printf("%10d%20.8f%15.8f%15.8f%15.8f\n",n, t, t / pow(n, 0.8),
                t / pow(n,1), t / pow(n, 1.2));
        } else printf("%s%7d%20.8f%15.8f%15.8f%15.8f\n", "(*)",n, t,
                        t / pow(n, 0.8), t / pow(n,1), t / pow(n, 1.2));
    }
    printf("Ordenacion por insercion con inicializacion aleatoria:\n");
    printf("%10s%20s%15s%15s%15s\n","n", "t(n)", "t(n)/n^1.8",
            "t(n)/n^2", "t(n)/n^2.2");
    for (n = 500; n <= 64000; n *= 2){
        t = medir_tempos(aleatorio, ord_ins, n, &repetir);
        if (repetir == 0){
          printf("%10d%20.8f%15.8f%15.8f%15.8f\n",n, t, t / pow(n, 1.8),
                    t / pow(n,2), t / pow(n, 2.2));
        } else printf("%s%7d%20.8f%15.8f%15.8f%15.8f\n", "(*)",n, t, 
                        t / pow(n, 1.8), t / pow(n,2), t / pow(n, 2.2));
    }
    printf("Ordenacion por insercion con inicializacion descendente:\n");
    printf("%10s%20s%15s%15s%15s\n","n", "t(n)", "t(n)/n^1.8",
                "t(n)/n^2", "t(n)/n^2.2");
    for (n = 500; n <= 64000; n *= 2){
        t = medir_tempos(descendente, ord_ins, n, &repetir);
        if (repetir == 0){
          printf("%10d%20.8f%15.8f%15.8f%15.8f\n",n, t, t / pow(n, 1.8),
                    t / pow(n,2), t / pow(n, 2.2));
        } else printf("%s%7d%20.8f%15.8f%15.8f%15.8f\n", "(*)",n, t,
                        t / pow(n, 1.8), t / pow(n,2), t / pow(n, 2.2));
    }
}
/*****************************************************************************/
void test3 (void) {
    double t;
    int n, repetir = 0;
    printf("Ordenacion de shell:\n");
    printf("Ordenacion de shell con inicializacion ascendente:\n");
    printf("%10s%20s%15s%15s%15s\n","n", "t(n)", "t(n)/n^1.02",
            "t(n)/n^1.12", "t(n)/n^1.22");
    for (n = 500; n <= 64000; n *= 2){
        t = medir_tempos(ascendente, ord_shell, n, &repetir);
        if (repetir == 0){
          printf("%10d%20.8f%15.8f%15.8f%15.8f\n",n, t, t / pow(n, 1.02),
                    t / pow(n,1.12), t / pow(n, 1.22));
        } else printf("%s%7d%20.8f%15.8f%15.8f%15.8f\n", "(*)",n, t, 
                    t / pow(n, 1.02), t / pow(n,1.12), t / pow(n, 1.22));
    }
    printf("Ordenacion de shell con inicializacion aleatoria:\n");
    printf("%10s%20s%15s%15s%15s\n","n", "t(n)", "t(n)/n^1.065",
                "t(n)/n^1.165", "t(n)/n^1.265");
    for (n = 500; n <= 64000; n *= 2){
        t = medir_tempos(aleatorio, ord_shell, n, &repetir);
        if (repetir == 0){
          printf("%10d%20.8f%15.8f%15.8f%15.8f\n",n, t, t / pow(n, 1.065), 
                    t / pow(n,1.165), t / pow(n, 1.265));
        } else printf("%s%7d%20.8f%15.8f%15.8f%15.8f\n", "(*)",n, t,
                    t / pow(n, 1.065), t / pow(n,1.165), t / pow(n, 1.265));
    }
    printf("Ordenacion de shell con inicializacion descendente:\n");
    printf("%10s%20s%15s%15s%15s\n","n", "t(n)", "t(n)/n^1.05",
                "t(n)/n^1.15", "t(n)/n^1.25");
    for (n = 500; n <= 64000; n *= 2){
        t = medir_tempos(descendente, ord_shell, n, &repetir);
        if (repetir == 0){
          printf("%10d%20.8f%15.8f%15.8f%15.8f\n",n, t, t / pow(n, 1.05),
                    t / pow(n, 1.15), t / pow(n, 1.25));
        } else printf("%s%7d%20.8f%15.8f%15.8f%15.8f\n", "(*)",n, t,
                    t / pow(n, 1.05), t / pow(n,1.15), t / pow(n, 1.25));
    }
}