#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

enum GeneralTypes
{
    registerType,
    memoryType,
    immediateType,
    segmentRegisterType
};

enum SpecificTypes
{
    eightBitRegister,
    sixteenBitRegister,
    eightBitMemory,
    sixteenBitMemory,
    eightBitImmediate,
    sixteenBitImmediate,
    segmentRegister
};

enum MoreDetails
{
    BXandSI,
    BXandDI,
    BPandSI,
    BPandDI,
    SI,
    DI,
    BPorDirect,
    BX
};

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

typedef struct type
{

    int generalType;
    int specificType;
    int moreDetails;
    int addressingMode;
} Type;

typedef struct Operand
{
    String value;
    Type type;
} Operand;

typedef struct
{
    String operation;
    String code;
} Map;

String registers[] = {
    "ax", "bx", "cx", "dx", "al", "bl", "cl", "dl", "ah", "bh", "ch", "dh", "bp", "si", "di", "sp"};

String SEXTENbitRegisters[] = {
    "ax", "bx", "cx", "dx", "bp", "si", "di", "sp"};

String segments[] = {
    "cs", "ds", "es", "ss"};

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
    {"sp", "100"}};
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

bool isOperandSegmentRegister(String operand)
{
    for (int i = 0; i < 4; i++)
    {
        if (strcmp(operand, segments[i]) == 0)
        {
            return true;
        }
    }
    return false;
}

