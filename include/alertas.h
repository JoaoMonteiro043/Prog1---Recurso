#ifndef ALERTAS_H
#define ALERTAS_H


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



endif