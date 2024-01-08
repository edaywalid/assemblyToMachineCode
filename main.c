#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

enum OperandType
{
    registerType,
    memoryType,
    immediateType,
    segmentRegisterType
};

enum TypeDetails
{
    eightBitRegister,
    sixteenBitRegister,
    eightBitImmediate,
    sixteenBitImmediate,
    segmentRegister
};

enum RegistersInvolvedInMemory
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

enum OperationType
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

    int OperandType;
    int TypeDetails;
    int RegistersInvlovedInMemory;
    int addressingMode;
    String dataOrDisp;
    String Disp;
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
} StringMap;
typedef struct
{
    int RegistersInvolved;
    String code;
} intMap;

typedef struct Instruction
{
    String name;
    Operand operand1;
    Operand operand2;

} Instruction;

// ------ checking  --------------
String sixTeenregisters[] = {"AX", "BX", "CX", "DX", "SI", "DI", "SP", "BP"};
String eightregisters[] = {"AL", "BL", "CL", "DL", "AH", "BH", "CH", "DH"};
String segmentRegisters[] = {"CS", "DS", "SS", "ES"};
StringMap reg[] = {
    {"AX", "000"},
    {"CX", "001"},
    {"DX", "010"},
    {"BX", "011"},
    {"AL", "000"},
    {"CL", "001"},
    {"DL", "010"},
    {"BL", "011"},
    {"AH", "100"},
    {"CH", "101"},
    {"DH", "110"},
    {"BH", "111"},
    {"SP", "100"},
    {"BP", "101"},
    {"SI", "110"},
    {"DI", "111"},

};

intMap rm[] = {
    {BXandSI, "000"},
    {BXandDI, "001"},
    {BPandSI, "010"},
    {BPandDI, "011"},
    {SI, "100"},
    {DI, "101"},
    {BPorDirect, "110"},
    {BX, "111"},

};

StringMap sr[] = {
    {"ES", "00"},
    {"CS", "01"},
    {"SS", "10"},
    {"DS", "11"},
};

bool isMemoryType(String operand)
{
    if (operand[0] == '[' && operand[strlen(operand) - 1] == ']')
    {
        return true;
    }
    return false;
}

bool isRegisterType(String operand)
{
    int i = 0;
    while (i < 8)
    {
        if (strcmp(operand, sixTeenregisters[i]) == 0)
        {
            return true;
        }
        i++;
    }
    i = 0;
    while (i < 8)
    {
        if (strcmp(operand, eightregisters[i]) == 0)
        {
            return true;
        }
        i++;
    }
    return false;
}

bool isSegmentRegister(String operand)
{
    int i = 0;
    while (i < 4)
    {
        if (strcmp(operand, segmentRegisters[i]) == 0)
        {
            return true;
        }
        i++;
    }
    return false;
}

bool isImmediate(String oprand)
{
    return !isRegisterType(oprand) && !isMemoryType(oprand) && !isSegmentRegister(oprand);
}

bool isSubStringExist(String sub, String super)
{
    return strstr(super, sub) != NULL;
}

String DecToHex(long dec)
{
    String hex = malloc(sizeof(char) * 5);
    int i = 0;
    while (dec != 0)
    {
        int temp = 0;
        temp = dec % 16;
        if (temp < 10)
        {
            hex[i] = temp + 48;
            i++;
        }
        else
        {
            hex[i] = temp + 55;
            i++;
        }
        dec = dec / 16;
    }
    hex[i] = '\0';

    return hex;
}
// ------------------------------------------------------//

String getInstructionType(String operation)
{
    int i = 0;
    while (i < strlen(operation) && operation[i] == ' ')
    {
        i++;
    }
    if (i == strlen(operation))
    {
        printf("Error: No operation found\n");
        exit(0);
    }
    String instructionType = (String)malloc(sizeof(char) * 10);
    int j = 0;
    while (i < strlen(operation) && operation[i] != ' ')
    {
        instructionType[j++] = operation[i++];
    }
    instructionType[j] = '\0';

    return instructionType;
}