bool isOperandImmediate(String operand, String registers[])
{
    return !isOperandRegister(operand, registers) && !isOperandMemory(operand) && !isOperandSegmentRegister(operand);
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
    else if (isOperandSegmentRegister(operand1) && isOperandMemory(operand2))
    {
        mod = rmToSreg;
    }
    else if (isOperandMemory(operand1) && isOperandSegmentRegister(operand2))
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
        strcpy(D, "1");
        break;
    case regToMem:
        strcpy(D, "0");
        break;
    default:
        strcpy(D, "");
        break;
    }

    String W = malloc(sizeof(char) * 2);

    if (mod == SregTorm || mod == rmToSreg)
    {
        strcpy(W, "");
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

    if (mod == regToReg || mod == regToMem || mod == memToReg || mod == SregTorm || mod == rmToSreg)
    {
        if (mod == regToReg)
        {
            strcpy(mode, "11");
        }
        else if (mod == regToMem)
        {
        }
    }
    else
    {
        strcpy(mode, "");
    }
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

void checkImmidiateType(Operand *operand)
{
    if (operand->value[strlen(operand->value) - 1] == 'h' && strlen(operand->value) > 3)
    {
        operand->type.specificType = sixteenBitImmediate;
    }
    else if (operand->value[strlen(operand->value) - 1] == 'h')
    {
        operand->type.specificType = eightBitImmediate;
    }
    else if (atoi(operand->value) > 255)
    {
        operand->type.specificType = sixteenBitImmediate;
    }
    else
    {
        operand->type.specificType = eightBitImmediate;
    }
}

void checkMemoryType(Operand *operand)
{
    int i = 1; // we are sure that the first character is '['
    while (operand->value[i] != ']' && operand->value[i] != 'b' && operand->value[i] != 's' && operand->value[i] != 'd')
    {
        i++;
    }

    if (operand->value[i] == ']')
    {
        if (operand->value[i - 1] == 'h' && strlen(operand->value) > 5)
        {
            operand->type.specificType = sixteenBitMemory;
        }
        else if (operand->value[i - 1] == 'h')
        {
            operand->type.specificType = eightBitMemory;
        }
        else if (operand->value[i - 1] != 'h')
        {
            String number = (String)malloc(6 * sizeof(char));
            strncpy(number, operand->value + 1, i - 2);
            if (atoi(number) > 255)
            {
                operand->type.specificType = sixteenBitMemory;
            }
            else
            {
                operand->type.specificType = eightBitMemory;
            }
        }
        operand->type.moreDetails = BPorDirect;
    }
    else if (strstr(operand->value, "bx") != NULL && strstr(operand->value, "si") != NULL)
    {
        operand->type.moreDetails = BXandSI;
        operand->type.specificType = sixteenBitMemory;
    }
    else if (strstr(operand->value, "bx") != NULL && strstr(operand->value, "di") != NULL)
    {
        operand->type.moreDetails = BXandDI;
        operand->type.specificType = sixteenBitMemory;
    }
    else if (strstr(operand->value, "bp") != NULL && strstr(operand->value, "si") != NULL)
    {
        operand->type.moreDetails = BPandSI;
        operand->type.specificType = sixteenBitMemory;
    }
    else if (strstr(operand->value, "bp") != NULL && strstr(operand->value, "di") != NULL)
    {
        operand->type.moreDetails = BPandDI;
        operand->type.specificType = sixteenBitMemory;
    }
    else if (strstr(operand->value, "si") != NULL)
    {
        operand->type.moreDetails = SI;
        operand->type.specificType = sixteenBitMemory;
    }
    else if (strstr(operand->value, "di") != NULL)
    {
        operand->type.moreDetails = DI;
        operand->type.specificType = sixteenBitMemory;
    }
    else if (strstr(operand->value, "bp") != NULL)
    {
        operand->type.moreDetails = BPorDirect;
        operand->type.specificType = sixteenBitMemory;
    }
    else if (strstr(operand->value, "bx") != NULL)
    {
        operand->type.moreDetails = BX;
        operand->type.specificType = sixteenBitMemory;
    }
}

void setValue(Operand *operand, String value)
{
    operand->value = value;
}

void setGeneralType(Operand *operand)
{
    if (isOperandRegister(operand->value, registers))
    {
        operand->type.generalType = registerType;
    }
    else if (isOperandMemory(operand->value))
    {
        operand->type.generalType = memoryType;
    }
    else if (isOperandImmediate(operand->value, registers))
    {
        operand->type.generalType = immediateType;
    }
    else if (isOperandSegmentRegister(operand->value))
    {
        operand->type.generalType = segmentRegisterType;
    }
}

void setSpecificType(Operand *operand)
{
    if (isOperandRegister(operand->value, registers))
    {
        if (isSEXTENbitRegister(operand->value))
        {
            operand->type.specificType = sixteenBitRegister;
        }
        else
        {
            operand->type.specificType = eightBitRegister;
        }
    }
    else if (isOperandSegmentRegister(operand->value))
    {
        operand->type.specificType = segmentRegister;
    }
    else if (isOperandImmediate(operand->value, registers))
    {
        checkImmidiateType(operand);
    }
    else if (isOperandMemory(operand->value))
    {
        checkMemoryType(operand);
    }
}

void setAddressingMode(Operand *operand)
{
    if (operand->type.generalType == registerType)
    {
        operand->type.addressingMode = registerMode;
        return;
    }

    

    if (operand->type.moreDetails == BXandSI)
    {
        int i = 1;
        if(operand->value[i]!= 'b' || operand->value[i]!= 's')
        {
            int count = 0 ;
            while(operand->value[i] != '+')
            {
                count++;
                i++;
            }
            
        }

    }
}

void setMovInstructionMod(Operand *operand1, Operand *operand2, int *mode)
{
    if (operand1->type.generalType == registerType && operand2->type.generalType == registerType)
    {
        *mode = regToReg;
    }
    else if (operand1->type.generalType == registerType && operand2->type.generalType == memoryType)
    {
        *mode = memToReg;
    }
    else if (operand1->type.generalType == memoryType && operand2->type.generalType == registerType)
    {
        *mode = regToMem;
    }
    else if (operand1->type.generalType == registerMode && operand2->type.generalType == immediateType)
    {
        *mode = ImmToReg;
    }
    else if (operand1->type.generalType == segmentRegisterType && operand2->type.generalType == memoryType)
    {
        *mode = rmToSreg;
    }
    else if (operand1->type.generalType == memoryType && operand2->type.generalType == segmentRegisterType)
    {
        *mode = SregTorm;
    }
    else if (operand1->type.generalType == registerType && operand2->type.generalType == segmentRegisterType)
    {
        *mode = SregTorm;
    }
    else if (operand1->type.generalType == segmentRegisterType && operand2->type.generalType == registerType)
    {
        *mode = rmToSreg;
    }
    else if (operand1->type.generalType == memoryType && operand2->type.generalType == immediateType)
    {
        *mode = ImmToRegM;
    }
}

void setModValue(Operand *operand1, Operand *operand2, int *mode, String *MOD)
{
    if (*mode == regToReg || *mode == regToMem || *mode == memToReg || *mode == SregTorm || *mode == rmToSreg)
    {
        if (*mode == regToReg)
        {
            strcpy(*MOD, "11");
        }
    }
    else
    {
        strcpy(*MOD, "");
    }
}

int main()
{

    printf("Enter an instruction: ");
    String sentence = (String)malloc(100 * sizeof(char));
    readSentence(sentence);

    String operation = selectOperation(sentence);
    String firstOperand = getFirstOperand(sentence);
    String secondOperand = getSecondOperand(sentence);

    Operand *operand1 = malloc(sizeof(Operand));
    Operand *operand2 = malloc(sizeof(Operand));

    setValue(operand1, firstOperand);
    setValue(operand2, secondOperand);

    setGeneralType(operand1);
    setGeneralType(operand2);

    setSpecificType(operand1);
    setSpecificType(operand2);

    setAddressingMode(operand1);
    setAddressingMode(operand2);

    printf("%d\n", operand1->type.addressingMode);
    printf("%d\n", operand2->type.addressingMode);

    return 0;
}
