# Planejamento de Rota no Piauí com Grafos

## 1. Objetivo do Projeto

Este projeto implementa uma solução em linguagem C para modelar e analisar um conjunto de rotas rodoviárias no estado do Piauí. O sistema representa as cidades como vértices e as estradas como arestas direcionadas em um grafo.

O objetivo principal é desenvolver uma função capaz de:
* Encontrar **todos os caminhos possíveis** entre uma cidade de origem (Corrente) e um destino (Parnaíba).
* Identificar, dentre todos os caminhos, qual é o **caminho mais curto** (com menos trechos).
* Identificar qual é o **caminho mais longo**.

## 2. Descrição do Problema

A jornada de um viajante, partindo de **Corrente** com destino a **Parnaíba**, é modelada através de um grafo direcionado.

#### Cidades (Vértices)
As cidades são representadas por índices numéricos para facilitar a manipulação no grafo:

| Índice | Cidade         |
| :----: | :------------- |
|   0    | Corrente       |
|   1    | Gilbués        |
|   2    | Bom Jesus      |
|   3    | Floriano       |
|   4    | Oeiras         |
|   5    | Teresina       |
|   6    | Campo Maior    |
|   7    | Piripiri       |
|   8    | Luís Correia   |
|   9    | Parnaíba       |

#### Estradas (Arestas)
As conexões diretas entre as cidades são:
* `Corrente → Gilbués`
* `Gilbués → Bom Jesus`
* `Bom Jesus → Floriano`
* `Floriano → Oeiras`
* `Oeiras → Teresina`
* `Teresina → Campo Maior`
* `Campo Maior → Piripiri`
* `Piripiri → Luís Correia`
* `Luís Correia → Parnaíba`

E também as rotas alternativas:
* `Bom Jesus → Teresina` (um atalho que pula Floriano e Oeiras)
* `Teresina → Parnaíba` (uma rota direta que pula o trecho norte)

## 3. Estrutura de Dados e Representação

Para modelar o mapa, foi utilizada a representação de **Lista de Adjacência**.

Essa estrutura foi escolhida porque é ideal para grafos esparsos (onde o número de arestas é muito menor que o número de vértices ao quadrado). Nesse caso, tem-se poucas estradas conectando as cidades, o que torna a lista de adjacência mais eficiente no quesito memória do que uma matriz de adjacência.

* **Implementação:**
    * `struct NoAdjacencia`: Trata-se da Representação do nó de uma lista, contendo o índice da cidade de `destino` e um ponteiro para o `proximo` vizinho.
    * `struct Grafo`: Sendo a estrutura principal, ela armazena o número de vértices, os nomes das cidades e um array de ponteiros, sendo que cada ponteiro é a cabeça de uma lista de adjacência para um vértice.

## 4. Explicação da Lógica Principal

A solução implementada para encontrar os caminhos foi baseada no algoritmo de **Busca em Profundidade (DFS - Depth-First Search)**, em combinação com a técnica de **Backtracking**. 
A lógica foi separada em duas funções principais para ter uma maior clareza e organização.

#### 4.1. `encontrarCaminhos()` - A Função Orquestradora

Esta função não faz a busca diretamente, porém prepara todo o ambiente necessário para ela:
1.  **Inicializa as Estruturas de Apoio:** São criados os vetores que serão usados pela busca recursiva:
    * `visitados[]`: Array booleano que marca as cidades que já fazem parte do caminho *atual*, evitando ciclos que sejam infinitos (ex: A → B → A).
    * `caminhoAtual[]`: Um array int para armazenar a sequência de cidades do caminho que está sendo explorado no momento.
    * `todosOsCaminhos[][]` e `comprimentosDosCaminhos[]`: Duas Estruturas que armazenam de forma permanente cada caminho válido encontrado da origem ao destino, junto com seu equivalente comprimento.
2.  **Inicia a Busca:** A chamada inicial é feita para a função recursiva `buscarCaminhosRecursivo`, que começa pela cidade de origem.
3.  **Analisa e Exibe os Resultados:** Após a busca ser concluida, esta função percorre os resultados que foram armazenados, faz a impressão todos os caminhos encontrados e, e então identifica e exibe o mais curto e o mais longo com base nos comprimentos guardados.

#### 4.2. `buscarCaminhosRecursivo()` - O Centro da Busca

Essa função recursiva é responsável por explorar cada "galho" do grafo até se encontre o destino ou um beco sem saída.

O processo para cada cidade visitada (`u`) é:

1.  **Marcar e Adicionar:** A cidade `u` é marcada como `visitada` e adicionada ao `caminhoAtual`.
2.  **Verificar o Destino :** Se `u` for a cidade de destino, significa que um caminho completo foi encontrado. Este caminho é copiado do `caminhoAtual` para a matriz `todosOsCaminhos`.
3.  **Explorar Vizinhos (Passo Recursivo):** Se `u` não for o destino, o algoritmo percorre sua lista de adjacência. Para cada vizinho que ainda **não foi visitado** no percurso atual, a função chama a si mesma, aprofundando a busca nesse novo ramo.
4.  **Backtracking:** Este é o passo mais importante. Após a exploração de todos os ramos a partir de `u` ter sido concluída (ou seja, quando a chamada recursiva retorna), a cidade `u` é **desmarcada** (`visitados[u] = false`). Isso "libera" a cidade, permitindo que ela possa ser incluída em **outros caminhos** que venham a ser explorados. Sem o backtracking, o algoritmo pararia após encontrar o primeiro caminho.

Essa abordagem assegura que todos os caminhos possíveis sejam explorados de forma correta.

