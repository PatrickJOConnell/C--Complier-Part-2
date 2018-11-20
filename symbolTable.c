// Patrick O'Connell
// CS 473
// Project #2
// oconne16

#include "symbolTable.h"


SymbolTableStackEntryPtr symbolStackTop;


int scopeDepth;


int hash(char *name) {
	int i = 0;
	int hashPartial = 13;
	while (name[i] != '\0') {
		hashPartial += 3*name[i];
		i++;
	}
	return hashPartial % MAXHASHSIZE;
}


int initSymbolTable() {
	scopeDepth = 1;  									//set scopeDepth to 0
	symbolStackTop = (SymbolTableStackEntryPtr)malloc(sizeof(SymbolTableStackEntry));	//allocate the global scope entry
	symbolStackTop->symbolTablePtr = (SymbolTablePtr)malloc(sizeof(struct symbolTable)); 	//allocate the symbol table
	return(!(symbolStackTop == NULL)&&!(symbolStackTop->symbolTablePtr == NULL));
}


ElementPtr symLookup(char *name){
	ElementPtr returnPtr = NULL;
	SymbolTableStackEntryPtr stsep = symbolStackTop;
	int findMe = hash(name);
	while(stsep){
		returnPtr = stsep->symbolTablePtr->hashTable[findMe];
		while(returnPtr){
			if(strcmp(returnPtr->id, name) == 0)
				return returnPtr;
			returnPtr = returnPtr->next;
		}
	}
	return NULL;	
}


ElementPtr symInsert(char *name, struct type *type, int line)
{
	ElementPtr insertMe = (HashTableEntry)malloc(sizeof(Element));
	if(!insertMe){
		return NULL;
	}
	insertMe->key = hash(name);
	insertMe->id = name;
	insertMe->linenumber = line;
	insertMe->scope = scopeDepth;
	insertMe->stype = type;
	insertMe->next = symbolStackTop->symbolTablePtr->hashTable[insertMe->key];
	symbolStackTop->symbolTablePtr->hashTable[insertMe->key] = insertMe;  //insert newly created element
	return insertMe; 
}


int enterScope(){
	SymbolTableStackEntryPtr enteringScope = (SymbolTableStackEntryPtr)malloc(sizeof(SymbolTableStackEntry));
	if(!enteringScope)
		return 0;
	enteringScope->symbolTablePtr = (SymbolTablePtr)malloc(sizeof(struct symbolTable));
	if(!(enteringScope->symbolTablePtr))
		return 0;
	enteringScope->prevScope = symbolStackTop;
	symbolStackTop = enteringScope;
	scopeDepth += 1;
	return 1;
}


void leaveScope(){
	if(scopeDepth == 0){ 
		return;
	}
	symbolStackTop = symbolStackTop->prevScope;
	scopeDepth--;
}


void printElement(ElementPtr symelement) {
	if (symelement != NULL) {
		printf("Line %d: %s", symelement->linenumber,symelement->id);
	}
	else printf("Wrong call! symbol table entry NULL");
}


void printSymbolTable(){
	int i;
	SymbolTableStackEntryPtr iteratingPtr1 = symbolStackTop;
	SymbolTablePtr iteratingPtr2 = NULL;
	while(iteratingPtr1) {
		iteratingPtr2 = iteratingPtr1->symbolTablePtr;
		for(i = 0; i < MAXHASHSIZE; ++i) {
			if(iteratingPtr2->hashTable[i]){
				printElement(iteratingPtr2->hashTable[i]);
				printf("\n");
			}
		}
		iteratingPtr1 = iteratingPtr1->prevScope;
	}
}














