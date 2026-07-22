#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/sensores.h"


void listarSensores(Sensor *lista)
{
  Sensor *atual = lista; 
    
  // percorre a lista
    while (atual != NULL){

      printf("%s\n%d\n%s\n%s\n%s\n%.2f\n%s\n%s\n%d\n%s\n%.2f\n%s\n%s\n", atual -> timestamp, atual -> sequence, atual -> codigo, atual -> descricao, atual -> tipo, atual -> valor, atual -> unidade, atual -> estado, atual -> severidade, atual -> tendencia, atual -> confianca, atual -> causa, atual -> mensagem);

      atual = atual -> seguinte;
    }

}


 int adicionarSensor(Sensor **lista, Sensor *novoSensor)
{
    Sensor *atual;

    //Validação se lista exista
    if (lista == NULL || novoSensor == NULL)
    {
        return 0;
    }

    //Novo sensor será o último
    novoSensor->seguinte = NULL;

    //Verificação do conteúdo, se vazia, vai ser o primeiro sensor
    if (*lista == NULL)
    {
        *lista = novoSensor;
        return 1;
    }

    //Inicializa ponteiro auxiliar
    atual = *lista;

    //Percorre a lista
    while (atual->seguinte != NULL)
    {
        atual = atual->seguinte;
    }
    
    //Liga último sensor ao novo sensor
    atual->seguinte = novoSensor;

    return 1;
}



Sensor *pesquisarSensor(Sensor *inicio, const char *codigo){
  Sensor *atual = inicio;    //Criar ponteiro auxiliar para nao perdermos o ponteiro inicio, atribuimos endereço do primeiro sensor

  while(atual != NULL){ //Percorre enquanto não chegarmos ao fim da lista
    if(strcmp(atual -> codigo, codigo) == 0){      //Vai comparar as duas strings, 0 se forem iguais
      return atual;
    }
    atual = atual -> seguinte;        //avançar para o elemento seguinte
  }
  return NULL;
}



int atualizarSensor(
    Sensor *sensor,
    const char *descricao,
    const char *tipo,
    float valor,
    const char *unidade,
    const char *estado,
    int severidade,
    const char *mensagem,
    const char *timestamp)
{
    int houveAlteracao = 0;

    if (sensor == NULL)
    {
      return 0;
    }

    
    if (sensor->valor != valor || strcmp(sensor->estado, estado) != 0 || sensor->severidade != severidade || strcmp(sensor->mensagem, mensagem) != 0)
    {
      houveAlteracao = 1;
    }

   
    strcpy(sensor->descricao, descricao);
    strcpy(sensor->tipo, tipo);
    sensor->valor = valor;
    strcpy(sensor->unidade, unidade);
    strcpy(sensor->estado, estado);
    sensor->severidade = severidade;
    strcpy(sensor->mensagem, mensagem);
    strcpy(sensor->timestamp, timestamp);

    return houveAlteracao;
}



void filtrarPorTipo(Sensor *lista, const char *tipo)
{
    Sensor *atual = lista;
    int encontrou = 0;

    while (atual != NULL)
    {
        if (strcmp(atual->tipo, tipo) == 0)
        {
          printf("Codigo: %s\n", atual->codigo);
          printf("Descricao: %s\n", atual->descricao);
          printf("Tipo: %s\n", atual->tipo);
          printf("Valor: %.2f\n", atual->valor);
          printf("Unidade: %s\n", atual->unidade);
          printf("Estado: %s\n", atual->estado);
          printf("Severidade: %d\n", atual->severidade);
          printf("Mensagem: %s\n", atual->mensagem);
          printf("Timestamp: %s\n", atual->timestamp);
          printf("--------------------\n");

          encontrou = 1;
        }

        atual = atual->seguinte;
    }

    if (encontrou == 0)
    {
      printf("Nenhum sensor encontrado com esse tipo.\n");
    }
}



