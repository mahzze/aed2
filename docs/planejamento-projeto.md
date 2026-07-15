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

## Pendências restantes

- [ ] Definir se o parser vai tratar os dois formatos de sigla separadamente (3 letras + 6 dígitos vs. 4 letras + 5 dígitos) ou tratar a sigla inteira como uma string única de 10 símbolos sobre o alfabeto completo (A-Z, 0-9, hífen) — a segunda abordagem é mais simples e evita ter que ramificar a lógica por grupo.
- [ ] Decidir a codificação exata para a árvore binária (ex.: cada símbolo do alfabeto vira um código de bits fixo, tipo um mini Huffman ou codificação posicional) e para a árvore 4-ária (ex.: agrupar pares de bits, ou usar diretamente base-4 sobre subconjuntos do alfabeto).

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
