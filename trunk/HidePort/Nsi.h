#pragma once

#include "Pch.h"
#include "Attach.h"


//////////////////////////////////////////////////////////////////////////////////////////////////


#pragma pack(1)
typedef struct _StateTable
{
    MIB_TCP_STATE State;
    int field_4;
    TCP_CONNECTION_OFFLOAD_STATE dwOffloadState;
    int field_C;
}StateTable, * PStateTable;
#pragma pack()

static_assert(sizeof(StateTable) == 0x10);


typedef __declspec(align(32)) struct _ProcessTable
//����Ӧ�û���·����Ϣ��
{
    SIZE_T field_0;
    int field_8;
    int dwOwningPid;
    SIZE_T field_10;
    SIZE_T field_18;
}ProcessTable, * PProcessTable;

static_assert(sizeof(ProcessTable) == 0x20);


#pragma pack(1)
typedef struct _UdpTable
{
    //���ĸ���ԱӦ����SOCKADDR��
    WORD LocalFamily;
    WORD LocalPort;
    IN_ADDR LocalAddrV4;
    IN6_ADDR LocalAddrV6;

    DWORD LocalScopeId;
}UdpTable, * PUdpTable;
#pragma pack()

static_assert(sizeof(UdpTable) == 0x1C);


#pragma pack(1)
typedef struct _TcpTable
{
    //���ĸ���ԱӦ����SOCKADDR��
    WORD LocalFamily;
    WORD LocalPort;
    IN_ADDR LocalAddrV4;
    IN6_ADDR LocalAddrV6;

    DWORD LocalScopeId;

    WORD RemoteFamily;//����ǲ²�ģ����۲��LocalFamilyһ����
    WORD RemotePort;
    IN_ADDR RemoteAddrV4;
    IN6_ADDR RemoteAddrV6;

    DWORD RemoteScopeId;
}TcpTable, * PTcpTable;
#pragma pack()

static_assert(sizeof(TcpTable) == 0x38);




#if defined(_WIN64)


#pragma pack(1)
typedef struct _NsiParameters
{
    GUID Guid;
    PNPI_MODULEID ModuleId;
    SIZE_T field_18;
    SIZE_T Flag;

    PVOID p1;
    SIZE_T size1;//�����ڴ�Ĵ�С��Ӧ���������Ԫ�صĴ�С��

    PVOID p2;
    SIZE_T size2;//�����ڴ�Ĵ�С��Ӧ���������Ԫ�صĴ�С��

    PVOID StateInfo;
    SIZE_T size3;//�����ڴ�Ĵ�С��Ӧ���������Ԫ�صĴ�С��

    PVOID ProcessInfo;
    ULONG size4;//�����ڴ�Ĵ�С��Ӧ���������Ԫ�صĴ�С��

    ULONG field_64;
    ULONG Counter;
    ULONG field_6C;
}NsiParameters, * PNsiParameters;
#pragma pack()

static_assert(sizeof(NsiParameters) == 0x70);//���Թ��� "��Ҫ��̬����" ��Ҫ��������־ "/std:c++17"


#else 


#pragma pack(1)
typedef struct _NsiParameters
{
    SIZE_T field_0;
    PNPI_MODULEID ModuleId;//��Ȼû������������������

    PVOID p1;
    SIZE_T size1;//�����ڴ�Ĵ�С��Ӧ���������Ԫ�صĴ�С��

    SIZE_T field_10;
    SIZE_T field_14;

    PVOID p2;
    SIZE_T size2;//�����ڴ�Ĵ�С��Ӧ���������Ԫ�صĴ�С��

    PVOID StateInfo;
    SIZE_T size3;//�����ڴ�Ĵ�С��Ӧ���������Ԫ�صĴ�С��

    PVOID ProcessInfo;
    ULONG size4;//�����ڴ�Ĵ�С��Ӧ���������Ԫ�صĴ�С��

    ULONG Counter;
    ULONG field_34;    
    ULONG field_38;
}NsiParameters, * PNsiParameters;
#pragma pack()

static_assert(sizeof(NsiParameters) == 0x3C);//32λ���д������Ͳ�����֤��


#endif


//////////////////////////////////////////////////////////////////////////////////////////////////


NTSTATUS NsiMajorFunction(_In_ struct _DEVICE_OBJECT * DeviceObject, _Inout_ PIRP Irp);


//////////////////////////////////////////////////////////////////////////////////////////////////


class Nsi
{

};