void filtrarPorEstado(Sensor *lista, const char *estado)
{
    Sensor *atual = lista;
    int encontrou = 0;

    while (atual != NULL)
    {
        if (strcmp(atual->estado, estado) == 0)
        {
            printf("Codigo: %s\n", atual->codigo);
            printf("Descricao: %s\n", atual->descricao);
            printf("Tipo: %s\n", atual->tipo);
            printf("Valor: %.2f\n", atual->valor);
            printf("Unidade: %s\n", atual->unidade);
            printf("Estado: %s\n", atual->estado);
            printf("Severidade: %d\n", atual->severidade);
            printf("Mensagem: %s\n", atual->mensagem);
            printf("Timestamp: %s\n", atual->timestamp);
            printf("--------------------\n");

            encontrou = 1;
        }

        atual = atual->seguinte;
    }

    if (encontrou == 0)
    {
        printf("Nenhum sensor encontrado com esse estado.\n");
    }
}



Sensor *criarSensor(const char *timestamp,
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
  const char *mensagem)
{
  Sensor *novoSensor = malloc(sizeof(Sensor));     //Variavel do tipo Sensor que aponta para memoria alocada.

  if(novoSensor == NULL){                          //Validação memoria alocada
    return NULL;
  }

  strcpy(novoSensor -> timestamp, timestamp);
  novoSensor -> sequence = sequence;

  strcpy(novoSensor -> codigo, codigo);
  strcpy(novoSensor -> descricao, descricao);
  strcpy(novoSensor -> tipo, tipo);
  novoSensor -> valor = valor;

  strcpy(novoSensor -> unidade, unidade);
  strcpy(novoSensor -> estado, estado);
  novoSensor -> severidade = severidade;

  strcpy(novoSensor -> tendencia, tendencia);
  novoSensor -> confianca = confianca;
  strcpy(novoSensor -> causa, causa);
  strcpy(novoSensor -> mensagem, mensagem);
  

  novoSensor -> seguinte = NULL;

  return novoSensor;
}


void listarSensoresOrdenados(Sensor *lista){


  //Antes de criar vetor verificar se a lista esta vazia
  int quantidade = 0;
  Sensor *atual = lista;

  while(atual != NULL){
    quantidade ++;
    atual = atual -> seguinte;
  }

  // Verificar se a lista está vazia     
  if (quantidade == 0)
    {
        printf("A lista está vazia.\n");
        return;
    }

  //Criar o vetor que vamos percorrer
  Sensor **vetor = malloc(quantidade * sizeof(Sensor*));  //Tamanho de um ponteiro para Sensor

  if (vetor == NULL)
    {
        printf("Erro ao reservar memória.\n");
        return;
    }

  
  atual = lista;
  
  //Percorrer o vetor
  for (int i = 0; i < quantidade; i++)
  {
    vetor[i] = atual;
    atual = atual->seguinte;
  }

  //Bubble sort

  for (int i = 0; i < quantidade - 1; i++)
  {
    for (int j = 0; j < quantidade - 1 - i; j++)
    {
      if (vetor[j]->severidade < vetor[j + 1]->severidade ||
         (vetor[j]->severidade == vetor[j + 1]->severidade &&
          strcmp(vetor[j]->codigo, vetor[j + 1]->codigo) > 0))
      {
        Sensor *temporario = vetor[j];
        vetor[j] = vetor[j + 1];
        vetor[j + 1] = temporario;
      }
    }
  }

  //Imprimir sensores ordenados
  for(int i = 0; i < quantidade; i ++){

    printf("Codigo: %s\n", vetor[i]->codigo);
    printf("Descricao: %s\n", vetor[i]->descricao);
    printf("Tipo: %s\n", vetor[i]->tipo);
    printf("Valor: %.2f\n", vetor[i]->valor);
    printf("Unidade: %s\n", vetor[i]->unidade);
    printf("Estado: %s\n", vetor[i]->estado);
    printf("Severidade: %d\n", vetor[i]->severidade);
    printf("Mensagem: %s\n", vetor[i]->mensagem);
    printf("Timestamp: %s\n", vetor[i]->timestamp);
    printf("--------------------\n");
  }

  //Libertar sensor
  free(vetor);

}