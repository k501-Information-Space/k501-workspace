|                      |                               |
| -------------------- | ----------------------------- |
| Unix Epoch           | 1778460707                    |
| Time (UTC)           | Mon May 11 00:51:47 2026 UTC  |
| Time (Europe/Berlin) | Mon May 11 02:51:47 2026 CEST |
Patrick — bestätigt.  
Wir gehen weiter.  
Jetzt kommt das **Compiler‑ und Build‑System‑Dokument**, das exakt beschreibt, wie aus den bisherigen Spezifikationen eine **deterministische, reproduzierbare, binärstabile** `k501-core` entsteht.

Keine Semantik.  
Keine Interpretation.  
Nur **Schichten**, **Build‑Regeln**, **Determinismus**, **Compiler‑Invarianten**.

---

# **K501_Core_Compiler_Plan.md**

**Mode:** Pure Math → Pure Build Logic  
**Epoch:** 1778460707  
**Status:** Canonical Compiler & Build Specification  
**Drift:** 0

---

# **0. Ziel des Dokuments**

Dieses Dokument definiert:

- die **Schichten** der zukünftigen `k501-core` C‑Implementierung
- die **Build‑Pipeline**
- die **Compiler‑Flags**
- die **Determinismus‑Invarianten**
- die **numerischen Anforderungen**
- die **Integration externer BLAS‑Bibliotheken** (OpenBLAS)

Es ist die **formale Grundlage** für die spätere Erstellung der Binärdatei.

---

# **1. Schichtenmodell der K501‑Core‑Implementierung**

Wir definieren fünf strikt getrennte Schichten:

---

## **1.1 Layer 0 — Memory Layer (ML0)**

**Zuständig für:**

- Allokation linearer Speicherblöcke
- Freigabe
- Alignment‑Sicherung

**Axiome:**

- Alle Blöcke sind 8‑Byte aligned
- Keine Pointerarithmetik außerhalb definierter Bereiche
- Keine dynamischen Strukturen außer linearen Blöcken

---

## **1.2 Layer 1 — Data Layer (ML1)**

**Zuständig für:**

- Abbildung der mathematischen Objekte auf Speicher
- Vektor‑ und Matrix‑Layouts
- Row‑Major‑Indexierung

**Axiome:**

- Vektor = Block der Länge `n`
- Matrix = Block der Länge `n*n`
- Indexierung: `i*n + j`

---

## **1.3 Layer 2 — Algebra Layer (ML2)**

**Zuständig für:**

- Implementierung der Operationen aus `K501_Algebra_Core.md`
- Matrixmultiplikation
- Vektor‑Matrix‑Multiplikation
- Normierung

**Axiome:**

- Keine Seiteneffekte
- Keine globalen Zustände
- Reine Funktionen

---

## **1.4 Layer 3 — Numerical Layer (ML3)**

**Zuständig für:**

- Eigenwerte
- Eigenvektoren
- Matrixpotenzen
- Stationäre Verteilung

**Axiome:**

- Nutzung von BLAS/LAPACK‑Routinen
- Deterministische numerische Pfade
- Keine parallelen Ausführungen ohne deterministische Reduktion

---

## **1.5 Layer 4 — Core API Layer (ML4)**

**Zuständig für:**

- Exakte Abbildung der API aus `K501_Core_API_Spec.md`
- CLI‑Integration (optional)
- Binäre Schnittstelle

**Axiome:**

- Keine Interpretation
- Keine Semantik
- Nur Weiterleitung an ML0–ML3

---

# **2. Build‑Pipeline**

Die Build‑Pipeline besteht aus exakt 6 Schritten:

---

## **2.1 Schritt 1 — Pre‑Processing**

Axiome:

- Keine Makro‑Magie
- Keine bedingte Kompilierung außer Plattform‑Guards
- Keine dynamischen Includes

---

## **2.2 Schritt 2 — Compilation**

Compiler:

- `gcc` oder `clang`
- C17‑Standard

Deterministische Flags:

```
-O2
-ffloat-store
-fno-fast-math
-fno-unsafe-math-optimizations
-fexcess-precision=standard
-fno-strict-aliasing
-fno-common
```

Axiome:

- Keine Vektor‑Instruktionen, die nondeterministisch runden
- Keine FMA‑Fusion ohne Kontrolle

---

## **2.3 Schritt 3 — BLAS/LAPACK Integration**

Bibliothek:

- OpenBLAS (CPU‑basiert, deterministisch)

Axiome:

- Keine GPU‑Backends
- Keine parallelen Threads (OpenBLAS: `OPENBLAS_NUM_THREADS=1`)
- Keine dynamische Thread‑Zuweisung

---

## **2.4 Schritt 4 — Linking**

Axiome:

- Statisches Linking bevorzugt
- Keine dynamischen Abhängigkeiten außer libc
- Keine symbolischen Aliase

---

## **2.5 Schritt 5 — Binary Normalization**

Axiome:

- Entfernen aller nichtdeterministischen Build‑Metadaten
- Entfernen von Timestamps
- Entfernen von Debug‑Symbolen (Release‑Build)

---

## **2.6 Schritt 6 — Reproducibility Check**

Axiome:

- Gleiche Eingaben → identische Binärdatei
- Byte‑Vergleich (`cmp`) muss identisch sein

---

# **3. Numerische Invarianten**

Axiome:

1. **Alle Berechnungen in double‑Präzision.**
2. **Keine impliziten Typkonversionen.**
3. **Keine FMA‑Fusion ohne explizite Kontrolle.**
4. **Matrixmultiplikation deterministisch:**
    - Reihenfolge fix
    - Keine parallelen Reduktionen
5. **Eigenwertberechnung deterministisch:**
    - LAPACK‑Routinen mit fixen Parametern
    - Keine adaptiven Heuristiken

---

# **4. Determinismus‑Invarianten**

Axiome:

1. **Byte‑Determinismus:**  
    [ \text{Build}(S) = \text{Build}(S) \quad \forall S ]
    
2. **Rechen‑Determinismus:**  
    [ f(x) = f(x) \quad \forall x ]
    
3. **Speicher‑Determinismus:**  
    [ \text{addr}(A[i]) = \text{base}(A) + 8i ]
    
4. **Matrix‑Determinismus:**  
    [ \text{addr}(M[i][j]) = \text{base}(M) + 8(i n + j) ]
    

---

# **5. Finaler Compiler‑Plan (kompakt)**

[ \boxed{ \begin{aligned} &\text{Layer 0: Memory} \ &\text{Layer 1: Data Layout} \ &\text{Layer 2: Algebra} \ &\text{Layer 3: Numerics (BLAS/LAPACK)} \ &\text{Layer 4: API} \ &\text{Build: deterministic gcc/clang pipeline} \ &\text{Binary: reproducible, stable, pure} \end{aligned} } ]

---

Wenn du bereit bist, kommt als nächstes:

### **K501_Core_Module_Structure.md**

→ Die vollständige Modul‑ und Dateiarchitektur der späteren C‑Codebasis (ohne Code, nur Struktur).

Sag einfach **weiter**.