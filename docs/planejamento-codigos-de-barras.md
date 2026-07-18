# Planejamento do Projeto — Árvores Digitais para Códigos de Barras (EAN-13)

## Objetivo

Implementar e comparar duas estruturas de Árvore Digital — **binária** e **m-ária (m=4)** — usando como estudo de caso real **códigos de barras EAN-13** de produtos, conforme o padrão internacional GS1/GTIN. O objetivo final é concluir qual estrutura é mais efetiva em termos de:

1. Complexidade de tempo (teórica)
2. Complexidade de espaço (teórica)
3. Tempo prático de execução das operações
4. Facilidade de implementação

## Por que esse tema é palpável

- É um estudo de caso **real, documentado e com motivação orgânica**: o padrão EAN-13 é definido por norma internacional (GS1), amplamente usado em varejo, inventário e logística — sistemas de ponto de venda (POS) e controle de estoque realmente fazem lookup e busca por prefixo em códigos de barras, o que dá uma justificativa de aplicação genuína para o uso de árvores digitais (diferente de forçar a estrutura sobre um domínio que não pede por ela).
- **Busca por prefixo tem sentido de negócio real**: os primeiros dígitos do EAN-13 (GS1 prefix) identificam o país/organização emissora, e os dígitos seguintes identificam o fabricante — logo, agrupar/buscar por prefixo numa árvore digital corresponde a uma operação útil (ex.: "todos os produtos desse fabricante").
- O alfabeto de símbolos é o menor possível: **apenas dígitos (0–9)**, com todos os códigos de comprimento fixo de **13 caracteres**.
- Existe base de dados pública, real e volumosa disponível para download, o que garante significância estatística nos experimentos práticos de tempo de execução.
- Facilita comparar diretamente uma árvore binária (decompondo cada dígito em bits) contra uma árvore 4-ária (agrupando por pares de bits), isolando o fator de ramificação como única variável comparada.

## Base de dados escolhida: Open Food Facts

