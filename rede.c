#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct no {
  char nome[50];
  struct no* next;
  struct no* previous;
} no;

no* buscar(no **rede, char nome[50]) {
  if (*rede == NULL) {
    return NULL;
  }
 
  no *aux = *rede;

  do {
    if (strcmp(aux->nome, nome) == 0) {
      return aux;
    }
    aux = aux->next;
  } while (aux != *rede);

  return NULL;
}

void Adicionar_PessoaF(no **rede, char nome[50]) {
  if (buscar(rede, nome) != NULL) {
    printf("[ERROR] ADD %s\n", nome);
    return;
  }

  no *nova = malloc(sizeof(no));

  if (nova) {
    strcpy(nova->nome, nome);
    nova->next = NULL;
    nova->previous = NULL;

    if (*rede == NULL) {
      *rede = nova;
      (*rede)->next = nova;
      (*rede)->previous = nova;
    } else {
      no *ultimo = (*rede)->previous;
      ultimo->next = nova;
      nova->previous = ultimo;
      nova->next = *rede;
      (*rede)->previous = nova;
    }

    printf("[ OK  ] ADD %s\n", nome);
  } else {
    printf("Erro ao alocar memória");
  }
}

void remover(no** rede, char nome[50]) {
  no* buscado = buscar(rede, nome);

  if (buscado) {
    if (buscado->previous) {
      buscado->previous->next = buscado->next;
    } else {
      *rede = buscado->next;
    }
    if (buscado->next) {
      buscado->next->previous = buscado->previous;
    }
    if (*rede == buscado) {
      *rede = (*rede)->next;
    }

    free(buscado);
  }
}

int main(int argc, char* argv[]) {
  FILE* input = fopen(argv[1], "r");
  FILE* output = fopen(argv[2], "w");
  char continuar[50];
  char nome[50];
  no* rede = NULL;
  char add[] = "ADD";
  char removendo[] ="REMOVE";
  char show[] = "SHOW";

  if (!input || !output) {
    printf("Erro ao abrir os arquivos.\n");
    return 1;
  }

  do {
    if (fscanf(input, "%s", continuar) != 1) {
      printf("Erro ao ler comando.\n");
      break;
    }

    if (strcmp(continuar, add) == 0) {
      if (fgets(nome, sizeof(nome), input) != NULL) {
        nome[strcspn(nome, "\n")] = '\0';  // Remover a nova linha
        Adicionar_PessoaF(&rede, nome);       
      }
      
    } else if (strcmp(continuar, show) == 0) {
      if (fgets(nome, sizeof(nome), input) != NULL) {
        nome[strcspn(nome, "\n")] = '\0';  // Remover a nova linha
        no* buscado = buscar(&rede, nome);
        if (buscado) {
          fprintf(output ,"[ OK  ] %s<-%s->%s\n", buscado->previous->nome, buscado->nome, buscado->next->nome);
        } else {
          fprintf(output, "[ERROR] ?<-%s->?\n", nome);
        }
      }

    } else if (strcmp(continuar, removendo) == 0) {
      if (fgets(nome, sizeof(nome), input) != NULL) {
        nome[strcspn(nome, "\n")] = '\0';  // Remover a nova linha
        no* buscado = buscar(&rede, nome);
        if (buscado != NULL) {
          remover(&rede, nome);
          fprintf(output, "[ OK  ] REMOVE %s\n", nome);
        } else {
          fprintf(output, "[ERROR] REMOVE %s\n", nome);
        }
      }

    } else if (strcmp(continuar, "EXIT") == 0 || strcmp(continuar, "exit") == 0) {
      fprintf(output, "Fim do programa\n");
    } else {
      fprintf(output, "Opção inválida\n");
    }
  } while (strcmp(continuar, "EXIT") != 0 && strcmp(continuar, "exit") != 0);

  fclose(input);
  fclose(output);

  return 0;
}