String getFirstOperand(String operation)
{
    int i = strlen(getInstructionType(operation));

    while (i < strlen(operation) && (operation[i] == ' '))
    {
        i++;
    }

    if (i == strlen(operation))
    {
        printf("Error: No operand found\n");
        exit(0);
    }

    String firstOperand = (String)malloc(sizeof(char) * 10);
    int j = 0;
    while (i < strlen(operation) && (operation[i] != ' ' && operation[i] != ','))
    {
        firstOperand[j++] = operation[i++];
    }
    firstOperand[j] = '\0';
    return firstOperand;
}

String getSecondOperand(String operation)
{
    int i = strlen(getInstructionType(operation));
    while (i < strlen(operation) && operation[i] == ' ')
    {
        i++;
    }

    if (i == strlen(operation))
    {
        printf("Error: No operand found\n");
        exit(0);
    }

    i += strlen(getFirstOperand(operation));

    while (i < strlen(operation) && (operation[i] == ' ' || operation[i] == ','))
    {
        i++;
    }

    String secondOperand = (String)malloc(sizeof(char) * 10);
    int j = 0;
    while (i < strlen(operation) && (operation[i] != ' ' && operation[i] != ','))
    {
        secondOperand[j++] = operation[i++];
    }

    secondOperand[j] = '\0';
    return secondOperand;
}

void setType(Operand *Operand)
{
    if (isRegisterType(Operand->value))
    {
        Operand->type.OperandType = registerType;
        return;
    }
    if (isMemoryType(Operand->value))
    {
        Operand->type.OperandType = memoryType;
        return;
    }
    if (isImmediate(Operand->value))
    {
        Operand->type.OperandType = immediateType;
        return;
    }
    if (isSegmentRegister(Operand->value))
    {
        Operand->type.OperandType = segmentRegisterType;
        return;
    }
}

void setTypeDetails(Operand *Operand)
{
    if (isRegisterType(Operand->value))
    {
        if (Operand->value[1] == 'X')
        {
            Operand->type.TypeDetails = sixteenBitRegister;
            return;
        }
        Operand->type.TypeDetails = eightBitRegister;
        return;
    }
    if (isImmediate(Operand->value))
    {
        if (strlen(Operand->value) <= 3)
        {
            Operand->type.TypeDetails = eightBitImmediate;
            return;
        }
        Operand->type.TypeDetails = sixteenBitImmediate;
        return;
    }

    if (isSegmentRegister(Operand->value))
    {
        Operand->type.TypeDetails = segmentRegister;
        return;
    }
    else
    {
        Operand->type.TypeDetails = -1;
    }
}

void SetAddressingMode(Operand *Operand)
{
    int counter = 0;
    int i = strlen(Operand->value) - 2;

    if (Operand->type.OperandType == memoryType)
    {
        if (Operand->value[i] != 'H')
        {
            Operand->type.addressingMode = memoryModeNoDisplacement;
            return;
        }
        else
        {
            while (i >= 0 && Operand->value[i] != '+')
            {
                counter++;
                i--;
            }

            if (i == -1)
            {
                Operand->type.addressingMode = memoryModeNoDisplacement;
                return;
            }
            else if (counter <= 3)
            {
                Operand->type.addressingMode = memoryMode8bitDisplacement;
                return;
            }
            else
            {
                Operand->type.addressingMode = memoryMode16bitDisplacement;
                return;
            }
        }
    }
    if (Operand->type.OperandType == registerType)
    {
        Operand->type.addressingMode = registerMode;
        return;
    }
    else
    {
        Operand->type.addressingMode = -1;
    }
}

