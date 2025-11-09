# Merge Sort Multihilo

Este proyecto implementa el algoritmo de ordenamiento Merge Sort utilizando múltiples hilos en C. Permite ordenar una lista de números leídos desde un archivo o ingresados por consola, aprovechando el paralelismo para mejorar el rendimiento en sistemas multiprocesador.

## Instalación

1. **Clona o descarga este repositorio.**
2. **Compila el programa ejecutando:**

   ```sh
   make
   ```

   Esto generará el ejecutable merge_sort en la raíz del proyecto.

## Uso

Puedes ejecutar el programa de las siguientes formas:

### Ordenar números desde un archivo

Supón que tienes un archivo llamado prueba con números separados por coma:

```
34, 7, 102, 5, 89, ...
```

Ejecuta:

```sh 
./merge_sort -i prueba
```

### Ordenar números ingresados por consola

Ejecuta:

```sh 
./merge_sort -c
```

Luego ingresa los números separados por coma cuando se solicite.

### Mostrar ayuda
```sh 
./merge_sort -h
```

## Opciones

- `-h`          Muestra la ayuda y termina.
- `-i FILE`     Especifica el archivo de entrada con los números.
- `-c`          Lee los números desde la consola.

## Requisitos

- GCC (compilador de C)
- POSIX Threads (`-pthread`)

## Archivos principales

- merge_sort.c: Código fuente principal.
- Makefile: Script de compilación.
- prueba: Ejemplo de archivo de entrada.
- merge_sort: Ejecutable generado tras la compilación.
