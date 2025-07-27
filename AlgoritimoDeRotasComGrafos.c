#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define NUM_CIDADES 10
#define MAX_CAMINHOS 50



typedef struct NoAdjacencia {
    int destino;
    struct NoAdjacencia* proximo;
} NoAdjacencia;


typedef struct Grafo {
    int numVertices;
    struct NoAdjacencia** listaAdj; 
    const char** nomesCidades;    
} Grafo;


NoAdjacencia* criarNo(int destino) {
    NoAdjacencia* novoNo = (NoAdjacencia*)malloc(sizeof(NoAdjacencia));
    novoNo->destino = destino;
    novoNo->proximo = NULL;
    return novoNo;
}


Grafo* criarGrafo(int V, const char* cidades[]) {
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
    grafo->numVertices = V;
    grafo->nomesCidades = cidades;
    
    grafo->listaAdj = (NoAdjacencia**)malloc(V * sizeof(NoAdjacencia*));
    
    int i;
    for ( i = 0; i < V; i++) {
        grafo->listaAdj[i] = NULL;
    }
    
    return grafo;
}


void adicionarAresta(Grafo* grafo, int origem, int destino) {
    NoAdjacencia* novoNo = criarNo(destino);
    novoNo->proximo = grafo->listaAdj[origem];
    grafo->listaAdj[origem] = novoNo;
}


void imprimirGrafo(Grafo* grafo) {
    printf("--- Representacao do Grafo (Listas de Adjacencia) ---\n");
    int v;
    for ( v = 0; v < grafo->numVertices; v++) {
        NoAdjacencia* temp = grafo->listaAdj[v];
        printf("Cidade %d (%s): ", v, grafo->nomesCidades[v]);
        while (temp) {
            printf("-> %s ", grafo->nomesCidades[temp->destino]);
            temp = temp->proximo;
        }
        printf("\n");
    }
    printf("------------------------------------------------------\n\n");
}



void buscarCaminhosRecursivo(
    Grafo* grafo, 
    int u, 
    int destino, 
    bool visitados[], 
    int caminhoAtual[], 
    int indiceCaminho, 
    int todosOsCaminhos[][NUM_CIDADES], 
    int comprimentosDosCaminhos[],
    int* totalCaminhosEncontrados 
) {
    
    visitados[u] = true;
    caminhoAtual[indiceCaminho] = u;
    
    
    if (u == destino) {
        int caminhoIdx = *totalCaminhosEncontrados;
        int i;
        for ( i = 0; i <= indiceCaminho; i++) {
            todosOsCaminhos[caminhoIdx][i] = caminhoAtual[i];
        }
        comprimentosDosCaminhos[caminhoIdx] = indiceCaminho;
        (*totalCaminhosEncontrados)++;
    } else {
        NoAdjacencia* vizinho = grafo->listaAdj[u];
        while (vizinho != NULL) {
            if (!visitados[vizinho->destino]) {
                buscarCaminhosRecursivo(grafo, vizinho->destino, destino, visitados, caminhoAtual, indiceCaminho + 1,
                                        todosOsCaminhos, comprimentosDosCaminhos, totalCaminhosEncontrados);
            }
            vizinho = vizinho->proximo;
        }
    }
    visitados[u] = false;
}


