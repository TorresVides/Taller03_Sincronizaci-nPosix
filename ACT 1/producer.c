/***************************************************************************************************************
*Autores: Santiago Hernandez Morales, Jose Jesus Cepeda Vargas, Karol Dayan Torres Vides,Andres Eudardo Meneses Rincon.
*Fecha: 14/11/2025
*Materia: Sistemas Operativos.
*Descripcion: Este programa modela el proceso productor utilizando semaforos con nombre y memoria compartida POSIX.
*Donde el productor agrega elementos a un buffer y sincroniza el acceso utilizando semaforos
**********************************************************************************************************************************/

//se delcara las bibliotecas que se van a utilizar en el programa
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>      // O_CREAT, O_RDWR
#include <sys/mman.h>   // shm_open, mmap
#include <semaphore.h>  // sem_open, sem_wait, sem_post

//se define una constante la cual representa el tamaño del buffer
#define BUFFER 20

typedef struct{ //se genera una estructura la cual define la memoria compartida
    int entrada;//representa el indice donde el productor va a escribir el siguiente elemento
    int bus[BUFFER]; //representa el buffer donde se van a almacenar los elementos
}compartir_datos; //representa el nombre de la estrctura

int main() {//se declara la funcion principal donde se va ajecutar el programa
    sem_t *vacio = sem_open("/vacio", O_CREAT, 0644, BUFFER); //se abre un semaforo vacio el cual va a representar la cantidad de espacios disponibles en el buffer el cual tiene un valor inicial
    sem_t *lleno = sem_open("/lleno", O_CREAT, 0644, 0); //se abre un semaforo lleno con un valor inicial de 0
    if (vacio == SEM_FAILED || lleno == SEM_FAILED) {//se valida que los semaforos se abran correctamente si no es asi se genera un error
        perror("sem_open");//se indica el error si se cumple la anterior condicion
        exit(EXIT_FAILURE);//se sale del programa debido a que los semaforos no se abrieron correctamente
    }

    int shm_fd = shm_open("/memoria_compartida", O_CREAT | O_RDWR, 0644);//se abre la memoria compartida la cual tiene permisos de lectura y escritura
    if (shm_fd < 0) {//se valida la apertura de la memoria compartida y si no se abre correctamente entonces se genera un error.
        perror("shm_open");//se indica el error si se cumple la anterior condicion
        exit(EXIT_FAILURE);//se sale del programa debido a que la memoria compartida no se abrio de forma correcta
    }
    ftruncate(shm_fd, sizeof(compartir_datos));//se ajusta el tamaño de la memoria compartida con el proposito de almacenar la estructura con nombre de compartir_datos.

    compartir_datos *compartir = mmap(NULL, sizeof(compartir_datos), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);//se mapea la memoria compartida
    compartir->entrada = 0;//se inicializa la variable de entrada en 0

    for (int i = 1; i <= 10; i++) { //se genera un ciclo for con el proposito de producir 10 elementos
        sem_wait(vacio);//se espera a que el semaforo este disponible
        compartir->bus[compartir->entrada] = i;//se escribe el elemento en el buffer en la posicion de entrada
        printf("Productor: Produce%d\n", i); //se imprime el elemento que se acaba de producir
        compartir->entrada = (compartir->entrada+1) % BUFFER;//se avanza el indice de entrada
        sem_post(lleno);//se indica que el buffer ya tiene un elemento adicional
        sleep(1);  //se espera un segundo con el proposito de simular el tiempo de produccion
    }

    munmap(compartir, sizeof(compartir_datos));//se deja de mapear la memoria compartida
    close(shm_fd);//se cierra la memoria compartida
    sem_close(vacio);//se cierra el semaforo vacio
  sem_unlink("/vacio");//se elimina el semaforo vacio
    shm_unlink("/memoria_compartida");//se elimina la memoria compartida
    return 0;//se retorna 0 con el proposito de indicar que el programa se ejecuto correctamente
}