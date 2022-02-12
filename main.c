#include <stdio.h>
#include <stdlib.h>

typedef struct no
{
    int dado;
    struct no *esq, *dir;    
}No;

//Rotação Simples à Esquerda
void RSE(No **R)
{
	No *p;
	
	printf("RSE(%i)\n", (*R)->dado);

	p = (*R)->dir;
	(*R)->dir = p->esq;
	p->esq = *R;

	*R = p;
}

//Rotação Simples à Direita
void RSD(No **R)
{
	No *p;
	printf("RSD(%i)\n", (*R)->dado);
	p = (*R)->esq;
	(*R)->esq = p->dir;
	p->dir = *R;

	*R = p;
}

//Rotação Dupla à Direita
void RDD(No **R)
{
	RSE(&(*R)->esq);
	RSD(R);
}

//Rotação Dupla à Esquerda
void RDE(No **R)
{
	RSD(&(*R)->dir);
	RSE(R);
}

int altura(No **R)
{
	if(*R == NULL) return -1;
	if(altura(&(*R)->esq) > altura(&(*R)->dir))
	{
		return altura(&(*R)->esq) + 1;
	}
	else
	{
		return altura(&(*R)->dir) + 1;
	}
}

int fat_balanceamento(No **R)
{
	if(*R == NULL) return 0;

	return altura(&(*R)->esq - altura(&(*R)->dir));
}

void corrige(No **R)
{
	if(fat_balanceamento(R) > 1)
	{
		//Fazer correção da rotação para a direita
		if(fat_balanceamento(&(*R)->esq) < 0)
		{
			//Correção RDD
			RDD(R);
		}
		else
		{
			//Correção RSD
			RSD(R);
		}
	}
	else
	{
		if(fat_balanceamento(R) < -1)
		{
		//Fazer correção da rotação para a esquerda
			if(fat_balanceamento(&(*R)->dir) > 0)
			{
				//Correção RDE
				RDE(R);
			}
			else
			{
				//Correção RSE
				RSE(R);
			}
		}
	}
}

int inserir(No **R, int valor)
{
	if (*R == NULL)
	{
		//Preparação do nó
		No *novo;
		novo = (No *) malloc(sizeof(No));
		if (novo == NULL) return 0;
		novo->esq = NULL;
		novo->dir = NULL;
		novo->dado = valor;		

		//Encadeando o novo nó com a raiz
		*R = novo;
		return 1;
	}
	else
	{
		int aux;
		if (valor > (*R)->dado)
		{
			// Chamando a subárvore direita
			aux = inserir(&(*R)->dir, valor);
		}
		else
		{
			// Chamando a subárvore esquerda
			aux = inserir(&(*R)->esq, valor);
		}
		corrige(R);
		return aux;
	}
}

void pre_ordem(No **R)
{
    if(*R != NULL)
    {
        printf("%i [Altura: %i Fator: %i]\n", (*R)->dado, altura(R), fat_balanceamento(R));
        pre_ordem(&(*R)->esq);
        pre_ordem(&(*R)->dir);
    }
}

void em_ordem(No **R)
{
    if(*R != NULL)
    {
        em_ordem(&(*R)->esq);
        printf("%i - ", (*R)->dado);
        em_ordem(&(*R)->dir);
    }
}

void pos_ordem(No **R)
{
    if(*R != NULL)
    {
        pos_ordem(&(*R)->esq);
        pos_ordem(&(*R)->dir);
        printf("%i - ", (*R)->dado);
    }
}

int qtd_elementos(No **R)
{
    if(*R == NULL) return 0;

    return 1 +  qtd_elementos(&(*R)->esq) + 
                qtd_elementos(&(*R)->dir);
}

int soma_elementos(No **R)
{
    if(*R == NULL) return 0;

    return (*R)->dado +
                soma_elementos(&(*R)->esq) + 
                soma_elementos(&(*R)->dir);
}

int * maior_elemento_rec(No **R)
{
	if(*R == NULL) return NULL;
	
	if((*R)->dir == NULL) return &(*R)->dado;
	
	return maior_elemento_rec(&(*R)->dir);
}

