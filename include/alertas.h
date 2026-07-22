#ifndef ALERTAS_H
#define ALERTAS_H

#include "sensores.h"

typedef struct Alerta
{
    int identificador;

    char dataHoraCriacao[30];
    char codigoSensor[30];
    char descricao[200];
    char estadoObservado[20];

    int severidade;

    char estadoAtendimento[20];
    char dataHoraConclusao[30];

    struct Alerta *seguinteLista;
    struct Alerta *seguinteFila;

} Alerta;


int criarAlertaManual(
    Alerta **lista,
    Alerta **fila,
    char codigoSensor[],
    char descricao[],
    char estadoObservado[],
    int severidade
);

int criarAlertaAutomatico(
    Alerta **lista,
    Alerta **fila,
    char codigoSensor[],
    char descricao[],
    char estadoObservado[],
    int severidade
);

int processarProximoAlerta(Alerta **fila);

int concluirAlerta(
    Alerta *lista,
    int identificador
);

void listarAlertasPorEstado(
    Alerta *lista,
    char estado[]
);

int pesquisarAlertaPorId(
    Alerta *lista,
    int identificador
);

int pesquisarAlertasPorSensor(
    Alerta *lista,
    char codigoSensor[]
);

#endif