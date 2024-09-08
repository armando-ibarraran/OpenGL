#include <stdio.h>
#include <stdlib.h>

#define MAX_STRING_SIZE 50  // Para estandarizar la longitud de los strings

// Estructuras usadas para almacenar la info
typedef struct {
    char calle[MAX_STRING_SIZE];
    int numero_externo;
    char codigo_postal[MAX_STRING_SIZE];
    char ciudad[MAX_STRING_SIZE];
    char pais[MAX_STRING_SIZE];
} Direccion;

typedef struct Empleado {
    char nombres[MAX_STRING_SIZE];
    char apellidos[MAX_STRING_SIZE];
    int numero_ingreso;
    Direccion direccion;
    float salario;
    char genero;
    char estado_civil;
    int numero_hijos;
    struct Empleado* siguiente;
} Empleado;

// Definición temporal de funciones
Empleado* crear_empleado();
int insertar_empleado(Empleado** lista);
int borrar_empleado(Empleado** lista, int numero_ingreso);
int num_empleados(Empleado* lista);
Empleado* buscar_empleado(Empleado* lista, int numero_ingreso);
void mostrar_empleado(Empleado* empleado);
void los_empleados(Empleado* lista);

int main() {
    Empleado* lista = NULL; // Inicializa la lista de empleados como vacía.
    int opcion, numero_ingreso;

    // Bucle principal del programa para gestionar las opciones del usuario.
    do {
        printf("\nPrograma de nómina\n");
        printf("1. Insertar empleado\n");
        printf("2. Borrar empleado\n");
        printf("3. Mostrar un empleado\n");
        printf("4. Mostrar todos los empleados\n");
        printf("5. Número de empleados\n");
        printf("6. Salir\n");
        printf("Seleccione una opción: ");
        scanf_s("%d", &opcion);

        printf("\n");

        switch (opcion) {
        case 1: {
            if (insertar_empleado(&lista)) {
                printf("Empleado insertado correctamente.\n");
            }
            else {
                printf("Error al insertar empleado.\n");
            }
            break;
        }
        case 2: {
            printf("Ingrese el número de ingreso del empleado a borrar: ");
            scanf_s("%d", &numero_ingreso);
            if (borrar_empleado(&lista, numero_ingreso)) {
                printf("Empleado borrado correctamente.\n");
            }
            else {
                printf("Empleado no encontrado.\n");
            }
            break;
        }
        case 3: {
            printf("Ingrese el número de ingreso del empleado a mostrar: ");
            scanf_s("%d", &numero_ingreso);
            Empleado* emp = buscar_empleado(lista, numero_ingreso);
            if (emp != NULL) {
                mostrar_empleado(emp);
            }
            else {
                printf("Empleado no encontrado.\n");
            }
            break;
        }
        case 4: {
            los_empleados(lista); // Muestra la lista de todos los empleados.
            break;
        }
        case 5: {
            printf("Número de empleados: %d\n", num_empleados(lista)); // Muestra el número total de empleados.
            break;
        }
        case 6: {
            printf("Saliendo del programa.\n");
            break;
        }
        default: {
            printf("Opción no válida.\n");
            break;
        }
        }
    } while (opcion != 6);

    return 0;
}

/**
 * Función: crear_empleado
 * Descripción: Crea un nuevo empleado solicitando los datos al usuario y los almacena en una estructura.
 * Retorno: Un puntero al empleado recién creado o NULL si hubo un error de memoria.
 */
Empleado* crear_empleado() {
    Empleado* nuevo = (Empleado*)malloc(sizeof(Empleado)); // Asigna memoria para un nuevo empleado.
    if (nuevo == NULL) {
        printf("Error: No hay memoria suficiente para almacenar un nuevo empleado.\n");
        return NULL;
    }

    // Solicita al usuario los datos del empleado.
    printf("Ingrese los datos del empleado.\n");
    printf("Nombres: ");
    scanf_s(" %[^\n]", nuevo->nombres, (unsigned int)MAX_STRING_SIZE);
    printf("Apellidos: ");
    scanf_s(" %[^\n]", nuevo->apellidos, (unsigned int)MAX_STRING_SIZE);
    printf("Número de ingreso: ");
    scanf_s("%d", &nuevo->numero_ingreso);

    // Captura los detalles de la dirección del empleado.
    printf("Dirección: \n");
    printf("\tCalle o avenida: ");
    scanf_s(" %[^\n]", nuevo->direccion.calle, (unsigned int)MAX_STRING_SIZE);
    printf("\tNúmero externo: ");
    scanf_s("%d", &nuevo->direccion.numero_externo);
    printf("\tCódigo postal: ");
    scanf_s(" %[^\n]", nuevo->direccion.codigo_postal, (unsigned int)MAX_STRING_SIZE);
    printf("\tCiudad: ");
    scanf_s(" %[^\n]", nuevo->direccion.ciudad, (unsigned int)MAX_STRING_SIZE);
    printf("\tPaís: ");
    scanf_s(" %[^\n]", nuevo->direccion.pais, (unsigned int)MAX_STRING_SIZE);

    printf("Salario: ");
    scanf_s("%f", &nuevo->salario);
    printf("Género (M/F): ");
    scanf_s(" %c", &nuevo->genero, 1);
    printf("Estado civil (S/C/D): ");
    scanf_s(" %c", &nuevo->estado_civil, 1);
    printf("Número de hijos: ");
    scanf_s("%d", &nuevo->numero_hijos);

    nuevo->siguiente = NULL; // El nuevo empleado apunta a NULL, ya que será el último en la lista.
    return nuevo;
}

