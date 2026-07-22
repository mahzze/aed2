# %%
import pandas as pd

df = pd.read_csv("./openfoodfacts.csv", usecols=["code", "product_name", "countries", "brands"], dtype=str)

# remove nulos e espaços
df["code"] = df["code"].str.strip()
df = df.dropna(subset=["code"])

# padrão: comprimento + se é só dígitos
df["is_digit"] = df["code"].str.isdigit()
df["length"] = df["code"].str.len()

# distribuição de comprimentos entre códigos numéricos (agora com zeros preservados)
print(df[df["is_digit"]]["length"].value_counts().sort_index())

# quantos NÃO são só dígitos (letras, espaços internos, etc.)
print("não-numéricos:", (~df["is_digit"]).sum())

# amostra dos não-numéricos, pra entender o tipo de sujeira
print(df[~df["is_digit"]]["code"].head(20).tolist())

print("nulos/NaN no code original:", df['code'].isna().sum())
# %%
def validar_ean13(codigo: str) -> bool:
    digitos = [int(d) for d in codigo]
    soma = sum(d if i % 2 == 0 else d * 3 for i, d in enumerate(digitos[:12]))
    dv_calculado = (10 - soma % 10) % 10
    return dv_calculado == digitos[12]

df13 = df[df["length"] == 13].copy()
df13["valido"] = df13["code"].apply(validar_ean13)
print(df13["valido"].value_counts())
df13 = df13[df13['product_name'] != ""]
df13 = df13.query('product_name != code')

# amostra de inválidos, se houver, pra entender o tipo de erro
print(df13[~df13["valido"]]["code"].head(10).tolist())
# %%
def validar_ean8(codigo: str) -> bool:
    digitos = [int(d) for d in codigo]
    soma = sum(d * 3 if i % 2 == 0 else d for i, d in enumerate(digitos[:7]))
    dv_calculado = (10 - soma % 10) % 10
    return dv_calculado == digitos[7]

df8 = df[df["length"] == 8].copy()
df8["valido"] = df8["code"].apply(validar_ean8)
print(df8["valido"].value_counts())
# %%
# 1. Filtrar apenas EAN-13 válidos
df_final = df13[df13["valido"]].copy()

# 2. Remover duplicatas (se houver produtos repetidos com o mesmo code)
antes = len(df_final)
df_final = df_final.drop_duplicates(subset="code")
print(f"Duplicatas removidas: {antes - len(df_final)}")

# 3. Conferência final
print(f"Total de EAN-13 válidos e únicos: {len(df_final)}")
assert df_final["code"].str.len().eq(13).all(), "Existe código com tamanho != 13"
assert df_final["code"].str.isdigit().all(), "Existe código não-numérico"

# 4. Salvar: um código por linha, texto puro (ideal para C)
# Substitui quebras de linha por espaço nos campos de texto
colunas_texto = ["product_name", "countries", "brands"]

for col in colunas_texto:
    df_final[col] = (
        df_final[col]
        .fillna("")
        .str.replace(r"[\r\n]+", " ", regex=True)  # quebra de linha -> espaço
        .str.replace(r"\s+", " ", regex=True)      # remove espaços duplicados
        .str.strip()                               # remove espaços nas pontas
    )
df_final[["code", "product_name", "countries","brands"]].to_csv("./ean13_dataset.txt", index=False, header=False)

print("Salvo em ean13_dataset.txt")
# %%