void encontrarCaminhos(Grafo* grafo, int inicio, int fim) {
    printf("Buscando caminhos de %s para %s...\n\n", grafo->nomesCidades[inicio], grafo->nomesCidades[fim]);

    
    bool visitados[NUM_CIDADES] = {false}; 
    int caminhoAtual[NUM_CIDADES]; 
    int todosOsCaminhos[MAX_CAMINHOS][NUM_CIDADES];
    int comprimentosDosCaminhos[MAX_CAMINHOS];
    int totalCaminhosEncontrados = 0;

    buscarCaminhosRecursivo(grafo, inicio, fim, visitados, caminhoAtual, 0,
                            todosOsCaminhos, comprimentosDosCaminhos, &totalCaminhosEncontrados);

   
    if (totalCaminhosEncontrados == 0) {
        printf("Nenhum caminho encontrado de %s ate %s.\n", grafo->nomesCidades[inicio], grafo->nomesCidades[fim]);
        return;
    }
    
    printf("--- Todos os %d caminhos possiveis ---\n", totalCaminhosEncontrados);
    int i,j;
    for ( i = 0; i < totalCaminhosEncontrados; i++) {
        printf("Caminho %d: ", i + 1);
        for ( j = 0; j <= comprimentosDosCaminhos[i]; j++) {
            printf("%s", grafo->nomesCidades[todosOsCaminhos[i][j]]);
            if (j < comprimentosDosCaminhos[i]) {
                printf(" -> ");
            }
        }
        printf(" (Total de %d trechos)\n", comprimentosDosCaminhos[i]);
    }

    int idxMaisCurto = 0;
    int idxMaisLongo = 0;
	
    for ( i = 1; i < totalCaminhosEncontrados; i++) {
        if (comprimentosDosCaminhos[i] < comprimentosDosCaminhos[idxMaisCurto]) {
            idxMaisCurto = i;
        }
        if (comprimentosDosCaminhos[i] > comprimentosDosCaminhos[idxMaisLongo]) {
            idxMaisLongo = i;
        }
    }
    
    printf("\n--- Resultados ---\n");

    
    printf("\n>> Caminho mais curto (%d trechos):\n", comprimentosDosCaminhos[idxMaisCurto]);
    for ( i = 0; i <= comprimentosDosCaminhos[idxMaisCurto]; i++) {
        printf("%s", grafo->nomesCidades[todosOsCaminhos[idxMaisCurto][i]]);
        if (i < comprimentosDosCaminhos[idxMaisCurto]) {
            printf(" -> ");
        }
    }
    printf("\n");


    printf("\n>> Caminho mais longo (%d trechos):\n", comprimentosDosCaminhos[idxMaisLongo]);
    for ( i = 0; i <= comprimentosDosCaminhos[idxMaisLongo]; i++) {
        printf("%s", grafo->nomesCidades[todosOsCaminhos[idxMaisLongo][i]]);
        if (i < comprimentosDosCaminhos[idxMaisLongo]) {
            printf(" -> ");
        }
    }
    printf("\n");
}


int main() {
	
    const char* cidades[NUM_CIDADES] = {
        "Corrente", "Gilbues", "Bom Jesus", "Floriano", "Oeiras",
        "Teresina", "Campo Maior", "Piripiri", "Luis Correia", "Parnaiba"
    };

    Grafo* G = criarGrafo(NUM_CIDADES, cidades);

    // 2. Adiconando as arestas (estradas)
    adicionarAresta(G, 0, 1); // Corrente -> Gilbués
    adicionarAresta(G, 1, 2); // Gilbués -> Bom Jesus
    adicionarAresta(G, 2, 3); // Bom Jesus -> Floriano
    adicionarAresta(G, 3, 4); // Floriano -> Oeiras
    adicionarAresta(G, 4, 5); // Oeiras -> Teresina
    adicionarAresta(G, 5, 6); // Teresina -> Campo Maior
    adicionarAresta(G, 6, 7); // Campo Maior -> Piripiri
    adicionarAresta(G, 7, 8); // Piripiri -> Luís Correia
    adicionarAresta(G, 8, 9); // Luís Correia -> Parnaíba
    
    // Outras conexões alternativas
    adicionarAresta(G, 5, 9); // Teresina -> Parnaíba (rota direta)
    adicionarAresta(G, 2, 5); // Bom Jesus -> Teresina (atalho)
    
    // 3. Impressão da estrutura do grafo 
    imprimirGrafo(G);

    // 4. Essa chamada executa a função principal para encontrar e exibir os caminhos
    // origem: 0 (Corrente)
    // destino: 9 (Parnaíba)
    encontrarCaminhos(G, 0, 9);

    // Liberação da memória alocada para o grafo
    free(G->listaAdj);
    free(G);

    return 0;
}