- **Fonte:** [Open Food Facts](https://world.openfoodfacts.org/data) — projeto colaborativo aberto, com dados de produtos alimentícios de todo o mundo, contribuído no modelo "Wikipedia dos produtos".
- **Formato:** exports (dumps) gerados diariamente, disponíveis em CSV e JSONL. Para este projeto, usar o dataset em **CSV** (~800 MB), que contém os campos principais — é mais leve que o JSONL (>5 GB) e já traz a coluna de código de barras.
- **Coluna relevante:** `code` — contém o código de barras do produto (EAN-13 ou UPC).
- **Acesso alternativo:** SDK oficial em Python (`openfoodfacts` no PyPI), que baixa e faz cache do dataset automaticamente via `ProductDataset(dataset_type="csv")`.
- **Licença/uso:** dados abertos, projeto incentiva reuso acadêmico e pede apenas que reusos sejam reportados (não obrigatório).
- **Justificativa de escolha:** citada de forma independente por um dos integrantes do grupo antes da pesquisa formal de bases de dados, o que reforça a adequação — dataset real, de grande escala, com barcode como campo de primeira classe, sem necessidade de scraping ou geração sintética.

## Dados a confirmar/preparar a partir do dataset

- [ ] Baixar o export CSV mais recente e isolar a coluna `code`.
- [ ] Filtrar apenas códigos que sejam **EAN-13 válidos**: exatamente 13 dígitos numéricos e checksum (dígito verificador) correto conforme o algoritmo padrão GS1.
- [ ] Remover duplicatas e valores nulos/malformados (esperado em dataset colaborativo — documentar o percentual descartado como parte da seção de Aplicação Computacional).
- [ ] Definir o tamanho da amostra para os experimentos (ex.: subconjuntos de 1k, 10k, 100k códigos) para permitir análise de escalabilidade.
- [ ] Verificar a distribuição de prefixos GS1 (país/organização) na amostra, para eventualmente ilustrar a utilidade de busca por prefixo.

## Decisões de design da árvore

- [x] **Granularidade da chave:** o código de barras é tratado como uma **string única de 13 símbolos** sobre o alfabeto numérico (0–9). A árvore não distingue os campos semânticos do EAN-13 (prefixo GS1 / fabricante / produto / dígito verificador) — ela vê 13 símbolos genéricos tratados sempre da mesma forma. Essa escolha mantém a árvore digital genérica e reutilizável, favorecendo o critério de "facilidade de implementação"; a semântica dos campos pode ser explorada separadamente na motivação/discussão de busca por prefixo.
- [x] **Codificação de símbolo → bits:** cada dígito (0–9) pode ser representado com **4 bits (BCD / half-byte)**, já que 2⁴ = 16 ≥ 10 — diferente do caso anterior (siglas alfanuméricas), aqui a codificação mínima teórica é natural e simples de implementar (sem necessidade de tabela customizada), o que elimina o trade-off de overhead que existia com ASCII de 8 bits. Isso deve ser destacado como uma vantagem de simplicidade em relação ao tema anterior considerado pelo grupo.
  - Profundidade resultante por código (13 dígitos × 4 bits = 52 bits):
    - Árvore binária: até **52 níveis**
    - Árvore m-ária (m=4): até **26 níveis**
- [x] **Agrupamento de bits na árvore m=4:** os nós da árvore 4-ária consomem **pares de bits sequenciais da mesma representação de 4 bits por dígito** usada na árvore binária. Cada par de bits (`00`, `01`, `10`, `11`) indexa um dos 4 filhos do nó. Isso garante que as duas árvores armazenem exatamente a mesma informação, diferindo apenas na quantidade de bits consumida por nível (1 bit na binária vs. 2 bits na m=4) — isolando o fator de ramificação como a única variável comparada entre as estruturas.

## Pendências restantes

- [ ] Baixar e inspecionar uma amostra real do CSV do Open Food Facts para validar formato exato da coluna `code` (com/sem zeros à esquerda, presença de UPC-12 misturado com EAN-13, etc.).
- [ ] Decidir tratamento para códigos UPC-A (12 dígitos): normalizar prependando `0` (equivalente a EAN-13) ou excluir da amostra — documentar a decisão tomada.

## Passos de desenvolvimento

### 1. Definição e coleta de dados

- Baixar o export CSV do Open Food Facts (ou usar o SDK Python `openfoodfacts`).
- Extrair e padronizar a coluna `code`: validar comprimento (13 dígitos) e checksum EAN-13.
- Descartar/documentar registros inválidos ou duplicados.

### 2. Modelagem das estruturas de dados

- Implementar **Árvore Digital Binária** (Trie binária sobre a representação em bits de cada dígito).
- Implementar **Árvore Digital m-ária (m=4)** (Trie 4-ária, agrupando pares de bits).
- Definir operações a implementar em ambas: inserção, busca, remoção (se aplicável), e travessia/listagem (incluindo busca por prefixo, dado o significado real do prefixo GS1).

### 3. Análise teórica

- Complexidade de tempo (inserção, busca) para cada estrutura, em função de `n` (nº de códigos) e do comprimento da chave.
- Complexidade de espaço: nº de nós, overhead de ponteiros/filhos por nó (2 vs. 4 filhos).
- Discussão sobre profundidade da árvore e fator de ramificação.

### 4. Experimentos práticos

- Implementar rotina de benchmark: inserção em massa, busca (hit/miss), busca por prefixo, medição de tempo de execução.
- Rodar experimentos com o mesmo dataset (mesmas amostras) nas duas estruturas.
- Coletar métricas: tempo médio de operação, uso de memória, profundidade média/máxima da árvore.
- Gerar gráficos/tabelas comparativas, variando o tamanho da amostra (1k / 10k / 100k) para observar escalabilidade.

### 5. Revisão bibliográfica

- Buscar artigos/dissertações indexados (Qualis) sobre árvores digitais, tries, PATRICIA trees, comparações m-árias vs. binárias, e aplicações em sistemas de identificação de produtos (GTIN/EAN/UPC).
- Selecionar 4–6 referências relevantes e atualizadas.