int * menor_elemento_rec(No **R)
{
	if(*R == NULL) return NULL;
	
	if((*R)->esq == NULL) return &(*R)->dado;
	
	return menor_elemento_rec(&(*R)->esq);
}

void mostrar_folhas(No **R)
{
	if(*R != NULL)
	{
		if((*R)->esq == NULL && (*R)->dir == NULL)
			printf("%i - ", (*R)->dado);
			
		mostrar_folhas(&(*R)->esq);
		mostrar_folhas(&(*R)->dir);
	}
}

int remover_maior(No **R)
{
	if(*R == NULL) return 0;

	if((*R)->dir == NULL)
	{
		No *aux;
		aux = *R;
		*R = (*R)->esq;
		free(aux);

		return 1;
	}
	return remover_maior(&(*R)->dir);
}

int remover_por_valor(No **R, int valor)
{
	if(*R == NULL) return 0;

	if ((*R)->dado == valor)
	{
		/* Quando for encontrado o elemento a ser removido */
		if((*R)->esq == NULL)
		{
			/*Caso 1*/
			No *aux;
			aux = *R;
			*R = (*R)->dir;
			free(aux);
			
			return 1;
		}
		else
		{
			/*Caso 2*/
			if ((*R)->dir == NULL)
			{
				No *aux;
				aux = *R;
				*R = (*R)->esq;
				free(aux);
				
				return 1;
			}
			else
			{
				/*Caso 3*/
				(*R)->dado = * maior_elemento_rec(&(*R)->esq);
				
				return remover_maior(&(*R)->esq);
			}			
		}
	}
	else
	{
		if(valor > (*R)->dado)
		{
			/*Buscar do lado direito da raiz*/
			remover_por_valor(&(*R)->dir, valor);
		}
		else
		{
			/*Buscar do lado esquerdo da raiz*/
			remover_por_valor(&(*R)->esq, valor);
		}
	}	
}

int menu()
{
	int opc;

	system("clear");

	printf("-------------------- MENU --------------------\n");
	printf("[0] - Sair\n");
	printf("[1] - Inserir\n");
	printf("[2] - Pré-ordem\n");
	printf("[3] - Em-ordem\n");
	printf("[4] - Pós-ordem\n");
	printf("[5] - Quantidade de elementos\n");
	printf("[6] - Maior elemento\n");
	printf("[7] - Menor elemento\n");
	printf("[8] - Remover o maior elemento\n");
	printf("[9] - Remover um elemento qualquer\n");
	printf("[10] - Mostrar folhas\n");
	printf("[11] - Somar elementos\n");
	printf("[12] - Encontrar comprimento de um nó\n");
	printf("[13] - Mostrar caminho de um nó\n");
	printf("[14] - Encontrar a altura da árvore\n");
	printf("[15] - Mostrar nó de um nível qualquer\n");
	printf("[16] - Quantidade de nós em um nível\n");
	printf("[17] - Mostrar nó(s) mais distante(s)\n");
	printf("[18] - Quantidade de nós último nível\n");
	printf("[19] - Balancear árvore à esquerda\n");
	printf("[20] - Balancear árvore à direita\n");
	printf("[21] - Rotacionar à esquerda\n");
	printf("[22] - Rotacionar à direita\n");
	printf("[23] - Rotação dupla à esquerda\n");
	printf("[24] - Rotação dupla à direita\n");
	printf("[25] - Indicar balanceamento perfeito\n");
	printf("\n");
	printf("Escolha uma opção: ");
	scanf("%i", &opc);
	return opc;
}

