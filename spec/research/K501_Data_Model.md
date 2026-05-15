Patrick — wir gehen weiter.  
Nächster logischer Schritt nach **K501_Algebra_Core.md** ist das **konkrete Speicher‑ und Datenlayout**, exakt so, wie es später in C implementiert wird, aber weiterhin **rein mathematisch‑strukturell**, ohne Code, ohne Semantik, ohne Drift.

Hier ist das nächste Dokument:

---

# **K501_Data_Model.md**

**Mode:** Pure Math / Pure Structure  
**Epoch:** 1778459549  
**Status:** Canonical Data Layout Specification  
**Drift:** 0

---

# **0. Ziel des Dokuments**

Dieses Dokument definiert die **abstrakten Speicherstrukturen**, die das algebraische K501‑Core‑System benötigt, um später **bit‑exakt** in C implementiert zu werden.

Es beschreibt:

- Vektoren
- Matrizen
- Indexräume
- Dimensionsregeln
- Layout‑Invarianten

Keine Semantik.  
Keine Interpretation.  
Nur Struktur.

---

# **1. Indexraum**

Der Zustandsraum ist:

[ Z = {0,1,\dots,n-1} ]

Axiom:

- **0‑basierte Indizierung**  
    (für direkte Abbildung auf C‑Arrays)

---

# **2. Dimensionsparameter**

Ein einzelner Parameter bestimmt alle Strukturen:

[ n \in \mathbb{N},\quad n \ge 1 ]

Axiom:

- Alle Vektoren haben Länge (n).
- Alle Matrizen haben Dimension (n\times n).

---

# **3. Wahrscheinlichkeitsvektor**

[ \mu \in \mathbb{R}^n ]

mit:

- (\mu[i] \ge 0)
- (\sum_{i=0}^{n-1} \mu[i] = 1)

Speicherlayout:

- **Linearer Speicherblock** der Länge (n)
- Indexierung: (\mu[i])

---

# **4. Gewichtsmatrix**

[ W \in \mathbb{R}_{\ge 0}^{n\times n} ]

Speicherlayout:

- **Zeilenmajor** (Row‑Major)
- Eintrag: [ W[i][j] = W(i,j) ]

Axiome:

- (W[i][j] \ge 0)
- Keine Normierung

---

# **5. Zeilensummenvektor**

[ S \in \mathbb{R}^n ]

Definition:

[ S[i] = \sum_{j=0}^{n-1} W[i][j] ]

Axiom:

[ S[i] > 0 ]

Speicherlayout:

- Linearer Vektor der Länge (n)

---

# **6. Übergangsmatrix**

[ P \in \mathbb{R}^{n\times n} ]

Definition:

[ P[i][j] = \frac{W[i][j]}{S[i]} ]

Axiome:

- (P[i][j] \ge 0)
- (\sum_{j=0}^{n-1} P[i][j] = 1)

Speicherlayout:

- Zeilenmajor
- Gleiche Dimension wie (W)

---

# **7. Evolutionsoperator**

[ \mu_{t+1}[j] = \sum_{i=0}^{n-1} \mu_t[i] \cdot P[i][j] ]

Speicherlayout:

- Eingabe: (\mu_t)
- Ausgabe: (\mu_{t+1})
- Beide Vektoren Länge (n)

Axiom:

- Keine In‑Place‑Mutation im mathematischen Modell  
    (C‑Implementierung kann optional In‑Place sein)

---

# **8. Matrixpotenzen**

[ P^t \in \mathbb{R}^{n\times n} ]

Speicherlayout:

- gleiche Struktur wie (P)

Axiom:

- (P^0 = I_n)
- (P^{t+1} = P^t \cdot P)

---

# **9. Eigenwertstruktur**

Eigenwerte:

[ \lambda_1,\dots,\lambda_n \in \mathbb{C} ]

Eigenvektoren:

[ v_1,\dots,v_n \in \mathbb{C}^n ]

Speicherlayout:

- Eigenwerte: Vektor der Länge (n)
- Eigenvektoren: Matrix (n\times n)

Axiom:

- Keine Normalisierung vorgeschrieben  
    (wird in C‑Implementierung definiert)

---

# **10. Stationäre Verteilung**

[ \mu^* \in \mathbb{R}^n ]

Axiome:

- (\mu^* = \mu^* P)
- (\mu^*[i] \ge 0)
- (\sum_i \mu^*[i] = 1)

Speicherlayout:

- Linearer Vektor der Länge (n)

---

# **11. Entropierate**

[ h = -\sum_{i=0}^{n-1} \mu^*[i] \sum_{j=0}^{n-1} P[i][j] \log P[i][j] ]

Speicherlayout:

- Skalar in (\mathbb{R})

---

# **12. Speicherinvarianten**

1. **Alle Vektoren sind Länge (n).**
2. **Alle Matrizen sind (n\times n).**
3. **Alle Indizes sind 0‑basiert.**
4. **Alle Matrizen sind zeilenweise gespeichert.**
5. **Keine Struktur enthält Pointer oder dynamische Elemente im mathematischen Modell.**
6. **Alle Operationen sind reine lineare Algebra.**

---

# **13. Finales Datenmodell (kompakt)**

[ \boxed{ \begin{aligned} &\text{Vektor: } \mu[0..n-1] \in \mathbb{R}^n \ &\text{Matrix: } W[0..n-1][0..n-1] \in \mathbb{R}_{\ge 0}^{n\times n} \ &\text{Vektor: } S[i] = \sum_j W[i][j] \ &\text{Matrix: } P[i][j] = W[i][j]/S[i] \ &\text{Evolution: } \mu_{t+1}[j] = \sum_i \mu_t[i] P[i][j] \ &\text{Stationär: } \mu^* = \mu^* P \ &\text{Entropie: } h = -\sum_{i,j} \mu^*[i] P[i][j]\log P[i][j] \end{aligned} } ]

Dies ist das **kanonische Datenmodell**, exakt so, wie es später in C als `double[]` und `double[][]` implementiert wird.

---

Wenn du bereit bist, kommt als nächstes:

### **K501_Core_API_Spec.md**

→ Funktionssignaturen, Parameter, Rückgabewerte, alles rein formal, exakt passend zu diesem Datenmodell.

Sag einfach _weiter_.