int setOperationType(Operand *Operand1, Operand *Operand2)
{
    if (Operand1->type.OperandType == registerType && Operand2->type.OperandType == registerType)
    {
        return regToReg;
    }
    if (Operand1->type.OperandType == registerType && Operand2->type.OperandType == memoryType)
    {
        if (strcmp(Operand1->value, "AX") == 0 || strcmp(Operand1->value, "AL") == 0)
        {
            return MemToAcc;
        }
        return memToReg;
    }
    if (Operand1->type.OperandType == memoryType && Operand2->type.OperandType == registerType)
    {
        if (strcmp(Operand2->value, "AX") == 0 || strcmp(Operand2->value, "AL") == 0)
        {
            return AccToMem;
        }
        return regToMem;
    }
    if (Operand2->type.OperandType == immediateType && (Operand1->type.OperandType == registerType || Operand1->type.OperandType == memoryType))
    {
        if (Operand1->type.OperandType == registerType)
        {
            return ImmToReg;
        }
        return ImmToRegM;
    }
    if ((Operand1->type.OperandType == memoryType || Operand1->type.OperandType == registerType) && Operand2->type.OperandType == segmentRegisterType)
    {
        return SregTorm;
    }
    if (Operand1->type.OperandType == segmentRegisterType && (Operand2->type.OperandType == memoryType || Operand2->type.OperandType == registerType))
    {
        return rmToSreg;
    }
    else
    {
        printf("Error: Operation type not found\n");
        exit(0);
    }
}
void setRegistersInvolvedInMemory(Operand *Operand)
{
    if (Operand->type.OperandType == memoryType)
    {
        if (isSubStringExist("BX", Operand->value) && isSubStringExist("SI", Operand->value))
        {
            Operand->type.RegistersInvlovedInMemory = BXandSI;
            return;
        }
        if (isSubStringExist("BX", Operand->value) && isSubStringExist("DI", Operand->value))
        {
            Operand->type.RegistersInvlovedInMemory = BXandDI;
            return;
        }
        if (isSubStringExist("BP", Operand->value) && isSubStringExist("SI", Operand->value))
        {
            Operand->type.RegistersInvlovedInMemory = BPandSI;
            return;
        }
        if (isSubStringExist("BP", Operand->value) && isSubStringExist("DI", Operand->value))
        {
            Operand->type.RegistersInvlovedInMemory = BPandDI;
            return;
        }
        if (isSubStringExist("SI", Operand->value))
        {
            Operand->type.RegistersInvlovedInMemory = SI;
            return;
        }
        if (isSubStringExist("DI", Operand->value))
        {
            Operand->type.RegistersInvlovedInMemory = DI;
            return;
        }
        if (isSubStringExist("BP", Operand->value))
        {
            Operand->type.RegistersInvlovedInMemory = BPorDirect;
            return;
        }
        if (isSubStringExist("BX", Operand->value))
        {
            Operand->type.RegistersInvlovedInMemory = BX;
            return;
        }
        else
        {
            Operand->type.RegistersInvlovedInMemory = BPorDirect;
        }
    }
    else
    {
        Operand->type.RegistersInvlovedInMemory = -1;
    }
}

void setData(Operand *Operand)
{
    String Low = malloc(sizeof(3));
    String High = malloc(sizeof(3));

    String all = malloc(sizeof(5));

    if (Operand->type.OperandType == immediateType)
    {
        if (Operand->type.TypeDetails == eightBitImmediate)
        {
            Low[0] = Operand->value[0];
            Low[1] = Operand->value[1];
            Low[2] = '\0';
            strcpy(High, "");
        }
        else
        {
            High[0] = Operand->value[0];
            High[1] = Operand->value[1];
            High[2] = '\0';
            Low[0] = Operand->value[2];
            Low[1] = Operand->value[3];
            Low[2] = '\0';
        }
    }
    else
    {
        Operand->type.dataOrDisp = "";
        return;
    }
    strcpy(all, Low);
    strcat(all, High);
    Operand->type.dataOrDisp = all;
}

void setDisp(Operand *Operand)
{
    String Disp = malloc(sizeof(5));
    String Low = malloc(sizeof(3));
    String High = malloc(sizeof(3));

    if (Operand->type.OperandType == memoryType)
    {
        if (Operand->type.RegistersInvlovedInMemory == BPorDirect)
        {
            int i = 1;
            int counter = 0;
            while (Operand->value[i++] != 'H')
            {
                counter++;
            }

            if (counter <= 2)
            {
                Low[0] = Operand->value[1];
                Low[1] = Operand->value[2];
                Low[2] = '\0';
                strcpy(High, "");
            }

            else
            {
                High[0] = Operand->value[1];
                High[1] = Operand->value[2];
                High[2] = '\0';
                Low[0] = Operand->value[3];
                Low[1] = Operand->value[4];
                Low[2] = '\0';
            }
        }
        else if (Operand->type.addressingMode != memoryModeNoDisplacement)
        {
            int i = strlen(Operand->value) - 2;
            while (i > 0 && Operand->value[i] != '+')
            {
                i--;
            }
            if (i != 0)
            {
                int j = 0;
                while (i < strlen(Operand->value) - 2)
                {
                    Disp[j++] = Operand->value[++i];
                }
                Disp[j] = '\0';
                if (strlen(Disp) <= 2)
                {
                    Low[0] = Disp[0];
                    Low[1] = Disp[1];
                    Low[2] = '\0';
                    strcpy(High, "");
                }
                else
                {
                    High[0] = Disp[0];
                    High[1] = Disp[1];
                    High[2] = '\0';
                    Low[0] = Disp[2];
                    Low[1] = Disp[3];
                    Low[2] = '\0';
                }
            }
        }
        else
        {
            Operand->type.Disp = "";
            return;
        }
    }
    else
    {
        Operand->type.Disp = "";
        return;
    }
    strcpy(Disp, Low);
    strcat(Disp, High);
    Operand->type.Disp = Disp;
}

