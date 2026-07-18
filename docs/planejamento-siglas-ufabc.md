# Planejamento do Projeto — Árvores Digitais para Siglas de Disciplinas UFABC

## Objetivo

Implementar e comparar duas estruturas de Árvore Digital — **binária** e **m-ária (m=4)** — usando como estudo de caso real as **siglas das disciplinas de graduação da UFABC**, conforme definidas pela Resolução CG nº 31/2022 (Boletim de Serviço nº 1195). O objetivo final é concluir qual estrutura é mais efetiva em termos de:

1. Complexidade de tempo (teórica)
2. Complexidade de espaço (teórica)
3. Tempo prático de execução das operações
4. Facilidade de implementação

## Por que esse tema é palpável

- É um estudo de caso **real e documentado**: a metodologia de geração das siglas está formalizada em norma institucional oficial, o que dá base sólida e citável para a seção de "Explicação do Problema de Pesquisa".
- O alfabeto de símbolos é pequeno e bem definido: letras (A–Z) + dígitos (0–9) + hífen, todas as siglas com exatamente **10 caracteres**.
- Volume de dados (centenas de siglas) suficiente para gerar experimentos práticos de tempo de execução com significância.
- Facilita comparar diretamente uma árvore binária (decompondo cada símbolo em bits) contra uma árvore 4-ária (agrupando por pares de bits ou por símbolo).

## Dados confirmados a partir do catálogo (`catalogo_disciplinas_graduacao_2024_2025.csv`)

- [x] **Contagem real: 1357 disciplinas**, não 1358. Todas com sigla única (sem duplicatas), todas normalizando para exatamente **10 caracteres**.
- [x] O catálogo é o mais recente (2024-2025) e **todas as 1357 siglas seguem o formato único da Resolução CG nº 31/2022** — não há mistura com formato legado (Resolução ConsEPE nº 150/2013) nesse arquivo. Ou seja, essa pendência está resolvida: a base é uniforme.
- [x] Confirmados os **dois grupos de formato**, com prefixos totalmente distintos (sem sobreposição):
  - **392 siglas** no formato "3 letras + 6 dígitos" (posições 0-2 letras, 3-6 dígitos, 7 hífen, 8-9 ano) — prefixos como `BCJ`, `BHO`, `BIQ`, `LCZ`, `LHE`, `LIZ`, `NHH`, etc. (cursos interdisciplinares).
  - **965 siglas** no formato "4 letras + 5 dígitos" (posições 0-3 letras, 4-6 dígitos, 7 hífen, 8-9 ano) — prefixos como `ESAE`, `ESTS`, `MCBM`, `MCTB`, `NHBB`, etc. (cursos de formação específica).
  - 392 + 965 = 1357 ✅ bate com o total.

## Decisões de design da árvore

- [x] **Granularidade da chave:** a sigla é tratada como uma **string única de 10 símbolos** sobre o alfabeto completo (A-Z, 0-9, hífen), sem distinguir os dois formatos de sigla (3 letras + 6 dígitos vs. 4 letras + 5 dígitos). A árvore não conhece a estrutura do domínio (letra vs. dígito por posição) — ela só vê 10 símbolos genéricos, tratados sempre da mesma forma. Essa escolha evita ramificar a lógica de inserção/busca por grupo e mantém a árvore digital genérica e reutilizável, o que favorece o critério de "facilidade de implementação".
- [x] **Codificação de símbolo → bits:** cada caractere é codificado usando seu **valor ASCII padrão de 8 bits**, em vez de uma tabela customizada de 6 bits (mínimo teórico para 37 símbolos, já que 2⁶=64 ≥ 37). Optamos pelo ASCII pela simplicidade de implementação (usa a conversão nativa da linguagem, sem precisar criar e manter uma tabela de mapeamento símbolo↔código). O trade-off documentado: isso torna a árvore ~33% mais profunda do que o mínimo teórico (80 bits/sigla com ASCII vs. 60 bits/sigla com 6 bits customizados), o que deve ser mencionado na análise de complexidade de espaço e no tempo prático de execução.
  - Profundidade resultante por sigla (10 símbolos × 8 bits = 80 bits):
    - Árvore binária: até **80 níveis**
    - Árvore m-ária (m=4): até **40 níveis**
- [x] **Agrupamento de bits na árvore m=4:** os nós da árvore 4-ária consomem **pares de bits sequenciais da mesma representação ASCII de 8 bits** usada na árvore binária (não uma tabela ou codificação base-4 separada). Cada par de bits (`00`, `01`, `10`, `11`) indexa um dos 4 filhos do nó. Isso garante que as duas árvores armazenem exatamente a mesma informação, diferindo apenas na quantidade de bits consumida por nível (1 bit na binária vs. 2 bits na m=4) — isolando o fator de ramificação como a única variável comparada entre as estruturas, sem contaminar a análise com uma codificação de dados diferente entre elas.

## Pendências restantes

Nenhuma pendência de design em aberto no momento. Próximo passo: iniciar a implementação (ver "Passos de desenvolvimento" abaixo).

## Passos de desenvolvimento

### 1. Definição e coleta de dados

- Levantar a base completa de siglas de disciplinas (fonte oficial a confirmar).
- Padronizar formato (string de 10 caracteres, incluindo hífen).
- Validar contra a Resolução CG nº 31/2022 (checar consistência de posições).

### 2. Modelagem das estruturas de dados

- Implementar **Árvore Digital Binária** (Trie binária sobre a representação em bits de cada símbolo).
- Implementar **Árvore Digital m-ária (m=4)** (Trie 4-ária, agrupando pares de bits ou usando outra codificação por símbolo).
- Definir operações a implementar em ambas: inserção, busca, remoção (se aplicável), e travessia/listagem.

### 3. Análise teórica

- Complexidade de tempo (inserção, busca) para cada estrutura, em função de `n` (nº de siglas) e do comprimento da chave.
- Complexidade de espaço: nº de nós, overhead de ponteiros/filhos por nó (2 vs. 4 filhos).
- Discussão sobre profundidade da árvore e fator de ramificação.

### 4. Experimentos práticos

- Implementar rotina de benchmark: inserção em massa, busca (hit/miss), medição de tempo de execução.
- Rodar experimentos com o mesmo dataset nas duas estruturas.
- Coletar métricas: tempo médio de operação, uso de memória, profundidade média/máxima da árvore.
- Gerar gráficos/tabelas comparativas.

### 5. Revisão bibliográfica

- Buscar artigos/dissertações indexados (Qualis) sobre árvores digitais, tries, PATRICIA trees, comparações m-árias vs. binárias.
- Selecionar 4–6 referências relevantes e atualizadas.
