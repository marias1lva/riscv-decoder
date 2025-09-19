# Decodificador de Instruções RISC-V

## Sobre o Projeto
O programa processa um **arquivo de memória de instrução (ROM)** contendo **instruções RISC-V** em **hexadecimal** ou **binário** e realiza a **classificação** completa das instruções de acordo com seu formato: **Tipo R, Tipo I, Tipo S, Tipo B, Tipo U e Tipo J**. Para identificar o tipo de instrução, o software lê os **7 bits menos significativos** (bits 0 a 6) e compara o opcode com os valores especificados no guia prático do RISC-V (página 49). Após a identificação do formato, cada instrução é **separada em seus campos correspondentes**, como registradores de destino (`rd`), registradores fonte (`rs1`, `rs2`), códigos de função (`funct3`, `funct7`) e imediatos, aplicando extensão de sinal quando necessário. Essa abordagem permite que tanto instruções diretas quanto pseudo-instruções utilizadas na disciplina de Arquitetura de Computadores I sejam corretamente decodificadas e exibidas de forma detalhada.

O objetivo principal é aplicar **conceitos de representação binária**, **manipulação de strings** e **decodificação de instruções**, permitindo que o usuário visualize os componentes internos das instruções de forma didática.

## Funcionalidades
- Leitura de instruções em hexadecimal ou binário de 32 bits via arquivo de texto.
- Conversão de instruções para representação binária de 32 bits.
- Identificação do opcode e determinação do formato da instrução (R, I, S, B, U ou J).
- Extração dos campos da instrução:
  - Registradores (`rd`, `rs1`, `rs2`)
  - Imediatos (com extensão de sinal quando necessário)
  - Funções (`funct3` e `funct7`)
- Exibição detalhada das instruções e seus componentes no terminal.

## Organização dos Arquivos
- `main.cpp` - Lógica principal do programa: leitura do arquivo, conversão das instruções, decodificação e exibição dos campos.

## Como Funciona
1. **Entrada das Instruções:** O programa lê instruções de um arquivo de texto (`instrucoesHexa.txt` ou `instrucoesBinario.txt`).
2. **Conversão para Binário:** Cada linha é convertida para uma representação binária de 32 bits.
3. **Extração do Opcode:** O opcode é obtido a partir dos 7 bits menos significativos da instrução.
4. **Decodificação da Instrução:** Com base no opcode, o programa identifica o formato da instrução e extrai seus campos.
5. **Exibição dos Campos:** O programa mostra no terminal os registradores, funções e imediatos, facilitando a análise das instruções.

## Exemplo de Execução
Suponha que o arquivo `instrucoesHexa.txt` contenha:
```bash
0x00a58533
0x00500513
0x00b50663
```
Saída no terminal:
```bash
Instrucao: 0x00a58533
Formato: R
rd = 10
rs1 = 11
rs2 = 10
funct3 = 000
funct7 = 0000000
----------------------------------
Instrucao: 0x00500513
Formato: I
rd = 10
rs1 = 0
funct3 = 000
imm = 5
----------------------------------
Instrucao: 0x00b50663
Formato: B
rs1 = 10
rs2 = 11
funct3 = 000
imm = 12
----------------------------------

```

## Tecnologias Utilizadas
- Linguagem: C++
- Estruturas utilizadas:
  - **Strings e bitset:** Para manipulação de bits e conversão de valores hexadecimais/binários.
  - **Funções auxiliares:** Para extração de campos, conversão e extensão de sinal de imediatos.
 
## Como usar
1. Clone o repositório em seu computador com o comando abaixo:
```bash
git clone https://github.com/marias1lva/riscv-decoder.git
cd riscv-decoder
```
2. Compile o código usando um compilador de C++, como o g++:
```bash
g++ -o riscVDecoder main.cpp
```
3. Execute o programa:
```bash
./riscVDecoder
```
4. Certifique-se de ter um arquivo de instruções no mesmo diretório (instrucoesHexa.txt ou instrucoesBinario.txt) para que o programa possa ler e decodificar.

## Referências
- **Guia Prático do RISC-V** – Especificação de instruções e opcodes.
- **RISC-V Instruction Set Manual - Volume I: User-Level ISA** - Documentação oficial do conjunto de instruções RISC-V.
- **Bit Manipulation in C++** - Referência para operações de bitset e manipulação de bits em C++.
- **C++ Reference - stoi, stoul e manipulação de strings** - Para conversão de strings binárias e hexadecimais para inteiros.
