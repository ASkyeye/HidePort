#pragma once

#include "Pch.h"
#include "Attach.h"


//////////////////////////////////////////////////////////////////////////////////////////////////


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


#pragma pack(1)
typedef struct _NsiParameters70
{
    GUID Guid;
    PNPI_MODULEID ModuleId;
    SIZE_T field_18;
    SIZE_T Flag;

    PVOID p1;
    SIZE_T size1;//�����ڴ�Ĵ�С��Ӧ���������Ԫ�صĴ�С��

    PVOID p2;
    SIZE_T size2;//�����ڴ�Ĵ�С��Ӧ���������Ԫ�صĴ�С��

    PVOID p3;
    SIZE_T size3;//�����ڴ�Ĵ�С��Ӧ���������Ԫ�صĴ�С��

    PVOID p4;
    ULONG size4;//�����ڴ�Ĵ�С��Ӧ���������Ԫ�صĴ�С��

    ULONG field_64;
    ULONG Counter;
    ULONG field_6C;
}NsiParameters70, * PNsiParameters70;
#pragma pack()

#if defined(_WIN64)
static_assert(sizeof(NsiParameters70) == 0x70);//���Թ��� "��Ҫ��̬����" ��Ҫ��������־ "/std:c++17"
#else 
static_assert(sizeof(NsiParameters70) == 0x38);//32λ���д�������ȥ��GUID��С���á�
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////


NTSTATUS NsiMajorFunction(_In_ struct _DEVICE_OBJECT * DeviceObject, _Inout_ PIRP Irp);


//////////////////////////////////////////////////////////////////////////////////////////////////


class Nsi
{

};

