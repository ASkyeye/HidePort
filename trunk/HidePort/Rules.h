#pragma once

#include "Pch.h"


//////////////////////////////////////////////////////////////////////////////////////////////////


typedef struct _HIDE_LOCAL_PORT {
    WORD LocalPort;//�����򣬷�������
} HIDE_LOCAL_PORT, * PHIDE_LOCAL_PORT;


#define MAX_LOCAL_PORTS 99 //���ض˿�֧�ֵĸ�����


//////////////////////////////////////////////////////////////////////////////////////////////////


void InitializeGenericTable();
bool InsertElementGenericTable(WORD LocalPort);
bool IsHideLocalPort(WORD LocalPort);
bool DeleteGenericTableElement(WORD LocalPort);
void EnumerateGenericTable();
void DeleteGenericTable();


//////////////////////////////////////////////////////////////////////////////////////////////////


class Rules
{

};
