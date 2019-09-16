#include <stdlib.h>
#include <string.h>
#include <ctype.h> 
#include <stdio.h>

typedef struct listSymbol {
    char name[220];
    char type[220];
    char value[220];
    int length;
    struct listSymbol* next;
} symbolNode;

typedef struct symbolIdentifier {
    char *value;
    struct symbolIdentifier* next;
} identifierNode;

symbolNode* symbolTable;
symbolNode* insert();
symbolNode* findSymbol();
void putTypeIdentifierOnSymbolTable();
void concatenate();
void removeChar();
void saveTable();
// Symbol Identifier auxiliars
identifierNode* identifierList;
identifierNode* insertIdentifier();
identifierNode* findIdentifier();
void clearIdentifierList();

symbolNode* insert(char* value) {
    symbolNode* foundNode = findSymbol(value);
    if (foundNode != NULL) {
        return foundNode;
    }
    
    symbolNode* node = (symbolNode*) malloc(sizeof(symbolNode));
    int len = strlen(value);
    char* valueToInsert = (char*) malloc(len+1);
    strcpy(valueToInsert, value);
    
    // Is it a string constant?
    int isConstant = 0;
    if (valueToInsert[0] == '"') {
        removeChar(valueToInsert, '"');
        node->length = strlen(valueToInsert);
        isConstant = 1;
        strcpy(node->type, "STRING_CTE");
        // Is it a float constant?
    } else if (strchr(valueToInsert, '.') != NULL) {
        isConstant = 1;
        strcpy(node->type, "FLOAT_CTE");
        // Is it a integer constant?
    } else if (isdigit(valueToInsert[0]) != 0) {
        isConstant = 1;
        strcpy(node->type, "INTEGER_CTE");        
    }
    
    if (isConstant == 1) {
        strcpy(node->name, "");
        node->name[0] = '_' ;
        concatenate(node->name, valueToInsert);
    } else {
        strcpy(node->name, valueToInsert);
    }

    if (isConstant == 1) {
        strcpy(node->value, valueToInsert);
    }

    node->next = symbolTable;
    symbolTable = node;
    return node;
}

identifierNode* insertIdentifier(char *name) {
    identifierNode* foundNode = findIdentifier(name);
    if (foundNode != NULL) {
        return foundNode;
    }
    identifierNode* node = (identifierNode*) malloc(sizeof(identifierNode));
    int len = strlen(name);
    char* valueToInsert = (char *) malloc(len+1);
    strcpy(valueToInsert, name);
    node->value = valueToInsert;
    node->next = identifierList;
    identifierList = node;
    return node;
}

identifierNode* findIdentifier(char* value) {
    identifierNode* identifierNode = identifierList;
    while(identifierNode != NULL) {
        if (strcmp(value, identifierNode->value) == 0) {
            return identifierNode;
        }
        identifierNode = identifierNode->next;
    }
    return NULL;
}

void putTypeIdentifierOnSymbolTable(char* type) {
    identifierNode* identifierNode = identifierList;
    while(identifierNode != NULL) {
        symbolNode* symbol = findSymbol(identifierNode->value);
        // Symbol should never be NULL but just in case..
        if (symbol != NULL) {
            if (strlen(symbol->type) != 0) {
                fprintf(stderr, "\n ERROR: Variable %s has been already declared", symbol->name);
                exit(1);
            }
            int len = strlen(type);
            char* valueToInsert = (char*) malloc(len+1);
            strcpy(valueToInsert, type);
            strcpy(symbol->type, valueToInsert);
        }
        identifierNode = identifierNode->next;
    }
    clearIdentifierList();
}

void clearIdentifierList() {
    identifierList = NULL;
}


void saveTable() {
    FILE *file = fopen("ts.txt", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    symbolNode* current = symbolTable;
    fprintf(file, "NOMBRE\tTIPODATO\tVALOR\tLONGITUD\n");
    while(current != NULL) {
        fprintf(file, "%s\t%s\t%s\t%d\n", current->name, current->type, current->value, current->length);
        current = current->next;
    }
    fclose(file);
}

void concatenate(char* original, char* add) {
    original++;     
    while(*add){
        *original = *add;
        add++;
        original++;
    }
    *original = '\0';
}

void removeChar(char *s, int c){ 
    int j, n = strlen(s); 
    int i;
    for (i=j=0; i<n; i++) {
        if (s[i] != c) {
            s[j++] = s[i];
        } 
    } 
    s[j] = '\0'; 
}

symbolNode* findSymbol(char* value) {
    symbolNode* tableNode = symbolTable;
    while(tableNode != NULL){
        if ((tableNode->value != NULL && strcmp(value, tableNode->value) == 0) || (strcmp(value, tableNode->name) == 0)) {
            return tableNode;
        }
        tableNode = tableNode->next;
    }
    return NULL;
}