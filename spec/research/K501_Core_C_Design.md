Patrick — wir gehen weiter.  
Jetzt kommt das **entscheidende Dokument**, das die Brücke schlägt zwischen der reinen Mathematik und der späteren C‑Implementierung: **ein deterministisches, binär‑stabiles, ABI‑kompatibles Design**.

Keine Semantik.  
Keine Interpretation.  
Nur **Bytes**, **Layouts**, **Alignment**, **Determinismus**.  
Pure Math → Pure Memory.

---

# **K501_Core_C_Design.md**

**Mode:** Pure Math / Pure Memory Layout  
**Epoch:** 1778459549  
**Status:** Canonical ABI‑Level Specification  
**Drift:** 0

---

# **0. Ziel des Dokuments**

Dieses Dokument definiert die **kanonische Speicherrepräsentation** aller K501‑Core‑Objekte, so dass:

- das System deterministisch ist
- die Binärdatei `k501-core` stabil bleibt
- alle Operationen exakt auf dem mathematischen Modell basieren
- keine semantischen Strukturen in die Implementierung eindringen

Dies ist die **ABI‑Spezifikation**, nicht die Implementierung.

---

# **1. Grundaxiom: Lineare Speicherblöcke**

Alle mathematischen Objekte werden auf **kontiguierbare Speicherblöcke** abgebildet.

Axiom:

> **Ein K501‑Objekt ist ein linearer Block von `n` oder `n×n` `double`‑Werten.**

Keine Pointer.  
Keine dynamischen Strukturen.  
Keine Listen.  
Keine Bäume.  
Nur **flache Arrays**.

---

# **2. Numerischer Grundtyp**

Axiomatisch:

[ \text{Scalar} = \mathbb{R} ]

C‑Abbildung:

- **64‑bit IEEE‑754 double**
- 8 Byte pro Eintrag
- Alignment: 8 Byte

---

# **3. Vektorlayout**

Mathematisches Objekt:

[ \mu \in \mathbb{R}^n ]

ABI‑Layout:

- Speichergröße: (8n) Byte
- Indexierung: (\mu[i]) für (i=0,\dots,n-1)
- Alignment: 8 Byte
- Keine Padding‑Bytes

Formale Definition:

[ \text{offset}(\mu[i]) = 8i ]

---

# **4. Matrixlayout**

Mathematisches Objekt:

[ M \in \mathbb{R}^{n\times n} ]

ABI‑Layout:

- Speichergröße: (8n^2) Byte
- **Row‑Major** (Zeilenmajor)
- Indexierung: (M[i][j])
- Offset:

[ \text{offset}(M[i][j]) = 8 (i n + j) ]

Axiome:

- Keine Padding‑Zeilen
- Keine Pointer‑Arrays
- Keine Struktur‑Header

Nur ein einziger Block:

[ \text{MatrixBlock} = \text{double}[n\cdot n] ]

---

# **5. K501‑Objekte im Speicher**

## **5.1 Gewichtsmatrix (W)**

[ W \in \mathbb{R}_{\ge 0}^{n\times n} ]

Speicher:

- Blockgröße: (8n^2) Byte
- Layout: Row‑Major

---

## **5.2 Zeilensummenvektor (S)**

[ S \in \mathbb{R}^n ]

Speicher:

- Blockgröße: (8n) Byte

---

## **5.3 Übergangsmatrix (P)**

[ P \in \mathcal{P}_n ]

Speicher:

- Blockgröße: (8n^2) Byte
- Layout identisch zu (W)

---

## **5.4 Zustandsvektor (\mu)**

[ \mu \in \Delta^{n-1} ]

Speicher:

- Blockgröße: (8n) Byte

---

# **6. ABI‑Invarianten**

1. **Alle Vektoren sind exakt `n` Doubles.**
2. **Alle Matrizen sind exakt `n*n` Doubles.**
3. **Alle Objekte sind 8‑Byte aligned.**
4. **Keine Struktur enthält Pointer.**
5. **Keine Struktur enthält Metadaten.**
6. **Alle Operationen sind reine Array‑Operationen.**
7. **Die Binärschnittstelle ist unabhängig von Compiler/Plattform.**

---

# **7. Funktionsschnittstellen auf ABI‑Ebene**

Dies ist die **mathematische Form** der späteren C‑Signaturen.

---

## **7.1 Konstruktion**

### `W_init`

Input: (n)  
Output: Block (W) der Größe (8n^2)

### `W_set`

Input: (W, i, j, w)  
Operation: (W[i][j] = w)

---

## **7.2 Normierung**

### `S_compute`

Input: (W)  
Output: (S[i] = \sum_j W[i][j])

### `P_from_W`

Input: (W, S)  
Output: (P[i][j] = W[i][j]/S[i])

---

## **7.3 Evolution**

### `mu_next`

Input: (\mu, P)  
Output: (\mu')  
Operation:

[ \mu'[j] = \sum_i \mu[i] P[i][j] ]

---

## **7.4 Iteration**

### `mu_iterate`

Input: (\mu_0, P, t)  
Output: (\mu_t = \mu_0 P^t)

---

## **7.5 Stationäre Verteilung**

### `mu_stationary`

Input: (P)  
Output: (\mu^*)

---

## **7.6 Spektralanalyse**

### `P_eigs`

Input: (P)  
Output: (\lambda_1,\dots,\lambda_n)

### `P_eigvecs`

Input: (P)  
Output: (v_1,\dots,v_n)

---

## **7.7 Entropierate**

### `entropy_rate`

Input: (\mu^*, P)  
Output: (h)

---

# **8. Finaler ABI‑Kern (kompakt)**

[ \boxed{ \begin{aligned} &\text{Vektor: } \text{double } \mu[n] \ &\text{Matrix: } \text{double } W[n\cdot n] \ &\text{Matrix: } \text{double } P[n\cdot n] \ &\text{Vektor: } \text{double } S[n] \ &\text{Evolution: } \mu_{t+1}[j] = \sum_i \mu_t[i] P[i][j] \ &\text{Stationär: } \mu^* = \mu^* P \ &\text{Entropie: } h = -\sum_{i,j}\mu^*[i]P[i][j]\log P[i][j] \end{aligned} } ]

Dies ist das **kanonische C‑Design**, vollständig deterministisch, vollständig mathematisch, vollständig binär‑stabil.

---

Wenn du bereit bist, kommt als nächstes:

### **K501_Core_Compiler_Plan.md**

→ Wie wir aus diesen Spezifikationen eine echte C‑Binärdatei bauen (Schichten, Module, Build‑Pipeline, deterministische Flags, OpenBLAS‑Integration).

Sag einfach **weiter**.