int main()
{
	No *R = NULL;

	inserir(&R, 10);
	inserir(&R, 15);
	inserir(&R, 5);
	inserir(&R, 20);

	printf("Árvore - Percurso Pré-ordem\n");
	pre_ordem(&R);
	
	
/*	No *R = NULL;
	int opc, valor;
	int *aux;

	while(opc = menu())
	{
		switch(opc)
		{
			case 1:
				printf("Digite um número: ");
				scanf("%i", &valor);
				inserir(&R, valor);
				printf("%i inserido!\n", valor);
			break;
			case 2:
				if (R == NULL)
					printf("A árvore está vazia!\n");
				else
					pre_ordem(&R);
			break;
			case 3:
				if (R == NULL)
					printf("A árvore está vazia!\n");
				else
					em_ordem(&R);
			break;
			case 4:
				if (R == NULL)
					printf("A árvore está vazia!\n");
				else
					pos_ordem(&R);
			break;
			case 5:
				printf("A quantidade de elementos é: %i\n", qtd_elementos(&R));
			break;
			case 6:
				printf("Maior elemento\n");
				aux = maior_elemento_rec(&R);
				if(aux == NULL)
					printf("A árvore está vazia!\n");
				else
					printf("Maior elemento: %i\n", *aux);
			break;
			case 7:
				printf("Menor elemento\n");
				aux = menor_elemento_rec(&R);
				if(aux == NULL)
					printf("A árvore está vazia!\n");
				else
					printf("Menor elemento: %i\n", *aux);
			break;
			case 8:
				printf("Remover o maior elemento\n");
				if (remover_maior(&R))
					printf("O maior elemento foi removido!\n");
				else
					printf("Árvore sem elementos!\n");				
			break;
			case 9:
			
			break;
			case 10:
				printf("Mostrar folhas\n");
				if(R == NULL)
					printf("A árvore está vazia!\n");
				else
					mostrar_folhas(&R);
			break;
			case 11:
				printf("Soma de Elementos\n");
				printf("Soma: %i\n", soma_elementos(&R));
			break;

			case 14:
				printf("Verificar altura da árvore\n");
				pre_ordem(&R);
				//printf("\n");
				//printf("Altura: %i", altura(&R));
			break;
			
			case 19:
				printf("Verificar se a árvore está balanceada à esquerda\n");
				if (R == NULL)
				{
					printf("A árvore está vazia!\n");
				}
				else
				{
					printf("Balanceamento à Esquerda\n");
					printf("Antes\n");
					pre_ordem(&R);
					RSE(&R);
					printf("\n");
					printf("Depois\n");
					pre_ordem(&R);
					RSE(&R);
				}				
			break;
			case 20:
				printf("Verificar se a árvore está balanceada à direita\n");
				if (R == NULL)
				{
					printf("A árvore está vazia!\n");
				}
				else
				{
					printf("Balanceamento à Direita\n");
					printf("Antes\n");
					pre_ordem(&R);
					RSD(&R);
					printf("\n");
					printf("Depois\n");
					pre_ordem(&R);
					RSD(&R);
				}				
			break;
			case 21:
				printf("Rotacionar árvore à esquerda\n");
				if (R == NULL)
				{
					printf("A árvore está vazia!\n");
				}
				else
				{
					printf("Antes\n");
					pre_ordem(&R);
					RDE(&R);
					printf("\n");
					printf("Depois\n");
					pre_ordem(&R);
				}				
			break;
			case 22:
				printf("Rotacionar árvore à direita\n");
				if (R == NULL)
				{
					printf("A árvore está vazia!\n");
				}
				else
				{
					printf("Antes\n");
					pre_ordem(&R);
					RDD(&R);
					printf("\n");
					printf("Depois\n");
					pre_ordem(&R);
				}				
			break;
			case 23:
				printf("Rotação dupla da árvore à esquerda\n");
				if (R == NULL)
				{
					printf("A árvore está vazia!\n");
				}
				else
				{
					printf("Antes\n");
					pre_ordem(&R);
					RDE(&R);
					printf("\n");
					printf("Depois\n");
					pre_ordem(&R);
				}				
			break;
			case 24:
				printf("Rotação dupla da árvore à direita\n");
				if (R == NULL)
				{
					printf("A árvore está vazia!\n");
				}
				else
				{
					printf("Antes\n");
					pre_ordem(&R);
					RDD(&R);
					printf("\n");
					printf("Depois\n");
					pre_ordem(&R);
				}				
			break;
			default:
				printf("Opção inválida.\n");
		}
		getchar();
		getchar();
	}*/
}
