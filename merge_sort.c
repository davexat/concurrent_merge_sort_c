#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_LINE_LENGTH 30000
#define MAX_ELEMENTS 10000

typedef struct {
    int *arr;
    int left;
    int right;
} ThreadData;

void *threaded_merge_sort(void *arg);
void merge_sort(int *arr, int left, int right);
void merge(int *arr, int left, int mid, int right);

int parse_input(char *line, int *arr);
void print_help();

int main(int argc, char **argv){
    int opt, count, num_threads, chunksize;
    int console = 0;

    char *name_file = NULL;
    char buffer[MAX_LINE_LENGTH];
    int numbers[MAX_ELEMENTS];
    
    FILE *file = NULL;

    // Manejo de opciones
    while((opt = getopt(argc, argv, "hi:c")) != -1){
        switch(opt){
            case 'h':
                print_help();
                return 0;
            case 'i':
                name_file = optarg;
                break;
            case 'c':
                console = 1;
                break;
            default:
                print_help();
                return 1;
        }
    }

    if (!console && !name_file) { print_help(); return 0; }

    // Seleccion de la fuente de datos
    if(name_file){
        file = fopen(name_file, "r");
        if (!file) {
            perror("Error al abrir el archivo");
            return 1;
        }   
    }else{
        file = stdin;
        printf("Ingrese los numeros separados por coma:\n");
    }

    // Lectura de datos
    if (fgets(buffer, MAX_LINE_LENGTH, file) == NULL) {
        perror("Error al leer los datos");
        if (file != stdin) fclose(file);
        return 1;
    }

    // Parseo de datos
    count = parse_input(buffer, numbers);
    
    // Obtener cantidad de hilos automaticamente
    num_threads = sysconf(_SC_NPROCESSORS_ONLN);
    if (num_threads > count) num_threads = count;
    if (num_threads < 1) num_threads = 1;

    // Definir la cantidad de hilos a crear
    pthread_t threads[num_threads];
    // Definir las estructuras para pasar datos a los hilos
    ThreadData thread_data[num_threads];

    // Aplicar mergesort a subconjuntos del arreglo en hilos separados
    chunksize = count / num_threads;

    for(int i = 0; i < num_threads; i++){
        thread_data[i].arr = numbers;
        thread_data[i].left = i * chunksize;
        thread_data[i].right = (i == num_threads - 1) ? (count - 1) : ((i + 1) * chunksize - 1);
        pthread_create(&threads[i], NULL, threaded_merge_sort, &thread_data[i]);
    }

    // Esperar a que todos los hilos terminen
    for(int i = 0; i < num_threads; i++){
        pthread_join(threads[i], NULL);
    }

    // Merge final de los segmentos ordenados
    int segmentSize = chunksize;
    while(segmentSize < count){
        for(int left = 0; left < count; left += 2 * segmentSize){
            int mid = left + segmentSize - 1;
            int right = (left + 2 * segmentSize - 1 <= count - 1) ? (left + 2 * segmentSize - 1) : (count - 1);
            if(mid < right)
                merge(numbers, left, mid, right);
        }
        segmentSize *= 2;
    }

    // Imprimir arreglo ordenado
    printf("Arreglo ordenado:\n");
    for(int i = 0; i < count; i++){
        if (i != count - 1)
            printf("%d, ", numbers[i]);
        else
            printf("%d\n", numbers[i]);
    }

    if (file != stdin) fclose(file);
    return 0;
}

void *threaded_merge_sort(void *arg){
    ThreadData *data = (ThreadData *)arg;
    merge_sort(data->arr, data->left, data->right);
    return NULL;
}

void merge_sort(int *arr, int left, int right){
    if (left >= right) return;

    int mid = left + (right - left) / 2;
    merge_sort(arr, left, mid);
    merge_sort(arr, mid + 1, right);

    merge(arr, left, mid, right);
}

void merge(int *arr, int left, int mid, int right){
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int L[n1], R[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1)
        arr[k++] = L[i++];

    while (j < n2)
        arr[k++] = R[j++];
}

int parse_input(char *line, int *arr){
    int count = 0;
    char *token = strtok(line, ", ");

    while(token != NULL && count < MAX_ELEMENTS){
        arr[count++] = atoi(token);
        token = strtok(NULL, ", ");
    }
    
    return count;
}

void print_help(){
    printf("Uso: merge_sort [OPCIONES]... \n");
    printf("Opciones:\n");
    printf("  -h, --help          Muestra esta ayuda y termina\n");
    printf("  -i, --input FILE    Especifica el archivo de entrada\n");
    printf("  -c, --console       Lee los numeros desde la consola\n");
}