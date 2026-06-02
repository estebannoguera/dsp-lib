# DSP-LIB Roadmap

## Visión

Construir una librería DSP moderna en C++ enfocada en:

* Aprender DSP desde fundamentos.
* Crear bloques reutilizables.
* Aplicar buenas prácticas de ingeniería de software.
* Tener cobertura de pruebas desde el inicio.
* Servir como base para futuras aplicaciones CLI, Qt, plugins y herramientas de audio.
* Poder construir efectos complejos a partir de componentes simples.

---

# Fase 0 - Infraestructura

## Sprint 0 - Proyecto Base

### Objetivo

Tener una base sólida de ingeniería antes de implementar DSP.

### Entregables

* Repositorio Git.
* Estructura de carpetas.
* CMake funcional.
* Google Test integrado.
* Primer test ejecutándose.
* Build reproducible.

### Estructura esperada

```text
DSP-LIB/
│
├── benchmarks/
├── docs/
├── examples/
├── tests/
├── tools/
├── validation/
│
├── dsp/
│   ├── containers/
│   ├── core/
│   ├── dsp/
│   └── utils/
│
└── CMakeLists.txt
```

---

# Fase 1 - Containers

## Sprint 1 - RingBuffer

### Objetivo

Implementar almacenamiento circular para historia temporal.

### Entregables

* RingBuffer<T>
* Tests completos
* Documentación

### Conceptos

* x[n]
* x[n-k]
* Historia temporal
* Circular Buffer
* Ring Buffer

### Tests mínimos

* Constructor válido
* Constructor inválido
* Buffer vacío
* Escritura simple
* Historia temporal
* Buffer lleno
* Wrap-around
* Sobrescritura
* Casos límite

---

# Fase 2 - Delay

## Sprint 2 - DelayLine

### Objetivo

Construir un delay fijo utilizando RingBuffer.

### Entregables

* DelayLine
* Tests completos

### Conceptos

* Delay en muestras
* Delay en milisegundos
* Sample Rate
* Conversión ms → samples

### Ejemplos

```text
10 ms @ 48 kHz = 480 samples
100 ms @ 48 kHz = 4800 samples
```

---

# Fase 3 - Primeros Filtros

## Sprint 3 - Moving Average

### Objetivo

Implementar el primer filtro FIR.

### Entregables

* MovingAverage
* Tests

### Conceptos

* FIR
* Ventana
* Promedio móvil
* Suavizado

---

## Sprint 4 - FIR Genérico

### Objetivo

Implementar convolución general.

### Entregables

* FIRFilter
* Coeficientes configurables
* Tests

### Conceptos

* Convolución
* Respuesta impulsiva
* Kernel
* Coeficientes

---

# Fase 4 - Generadores

## Sprint 5 - Generadores Básicos

### Objetivo

Crear señales de prueba.

### Entregables

* ImpulseGenerator
* ConstantGenerator
* WhiteNoiseGenerator

### Conceptos

* Impulso
* Señal constante
* Ruido blanco

---

## Sprint 6 - Oscillators

### Objetivo

Crear osciladores básicos.

### Entregables

* SineOscillator
* SquareOscillator
* SawOscillator
* TriangleOscillator

### Conceptos

* Frecuencia
* Fase
* Amplitud
* Phase Accumulator

---

# Fase 5 - DSP Core

## Sprint 7 - Gain

### Objetivo

Primer bloque DSP reutilizable.

### Entregables

* Gain
* Volume
* Mute

### Conceptos

* Escalamiento
* Ganancia
* Nivel

---

## Sprint 8 - DSP Block Interface

### Objetivo

Definir la interfaz común para todos los bloques DSP.

### Entregables

* DSPBlock base

### Conceptos

* Abstracción
* Polimorfismo
* Reutilización

### Ejemplo conceptual

```text
processSample()
```

---

## Sprint 9 - DSP Graph

### Objetivo

Conectar bloques DSP entre sí.

### Entregables

* DSPGraph
* Conexiones básicas

### Conceptos

* Flujo de señal
* Procesamiento encadenado
* Nodos DSP

### Ejemplo

```text
Oscillator
    ↓
Gain
    ↓
Delay
```

---

# Fase 6 - Efectos

## Sprint 10 - Simple Delay

### Objetivo

Primer efecto audible.

### Entregables

* Delay simple
* Tests

---

## Sprint 11 - Feedback Delay

### Objetivo

Agregar realimentación.

### Entregables

* FeedbackDelay
* Tests

### Conceptos

* Feedback
* Estabilidad

---

## Sprint 12 - Comb Filter

### Objetivo

Primer bloque fundamental para reverb y chorus.

### Entregables

* CombFilter
* Tests

### Conceptos

* Resonancia
* Cancelaciones
* Feedback

---

# Fase 7 - Modulación

## Sprint 13 - LFO

### Objetivo

Modular parámetros.

### Entregables

* LFO
* Sine LFO
* Triangle LFO

### Conceptos

* Modulación
* Frecuencia baja

---

## Sprint 14 - Chorus

### Objetivo

Implementar chorus clásico.

### Entregables

* Chorus
* Tests

### Conceptos

* Delay modulado
* LFO
* Mezcla dry/wet

---

## Sprint 15 - TriChorus

### Objetivo

Implementar un chorus de tres voces.

### Entregables

* TriChorus
* Tests

### Conceptos

* Múltiples delays
* Múltiples LFOs
* Espacialidad

---

# Fase 8 - Reverb

## Sprint 16 - Schroeder Reverb

### Objetivo

Primera reverb funcional.

### Entregables

* SchroederReverb
* Tests

### Conceptos

* Comb Filters
* All-Pass Filters
* Decaimiento

---

# Fase 9 - Herramientas

## Sprint 17 - WAV Reader/Writer

### Objetivo

Procesar audio offline.

### Entregables

* WAV Reader
* WAV Writer

---

## Sprint 18 - FFT

### Objetivo

Ingresar al dominio de frecuencia.

### Entregables

* FFT
* Magnitude Spectrum
* Utilities

### Conceptos

* DFT
* FFT
* Frequency Domain

---

# Fase 10 - Aplicaciones

## Sprint 19 - CLI DSP Playground

### Objetivo

Probar bloques DSP desde terminal.

### Entregables

* CLI
* Procesamiento de WAV

---

## Sprint 20 - Qt DSP Playground

### Objetivo

Visualizar y probar DSP gráficamente.

### Entregables

* Aplicación Qt
* Gráficas
* Controles

---

# Meta Final

Ser capaz de construir:

* Delay
* Chorus
* TriChorus
* Flanger
* Comb Filters
* Reverb
* Cadenas DSP arbitrarias
* Herramientas de análisis
* Aplicaciones GUI
* Procesadores de audio reutilizables

Todo basado en bloques DSP propios, completamente probados y documentados.
