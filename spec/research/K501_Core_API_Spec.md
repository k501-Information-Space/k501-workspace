Patrick — wir gehen weiter.  
Hier ist das nächste Dokument der K501‑Core‑Spezifikationskette: **die API‑Spezifikation**, vollständig formal, rein mathematisch‑strukturell, exakt passend zu den vorherigen Dokumenten.

Keine Semantik.  
Keine Interpretation.  
Nur **Funktionen**, **Signaturen**, **Parameter**, **Rückgabewerte**, **Invarianten**.

---

# **K501_Core_API_Spec.md**

**Mode:** Pure Math / Pure Interface  
**Epoch:** 1778459549  
**Status:** Canonical API Specification  
**Drift:** 0

---

# **0. Ziel des Dokuments**

Dieses Dokument definiert die **abstrakten API‑Operationen**, die das K501‑Core‑System benötigt, um später als C‑Binärdatei (`k501-core`) implementiert zu werden.

Alle Funktionen operieren ausschließlich auf den mathematischen Objekten aus:

- `K501_Algebra_Core.md`
- `K501_Data_Model.md`

Es gibt **keine Semantik**, **keine Interpretation**, **keine externen Abhängigkeiten**.

---

# **1. Grundtypen**

Wir definieren abstrakte Typen, die später exakt auf C‑Typen abgebildet werden:

- **Indexraum:**  
    [ i,j,k \in {0,\dots,n-1} ]
    
- **Vektor:**  
    [ \text{Vector}(n) = \mathbb{R}^n ]
    
- **Matrix:**  
    [ \text{Matrix}(n,n) = \mathbb{R}^{n\times n} ]
    

Alle Funktionen operieren auf diesen Typen.

---

# **2. API‑Modul: Konstruktion von (W)**

### **2.1 Funktion: `W_init`**

**Signatur:**  
[ W_{\text{init}} : n \mapsto W ]

**Beschreibung:**  
Erzeugt eine (n\times n)‑Matrix (W) mit allen Einträgen = 0.

**Invarianten:**  
[ W[i][j] = 0 ]

---

### **2.2 Funktion: `W_set`**

**Signatur:**  
[ W_{\text{set}} : (W,i,j,w) \mapsto W' ]

**Beschreibung:**  
Setzt den Eintrag (W[i][j] = w).

**Axiome:**  
[ w \ge 0 ]

---

# **3. API‑Modul: Zeilensummen (S)**

### **3.1 Funktion: `S_compute`**

**Signatur:**  
[ S_{\text{compute}} : W \mapsto S ]

**Definition:**  
[ S[i] = \sum_{j=0}^{n-1} W[i][j] ]

**Axiom:**  
[ S[i] > 0 ]

---

# **4. API‑Modul: Übergangsmatrix (P)**

### **4.1 Funktion: `P_from_W`**

**Signatur:**  
[ P_{\text{fromW}} : (W,S) \mapsto P ]

**Definition:**  
[ P[i][j] = \frac{W[i][j]}{S[i]} ]

**Invarianten:**

- (P[i][j] \ge 0)
- (\sum_j P[i][j] = 1)

---

# **5. API‑Modul: Evolution (\mu_{t+1} = \mu_t P)**

### **5.1 Funktion: `mu_next`**

**Signatur:**  
[ \mu_{\text{next}} : (\mu,P) \mapsto \mu' ]

**Definition:**  
[ \mu'[j] = \sum_{i=0}^{n-1} \mu[i] P[i][j] ]

**Invarianten:**

- (\mu'[j] \ge 0)
- (\sum_j \mu'[j] = 1)

---

# **6. API‑Modul: Iteration**

### **6.1 Funktion: `mu_iterate`**

**Signatur:**  
[ \mu_{\text{iterate}} : (\mu_0,P,t) \mapsto \mu_t ]

**Definition:**  
[ \mu_t = \mu_0 P^t ]

**Axiom:**  
(t \in \mathbb{N})

---

# **7. API‑Modul: Stationäre Verteilung**

### **7.1 Funktion: `mu_stationary`**

**Signatur:**  
[ \mu_{\text{stationary}} : P \mapsto \mu^* ]

**Definition:**  
[ \mu^* = \mu^* P ]

**Invarianten:**

- (\mu^*(i)\ge 0)
- (\sum_i \mu^*(i)=1)

---

# **8. API‑Modul: Matrixpotenzen**

### **8.1 Funktion: `P_power`**

**Signatur:**  
[ P_{\text{power}} : (P,t) \mapsto P^t ]

**Definition:**  
[ P^0 = I_n,\quad P^{t+1} = P^t P ]

---

# **9. API‑Modul: Spektralanalyse**

### **9.1 Funktion: `P_eigenvalues`**

**Signatur:**  
[ P_{\text{eigs}} : P \mapsto (\lambda_1,\dots,\lambda_n) ]

---

### **9.2 Funktion: `P_eigenvectors`**

**Signatur:**  
[ P_{\text{eigvecs}} : P \mapsto (v_1,\dots,v_n) ]

---

# **10. API‑Modul: Entropierate**

### **10.1 Funktion: `entropy_rate`**

**Signatur:**  
[ h_{\text{rate}} : (\mu^*,P) \mapsto h ]

**Definition:**  
[ h = -\sum_{i=0}^{n-1} \mu^*(i) \sum_{j=0}^{n-1} P(i,j)\log P(i,j) ]

---

# **11. Finaler API‑Kern (kompakt)**

[ \boxed{ \begin{aligned} & W_{\text{init}}(n) \ & W_{\text{set}}(W,i,j,w) \ & S_{\text{compute}}(W) \ & P_{\text{fromW}}(W,S) \ & \mu_{\text{next}}(\mu,P) \ & \mu_{\text{iterate}}(\mu_0,P,t) \ & \mu_{\text{stationary}}(P) \ & P_{\text{power}}(P,t) \ & P_{\text{eigs}}(P) \ & P_{\text{eigvecs}}(P) \ & h_{\text{rate}}(\mu^*,P) \end{aligned} } ]

Dies ist die **kanonische API**, exakt so, wie sie später in C implementiert wird.

---

Wenn du bereit bist, kommt als nächstes:

### **K501_Core_C_Design.md**

→ Speicherlayout in Bytes, Struct‑Layouts, Alignment, deterministische ABI‑Form.

Sag einfach **weiter**.