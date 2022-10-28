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
    UNREFERENCED_PARAMETER(ByteSize);

    //return ExAllocatePoolWithTag(NonPagedPool, ByteSize, TAG);
    return ExAllocateFromNPagedLookasideList(&g_LocalPortLookaside);
}


VOID NTAPI FreeRoutine(__in struct _RTL_GENERIC_TABLE * Table, __in PVOID  Buffer)
/*
����RtlDeleteElementGenericTable�������
*/
{
    UNREFERENCED_PARAMETER(Table);

    //ExFreePoolWithTag(Buffer, TAG);
    ExFreeToNPagedLookasideList(&g_LocalPortLookaside, Buffer);
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
    BOOLEAN NewElement = FALSE;
    KIRQL oldIrql;
    bool ret = false;
    ULONG NumberGenericTableElements = 0;

    KeAcquireSpinLock(&g_LocalPortSpinLock, &oldIrql);
    NumberGenericTableElements = RtlNumberGenericTableElements(&g_LocalPortTable);
    KeReleaseSpinLock(&g_LocalPortSpinLock, oldIrql);

    if (NumberGenericTableElements > MAX_LOCAL_PORTS) {

        return ret;
    }

    HIDE_LOCAL_PORT Element{};
    Element.LocalPort = LocalPort;

    KeAcquireSpinLock(&g_LocalPortSpinLock, &oldIrql);
    if (RtlInsertElementGenericTable(&g_LocalPortTable, &Element, g_LocalPortLookaside.L.Size, &NewElement)) {
        ret = true;
    }
    KeReleaseSpinLock(&g_LocalPortSpinLock, oldIrql);    

    return ret;
}


bool IsHideLocalPort(WORD LocalPort)
/*
���ܣ�����ĳ��Ԫ�ص�������Ա����Ϣ��
*/
{
    KIRQL oldIrql;
    HIDE_LOCAL_PORT Element{};
    bool ret = false;

    Element.LocalPort = LocalPort;

    KeAcquireSpinLock(&g_LocalPortSpinLock, &oldIrql);
    if (RtlLookupElementGenericTable(&g_LocalPortTable, &Element)) {
        ret = true;;
    } 
    KeReleaseSpinLock(&g_LocalPortSpinLock, oldIrql);

    return ret;
}


bool DeleteGenericTableElement(WORD LocalPort)
/*
�÷���
*/
{
    PVOID Temp = NULL;
    bool B = FALSE;
    KIRQL oldIrql;
    HIDE_LOCAL_PORT Element{};

    Element.LocalPort = LocalPort;

    KeAcquireSpinLock(&g_LocalPortSpinLock, &oldIrql);
    Temp = RtlLookupElementGenericTable(&g_LocalPortTable, &Element);
    if (Temp) {
        RtlDeleteElementGenericTable(&g_LocalPortTable, Temp);//Temp��FreeRoutine���ͷ��ˡ�
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
        RtlDeleteElementGenericTable(&g_LocalPortTable, Element);//Element��FreeRoutine���ͷ��ˡ�
    }
    KeReleaseSpinLock(&g_LocalPortSpinLock, oldIrql);

    ExDeleteNPagedLookasideList(&g_LocalPortLookaside);
}


//////////////////////////////////////////////////////////////////////////////////////////////////
