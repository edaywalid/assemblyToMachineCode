#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef char *String;

typedef struct
{
    String operation;
    String code;
} Map;

Map operationMap[] = {
    {"mov", "100010"}};

String registers[] = {
    "ax", "bx", "cx", "dx", "al", "bl", "cl", "dl", "ah", "bh", "ch", "dh", "bp", "si", "di", "sp"};

String SEXTENbitRegisters[] = {
    "ax", "bx", "cx", "dx", "bp", "si", "di", "sp"};

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

bool isRegister(String operand)
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

String selectOperation(String sentence) {
    String operation = (String)malloc(3 * sizeof(char));
    int i = 0;
    while (sentence[i] == ' ') {
        i++;
    }
    int j = 0;
    while (sentence[i] != ' ') {
        operation[j] = sentence[i];
        j++;
        i++;
    }
    return operation;
}

String findInMap(String operation, Map map[], int mapSize)
{
    for (int i = 0; i < mapSize; i++)
    {
        if (strcmp(operation, map[i].operation) == 0)
        {
            return map[i].code;
        }
    }
    return "Opcode not found";
}

String getRegOrRM(String operand)
{
    if (isRegister(operand))
    {
        return findInMap(operand, REG, 16);
    }
    else
    {
        return "000";
    }
}
int main()
{
    int mapSize = sizeof(operationMap) / sizeof(operationMap[0]);

    printf("Enter an instruction: ");
    String sentence = (String)malloc(100 * sizeof(char));
    readSentence(sentence);

    String operation = selectOperation(sentence);

    String opcode = findInMap(operation, operationMap, mapSize);
    String firstOperand = getFirstOperand(sentence);
    String secondOperand = getSecondOperand(sentence);

    bool isDestinationRegister = isRegister(firstOperand);
    bool isSourceRegister = isRegister(secondOperand);



    String reg = malloc(4 * sizeof(char));
    String rm = malloc(4 * sizeof(char));


    String D = malloc(2 * sizeof(char));
    String W = malloc(2 * sizeof(char));


    if(isSEXTENbitRegister(firstOperand) && isSEXTENbitRegister(secondOperand)){
        strcpy(W, "1");
    }else {
        strcpy(W, "0");
    }


    if (isDestinationRegister)
    {
        strcpy(D, "1");
        reg = findInMap(firstOperand, REG, 16);
        rm = getRegOrRM(secondOperand);
    }
    else
    {
        strcpy(W, "1");
        reg = findInMap(secondOperand, REG, 16);
        rm = getRegOrRM(firstOperand);
    }

    String result = malloc(20 * sizeof(char));

    strcpy(result , opcode);
    strcat(result , D);
    strcat(result , W);
    strcat(result , "11");
    strcat(result , reg);
    strcat(result , rm);

    printf("result: %s\n", result);
    free(sentence);
    return 0;
}
