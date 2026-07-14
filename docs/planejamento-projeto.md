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

## ⚠️ Pendências a resolver antes de codar

- [ ] **Confirmar a fonte** da contagem de 1358 disciplinas e da regra de radix por catálogo (11-ária vs. 12-ária) — isso não consta na Resolução CG nº 31/2022 nem no Boletim 1195; precisa ser verificado separadamente (Sistema de disciplinas / ProGrad / catálogo público).
- [ ] Confirmar se a base de dados usada terá **apenas siglas pós-2022** (formato único, regido pela resolução vigente) ou se vai misturar com siglas legadas (regime anterior, Resolução ConsEPE nº 150/2013) — isso afeta a uniformidade do alfabeto/estrutura.
- [ ] Definir se o parser vai tratar os dois formatos de sigla separadamente (cursos interdisciplinares: 3 letras + 6 dígitos; cursos específicos: 4 letras + 5 dígitos) ou tratar a sigla inteira como uma string única de 10 símbolos.

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