String getRegisterCode(String value)
{
    int i = 0;
    while (i < 16)
    {
        if (strcmp(value, reg[i].operation) == 0)
        {
            return reg[i].code;
        }
        i++;
    }
    return NULL;
}

String getRMcode(Operand Operand)
{

    int i = 0;
    while (i < 8)
    {
        if (Operand.type.RegistersInvlovedInMemory == rm[i].RegistersInvolved)
        {
            return rm[i].code;
        }
        i++;
    }
    return NULL;
}

String getSregCode(String value)
{
    int i = 0;
    while (i < 4)
    {
        if (strcmp(value, sr[i].operation) == 0)
        {
            return sr[i].code;
        }
        i++;
    }
    return NULL;
}

String regToreg(Operand op1, Operand op2)
{
    String W = malloc(sizeof(char) * 2);
    String D = malloc(sizeof(char) * 2);
    String MOD = malloc(sizeof(char) * 3);
    String reg = malloc(sizeof(char) * 4);
    String rm = malloc(sizeof(char) * 4);

    if (op1.type.TypeDetails == sixteenBitRegister)
    {
        strcpy(W, "1");
    }
    else
    {
        strcpy(W, "0");
    }

    strcpy(D, "1");

    strcpy(MOD, "11");

    strcpy(reg, getRegisterCode(op1.value));
    strcpy(rm, getRegisterCode(op2.value));

    String code = malloc(sizeof(char) * 20);
    strcpy(code, "100010");
    strcat(code, D);
    strcat(code, W);
    strcat(code, MOD);
    strcat(code, reg);
    strcat(code, rm);

    free(W);
    free(D);
    free(MOD);
    free(reg);
    free(rm);
    return code;
}
String regTomem(Operand op1, Operand op2)
{
    String W = malloc(sizeof(char) * 2);
    String D = malloc(sizeof(char) * 2);
    String MOD = malloc(sizeof(char) * 3);
    String reg = malloc(sizeof(char) * 4);
    String rm = malloc(sizeof(char) * 4);

    if (op2.type.TypeDetails == sixteenBitRegister)
    {
        strcpy(W, "1");
    }
    else
    {
        strcpy(W, "0");
    }

    strcpy(D, "0");

    switch (op1.type.addressingMode)
    {
    case memoryModeNoDisplacement:
        strcpy(MOD, "00");
        break;

    case memoryMode8bitDisplacement:
        strcpy(MOD, "01");
        break;

    case memoryMode16bitDisplacement:
        strcpy(MOD, "10");
        break;

    default:
        break;
    }

    strcpy(reg, getRegisterCode(op2.value));
    strcpy(rm, getRMcode(op1));

    String code = malloc(sizeof(char) * 20);
    strcpy(code, "100010");
    strcat(code, D);
    strcat(code, W);
    strcat(code, MOD);
    strcat(code, reg);
    strcat(code, rm);

    free(W);
    free(D);
    free(MOD);
    free(reg);
    free(rm);
    return code;
}
String memToreg(Operand op1, Operand op2)
{
    String W = malloc(sizeof(char) * 2);
    String D = malloc(sizeof(char) * 2);
    String MOD = malloc(sizeof(char) * 3);
    String reg = malloc(sizeof(char) * 4);
    String rm = malloc(sizeof(char) * 4);

    if (op1.type.TypeDetails == sixteenBitRegister)
    {
        strcpy(W, "1");
    }
    else
    {
        strcpy(W, "0");
    }

    strcpy(D, "1");

    switch (op2.type.addressingMode)
    {
    case memoryModeNoDisplacement:
        strcpy(MOD, "00");
        break;

    case memoryMode8bitDisplacement:
        strcpy(MOD, "01");
        break;

    case memoryMode16bitDisplacement:
        strcpy(MOD, "10");
        break;

    default:
        break;
    }

    strcpy(reg, getRegisterCode(op1.value));
    strcpy(rm, getRMcode(op2));

    String code = malloc(sizeof(char) * 20);
    strcpy(code, "100010");
    strcat(code, D);
    strcat(code, W);
    strcat(code, MOD);
    strcat(code, reg);
    strcat(code, rm);

    free(W);
    free(D);
    free(MOD);
    free(reg);
    free(rm);
    return code;
}
String immToRegM(Operand op1, Operand op2)
{
    // we assume that the first operand is memory Type
    String W = malloc(sizeof(char) * 2);
    String D = malloc(sizeof(char) * 2);
    String MOD = malloc(sizeof(char) * 3);
    String reg = malloc(sizeof(char) * 4);
    String rm = malloc(sizeof(char) * 4);

    if (op2.type.TypeDetails == sixteenBitImmediate)
    {
        strcpy(W, "1");
    }
    else
    {
        strcpy(W, "0");
    }

    strcpy(D, "");

    switch (op1.type.addressingMode)
    {
    case memoryModeNoDisplacement:
        strcpy(MOD, "00");
        break;

    case memoryMode8bitDisplacement:
        strcpy(MOD, "01");
        break;

    case memoryMode16bitDisplacement:
        strcpy(MOD, "10");
        break;

    default:
        break;
    }

    strcpy(reg, "000");
    strcpy(rm, getRMcode(op1));

    String code = malloc(sizeof(char) * 20);
    strcpy(code, "1100011");
    strcat(code, D);
    strcat(code, W);
    strcat(code, MOD);
    strcat(code, reg);
    strcat(code, rm);

    free(W);
    free(D);
    free(MOD);
    free(reg);
    free(rm);
    return code;
}
String immToReg(Operand op1, Operand op2)
{
    String W = malloc(sizeof(char) * 2);
    String D = malloc(sizeof(char) * 2);
    String MOD = malloc(sizeof(char) * 3);
    String reg = malloc(sizeof(char) * 4);
    String rm = malloc(sizeof(char) * 4);

    if (op1.type.TypeDetails == sixteenBitRegister)
    {
        strcpy(W, "1");
    }
    else
    {
        strcpy(W, "0");
    }

    strcpy(D, "");

    strcpy(MOD, "");

    strcpy(reg, getRegisterCode(op1.value));
    strcpy(rm, "");

    String code = malloc(sizeof(char) * 20);
    strcpy(code, "1011");
    strcat(code, D);
    strcat(code, W);
    strcat(code, MOD);
    strcat(code, reg);
    strcat(code, rm);

    free(W);
    free(D);
    free(MOD);
    free(reg);
    free(rm);
    return code;
}
String memToAcc(Operand op1)
{
    String W = malloc(sizeof(char) * 2);

    if (op1.type.TypeDetails == sixteenBitRegister)
    {
        strcpy(W, "1");
    }
    else
    {
        strcpy(W, "0");
    }

    String code = malloc(sizeof(char) * 20);
    strcpy(code, "1010000");
    strcat(code, W);

    free(W);
    return code;
}
String accToMem(Operand op2)
{
    String W = malloc(sizeof(char) * 2);

    if (op2.type.TypeDetails == sixteenBitRegister)
    {
        strcpy(W, "1");
    }
    else
    {
        strcpy(W, "0");
    }

    String code = malloc(sizeof(char) * 20);
    strcpy(code, "1010001");
    strcat(code, W);

    free(W);
    return code;
}
String rmTosreg(Operand op1, Operand op2)
{

    String MOD = malloc(sizeof(char) * 3);
    String SR = malloc(sizeof(char) * 3);
    String rm = malloc(sizeof(char) * 4);

    switch (op2.type.addressingMode)
    {
    case memoryModeNoDisplacement:
        strcpy(MOD, "00");
        break;

    case memoryMode8bitDisplacement:
        strcpy(MOD, "01");
        break;

    case memoryMode16bitDisplacement:
        strcpy(MOD, "10");
        break;

    case registerMode:
        strcpy(MOD, "11");
        break;

    default:
        break;
    }

    strcpy(SR, getSregCode(op1.value));
    strcpy(rm , op2.type.OperandType == registerType ? getRegisterCode(op2.value) : getRMcode(op2));

    String code = malloc(sizeof(char) * 20);
    strcpy(code, "10001110");
    strcat(code, MOD);
    strcat(code, "0");
    strcat(code, SR);
    strcat(code, rm);

    free(MOD);
    free(SR);
    free(rm);

    return code;
}
String sregTorm(Operand op1, Operand op2)
{
    String MOD = malloc(sizeof(char) * 3);
    String SR = malloc(sizeof(char) * 3);
    String rm = malloc(sizeof(char) * 4);

    switch (op1.type.addressingMode)
    {
    case memoryModeNoDisplacement:
        strcpy(MOD, "00");
        break;

    case memoryMode8bitDisplacement:
        strcpy(MOD, "01");
        break;

    case memoryMode16bitDisplacement:
        strcpy(MOD, "10");
        break;

    case registerMode:
        strcpy(MOD, "11");
        break;

    default:
        break;
    }

    SR = getSregCode(op2.value);
    rm = op1.type.OperandType == registerType ? getRegisterCode(op2.value) : getRMcode(op2);

    String code = malloc(sizeof(char) * 20);
    strcpy(code, "10001110");
    strcat(code, MOD);
    strcat(code, "0");
    strcat(code, SR);
    strcat(code, rm);

    free(MOD);
    free(SR);
    free(rm);

    return code;
}

