#include "Rules.h"


#ifdef RTL_USE_AVL_TABLES
RTL_AVL_TABLE g_LocalPortTable = {0}; //����ʱRtlInitializeGenericTable��һ���������Ͳ��ԡ�
#else
RTL_GENERIC_TABLE g_LocalPortTable = {0};
#endif

KSPIN_LOCK  g_LocalPortSpinLock;//���ڱ������������ͬ����ע�⣺DPC��

NPAGED_LOOKASIDE_LIST g_LocalPortLookaside;


//////////////////////////////////////////////////////////////////////////////////////////////////


RTL_GENERIC_COMPARE_RESULTS NTAPI CompareRoutine(__in struct _RTL_GENERIC_TABLE * Table,
                                                 __in PVOID  FirstStruct,
                                                 __in PVOID  SecondStruct
)
/*
RtlInsertElementGenericTable�ĵ�һ�ε��ò�����������ܷ��ֱ��ǿյġ�
*/
{
    PHIDE_LOCAL_PORT p1 = (PHIDE_LOCAL_PORT)FirstStruct;
    PHIDE_LOCAL_PORT p2 = (PHIDE_LOCAL_PORT)SecondStruct;

    UNREFERENCED_PARAMETER(Table);

    if (p1->LocalPort < p2->LocalPort) {
        return GenericLessThan;
    } else if (p1->LocalPort > p2->LocalPort) {
        return GenericGreaterThan;
    } else {
        return GenericEqual;
    }
}


PVOID NTAPI AllocateRoutine(__in struct _RTL_GENERIC_TABLE * Table, __in CLONG  ByteSize)
/*
����RtlInsertElementGenericTable�������
*/
{
    UNREFERENCED_PARAMETER(Table);

    return ExAllocatePoolWithTag(NonPagedPool, ByteSize, TAG);
}


VOID NTAPI FreeRoutine(__in struct _RTL_GENERIC_TABLE * Table, __in PVOID  Buffer)
/*
����RtlDeleteElementGenericTable�������
*/
{
    UNREFERENCED_PARAMETER(Table);

    ExFreePoolWithTag(Buffer, TAG);
}


//////////////////////////////////////////////////////////////////////////////////////////////////


void InitializeGenericTable()
{
    KeInitializeSpinLock(&g_LocalPortSpinLock);

    RtlInitializeGenericTable(&g_LocalPortTable, CompareRoutine, AllocateRoutine, FreeRoutine, NULL);

    ExInitializeNPagedLookasideList(&g_LocalPortLookaside, NULL, NULL, 0, sizeof(HIDE_LOCAL_PORT), TAG, 0);
}


bool InsertElementGenericTable(WORD LocalPort)
{
    PVOID p = NULL;
    BOOLEAN NewElement = FALSE;
    KIRQL oldIrql;
    bool ret = false;

    PHIDE_LOCAL_PORT Element = (PHIDE_LOCAL_PORT)ExAllocateFromNPagedLookasideList(&g_LocalPortLookaside);
    if (!Element) {

        return ret;
    }

    Element->LocalPort = LocalPort;

    KeAcquireSpinLock(&g_LocalPortSpinLock, &oldIrql);
    p = RtlInsertElementGenericTable(&g_LocalPortTable, Element, g_LocalPortLookaside.L.Size, &NewElement);
    KeReleaseSpinLock(&g_LocalPortSpinLock, oldIrql);

    if (!p) {
        ExFreeToNPagedLookasideList(&g_LocalPortLookaside, Element);
    } else {
        ret = true;
    }

    return ret;
}


PHIDE_LOCAL_PORT LookupElementGenericTable(WORD LocalPort)
/*
���ܣ�����ĳ��Ԫ�ص�������Ա����Ϣ��
*/
{
    PVOID p = NULL;
    PHIDE_LOCAL_PORT pc = NULL;
    KIRQL oldIrql;
    HIDE_LOCAL_PORT Element{};

    Element.LocalPort = LocalPort;

    KeAcquireSpinLock(&g_LocalPortSpinLock, &oldIrql);
    p = RtlLookupElementGenericTable(&g_LocalPortTable, &Element);
    if (p) {
        pc = (PHIDE_LOCAL_PORT)p;
    } 
    KeReleaseSpinLock(&g_LocalPortSpinLock, oldIrql);

    return pc;
}


BOOL DeleteGenericTableElement(WORD LocalPort)
/*
�÷���
*/
{
    PVOID p = NULL;
    BOOL B = FALSE;
    KIRQL oldIrql;
    HIDE_LOCAL_PORT Element{};

    Element.LocalPort = LocalPort;

    KeAcquireSpinLock(&g_LocalPortSpinLock, &oldIrql);
    p = RtlLookupElementGenericTable(&g_LocalPortTable, &Element);
    if (p) {
        RtlDeleteElementGenericTable(&g_LocalPortTable, p);
        ExFreeToNPagedLookasideList(&g_LocalPortLookaside, p);
        B = TRUE;
    }
    KeReleaseSpinLock(&g_LocalPortSpinLock, oldIrql);

    return B;
}


void EnumerateGenericTable()
{
    ULONG i = 0;
    KIRQL oldIrql;

    KeAcquireSpinLock(&g_LocalPortSpinLock, &oldIrql);
    for (; i < RtlNumberGenericTableElements(&g_LocalPortTable); i++) {
        PHIDE_LOCAL_PORT Element = (PHIDE_LOCAL_PORT)RtlGetElementGenericTable(&g_LocalPortTable, i);
        DBG_UNREFERENCED_LOCAL_VARIABLE(Element);
    }
    KeReleaseSpinLock(&g_LocalPortSpinLock, oldIrql);
}


void DeleteGenericTable()
/*
���ܣ�ɾ��������
ժ�ԣ�Windows 8 Driver Samples\AvScan File System Minifilter Driver\C++\filter\avscan.c
*/
{
    KIRQL oldIrql;

    KeAcquireSpinLock(&g_LocalPortSpinLock, &oldIrql);
    while (!RtlIsGenericTableEmpty(&g_LocalPortTable)) {
        PVOID Element = RtlGetElementGenericTable(&g_LocalPortTable, 0);
        RtlDeleteElementGenericTable(&g_LocalPortTable, Element);
        ExFreeToNPagedLookasideList(&g_LocalPortLookaside, Element);
    }
    KeReleaseSpinLock(&g_LocalPortSpinLock, oldIrql);

    ExDeleteNPagedLookasideList(&g_LocalPortLookaside);
}


//////////////////////////////////////////////////////////////////////////////////////////////////
