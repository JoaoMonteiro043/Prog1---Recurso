#ifndef SENSORES_H
#define SENSORES_H

typedef struct Sensor {
    char timestamp[30];
    int sequence;
    char codigo[30];
    char descricao[100];
    char tipo[30];
    float valor;
    char unidade[10];
    char estado[20];
    int severidade;
    char tendencia[20];
    float confianca;
    char causa[50];
    char mensagem[200];

    struct Sensor *seguinte;
} Sensor;

Sensor *criarSensor(
  const char *timestamp,
  int sequence,
  const char *codigo,
  const char *descricao,
  const char *tipo,
  float valor,
  const char *unidade,
  const char *estado,
  int severidade,
  const char *tendencia,
  float confianca,
  const char *causa,
  const char *mensagem
);

#endif