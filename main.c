#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

enum ModValues
{
    regToReg,
    regToMem,
    memToReg,
    ImmToRegM,
    ImmToReg,
    MemToAcc,
    AccToMem,
    rmToSreg,
    SregTorm
};

enum AddressingModes
{
    
    memoryModeNoDisplacement,
    memoryMode8bitDisplacement,
    memoryMode16bitDisplacement,
    registerMode,
};  

typedef char *String;

typedef struct
{
    String operation;
    String code;
} Map;


String registers[] = {
    "ax", "bx", "cx", "dx", "al", "bl", "cl", "dl", "ah", "bh", "ch", "dh", "bp", "si", "di", "sp"};

String SEXTENbitRegisters[] = {
    "ax", "bx", "cx", "dx", "bp", "si", "di", "sp"};

String segments [] = {
    "cs", "ds", "es", "ss"
};

bool isSEXTENbitRegister(String operand)
{
    for (int i = 0; i < 8; i++)
    {
        if (strcmp(operand, SEXTENbitRegisters[i]) == 0)
        {
            return true;
        }
    }
    return false;
}
Map REG[] = {
    {"ax", "000"},
    {"bx", "001"},
    {"cx", "010"},
    {"dx", "011"},
    {"al", "000"},
    {"bl", "001"},
    {"cl", "010"},
    {"dl", "011"},
    {"ah", "100"},
    {"bh", "101"},
    {"ch", "110"},
    {"dh", "111"},
    {"bp", "101"},
    {"si", "110"},
    {"di", "111"},
    {"sp", "100"}

};
bool isOperandRegister(String operand, String registers[])
{
    for (int i = 0; i < 16; i++)
    {
        if (strcmp(operand, registers[i]) == 0)
        {
            return true;
        }
    }
    return false;
}

bool isOperandMemory(String operand)
{
    if (operand[0] == '[' && operand[strlen(operand) - 1] == ']')
    {
        return true;
    }
    return false;
}

bool isOperandImmediate(String operand, String registers[])
{
    return !isOperandRegister(operand, registers) && !isOperandMemory(operand);
}

String MovInstruction(String operand1, String operand2, String registers[])
{
    String opcode[] = {
        "100010", "1100011", "1011", "1010000", "1010001", "10001110", "10001100"};
    int mod;
    if (isOperandRegister(operand1, registers) && isOperandRegister(operand2, registers))
    {
        mod = regToReg;
    }
    else if (isOperandRegister(operand1, registers) && isOperandMemory(operand2))
    {
        mod = regToMem;
    }
    else if (isOperandMemory(operand1) && isOperandRegister(operand2, registers))
    {
        mod = memToReg;
    }
    else if (isOperandImmediate(operand2, registers) && isOperandRegister(operand1, registers))
    {
        mod = ImmToReg;
    }
    else if (isOperandImmediate(operand2, registers) && isOperandMemory(operand1))
    {
        mod = ImmToRegM; 
    }
    else if (isOperandMemory(operand1) && strcmp(operand2, "ax") == 0)
    {
        mod = MemToAcc;
    }
    else if (strcmp(operand1, "ax") == 0 && isOperandMemory(operand2))
    {
        mod = AccToMem;
    }
    else if (isSEXTENbitRegister(operand1) && isOperandMemory(operand2))
    {
        mod = rmToSreg;
    }
    else if (isOperandMemory(operand1) && isSEXTENbitRegister(operand2))
    {
        mod = SregTorm;
    }
    else
    {
        printf("Invalid operands\n");
        exit(0);
    }

    String MovOpcode = malloc(sizeof(char) * 8);
    switch (mod)
    {
    case regToReg:
        strcpy(MovOpcode, opcode[0]);
        break;
    case regToMem:
        strcpy(MovOpcode, opcode[0]);
        break;
    case memToReg:
        strcpy(MovOpcode, opcode[0]);
        break;
    case ImmToRegM:
        strcpy(MovOpcode, opcode[1]);
        break;
    case ImmToReg:
        strcpy(MovOpcode, opcode[2]);
        break;
    case MemToAcc:
        strcpy(MovOpcode, opcode[3]);
        break;
    case AccToMem:
        strcpy(MovOpcode, opcode[4]);
        break;
    case rmToSreg:
        strcpy(MovOpcode, opcode[5]);
        break;
    case SregTorm:
        strcpy(MovOpcode, opcode[6]);
        break;
    default:

        break;
    }

    String D = malloc(sizeof(char) * 2);

    switch (mod)
    {

    case regToReg:
        D = "1";
        break;
    case regToMem:
        D = "0";
        break;
    default:
        D = "";
        break;
    }

    String W = malloc(sizeof(char) * 2);

    if (mod == SregTorm || mod == rmToSreg)
    {
        W = "";
    }
    else if (isSEXTENbitRegister(operand1) || isSEXTENbitRegister(operand2))
    {
        strcpy(W, "1");
    }
    else
    {
        strcpy(W, "0");
    }
    String mode = malloc(sizeof(char) * 3);
    String reg = malloc(4 * sizeof(char));
    String rm = malloc(4 * sizeof(char));

    return MovOpcode;
}



void readSentence(String sentence)
{
    fgets(sentence, 100, stdin);
    sentence[strcspn(sentence, "\n")] = '\0';
}

String getFirstOperand(String sentence)
{
    String operand = (String)malloc(7 * sizeof(char));
    sscanf(sentence, "%*s %s", operand);
    return operand;
}

String getSecondOperand(String sentence)
{
    String operand = (String)malloc(7 * sizeof(char));
    int i = 0;
    while (sentence[i] == ' ')
    {
        i++;
    }
    while (sentence[i] != ' ')
    {
        i++;
    }
    while (sentence[i] == ' ')
    {
        i++;
    }
    while (sentence[i] != ',')
    {
        i++;
    }
    i++;
    while (sentence[i] == ' ')
    {
        i++;
    }
    int j = 0;
    while (sentence[i] != '\0')
    {
        operand[j] = sentence[i];
        i++;
        j++;
    }
    return operand;
}

String selectOperation(String sentence)
{
    String operation = (String)malloc(3 * sizeof(char));
    int i = 0;
    while (sentence[i] == ' ')
    {
        i++;
    }
    int j = 0;
    while (sentence[i] != ' ')
    {
        operation[j] = sentence[i];
        j++;
        i++;
    }
    return operation;
}




int main()
{
    
    printf("Enter an instruction: ");
    String sentence = (String)malloc(100 * sizeof(char));
    readSentence(sentence);

    String operation = selectOperation(sentence);
    String firstOperand = getFirstOperand(sentence);
    String secondOperand = getSecondOperand(sentence);

    String result = MovInstruction(firstOperand, secondOperand, registers);

    printf("result: %s\n", result);

    free(sentence);
    return 0;
}
