#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/sensores.h"
#include "../include/alertas.h"


int main()
{
    Sensor *lista = NULL;

    Sensor *novoSensor = criarSensor(/* dados do sensor */);

    adicionarSensor(&lista, novoSensor);

    return 0;
}