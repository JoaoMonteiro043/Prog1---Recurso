#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>   //Precisamos guardar data e hora

#include "../include/registos.h"


static void obterDataHora(char data[])
{
    time_t agora;
    struct tm *info;

    agora = time(NULL);
    info = localtime(&agora);

    strftime(data, 30, "%d/%m/%Y %H:%M:%S", info);
}


static Registo *criarRegisto(
  char resultadoSincronizacao[],
  int nLeituras,
  int nSensoresNovos,
  int nSensoresAtualizados,
  int nLeiturasSemAlteracao,
  int nRegistosRejeitados,
  int nAlertasGerados)
{
 Registo *novo;

 novo = malloc(sizeof(Registo));

    if (novo == NULL)
    {
        return NULL;
    }

  obterDataHora(novo->dataHora);

  strcpy(novo->resultadoSincronizacao, resultadoSincronizacao);
  novo->nLeituras = nLeituras;
  novo->nSensoresNovos = nSensoresNovos;
  novo->nSensoresAtualizados = nSensoresAtualizados;
  novo->nLeiturasSemAlteracao = nLeiturasSemAlteracao;
  novo->nRegistosRejeitados = nRegistosRejeitados;
  novo->nAlertasGerados = nAlertasGerados;
  
  novo->seguinte = NULL;

  return novo;
  
}


int registarSincronizacao(Registo **pilha,
  char resultadoSincronizacao[],
  int nLeituras,
  int nSensoresNovos,
  int nSensoresAtualizados,
  int nLeiturasSemAlteracao,
  int nRegistosRejeitados,
  int nAlertasGerados)
{
  Registo *novo;

  novo = criarRegisto(
   resultadoSincronizacao[],
   nLeituras,
   nSensoresNovos,
   nSensoresAtualizados,
   nLeiturasSemAlteracao,
   nRegistosRejeitados,
   nAlertasGerados);

  if (novo == NULL)
    {
        return 0;
    }
    
    //Coloca novo registo no topo da pilha
    novo->seguinte = *pilha;

    *pilha = novo;

    return 1;
}


void consultarUltimaSincronizacao(Registo *pilha)
{
  if(pilha == NULL)
  {
    printf("Nao existe nenhuma sincronizacao");
    return;
  }

  printf("\n=== Ultima Sincronizacao ===\n");
  printf("Data/Hora: %s\n", pilha->dataHora);
  printf("Resultado: %s\n", pilha->resultadoSincronizacao);
  printf("Leituras: %d\n", pilha->nLeituras);
  printf("Sensores Novos: %d\n", pilha->nSensoresNovos);
  printf("Sensores Atualizados: %d\n", pilha->nSensoresAtualizados);
  printf("Leituras sem Alteracao: %d\n", pilha->nLeiturasSemAlteracao);
  printf("Registos Rejeitados: %d\n", pilha->nRegistosRejeitados);
  printf("Alertas Gerados: %d\n", pilha->nAlertasGerados);

}


void listarUltimasSincronizacoes(Registo *pilha, int N)
{
  if(pilha == NULL)
  {
    printf("Nao existe nenhuma sincronizacao");
    return;
  }

  Registo *aux = pilha;

  int contador = 0; 

  printf("\n=== Ultimas %d Sincronizacoes ===\n", N);

  while(aux != NULL && contador < N)
  {
  
   printf("Data/Hora: %s\n", aux->dataHora);
   printf("Resultado: %s\n", aux->resultadoSincronizacao);
   printf("Leituras: %d\n", aux->nLeituras);
   printf("Sensores Novos: %d\n", aux->nSensoresNovos);
   printf("Sensores Atualizados: %d\n", aux->nSensoresAtualizados);
   printf("Leituras sem Alteracao: %d\n", aux->nLeiturasSemAlteracao);
   printf("Registos Rejeitados: %d\n", aux->nRegistosRejeitados);
   printf("Alertas Gerados: %d\n", aux->nAlertasGerados);

   aux = aux->seguinte;
   contador++;
  }
}


int guardarHistoricoBinario(Registo *pilha, char nomeFicheiro[])
{
    FILE *ficheiro;
    Registo *aux;

    ficheiro = fopen(nomeFicheiro, "wb");

    if (ficheiro == NULL)
    {
        printf("Erro ao abrir o ficheiro.\n");
        return 0;
    }

    aux = pilha;

    while (aux != NULL)
    {
        fwrite(aux->dataHora, sizeof(char), 30, ficheiro);
        fwrite(aux->resultadoSincronizacao, sizeof(char), 20, ficheiro);

        fwrite(&aux->nLeituras, sizeof(int), 1, ficheiro);
        fwrite(&aux->nSensoresNovos, sizeof(int), 1, ficheiro);
        fwrite(&aux->nSensoresAtualizados, sizeof(int), 1, ficheiro);
        fwrite(&aux->nLeiturasSemAlteracao, sizeof(int), 1, ficheiro);
        fwrite(&aux->nRegistosRejeitados, sizeof(int), 1, ficheiro);
        fwrite(&aux->nAlertasGerados, sizeof(int), 1, ficheiro);

        aux = aux->seguinte;
    }

    fclose(ficheiro);

    return 1;
}


int carregarHistoricoBinario(Registo **pilha, char nomeFicheiro[])
{
    FILE *ficheiro;
    Registo *novo;
    Registo *ultimo;

    ficheiro = fopen(nomeFicheiro, "rb");

    if (ficheiro == NULL)
    {
        return 0;
    }

    while (1)
    {
        novo = malloc(sizeof(Registo));

        if (novo == NULL)
        {
            fclose(ficheiro);
            return 0;
        }

        if (fread(novo->dataHora, sizeof(char), 30, ficheiro) != 30)
        {
            free(novo);
            break;
        }

        if (fread(novo->resultadoSincronizacao, sizeof(char), 20, ficheiro) != 20 ||
            fread(&novo->nLeituras, sizeof(int), 1, ficheiro) != 1 ||
            fread(&novo->nSensoresNovos, sizeof(int), 1, ficheiro) != 1 ||
            fread(&novo->nSensoresAtualizados, sizeof(int), 1, ficheiro) != 1 ||
            fread(&novo->nLeiturasSemAlteracao, sizeof(int), 1, ficheiro) != 1 ||
            fread(&novo->nRegistosRejeitados, sizeof(int), 1, ficheiro) != 1 ||
            fread(&novo->nAlertasGerados, sizeof(int), 1, ficheiro) != 1)
        {
            free(novo);
            fclose(ficheiro);
            return 0;
        }

        novo->seguinte = NULL;

        if (*pilha == NULL)
        {
            *pilha = novo;
        }
        else
        {
            ultimo = *pilha;

            while (ultimo->seguinte != NULL)
            {
                ultimo = ultimo->seguinte;
            }

            ultimo->seguinte = novo;
        }
    }

    fclose(ficheiro);

    return 1;
}