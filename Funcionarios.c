/*
  Codigo da Prova 3 de AEDS2 disciplina ministrada pela professora Tatiana Alves Costa
  UFOP Campus Icea
  Alunos:
    Arthur Mendonça Feu
    Daniel Ângelo Rosa Morais
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MAX 3
#define MIN 2

struct VERTICE_B
{
  int item[MAX + 1], contador;
  struct VERTICE_B *linker[MAX + 1];
};

struct VERTICE_B *raiz;

struct VERTICE_B *criarVertice(int item, struct VERTICE_B *verticeFilho)
{
  struct VERTICE_B *novoVertice;
  novoVertice = (struct VERTICE_B *)malloc(sizeof(struct VERTICE_B));
  novoVertice->item[1] = item;
  novoVertice->contador = 1;
  novoVertice->linker[0] = raiz;
  novoVertice->linker[1] = verticeFilho;
  return novoVertice;
}

void adicionarValor(int item, int pos, struct VERTICE_B *node, struct VERTICE_B *verticeFilho)
{
  int j = node->contador;
  while (j > pos)
  {
    node->item[j + 1] = node->item[j];
    node->linker[j + 1] = node->linker[j];
    j--;
  }
  node->item[j + 1] = item;
  node->linker[j + 1] = verticeFilho;
  node->contador++;
}

void dividirVertice(int item, int *pval, int pos, struct VERTICE_B *node, struct VERTICE_B *verticeFilho, struct VERTICE_B **novoVertice)
{
  int median, j;

  if (pos > MIN)
    median = MIN + 1;
  else
    median = MIN;

  *novoVertice = (struct VERTICE_B *)malloc(sizeof(struct VERTICE_B));
  j = median + 1;
  while (j <= MAX)
  {
    (*novoVertice)->item[j - median] = node->item[j];
    (*novoVertice)->linker[j - median] = node->linker[j];
    j++;
  }
  node->contador = median;
  (*novoVertice)->contador = MAX - median;

  if (pos <= MIN)
  {
    adicionarValor(item, pos, node, verticeFilho);
  }
  else
  {
    adicionarValor(item, pos - median, *novoVertice, verticeFilho);
  }
  *pval = node->item[node->contador];
  (*novoVertice)->linker[0] = node->linker[node->contador];
  node->contador--;
}

int preencherValorVertice(int item, int *pval, struct VERTICE_B *node, struct VERTICE_B **verticeFilho)
{
  int pos;
  if (!node)
  {
    *pval = item;
    *verticeFilho = NULL;
    return 1;
  }

  if (item < node->item[1])
  {
    pos = 0;
  }
  else
  {
    for (pos = node->contador;
         (item < node->item[pos] && pos > 1); pos--)
      ;
    if (item == node->item[pos])
    {
      printf("\nImpossivel repetir valores\n");
      return 0;
    }
  }
  if (preencherValorVertice(item, pval, node->linker[pos], verticeFilho))
  {
    if (node->contador < MAX)
    {
      adicionarValor(*pval, pos, node, *verticeFilho);
    }
    else
    {
      dividirVertice(*pval, pval, pos, node, *verticeFilho, verticeFilho);
      return 1;
    }
  }
  return 0;
}

void inserirElemento(int item)
{
  int flag, i;
  struct VERTICE_B *verticeFilho;

  flag = preencherValorVertice(item, &i, raiz, &verticeFilho);
  if (flag)
    raiz = criarVertice(i, verticeFilho);
}

void copiarVerticeSucessor(struct VERTICE_B *meuVertice, int pos)
{
  struct VERTICE_B *dummy;
  dummy = meuVertice->linker[pos];

  for (; dummy->linker[0] != NULL;)
    dummy = dummy->linker[0];
  meuVertice->item[pos] = dummy->item[1];
}

void excluirValor(struct VERTICE_B *meuVertice, int pos)
{
  int i = pos + 1;
  while (i <= meuVertice->contador)
  {
    meuVertice->item[i - 1] = meuVertice->item[i];
    meuVertice->linker[i - 1] = meuVertice->linker[i];
    i++;
  }
  meuVertice->contador--;
}

void saltoDireita(struct VERTICE_B *meuVertice, int pos)
{
  struct VERTICE_B *x = meuVertice->linker[pos];
  int j = x->contador;

  while (j > 0)
  {
    x->item[j + 1] = x->item[j];
    x->linker[j + 1] = x->linker[j];
  }
  x->item[1] = meuVertice->item[pos];
  x->linker[1] = x->linker[0];
  x->contador++;

  x = meuVertice->linker[pos - 1];
  meuVertice->item[pos] = x->item[x->contador];
  meuVertice->linker[pos] = x->linker[x->contador];
  x->contador--;
  return;
}

void saltoEsquerda(struct VERTICE_B *meuVertice, int pos)
{
  int j = 1;
  struct VERTICE_B *x = meuVertice->linker[pos - 1];

  x->contador++;
  x->item[x->contador] = meuVertice->item[pos];
  x->linker[x->contador] = meuVertice->linker[pos]->linker[0];

  x = meuVertice->linker[pos];
  meuVertice->item[pos] = x->item[1];
  x->linker[0] = x->linker[1];
  x->contador--;

  while (j <= x->contador)
  {
    x->item[j] = x->item[j + 1];
    x->linker[j] = x->linker[j + 1];
    j++;
  }
  return;
}

void unirVertices(struct VERTICE_B *meuVertice, int pos)
{
  int j = 1;
  struct VERTICE_B *x1 = meuVertice->linker[pos], *x2 = meuVertice->linker[pos - 1];

  x2->contador++;
  x2->item[x2->contador] = meuVertice->item[pos];
  x2->linker[x2->contador] = meuVertice->linker[0];

  while (j <= x1->contador)
  {
    x2->contador++;
    x2->item[x2->contador] = x1->item[j];
    x2->linker[x2->contador] = x1->linker[j];
    j++;
  }

  j = pos;
  while (j < meuVertice->contador)
  {
    meuVertice->item[j] = meuVertice->item[j + 1];
    meuVertice->linker[j] = meuVertice->linker[j + 1];
    j++;
  }
  meuVertice->contador--;
  free(x1);
}

void ajustaVertice(struct VERTICE_B *meuVertice, int pos)
{
  if (!pos)
  {
    if (meuVertice->linker[1]->contador > MIN)
    {
      saltoEsquerda(meuVertice, 1);
    }
    else
    {
      unirVertices(meuVertice, 1);
    }
  }
  else
  {
    if (meuVertice->contador != pos)
    {
      if (meuVertice->linker[pos - 1]->contador > MIN)
      {
        saltoDireita(meuVertice, pos);
      }
      else
      {
        if (meuVertice->linker[pos + 1]->contador > MIN)
        {
          saltoEsquerda(meuVertice, pos + 1);
        }
        else
        {
          unirVertices(meuVertice, pos);
        }
      }
    }
    else
    {
      if (meuVertice->linker[pos - 1]->contador > MIN)
        saltoDireita(meuVertice, pos);
      else
        unirVertices(meuVertice, pos);
    }
  }
}

int removerValorDoVertice(int item, struct VERTICE_B *meuVertice)
{
  int pos, flag = 0;
  if (meuVertice)
  {
    if (item < meuVertice->item[1])
    {
      pos = 0;
      flag = 0;
    }
    else
    {
      for (pos = meuVertice->contador; (item < meuVertice->item[pos] && pos > 1); pos--)
        ;
      if (item == meuVertice->item[pos])
      {
        flag = 1;
      }
      else
      {
        flag = 0;
      }
    }
    if (flag)
    {
      if (meuVertice->linker[pos - 1])
      {
        copiarVerticeSucessor(meuVertice, pos);
        flag = removerValorDoVertice(meuVertice->item[pos], meuVertice->linker[pos]);
        if (flag == 0)
        {
          printf("\nValor nao encontrado na arvore\n");
        }
      }
      else
      {
        excluirValor(meuVertice, pos);
      }
    }
    else
    {
      flag = removerValorDoVertice(item, meuVertice->linker[pos]);
    }
    if (meuVertice->linker[pos])
    {
      if (meuVertice->linker[pos]->contador < MIN)
        ajustaVertice(meuVertice, pos);
    }
  }
  return flag;
}

void excluirElemento(int item, struct VERTICE_B *meuVertice)
{
  struct VERTICE_B *tmp;
  if (!removerValorDoVertice(item, meuVertice))
  {
    printf("\nElemento nao encontrado\n");
    return;
  }
  else
  {
    if (meuVertice->contador == 0)
    {
      tmp = meuVertice;
      meuVertice = meuVertice->linker[0];
      free(tmp);
    }
    printf("\nElemento removido\n");
  }
  raiz = meuVertice;
  return;
}

int buscaElemento(int item, int *pos, struct VERTICE_B *meuVertice)
{
  if (!meuVertice)
  {
    return;
  }

  if (item < meuVertice->item[1])
  {
    *pos = 0;
  }
  else
  {
    for (*pos = meuVertice->contador;
         (item < meuVertice->item[*pos] && *pos > 1); (*pos)--)
      ;
    if (item == meuVertice->item[*pos])
    {
      printf("Elemento encontrado: %d", item);
      return 1;
    }
  }
  buscaElemento(item, pos, meuVertice->linker[*pos]);
  return 0;
}

void exibicao(struct VERTICE_B *meuVertice)
{
  int i;
  if (meuVertice)
  {
    for (i = 0; i < meuVertice->contador; i++)
    {
      exibicao(meuVertice->linker[i]);
      printf("%d ", meuVertice->item[i + 1]);
    }
    exibicao(meuVertice->linker[i]);
  }
}

typedef struct Funcionario
{
  int cod;
  char nome[50];
  char cpf[15];
  char data_nascimento[11];
  double salario;
  int prox;
  int ocupado;
} TFunc;

void salva(TFunc *func, FILE *out)
{
  fwrite(&func->cod, sizeof(int), 1, out);
  fwrite(func->nome, sizeof(char), sizeof(func->nome), out);
  fwrite(func->cpf, sizeof(char), sizeof(func->cpf), out);
  fwrite(func->data_nascimento, sizeof(char), sizeof(func->data_nascimento), out);
  fwrite(&func->salario, sizeof(double), 1, out);
}

TFunc *le(FILE *in)
{
  TFunc *func = (TFunc *)malloc(sizeof(TFunc));
  if (0 >= fread(&func->cod, sizeof(int), 1, in))
  {
    free(func);
    return NULL;
  }
  fread(func->nome, sizeof(char), sizeof(func->nome), in);
  fread(func->cpf, sizeof(char), sizeof(func->cpf), in);
  fread(func->data_nascimento, sizeof(char), sizeof(func->data_nascimento), in);
  fread(&func->salario, sizeof(double), 1, in);
  return func;
}

int tamanho_arquivo(FILE *arq)
{
  fseek(arq, 0, SEEK_END);
  int tam = trunc(ftell(arq) / sizeof(TFunc));
  return tam;
}

void troca(int *a, int *b)
{
  int temp = *a;
  *a = *b;
  *b = temp;
}

void keySort(int *rnn, int *keys, int tam)
{
  int i;
  int j;
  for (i = 0; i < tam - 1; i++)
  {
    int melhor_pos = i;
    for (j = i + 1; j < tam; j++)
    {
      if (keys[j] < keys[melhor_pos])
      {
        melhor_pos = j;
      }
    }
    troca(&rnn[i], &rnn[melhor_pos]);
    troca(&keys[i], &keys[melhor_pos]);
  }
}

void questao1()
{
  // questao 1
  FILE *arq;
  int i = 0;
  TFunc buscado;

  fprintf(stderr, "=== QUESTAO 1 ===\n");
  srand(time(NULL));
  arq = fopen("dados.db", "wb+");
  if (arq != NULL)
  {
    TFunc f1;
    for (i = 1; i <= 100; i++)
    {
      f1.cod = (rand() % 100) * 33 + i; // criacao com 33 para variacao de dados, com 100 para uniformidade
      inserirElemento(f1.cod);          // criando a árvore para a questão 5
      if (i == 69)
      {
        buscado.cod = f1.cod;
      }
      f1.salario = i * 10.0;
      sprintf(f1.nome, "Funcionario %i", i);
      strcpy(f1.cpf, "XXXXXX");
      strcpy(f1.data_nascimento, "DD/MM/AAAA");
      fseek(arq, (i - 1) * sizeof(TFunc), SEEK_SET);
      salva(&f1, arq);
    }
    fclose(arq);
    printf("Arquivo criado!\n");
  }
  else
  {
    printf("\nErro ao abrir o arquivo\n");
    exit(1);
  }
}

void questao2()
{
  FILE *arq;
  int i = 0;
  TFunc buscado;
  // questao 2
  fprintf(stderr, "=== QUESTAO 2 ===\n");
  arq = fopen("dados.db", "rb");
  if (arq != NULL)
  {
    clock_t begin = clock();
    TFunc *func;
    for (i = 1; i <= 100; i++)
    {
      fseek(arq, (i - 1) * sizeof(TFunc), SEEK_SET);
      fprintf(stderr, "Index: %i, file pointer: %li\n", i, ftell(arq));
      func = le(arq);
      if (buscado.cod == func->cod)
      {
        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("Encontrado:\n\t%d\t%f ", func->cod, func->salario);
        printf("Numero de acessos: %d ", i);
        printf("Tempo de acesso: %lf\n", time_spent);
        break;
      }
    }
  }
  else
  {
    printf("\nErro ao abrir o arquivo\n");
    exit(1);
  }
}

void questoes3e4()
{
  FILE *arq;
  int i = 0;
  TFunc buscado;
  // questao 3
  fprintf(stderr, "=== QUESTAO 3 ===\n");
  arq = fopen("dados.db", "rb");

  fseek(arq, 0, SEEK_SET);
  int rnn[100], keys[100];

  for (i = 0; i < 100; i++)
  {
    TFunc *func = le(arq);
    fprintf(stderr, "Codigo: %i, index: %i\n", func->cod, i);
    rnn[i] = i;
    keys[i] = func->cod;
  };

  keySort(rnn, keys, 100);

  fprintf(stderr, "Ordenando...\n");
  clock_t begin = clock();
  FILE *ordenado = fopen("ordenado.db", "wb+");
  TFunc *ordenador;

  for (i = 0; i < 100; i++)
  {
    fseek(arq, rnn[i] * sizeof(TFunc), SEEK_SET);
    ordenador = le(arq);
    fprintf(stderr, "Codigo: %i, index: %i\n", ordenador->cod, rnn[i]);
    salva(ordenador, ordenado);
  }
  fclose(ordenado);
  fclose(arq);
  printf("Arquivo ordenado!\n");
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Tempo de ordenacao: %lf\n", time_spent);
  // questao 4
  fprintf(stderr, "\n\n=== QUESTAO 4 ===\n");
  ordenado = fopen("ordenado.db", "rb");
  int tam = tamanho_arquivo(ordenado);
  int esq = 0;
  int dir = tam - 1;

  if (arq != NULL)
  {
    clock_t begin = clock();
    i = 0;
    while (esq <= dir)
    {
      int meio = (esq + dir) / 2;
      fseek(ordenado, meio * sizeof(TFunc), SEEK_SET);
      TFunc *func = le(ordenado);
      fprintf(stderr, "Index: %i, codigo: %i\n", meio, func->cod);
      if (buscado.cod == func->cod)
      {
        clock_t end = clock();
        time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("Encontrado:\n\t%d\t%f ", func->cod, func->salario);
        printf("Numero de acessos: %d ", i);
        printf("Tempo de acesso: %lf\n", time_spent);
        break;
      }
      else if (func->cod < buscado.cod)
      {
        esq = meio + 1;
      }
      else
      {
        dir = meio - 1;
      }
      i++;
    }
  }

  fclose(ordenado);
}

// Funções da Selecao Natural
int verificaVazio(TFunc **vetor_func, int n)
{
  int vazio = 1;
  int i;
  for (i = 0; i < n; i++)
  {
    if (vetor_func[i] != NULL)
      vazio = 0;
  }
  return vazio;
}

void imprimeParticao(int total_particao)
{
  FILE *part;
  TFunc *func;

  int cont = 1;
  int num_func = 0;
  char destino[100];

  while (cont <= total_particao)
  {
    snprintf(destino, 100, "Part_%d.dat", cont);
    printf("\n Particao Numero %d\n", cont);
    part = fopen(destino, "a+b");
    num_func = 0;

    printf("| ");
    while (!feof(part))
    {
      func = le(part);

      if (func != NULL)
      {
        printf("%d | ", func->cod);

        num_func++;
      }
    }
    printf("\n");
    printf("Quantidade de funcionarios na particao: %d \n", num_func);
    cont++;
    fclose(part);
  }
  free(func);
}

int *encontraMenorCodigo(TFunc **vetor_men, int m)
{
  int *menor_cod_mem = NULL;
  int i;
  for (i = 0; i < m; i++) // Pega o menor codigo do array
  {
    if (vetor_men[i] != NULL)
    {
      if (menor_cod_mem == NULL)
      {
        menor_cod_mem = (int *)malloc(sizeof(int));
        *menor_cod_mem = i;
      }
      else if (vetor_men[i]->cod < vetor_men[*menor_cod_mem]->cod)
      {
        *menor_cod_mem = i;
      }
    }
  }
  return menor_cod_mem;
}

void selecaoNatural(int m, int n) // m = tam_res, n = tam mem
{
  FILE *reservatorio;
  FILE *arquivo;
  ;
  TFunc **vetor_men = (TFunc **)malloc(sizeof(TFunc) * n);
  TFunc *func_temp = (TFunc *)malloc(sizeof(TFunc));
  ;
  int num_particao = 1, res_total = 0, part_total = 0;
  arquivo = fopen("dados.db", "rb+");
  int fim_arq = feof(arquivo);

  if (arquivo == NULL)
  {
    printf("nao foi possivel abrir o arquivo!\n");
  }

  reservatorio = fopen("reservatorio.dat", "wb+");

  if (reservatorio == NULL)
  {
    printf("nao foi possivel criar o reservatorio!\n");
  }

  // criando vetor de memoria
  int i;
  for (i = 0; i < n; i++)
  {
    vetor_men[i] = le(arquivo);
  }

  // verifica se o arquivo chegou ao fim ou não há mais nada no reservatorio
  while (!fim_arq || res_total != 0)
  {
    if (!verificaVazio(vetor_men, n)) // se o vetor nao estiver vazio
    {
      int *menor_cod_mem = encontraMenorCodigo(vetor_men, m);
      char destino[100];
      FILE *particao_final;
      snprintf(destino, 100, "Part_%d.dat", num_particao);
      if (part_total == 0)
        particao_final = fopen(destino, "wb+");
      else
        particao_final = fopen(destino, "ab+");

      salva(vetor_men[*menor_cod_mem], particao_final); // salva o codigo do funcionario
      fclose(particao_final);                           // libera a particao

      part_total++;

      if (!fim_arq) // Caso o arquivo nao tenha chegado ao fim
      {
        if (res_total < n)
        {
          func_temp = le(arquivo);
          // Verifica se o codigo do funcionario é menor e valido, e a capacidade do reservatorio
          while (func_temp != NULL && func_temp->cod < vetor_men[*menor_cod_mem]->cod && res_total < n)
          {
            salva(func_temp, reservatorio);
            res_total++;
            if (res_total < n)
            {
              func_temp = le(arquivo);
              fim_arq - feof(arquivo);
            }
            else
            {
              func_temp = NULL;
            }
          }
          vetor_men[*menor_cod_mem] = func_temp;
        }
        else
        {
          vetor_men[*menor_cod_mem] = NULL;
        }

        fim_arq = feof(arquivo);
      }
      else
      {
        vetor_men[*menor_cod_mem] = NULL;
      }
      free(menor_cod_mem);
    }
    else
    {
      num_particao++;
      rewind(reservatorio);
      int i;
      for (i = 0; i < m; i++)
      {
        vetor_men[i] = le(reservatorio);
      }
      res_total = 0;
      part_total = 0;
      rewind(reservatorio);
    }
  }

  if (!verificaVazio(vetor_men, m))
  {
    while (!verificaVazio(vetor_men, m))
    {
      int *menor_cod_mem = encontraMenorCodigo(vetor_men, m);
      char destino[100];
      FILE *partificao_final;
      snprintf(destino, 100, "Part_%d.dat", num_particao);
      if (part_total == 0)
        partificao_final = fopen(destino, "wb+");
      else
        partificao_final = fopen(destino, "ab+");

      salva(vetor_men[*menor_cod_mem], partificao_final);
      fclose(partificao_final);

      part_total++;
      vetor_men[*menor_cod_mem] = NULL;
      free(menor_cod_mem);
    }
  }
  fclose(arquivo);
  fclose(reservatorio);
  imprimeParticao(num_particao);
  free(vetor_men);
}

int hashDivisao(int x, int y) {
    return x % y;
}

void hashFunction()
{
  FILE *arq;
  FILE *hashArq;
  TFunc *func;
  arq = fopen("dados.db", "rb");
  hashArq = fopen("hash.db", "wb+");

  int i;
  int bucketSize = 7;
  int buckets[bucketSize];
  int hash;
  for (i = 0; i < bucketSize; i++)
  {
    buckets[i] = 0;
  }
  for (i = 1; i <= 100; i++)
  {
    fseek(arq, (i - 1) * sizeof(TFunc), SEEK_SET);
    func = le(arq);
    hash = hashDivisao(func->cod, bucketSize);
    fprintf(stderr, "Hash: %d, Index: %i, Codigo: %d\n", hash, i, func->cod);
    salva(func, hashArq);
    buckets[hash] += 1;
  }
  printf("\n\nHASH TABLE\n\n");
  for (i = 0; i < bucketSize; i++)
  {
    printf("bucket[%d] has %d elements\n", i, buckets[i]);
  }
  fclose(arq);
  fclose(hashArq);
  free(func);
}

void hashFunction2()
{ // Hash table com tratamento de colisao por End. aberto linear
  FILE *arq;
  FILE *hashArq2;
  TFunc *func;
  arq = fopen("dados.db", "rb");
  hashArq2 = fopen("hash2.db", "wb+");

  int i;
  int bucketSize = 7;
  int buckets[bucketSize];
  int hash;
  for (i = 0; i < bucketSize; i++)
  {
    buckets[i] = 0;
  }
  for (i = 1; i <= 100; i++)
  {
    fseek(arq, (i - 1) * sizeof(TFunc), SEEK_SET);
    func = le(arq);
    hash = hashDivisao(func->cod, bucketSize);
    while (buckets[hash] != 0)
      hash = hashDivisao(hash + 1, bucketSize);
    fprintf(stderr, "Hash: %d, Index: %i, Codigo: %d\n", hash, i, func->cod);
    salva(func, hashArq2);
    buckets[hash] += 1;
    if ((i) >= bucketSize)
    {
      printf("\nOVERFLOW, nao eh possivel adicionar mais elementos na tabela!\n");
      break;
    }
  }
  printf("\n\nHASH TABLE\n\n");
  for (i = 0; i < bucketSize; i++)
  {
    printf("bucket[%d] has %d elements\n", i, buckets[i]);
  }
  fclose(arq);
  fclose(hashArq2);
  free(func);
}

void menu()
{
  printf("\n***\tMENU\t***");
  char op = '.';
  int ch, elemento;

  while (op != '0')
  {
    printf("\n1 - Criar base de dados\n");
    printf("2 - Executar questao 2\n");
    printf("3 - Executar questoes 3 e 4\n");
    printf("4 - Buscar elemento\n");
    printf("5 - Excluir elemento\n");
    printf("6 - Exibir arvore\n");
    printf("7 - Particionar o arquivo pela Selecao Natural\n");
    printf("8 - HASH TABLE\n");
    printf("9 - Linear probing HASH TABLE\n");
    printf("0 - Sair\n\nOpcao: ");
    scanf(" %c", &op);

    switch (op)
    {
    case '1':
      questao1();
      break;
    case '2':
      questao2();
      break;
    case '3':
      questoes3e4();
      break;
    case '4':
      printf("\nDigite o valor a ser buscado: ");
      scanf("%d", &elemento);
      int retorno = buscaElemento(elemento, &ch, raiz);
      if (retorno == 0)
        printf("\nElemento não encontrado");
      break;
    case '5':
      printf("\nDigite o valor a ser excluido: ");
      scanf("%d", &elemento);
      excluirElemento(elemento, raiz);
      break;
    case '6':
      exibicao(raiz);
      printf("\n");
      break;
    case '7':
      selecaoNatural(6, 6);
      break;
    case '8':
      hashFunction();
      break;
    case '9':
      hashFunction2();
      break;
    }
  }
}

int main()
{
  menu();
  return 0;
}
