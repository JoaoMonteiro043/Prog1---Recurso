#ifndef REGISTOS_H
#define REGISTOS_H

typedef struct Registo
{
  char dataHora[30];
  char resultadoSincronizacao[20];
  int nLeituras;
  int nSensoresNovos;
  int nSensoresAtualizados;
  int nLeiturasSemAlteracao;
  int nRegistosRejeitados;
  int nAlertasGerados;

  struct Registo *seguinte;
}Registo;

int registarSincronizacao(
    Registo **pilha,
    char resultadoSincronizacao[],
    int nLeituras,
    int nSensoresNovos,
    int nSensoresAtualizados,
    int nLeiturasSemAlteracao,
    int nRegistosRejeitados,
    int nAlertasGerados
);

void consultarUltimaSincronizacao(Registo *pilha);

void listarUltimasSincronizacoes(Registo *pilha, int N);

int guardarHistoricoBinario(
    Registo *pilha,
    char nomeFicheiro[]
);

int carregarHistoricoBinario(
    Registo **pilha,
    char nomeFicheiro[]
);


#endif