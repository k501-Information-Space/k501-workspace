Patrick — hier ist **K501_Algebra_Core.md**, vollständig formal, rein algebraisch, ohne Semantik, ohne Drift, exakt so strukturiert, dass es später 1:1 in C‑Strukturen, Vektoren und Matrizen übersetzt werden kann.

---

# **K501_Algebra_Core.md**

**Mode:** Pure Math  
**Epoch:** 1778459549  
**Status:** Canonical Algebraic Core  
**Drift:** 0

---

# **0. Algebraische Grundobjekte**

Das K501‑Core‑System basiert auf folgenden algebraischen Objekten:

- **Endlicher Indexraum:**  
    [ Z = {1,\dots,n} ]
    
- **Vektorraum der Wahrscheinlichkeiten:**  
    [ \Delta^{n-1} = \left{ \mu \in \mathbb{R}^n \mid \mu(i)\ge 0,\ \sum_{i=1}^n \mu(i)=1 \right} ]
    
- **Nichtnegative Matrizen:**  
    [ \mathbb{R}_{\ge 0}^{n\times n} ]
    
- **Stochastische Matrizen:**  
    [ \mathcal{P}_n = \left{ P\in\mathbb{R}_{\ge 0}^{n\times n} \mid \sum_{j=1}^n P(i,j)=1\ \forall i \right} ]
    

---

# **1. Gewichtsmatrix (W)**

Für alle (i,j\in Z):

[ W(i,j) \in \mathbb{R}_{\ge 0} ]

Axiom:

[ W \in \mathbb{R}_{\ge 0}^{n\times n} ]

---

# **2. Zeilensummen**

Für jede Zeile (i):

[ S(i) = \sum_{k=1}^n W(i,k) ]

Axiom:

[ S(i) > 0\quad \forall i\in Z ]

---

# **3. Übergangsmatrix (P)**

Definition:

[ P(i,j) = \frac{W(i,j)}{S(i)} ]

Axiome:

1. **Nichtnegativität:**  
    [ P(i,j)\ge 0 ]
    
2. **Zeilennormierung:**  
    [ \sum_{j=1}^n P(i,j)=1 ]
    

Damit:

[ P \in \mathcal{P}_n ]

---

# **4. Evolution des Zustandsvektors**

Sei (\mu_t \in \Delta^{n-1}).  
Die Evolution ist definiert durch:

[ \mu_{t+1} = \mu_t P ]

Komponentenweise:

[ \mu_{t+1}(j) = \sum_{i=1}^n \mu_t(i) P(i,j) ]

Axiome:

- (\mu_{t+1}(j)\ge 0)
- (\sum_j \mu_{t+1}(j)=1)

---

# **5. Iteration**

Durch Induktion:

[ \mu_t = \mu_0 P^t ]

---

# **6. Spektrale Struktur**

Eigenwertproblem:

[ P v_k = \lambda_k v_k ]

mit:

- (\lambda_1 = 1)
- (|\lambda_k| \le 1)

Falls (P) diagonalisierbar:

[ P = V \Lambda V^{-1} ]

[ P^t = V \Lambda^t V^{-1} ]

[ \mu_t = \mu_0 V \Lambda^t V^{-1} ]

---

# **7. Stationäre Verteilung**

Eine stationäre Verteilung (\mu^*) erfüllt:

[ \mu^* = \mu^* P ]

mit:

[ \mu^_(i)\ge 0,\quad \sum_i \mu^_(i)=1 ]

Dies ist der linke Eigenvektor zu (\lambda_1=1).

---

# **8. Konvergenz**

Falls:

[ |\lambda_k| < 1\quad (k\ge 2) ]

dann:

[ \lim_{t\to\infty} \mu_t = \mu^* ]

---

# **9. Entropierate**

Für stationäre (\mu^*):

[ h = -\sum_{i=1}^n \mu^*(i)\sum_{j=1}^n P(i,j)\log P(i,j) ]

---

# **10. Algebraischer Kern des K501‑Systems**

Das gesamte K501‑Core‑System reduziert sich algebraisch auf:

[ \boxed{ \begin{aligned} &\text{(1) } W \in \mathbb{R}_{\ge 0}^{n\times n} \ &\text{(2) } S(i)=\sum_{k=1}^n W(i,k) > 0 \ &\text{(3) } P(i,j)=\frac{W(i,j)}{S(i)} \ &\text{(4) } \mu_{t+1}=\mu_t P \ &\text{(5) } \mu^_=\mu^_ P \ &\text{(6) } h=-\sum_{i,j}\mu^*(i)P(i,j)\log P(i,j) \end{aligned} } ]

Dies ist der **vollständig verdichtete algebraische Kern**, exakt so, wie er später in C implementiert wird:

- Vektoren: `double mu[n]`
- Matrizen: `double W[n][n]`, `double P[n][n]`
- Operationen: Matrixnormierung, Matrixmultiplikation, Eigenwertanalyse

---

Wenn du willst, erstelle ich jetzt:

- **K501_Data_Model.md** (konkrete Speicherlayouts für C)  
    oder
- **K501_Core_API_Spec.md** (Funktionssignaturen für die spätere C‑Binärdatei)

Sag einfach _weiter_.