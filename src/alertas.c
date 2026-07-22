#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>   //Precisamos guardar data e hora

#include "../include/alertas.h"


//Guardar data e hora atual
static void obterDataHora(char data[])
{
    time_t agora;
    struct tm *info;

    agora = time(NULL);
    info = localtime(&agora);

    strftime(data, 30, "%d/%m/%Y %H:%M:%S", info);
}

//Função auxiliar usada posteriormente na criação manual e automática de alertas
static Alerta *criarAlerta(
    char codigoSensor[],
    char descricao[],
    char estadoObservado[],
    int severidade
)
{
    static int proximoId = 1;

    Alerta *novo;

    novo = malloc(sizeof(Alerta));

    if (novo == NULL)
    {
        return NULL;
    }

    novo->identificador = proximoId;
    proximoId++;

    obterDataHora(novo->dataHoraCriacao);

    strcpy(novo->codigoSensor, codigoSensor);
    strcpy(novo->descricao, descricao);
    strcpy(novo->estadoObservado, estadoObservado);

    novo->severidade = severidade;

    strcpy(novo->estadoAtendimento, "Pendente");
    strcpy(novo->dataHoraConclusao, "");

    novo->seguinteLista = NULL;
    novo->seguinteFila = NULL;

    return novo;
}

static int registarAlerta(
    Alerta **lista,
    Alerta **fila,
    char codigoSensor[],
    char descricao[],
    char estadoObservado[],
    int severidade
)
{
    Alerta *novo;
    Alerta *atual;


    //Evita alertas repetidos
    if (existeAlertaAtivo(*lista, codigoSensor, estadoObservado, severidade))
    {
      return 0;
    }

    novo = criarAlerta(
        codigoSensor,
        descricao,
        estadoObservado,
        severidade
    );

    if (novo == NULL)
    {
        return 0;
    }

    // Adicionar à lista
    if (*lista == NULL)
    {
        *lista = novo;
    }
    else
    {
        atual = *lista;

        while (atual->seguinteLista != NULL)
        {
            atual = atual->seguinteLista;
        }

        atual->seguinteLista = novo;
    }

    // Adicionar à fila
    if (*fila == NULL)
    {
        *fila = novo;
    }
    else
    {
        atual = *fila;

        while (atual->seguinteFila != NULL)
        {
            atual = atual->seguinteFila;
        }

        atual->seguinteFila = novo;
    }

    return 1;
}

int criarAlertaManual(
    Alerta **lista,
    Alerta **fila,
    char codigoSensor[],
    char descricao[],
    char estadoObservado[],
    int severidade
)
{
    return registarAlerta(
        lista,
        fila,
        codigoSensor,
        descricao,
        estadoObservado,
        severidade
    );
}

int criarAlertaAutomatico(
    Alerta **lista,
    Alerta **fila,
    char codigoSensor[],
    char descricao[],
    char estadoObservado[],
    int severidade
)
{
    //Validação alerta automático
    if (strcmp(estadoObservado, "NORMAL") == 0 && severidade == 0)
    {
        return 0;
    }

    return registarAlerta(
        lista,
        fila,
        codigoSensor,
        descricao,
        estadoObservado,
        severidade
    );
}


static int existeAlertaAtivo(
    Alerta *lista,
    char codigoSensor[],
    char estadoObservado[],
    int severidade
)
{
    Alerta *atual;

    atual = lista;

    while (atual != NULL)
    {   
        // Verifica se os dados do alerta coincidem
        if (strcmp(atual->codigoSensor, codigoSensor) == 0 && strcmp(atual->estadoObservado, estadoObservado) == 0 &&atual->severidade == severidade && (strcmp(atual->estadoAtendimento, "Pendente") == 0 || strcmp(atual->estadoAtendimento, "Em Curso") == 0))
        {
            return 1;
        }

        atual = atual->seguinteLista;
    }

    return 0;
}

int processarProximoAlerta(Alerta **fila)
{
    Alerta *atual;

    if (*fila == NULL)
    {
        return 0;
    }

    atual = *fila;

    *fila = atual->seguinteFila;

    atual->seguinteFila = NULL;

    strcpy(atual->estadoAtendimento, "Em Curso");

    return 1;
}

int concluirAlerta(Alerta *lista, int identificador)
{
    Alerta *atual;

    atual = lista;

    while (atual != NULL)
    {
        if (
            atual->identificador == identificador &&             //Percorre a lista e procura identificador e estado em Curso
            strcmp(atual->estadoAtendimento, "Em Curso") == 0
        )
        {
            strcpy(atual->estadoAtendimento, "Concluido");      //Muda o estado para Concluído

            obterDataHora(atual->dataHoraConclusao);

            return 1;
        }

        atual = atual->seguinteLista;
    }

    return 0;
}

static void mostrarAlerta(Alerta *alerta)
{
    printf("\nIdentificador: %d\n", alerta->identificador);
    printf("Data e hora de criacao: %s\n", alerta->dataHoraCriacao);
    printf("Codigo do sensor: %s\n", alerta->codigoSensor);
    printf("Descricao: %s\n", alerta->descricao);
    printf("Estado observado: %s\n", alerta->estadoObservado);
    printf("Severidade: %d\n", alerta->severidade);
    printf("Estado de atendimento: %s\n", alerta->estadoAtendimento);

    if (strcmp(alerta->estadoAtendimento, "Concluido") == 0)
    {
        printf("Data e hora de conclusao: %s\n",
               alerta->dataHoraConclusao);
    }
}

void listarAlertasPorEstado(Alerta *lista, char estado[])
{
    Alerta *atual;
    int encontrou;

    atual = lista;
    encontrou = 0;

    while (atual != NULL)
    {
        if (strcmp(atual->estadoAtendimento, estado) == 0)
        {
            mostrarAlerta(atual);
            encontrou = 1;
        }

        atual = atual->seguinteLista;
    }

    if (encontrou == 0)
    {
        printf("\nNao existem alertas com o estado %s.\n", estado);
    }
}

int pesquisarAlertaPorId(Alerta *lista, int identificador)
{
    Alerta *atual;

    atual = lista;

    while (atual != NULL)
    {
        if (atual->identificador == identificador)
        {
            mostrarAlerta(atual);
            return 1;
        }

        atual = atual->seguinteLista;
    }

    return 0;
}


int pesquisarAlertasPorSensor(
    Alerta *lista,
    char codigoSensor[]
)
{
    Alerta *atual;
    int encontrou;

    atual = lista;
    encontrou = 0;

    while (atual != NULL)
    {
        if (strcmp(atual->codigoSensor, codigoSensor) == 0)
        {
            mostrarAlerta(atual);
            encontrou = 1;
        }

        atual = atual->seguinteLista;
    }

    return encontrou;
}
