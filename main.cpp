#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
using namespace std;

//pra converter um número inteiro de 32 bits em uma string binária de 32 caracteres
string toBin32(uint32_t x){
    bitset<32> b(x); //cria conjunto de 32 bits
    return b.to_string(); 
}

//converte uma linha do arquivo para binário de 32 bits => isso serve tanto pra hexadecimal quanto para binário puro
string lineToBin32(const string &line){
    string cleanLine = line;

    //remove prefixo "0x" ou "0X" se tiver  
    if(cleanLine.rfind("0x", 0) == 0 || cleanLine.rfind("0X", 0) == 0){
        cleanLine = cleanLine.substr(2);
    }

    //se a linha tiver apenas 0s e 1s e ter 32 caracteres = já é binário
    if(cleanLine.find_first_not_of("01") == string::npos && cleanLine.size() == 32){
        return cleanLine;
    }else{
        //se não, considera que é hexadecimal e converte para binário
        unsigned int x = stoul(cleanLine, nullptr, 16); //stoul converte string => inteiro base 16
        return toBin32(x); //retorna o número convertido para binário de 32 bits
    }
}

//extrai os bits do opcode => 7 bits menos significativos = posição 0 a 6
string getOpcode(const string &binStr){
    return binStr.substr(25, 7); //pega 7 caracteres a partir da posição 25
}

//converte string binária para inteiro
int binStrToInt(const string &s){
    return stoi(s, nullptr, 2);
}

//função que vai fazer extensão de sinal em imediatos => recebe uma string binária e devolve o valor em inteiro (com sinal!!)
int signExtend(const string &binStr){
    int val = stoi(binStr, nullptr, 2); //converte para inteiro sem sinal
    int bits = binStr.size(); //quantos bits o imediato tem
    if(binStr[0] == '1'){ //se o bit mais significativo for 1 => número negativo
        val -= (1 << bits); //aplica extensão de sinal (subtrai 2^bits)
    }
    return val;
}

int main(){
    //abre o arquivo de instruções (pode ser hexa ou binário)
    ifstream file("instrucoesHexa.txt"); //mudar para 'instrucoesBinario.txt' ou 'instrucoesHexa.txt' dependendo do que quiser ler
    string line;

    //lê o arquivo linha por linha
    while(file >> line){
        string binInstr = lineToBin32(line); //converte para binário de 32 bits

        //extrai opcode em int (pra usar no switch)
        int opc = binStrToInt(getOpcode(binInstr));

        //exibe a instrução original
        cout << "Instrucao: " << line << endl;

        //usa switch-case pra decidir qual formato de instrução é (R, I, S, B, U, J)
        //cada case corresponde a um opcode específico do risc-v
        //decodificação conforme o opcode usando switch-case
        switch(opc){
            case 0b0110011: //tipo R
                cout << "Formato: R" << endl;
                //R: [funct7][rs2][rs1][funct3][rd][opcode]
                cout << "rd = " << stoi(binInstr.substr(20, 5), nullptr, 2) << endl; //bits [11..7]
                cout << "rs1 = " << stoi(binInstr.substr(12, 5), nullptr, 2) << endl; //bits [19..15]
                cout << "rs2 = " << stoi(binInstr.substr(7, 5), nullptr, 2) << endl;  //bits [24..20]
                cout << "funct3 = " << binInstr.substr(17, 3) << endl; //bits [14..12]
                cout << "funct7 = " << binInstr.substr(0, 7) << endl;  //bits [31..25]
                break;

            case 0b0010011: //tipo I (OP-IMM)
            case 0b0000011: //tipo I (LOAD)
            case 0b1100111: //tipo I (JALR)
                cout << "Formato: I" << endl;
                //I: [imm][rs1][funct3][rd][opcode]
                cout << "rd = " << stoi(binInstr.substr(20, 5), nullptr, 2) << endl;
                cout << "rs1 = " << stoi(binInstr.substr(12, 5), nullptr, 2) << endl;
                cout << "funct3 = " << binInstr.substr(17, 3) << endl;
                cout << "imm = " << signExtend(binInstr.substr(0, 12)) << endl; //bits [31..20]
                break;

            case 0b0100011: { //tipo S
                cout << "Formato: S" << endl;
                //S: [imm(11..5)][rs2][rs1][funct3][imm(4..0)][opcode] (o imediato é formado por duas partes)
                string imm = binInstr.substr(0, 7) + binInstr.substr(20, 5); //junta imm[11..5] + imm[4..0]
                cout << "rs1 = " << stoi(binInstr.substr(12, 5), nullptr, 2) << endl;
                cout << "rs2 = " << stoi(binInstr.substr(7, 5), nullptr, 2) << endl;
                cout << "funct3 = " << binInstr.substr(17, 3) << endl;
                cout << "imm = " << signExtend(imm) << endl;
                break;
            }

            case 0b1100011: { //tipo B
                cout << "Formato: B" << endl;
                //B: [imm(12)][imm(10..5)][rs2][rs1][funct3][imm(4..1)][imm(11)][opcode] (o imediato é montado de várias partes)
                string imm = binInstr.substr(0,1) + //imm[12] = bit 31
                             binInstr.substr(24,1) + //imm[11] = bit 7
                             binInstr.substr(1,6) +  //imm[10..5] = bits [30..25]
                             binInstr.substr(20,4) + "0"; //imm[4..1] = bits [11..8], bit menos significativo = sempre 0
                cout << "rs1 = " << stoi(binInstr.substr(12, 5), nullptr, 2) << endl;
                cout << "rs2 = " << stoi(binInstr.substr(7, 5), nullptr, 2) << endl;
                cout << "funct3 = " << binInstr.substr(17, 3) << endl;
                cout << "imm = " << signExtend(imm) << endl;
                break;
            }

            case 0b0110111: //tipo U (LUI)
            case 0b0010111: //tipo U (AUIPC)
                cout << "Formato: U" << endl;
                //U: [imm(31..12)][rd][opcode] => imediato ocupa 20 bits e termina com 12 zeros
                cout << "rd = " << stoi(binInstr.substr(20, 5), nullptr, 2) << endl;
                cout << "imm = " << signExtend(binInstr.substr(0, 20) + "000000000000") << endl;
                break;

            case 0b1101111: { //tipo J (JAL)
                cout << "Formato: J" << endl;
                // J: [imm(20)][imm(10..1)][imm(11)][imm(19..12)][rd][opcode] (imediato é quebrado em várias partes tbm)
                string imm = binInstr.substr(0,1) + //imm[20] = bit 31
                             binInstr.substr(12,8) + //imm[19..12] = bits [19..12]
                             binInstr.substr(11,1) + //imm[11] = bit 20
                             binInstr.substr(1,10) + "0"; //imm[10..1] = bits [30..21], bit menos significativo = sempre 0
                cout << "rd = " << stoi(binInstr.substr(20, 5), nullptr, 2) << endl;
                cout << "imm = " << signExtend(imm) << endl;
                break;
            }

            default:
                cout << "Formato: Desconhecido" << endl;
                break;
        }

        cout << "----------------------------------" << endl;
    }
    return 0;
}
