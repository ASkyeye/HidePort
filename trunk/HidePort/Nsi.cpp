#include "Nsi.h"


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
        //PrintEx(DPFLTR_DEFAULT_ID, DPFLTR_INFO_LEVEL, "Warning: IrpName: %s, Status:%#x",
        //        FltGetIrpName(MajorFunction), Status);
    }

    return Status;
}


void EnumUdpTable(_In_ PNsiParameters70 NsiParam)
{
    UNREFERENCED_PARAMETER(NsiParam);

}


void EnumTcpTable(_In_ PNsiParameters70 NsiParam)
{
    if (NsiParam->p1) {//�����ɶ�ṹ�أ����Է���GetTcp6Table2��
    //ASSERT(NsiParam->size1 == 0x38);//���Կ϶�����ṹ�Ĵ�С��0x38��

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

        PTcpTable Table = (PTcpTable)NsiParam->p1;

        PrintEx(DPFLTR_DEFAULT_ID, DPFLTR_INFO_LEVEL, "dwNumEntries: %d", NsiParam->Counter);

        for (ULONG i = 0; i < NsiParam->Counter; i++, Table++) {
            switch (Table->LocalFamily) {
            case AF_INET:
            {
                PrintEx(DPFLTR_DEFAULT_ID, DPFLTR_INFO_LEVEL, "LocalPort: %d, RemotePort:%d",
                        RtlUshortByteSwap(Table->LocalPort), RtlUshortByteSwap(Table->RemotePort));
                break;
            }
            case AF_INET6:
            {
                PrintEx(DPFLTR_DEFAULT_ID, DPFLTR_INFO_LEVEL, "LocalPort: %d, RemotePort:%d",
                        RtlUshortByteSwap(Table->LocalPort), RtlUshortByteSwap(Table->RemotePort));
                break;
            }
            default:
                PrintEx(DPFLTR_DEFAULT_ID, DPFLTR_INFO_LEVEL, "LocalFamily: %d", Table->LocalFamily);
                break;
            }
        }
    }

    if (NsiParam->p2) {//�����ɶ�ṹ�أ����Է���GetTcp6Table2��
        /*
        ����ṹ��ָ����ΪNULL��
        */

        PrintEx(DPFLTR_DEFAULT_ID, DPFLTR_INFO_LEVEL, "size2: %d", NsiParam->size2);
    }

    if (NsiParam->p3) {//�����ɶ�ṹ�أ����Է���GetTcp6Table2��
        /*
        �����ԣ�����ṹ�Ĵ�С��0x10.

        ����ṹ�������
        State
        dwOffloadState

        */

        PrintEx(DPFLTR_DEFAULT_ID, DPFLTR_INFO_LEVEL, "size3: %d", NsiParam->size3);
    }

    if (NsiParam->p4) {//�����ɶ�ṹ�أ����Է���GetTcp6Table2��
        /*
        �����ԣ�����ṹ�Ĵ�С��0x20.

        ����ṹ�������
        dwOwningPid

        */

        PrintEx(DPFLTR_DEFAULT_ID, DPFLTR_INFO_LEVEL, "size4: %d", NsiParam->size4);
    }
}


NTSTATUS MyNsippEnumerateObjectsAllParameters(_In_ struct _DEVICE_OBJECT * DeviceObject, _Inout_ PIRP Irp)
//NTSTATUS MyNsippEnumerateObjectsAllParameters(PVOID InputBuffer,
//                                              SIZE_T Length,
//                                              KPROCESSOR_MODE RequestorMode,
//                                              PULONG_PTR Information
//)
/*
���ܣ����ض˿ڡ�

������ժ����Ҫָ��ժ������Ϣ��

�ص㣺InputBuffer�Ľṹ���͡�

�۸�ʱ����IoCallDriver֮��

InputBufferLength����С��0x3C��Ҳ��С��0x70�����۲춼��0x70��

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
        PNsiParameters70 NsiParam = (PNsiParameters70)Type3InputBuffer;
        PNPI_MODULEID ModuleId = NsiParam->ModuleId;

        if (NmrIsEqualNpiModuleId(ModuleId, &NPI_MS_TCP_MODULEID)) {
            EnumTcpTable(NsiParam);
        }

        if (NmrIsEqualNpiModuleId(ModuleId, &NPI_MS_UDP_MODULEID)) {
            EnumUdpTable(NsiParam);
        }


    } __except (EXCEPTION_EXECUTE_HANDLER) {
        Print(DPFLTR_DEFAULT_ID, DPFLTR_ERROR_LEVEL, "ExceptionCode:%#X", GetExceptionCode());
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
        //MyNsippEnumerateObjectsAllParameters(Type3InputBuffer,
        //                                     (SIZE_T)InputBufferLength,
        //                                     RequestorMode,
        //                                     &Irp->IoStatus.Information);
        Status = MyNsippEnumerateObjectsAllParameters(DeviceObject, Irp);
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
        PrintEx(DPFLTR_DEFAULT_ID, DPFLTR_WARNING_LEVEL,
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
        PrintEx(DPFLTR_DEFAULT_ID, DPFLTR_WARNING_LEVEL, "Warning: MajorFunction: %d, IrpName: %s",
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