/**
 * Función: insertar_empleado
 * Descripción: Inserta un nuevo empleado en la lista enlazada de empleados en orden creciente según su número de ingreso.
 * Parámetros:
 *      - lista: Un puntero doble a la lista de empleados.
 * Retorno: Devuelve 1 si la inserción fue exitosa, 0 si hubo un error de memoria.
 */
int insertar_empleado(Empleado** lista) {
    Empleado* nuevo = crear_empleado();
    if (nuevo == NULL) {
        return 0; // Error de memoria
    }

    if (*lista == NULL || (*lista)->numero_ingreso > nuevo->numero_ingreso) {
        // Caso en que el empleado debe insertarse al inicio de la lista.
        nuevo->siguiente = *lista;
        *lista = nuevo;
    }
    else {
        Empleado* actual = *lista;
        // Itera para encontrar la posición adecuada del nuevo empleado en la lista.
        while (actual->siguiente != NULL && actual->siguiente->numero_ingreso < nuevo->numero_ingreso) {
            actual = actual->siguiente;
        }
        nuevo->siguiente = actual->siguiente;
        actual->siguiente = nuevo;
    }

    return 1;
}

/**
 * Función: borrar_empleado
 * Descripción: Elimina un empleado de la lista enlazada según su número de ingreso.
 * Parámetros:
 *      - lista: Un puntero doble a la lista de empleados.
 *      - numero_ingreso: El número de ingreso del empleado a eliminar.
 * Retorno: Devuelve 1 si el empleado fue encontrado y eliminado, 0 si no se encontró.
 */
int borrar_empleado(Empleado** lista, int numero_ingreso) {
    if (*lista == NULL) {
        return 0; // Lista vacía.
    }

    Empleado* actual = *lista;
    Empleado* anterior = NULL;

    // Itera sobre la lista para encontrar el empleado con el número de ingreso especificado.
    while (actual != NULL && actual->numero_ingreso != numero_ingreso) {
        anterior = actual;
        actual = actual->siguiente;
    }

    // Si no se encuentra el empleado.
    if (actual == NULL) {
        return 0;
    }

    // Si el empleado a eliminar es el primero en la lista.
    if (anterior == NULL) {
        *lista = actual->siguiente;
    }
    else {
        // Se salta el empleado a eliminar.
        anterior->siguiente = actual->siguiente;
    }

    free(actual); // Libera la memoria ocupada por el empleado eliminado.
    return 1;
}

/**
 * Función: num_empleados
 * Descripción: Cuenta el número total de empleados en la lista enlazada.
 * Parámetros:
 *      - lista: Un puntero a la lista de empleados.
 * Retorno: El número de empleados en la lista.
 */
int num_empleados(Empleado* lista) {
    int contador = 0;

    // Recorre la lista y cuenta cada empleado.
    while (lista != NULL) {
        contador++;
        lista = lista->siguiente;
    }

    return contador; // Devuelve el número total de empleados.
}

/**
 * Función: buscar_empleado
 * Descripción: Busca un empleado en la lista enlazada según su número de ingreso.
 * Parámetros:
 *      - lista: Un puntero a la lista de empleados.
 *      - numero_ingreso: El número de ingreso del empleado que se busca.
 * Retorno: Un puntero al empleado si se encuentra, o NULL si no se encuentra.
 */
Empleado* buscar_empleado(Empleado* lista, int numero_ingreso) {
    // Recorre la lista en busca del empleado con el número de ingreso especificado.
    while (lista != NULL) {
        if (lista->numero_ingreso == numero_ingreso) {
            return lista; // Retorna el empleado si se encuentra.
        }
        lista = lista->siguiente;
    }

    return NULL; // Retorna NULL si no se encuentra el empleado.
}

/**
 * Función: mostrar_empleado
 * Descripción: Muestra la información de un empleado específico.
 * Parámetros:
 *      - empleado: Un puntero al empleado cuya información se va a mostrar.
 */
void mostrar_empleado(Empleado* empleado) {
    if (empleado != NULL) {
        printf("\n--- Información del Empleado ---\n");
        printf("Nombres: %s\n", empleado->nombres);
        printf("Apellidos: %s\n", empleado->apellidos);
        printf("Número de ingreso: %d\n", empleado->numero_ingreso);
        printf("Dirección: %s, %d, %s, %s, %s\n", empleado->direccion.calle, empleado->direccion.numero_externo,
            empleado->direccion.codigo_postal, empleado->direccion.ciudad, empleado->direccion.pais);
        printf("Salario: %.2f\n", empleado->salario);
        printf("Género: %c\n", empleado->genero);
        printf("Estado civil: %c\n", empleado->estado_civil);
        printf("Número de hijos: %d\n", empleado->numero_hijos);
    }
}

/**
 * Función: los_empleados
 * Descripción: Muestra la información de todos los empleados en la lista enlazada.
 * Parámetros:
 *      - lista: Un puntero a la lista de empleados.
 */
void los_empleados(Empleado* lista) {
    if (lista == NULL) {
        printf("No hay empleados en la lista.\n");
    }
    else {
        // Recorre la lista mostrando cada empleado.
        while (lista != NULL) {
            mostrar_empleado(lista);
            lista = lista->siguiente;
        }
    }
}