int main()
{
    String operation = "MOV BL , [SI]";
    Instruction instruction;
    instruction.name = getInstructionType(operation);
    instruction.operand1.value = getFirstOperand(operation);
    instruction.operand2.value = getSecondOperand(operation);

    setType(&instruction.operand1);
    setType(&instruction.operand2);

    setTypeDetails(&instruction.operand1);
    setTypeDetails(&instruction.operand2);

    SetAddressingMode(&instruction.operand1);
    SetAddressingMode(&instruction.operand2);

    setRegistersInvolvedInMemory(&instruction.operand1);
    setRegistersInvolvedInMemory(&instruction.operand2);

    setDisp(&instruction.operand1);
    setDisp(&instruction.operand2);

    setData(&instruction.operand1);
    setData(&instruction.operand2);

    int OperationType = setOperationType(&instruction.operand1, &instruction.operand2);
    switch (OperationType)
    {
    case regToReg:
        printf("%lX", strtol(regToreg(instruction.operand1, instruction.operand2), NULL, 2));
        break;
    case regToMem:
        printf("%lX%s\n", strtol(regTomem(instruction.operand1, instruction.operand2), NULL, 2), instruction.operand1.type.Disp);
        break;
    case memToReg:
        printf("%lX%s\n", strtol(memToreg(instruction.operand1, instruction.operand2), NULL, 2), instruction.operand2.type.Disp);
        break;
    case ImmToRegM:
        printf("%lX%s%s\n", strtol(immToRegM(instruction.operand1, instruction.operand2), NULL, 2), instruction.operand2.type.dataOrDisp, instruction.operand1.type.Disp);
        break;
    case ImmToReg:
        printf("%lX%s\n", strtol(immToReg(instruction.operand1, instruction.operand2), NULL, 2), instruction.operand2.type.dataOrDisp);
        break;
    case MemToAcc:
        printf("%lX%s\n", strtol(memToAcc(instruction.operand1), NULL, 2), instruction.operand2.type.Disp);
        break;
    case AccToMem:
        printf("%lX%s\n", strtol(accToMem(instruction.operand2), NULL, 2), instruction.operand1.type.Disp);
        break;
    case rmToSreg:
        printf("%s%s\n", rmTosreg(instruction.operand1, instruction.operand2), instruction.operand2.type.Disp);
        break;
    case SregTorm:
        printf("%lX%s\n", strtol(sregTorm(instruction.operand1, instruction.operand2), NULL, 2), instruction.operand1.type.Disp);
        break;
    default:
        break;
    }


    return 0;
}