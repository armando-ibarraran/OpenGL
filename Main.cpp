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
    Empleado* lista = NULL;
    int opcion, numero_ingreso;

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
            // Primero busca el epmpleado, si existe, muéstralo
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
            los_empleados(lista);
            break;
        }
        case 5: {
            printf("Número de empleados: %d\n", num_empleados(lista));
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

Empleado* crear_empleado() {
    Empleado* nuevo = (Empleado*)malloc(sizeof(Empleado)); // Aparta espacio en memoria para este nuevo empleado
    if (nuevo == NULL) {
        printf("Error: No hay memoria suficiente para almacenar un nuevo empleado.\n");
        return NULL;
    }

    // Pide al usuario que teclee los datos
    printf("Ingrese los datos del empleado.\n");
    printf("Nombres: ");
    scanf_s(" %[^\n]", nuevo->nombres, (unsigned int)MAX_STRING_SIZE); // strings ya son apuntador, no necesitamos el ampersand
    printf("Apellidos: ");
    scanf_s(" %[^\n]", nuevo->apellidos, (unsigned int)MAX_STRING_SIZE);
    printf("Número de ingreso: ");
    scanf_s("%d", &nuevo->numero_ingreso);
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

    nuevo->siguiente = NULL;
    return nuevo;
}

int insertar_empleado(Empleado** lista) {
    Empleado* nuevo = crear_empleado();
    if (nuevo == NULL) {
        return 0; // No hubo memoria
    }

    
    if (*lista == NULL || (*lista)->numero_ingreso > nuevo->numero_ingreso) {
        // Caso en que el empleado debe or en el "head" de la lista
        nuevo->siguiente = *lista;
        *lista = nuevo;
    }
    else {
        Empleado* actual = *lista;
        // Itera hasta encontrar el lugar que le corresponde a este empleado (según su número de ingreso de mayor a menor)
        while (actual->siguiente != NULL && actual->siguiente->numero_ingreso < nuevo->numero_ingreso) {
            actual = actual->siguiente;
        }
        nuevo->siguiente = actual->siguiente;
        actual->siguiente = nuevo;
    }

    return 1;
}

int borrar_empleado(Empleado** lista, int numero_ingreso) {
    if (*lista == NULL) {
        return 0; // Lista vacía
    }

    Empleado* actual = *lista;
    Empleado* anterior = NULL;

    // Itera sobre la lista buscando al empleado
    while (actual != NULL && actual->numero_ingreso != numero_ingreso) {
        anterior = actual;
        actual = actual->siguiente;
    }

    // Caso en que se llega al fin de la lista (no se encontró el empleado)
    if (actual == NULL) {
        return 0;
    }

    if (anterior == NULL) {
        // Caso en que el empleado a borrar está en el "head"
        *lista = actual->siguiente;
    }
    else {
        // Sáltate el empleado actual en la lista
        anterior->siguiente = actual->siguiente;
    }

    free(actual); // Borra al empleado actual de la memoria
    return 1;
}

int num_empleados(Empleado* lista) {
    int contador = 0;
    // Itera con un contador sobre la lista
    while (lista != NULL) {
        contador++;
        lista = lista->siguiente;
    }
    return contador;
}

Empleado* buscar_empleado(Empleado* lista, int numero_ingreso) {
    while (lista != NULL) {
        // si el empleado apuntado no es nulo, checa si es el deseado
        if (lista->numero_ingreso == numero_ingreso) {
            return lista;
        }
        lista = lista->siguiente;
    }
    return NULL;
}

void mostrar_empleado(Empleado* empleado) {
    if (empleado != NULL) {
        printf("\n--- Información del Empleado ---\n");
        printf("Nombres: %s\n", empleado->nombres);
        printf("Apellidos: %s\n", empleado->apellidos);
        printf("Número de ingreso: %d\n", empleado->numero_ingreso);
        printf("Dirección: %s, %d, %s, %s, %s\n", empleado->direccion.calle, empleado->direccion.numero_externo, empleado->direccion.codigo_postal, empleado->direccion.ciudad, empleado->direccion.pais);
        printf("Salario: %.2f\n", empleado->salario);
        printf("Género: %c\n", empleado->genero);
        printf("Estado Civil: %c\n", empleado->estado_civil);
        printf("Número de hijos: %d\n", empleado->numero_hijos);
    }
}

void los_empleados(Empleado* lista) {
    if (lista == NULL) {
        printf("No hay empleados en la lista.\n");
    }
    else {
        while (lista != NULL) {
            mostrar_empleado(lista);
            lista = lista->siguiente;
        }
    }
}
