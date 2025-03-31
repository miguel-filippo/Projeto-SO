# Módulo 1 – Chamadas ao Sistema no Linux

## 📌 Objetivo

Este projeto tem como objetivo demonstrar o uso de **chamadas ao sistema (system calls)** no ambiente Linux, por meio de programas em linguagem C que exemplificam as principais categorias de chamadas:

- Gerenciamento de Entrada/Saída (E/S)
- Gerenciamento de Processos
- Gerenciamento de Memória

A análise de desempenho e comportamento dos programas é feita utilizando as ferramentas `strace` e `/usr/bin/time`, com foco na identificação de processos do tipo **CPU-bound** e **I/O-bound**.

---

## 📁 Estrutura do Projeto

O projeto está organizado em subpastas por categoria de chamadas ao sistema, separando os códigos-fonte, binários e arquivos de entrada:

```
checkpoint_1/
├── IO_Files/
│   ├── bin/                   # Binários compilados (E/S)
│   ├── src/                   # Códigos-fonte relacionados a E/S
│   ├── data                   # Arquivo de entrada para os testes de backup
│   ├── cpu_backup             # Arquivo gerado pelo backup CPU-bound
│   └── io_backup              # Arquivo gerado pelo backup I/O-bound
├── Memory_Files/
│   ├── bin/                   # Binário compilado da aplicação de memória
│   └── src/                   # Código-fonte utilizando mmap, munmap, getpagesize
├── Process/
│   ├── bin/                   # Binário do gerenciador de processos
│   └── src/                   # Código-fonte do process_manager
├── build.sh                   # Script para compilar todos os binários
└── README.md                  # Este arquivo
```

---

## 🔧 Compilação

Para compilar todos os programas, basta executar o script `build.sh` a partir da raiz do projeto:

```bash
chmod +x build.sh
./build.sh
```

O script compila os arquivos C presentes em cada subpasta e salva os executáveis nas respectivas pastas `bin/`.\
As dependências utilizadas são padrão do GCC e não requerem bibliotecas externas.

---

## ▶️ Execução

A execução principal é feita pelo programa `process_manager`, localizado em:

```
Process/bin/process_manager
```

### 📥 Comando:

```bash
./Process/bin/process_manager
```

Esse programa cria dois processos filhos:

1. **Filho IO-bound** – Executa um programa de backup com buffer pequeno, simulando um cenário intensivo em entrada/saída (I/O).
2. **Filho MEMÓRIA (CPU-bound)** – Executa um programa que aloca memória com `mmap`, preenche e processa dados, simulando um cenário com uso intenso de CPU.

O processo pai aguarda a finalização de ambos os filhos e exibe seus PIDs e códigos de saída.

---

## 🔍 Descrição das Primitivas Utilizadas

### 📂 Entrada/Saída (E/S)

Local: `IO_Files/src/`

Chamadas de sistema utilizadas:

- **`open()`** – Abre arquivos para leitura ou escrita.
- **`read()`** – Lê dados de um arquivo para a memória.
- **`write()`** – Escreve dados da memória para um arquivo.

Dois programas foram criados:

- `io_primitives_iobound.c`: utiliza `BUFFER_SIZE = 64`, realizando muitas chamadas `read()`/`write()`, comportamento I/O-bound.
- `io_primitives_cpubound.c`: utiliza `BUFFER_SIZE = 4096`, com menos chamadas e maior uso de CPU, comportamento mais próximo de CPU-bound.

---

### 🧠 Memória

Local: `Memory_Files/src/`

Chamadas de sistema utilizadas:

- **`mmap()`** – Aloca uma região de memória diretamente do kernel.
- **`munmap()`** – Libera a região alocada.
- **`getpagesize()`** – Obtém o tamanho de página de memória do sistema.

O programa `mem_cpu_bound.c` demonstra alocação de múltiplas páginas de memória, preenchimento e soma de inteiros. Isso simula um cenário intensivo em CPU sem I/O, evidenciando o uso de primitivas de memória.

---

### ⚙️ Processos

Local: `Process/src/`

Chamadas de sistema utilizadas:

- **`fork()`** – Cria um novo processo.
- **`execve()`** – Substitui o processo atual por outro binário.
- **`wait()`** – Aguarda o término de um processo filho.

O programa `process_manager.c` ilustra a criação de dois filhos, cada um executando uma categoria diferente (I/O e memória). O processo pai sincroniza com ambos e exibe os resultados.

---

## 🪖 Comandos para Execução com Análise

### 🔁 Execução Normal

```bash
./Process/bin/process_manager
./IO_Files/bin/io_primitives_iobound ./IO_Files/data ./IO_Files/io_backup
./Memory_Files/bin/mem_cpu_bound
```

### 📊 Execução com `/usr/bin/time`

```bash
/usr/bin/time -v ./Process/bin/process_manager
/usr/bin/time -v ./IO_Files/bin/io_primitives_iobound ./IO_Files/data ./IO_Files/io_backup
/usr/bin/time -v ./Memory_Files/bin/mem_cpu_bound
```

### 🔍 Execução com `strace`

```bash
strace -f -o trace_process.txt ./Process/bin/process_manager
strace -o trace_io.txt ./IO_Files/bin/io_primitives_iobound ./IO_Files/data ./IO_Files/io_backup
strace -o trace_mem.txt ./Memory_Files/bin/mem_cpu_bound
```

---

## 👨‍💼 Autores

- Miguel Filippo Calhabeu, Marlon Mota, Raiad Scaggion
- Disciplina: SSC0541 - Sistemas Operacionais I
- Curso: Bacharelado em Sistemas de Informação - ICMC
- 1º semestre de 2025
