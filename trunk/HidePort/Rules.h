#pragma once

#include "Pch.h"


//////////////////////////////////////////////////////////////////////////////////////////////////


typedef struct _HIDE_LOCAL_PORT {
    WORD LocalPort;
} HIDE_LOCAL_PORT, * PHIDE_LOCAL_PORT;


//////////////////////////////////////////////////////////////////////////////////////////////////


void InitializeGenericTable();
bool InsertElementGenericTable(WORD LocalPort);
PHIDE_LOCAL_PORT LookupElementGenericTable(WORD LocalPort);
BOOL DeleteGenericTableElement(WORD LocalPort);
void EnumerateGenericTable();
void DeleteGenericTable();


//////////////////////////////////////////////////////////////////////////////////////////////////


class Rules
{

};
