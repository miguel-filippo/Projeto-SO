#!/bin/bash

echo "🔧 Compilando arquivos..."

# Diretórios de saída para os binários
mkdir -p IO_Files/bin
mkdir -p Memory/bin
mkdir -p Process/bin

# Compilando I/O
gcc -O2 -o IO_Files/bin/io_primitives_cpubound IO_Files/src/io_primitives_cpubound.c && \
echo "✅ Compilado: io_primitives_cpubound"

gcc -O2 -o IO_Files/bin/io_primitives_iobound IO_Files/src/io_primitives_iobound.c && \
echo "✅ Compilado: io_primitives_iobound"

# Compilando memória
gcc -O2 -o Memory/bin/mem_cpu_bound Memory/src/mem_cpu_bound.c && \
echo "✅ Compilado: mem_cpu_bound"

# Compilando processo
gcc -O2 -o Process/bin/process_manager Process/src/process_manager.c && \
echo "✅ Compilado: process_manager"

echo "🎉 Todos os binários foram compilados com sucesso!"
