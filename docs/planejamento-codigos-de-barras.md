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

- [x] Baixar o export CSV e isolar a coluna `code` (**lida como `dtype=str`** — ver nota crítica abaixo).
- [x] Filtrar apenas códigos que sejam **EAN-13 válidos**: exatamente 13 dígitos numéricos e checksum (dígito verificador) correto conforme o algoritmo padrão GS1.
- [x] Remover duplicatas.
- [ ] Definir o tamanho da amostra para os experimentos (ex.: subconjuntos de 1k, 10k, 100k códigos) para permitir análise de escalabilidade.
- [ ] Verificar a distribuição de prefixos GS1 (país/organização) na amostra, para eventualmente ilustrar a utilidade de busca por prefixo.

### ⚠️ Nota crítica: coluna `code` deve ser lida como string

A coluna `code` do CSV é numérica por natureza, e se lida como `int64` (comportamento padrão do pandas), **zeros à esquerda são silenciosamente perdidos** — um EAN-13 como `0611103500043` vira o inteiro `611103500043` (12 dígitos), tornando-se indistinguível de um UPC-A genuíno. Isso inflava artificialmente as contagens de códigos com 5, 6, 7, 10, 11 e 12 dígitos na primeira inspeção. Ao reler com `dtype=str`, a distribuição real revelou apenas **dois formatos presentes na base**: EAN-8 e EAN-13 — sem sujeira adicional (0 valores nulos, 0 não-numéricos). **Sempre carregar a coluna `code` como string desde a leitura do CSV.**

### Resultado da limpeza (amostra utilizada)

| Etapa | Quantidade |
| --- | --- |
| Códigos com 13 dígitos (formato EAN-13) | 8878 |
| Códigos com 8 dígitos (formato EAN-8 — fora do escopo do trabalho) | 926 |
| EAN-13 com checksum válido | 8874 |
| EAN-13 com checksum inválido (descartados) | 4 |
| **EAN-13 válidos e únicos (dataset final)** | **8874*** |

\* Os 4 códigos inválidos (`0005100000100`, `2000000002603`, `0510000010000`, `2000000037124`) seguem o padrão de prefixos `00`/`05`/`20`, característicos de **RCN — Restricted Circulation Numbers**: códigos internos de varejo (ex.: produtos pesados no açougue/hortifruti), reservados pelo GS1 para uso interno e sem checksum padronizado globalmente. Não são erro do dataset — foram descartados por estarem fora do escopo de "EAN-13 de catálogo".

**Decisão de escopo:** os 926 códigos EAN-8 foram **excluídos** da amostra principal. O trabalho foca exclusivamente em EAN-13 (13 dígitos), conforme as decisões de design da árvore já definidas (52 bits, 26 níveis na árvore m=4). A exclusão do EAN-8 deve constar como critério de escopo, não como limitação, na seção de Aplicação Computacional do artigo.

### Arquivo de entrada para o programa em C

O dataset final é exportado como um arquivo de texto simples — **um código EAN-13 por linha, sem cabeçalho, sem separadores** — formato ideal para leitura direta em C sem necessidade de parser:

```python
# Filtrar apenas EAN-13 válidos
df_final = df13[df13["valido"]].copy()

# Remover duplicatas
df_final = df_final.drop_duplicates(subset="code")

# Conferência final
assert df_final["code"].str.len().eq(13).all()
assert df_final["code"].str.isdigit().all()

# Salvar: um código por linha, texto puro
df_final["code"].to_csv("ean13_dataset.txt", index=False, header=False)
```

Arquivo resultante (`ean13_dataset.txt`):

```txt
0611103500043
7891000100103
3017620422003
...
```

Esse é o arquivo oficial de entrada para os experimentos de inserção/busca nas duas árvores.

## Decisões de design da árvore

- [x] **Granularidade da chave:** o código de barras é tratado como uma **string única de 13 símbolos** sobre o alfabeto numérico (0–9). A árvore não distingue os campos semânticos do EAN-13 (prefixo GS1 / fabricante / produto / dígito verificador) — ela vê 13 símbolos genéricos tratados sempre da mesma forma. Essa escolha mantém a árvore digital genérica e reutilizável, favorecendo o critério de "facilidade de implementação"; a semântica dos campos pode ser explorada separadamente na motivação/discussão de busca por prefixo.
- [x] **Codificação de símbolo → bits:** cada dígito (0–9) pode ser representado com **4 bits (BCD / half-byte)**, já que 2⁴ = 16 ≥ 10 — diferente do caso anterior (siglas alfanuméricas), aqui a codificação mínima teórica é natural e simples de implementar (sem necessidade de tabela customizada), o que elimina o trade-off de overhead que existia com ASCII de 8 bits. Isso deve ser destacado como uma vantagem de simplicidade em relação ao tema anterior considerado pelo grupo.
  - Profundidade resultante por código (13 dígitos × 4 bits = 52 bits):
    - Árvore binária: até **52 níveis**
    - Árvore m-ária (m=4): até **26 níveis**
- [x] **Agrupamento de bits na árvore m=4:** os nós da árvore 4-ária consomem **pares de bits sequenciais da mesma representação de 4 bits por dígito** usada na árvore binária. Cada par de bits (`00`, `01`, `10`, `11`) indexa um dos 4 filhos do nó. Isso garante que as duas árvores armazenem exatamente a mesma informação, diferindo apenas na quantidade de bits consumida por nível (1 bit na binária vs. 2 bits na m=4) — isolando o fator de ramificação como a única variável comparada entre as estruturas.

## Pendências restantes

- [x] ~~Baixar e inspecionar uma amostra real do CSV do Open Food Facts~~ — feito; formato real confirmado (EAN-8 e EAN-13, sem UPC-A/mistura de padrões nessa amostra).
- [x] ~~Decidir tratamento para códigos UPC-A (12 dígitos)~~ — não se aplicou; a base não continha UPC-A genuíno (as ocorrências de 12 dígitos eram artefato da leitura como `int64`, corrigidas ao reler como string).
- [ ] Definir o(s) tamanho(s) de amostra para os experimentos (1k / 10k / 100k) a partir dos 8874 códigos disponíveis.

## Exemplo de leitura do dataset em C

Leitura do `ean13_dataset.txt` (13 dígitos + `\n` por linha) usando buffer de tamanho fixo, pronta para alimentar a inserção nas duas árvores:

```c
#include <stdio.h>
#include <string.h>

#define TAM_CODIGO 13

int main() {
    FILE *f = fopen("ean13_dataset.txt", "r");
    if (!f) {
        perror("erro ao abrir arquivo");
        return 1;
    }

    char code[TAM_CODIGO + 1]; // 13 dígitos + '\0'
    int count = 0;

    while (fscanf(f, "%13s", code) == 1) {
        // inserir 'code' na árvore digital (binária e m-ária)
        // ex.: arvore_binaria_inserir(raiz_bin, code);
        //      arvore_m4_inserir(raiz_m4, code);
        count++;
    }

    printf("Total de códigos lidos: %d\n", count);

    fclose(f);
    return 0;
}
```

Notas de implementação:

- `%13s` no `fscanf` limita a leitura a 13 caracteres por chamada, evitando overflow de buffer mesmo que uma linha esteja malformada.
- O buffer `code[14]` reserva o byte extra para o terminador `'\0'`, já que `fscanf` com `%s` sempre insere o terminador automaticamente.
- Para a etapa de codificação em bits (BCD, 4 bits por dígito — ver "Decisões de design da árvore"), cada caractere de `code` pode ser convertido com `code[i] - '0'` para obter o valor numérico do dígito antes de extrair os bits.

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
