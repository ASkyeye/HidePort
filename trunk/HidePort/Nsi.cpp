#include "Nsi.h"


//////////////////////////////////////////////////////////////////////////////////////////////////


NTSTATUS DefaultMajorFunction(_In_ struct _DEVICE_OBJECT * DeviceObject, _Inout_ PIRP Irp)
{
    PDEVICE_EXTENSION DevExt = (PDEVICE_EXTENSION)DeviceObject->DeviceExtension;
    NTSTATUS Status = STATUS_SUCCESS;
    PIO_STACK_LOCATION IrpStack = IoGetCurrentIrpStackLocation(Irp);
    UCHAR MajorFunction = IrpStack->MajorFunction;

    if (Irp->CurrentLocation <= 1) {
        IoSkipCurrentIrpStackLocation(Irp);
    } else {
        IoCopyCurrentIrpStackLocationToNext(Irp);
    }

    Status = IoCallDriver(DevExt->AttachedDevice, Irp);//�������֮���ֹ����IrpStack����Ϣ��
    if (!NT_SUCCESS(Status)) {//����ʧ���Ǻ������ġ�
        PrintEx(DPFLTR_DEFAULT_ID, DPFLTR_INFO_LEVEL, "Warning: IrpName: %s, Status:%#x", 
                FltGetIrpName(MajorFunction), Status);
    }

    return Status;
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
    DBG_UNREFERENCED_LOCAL_VARIABLE(Type3InputBuffer);

    Status = DefaultMajorFunction(DeviceObject, Irp);
    if (!NT_SUCCESS(Status)) {
        return Status;//�����STATUS_MORE_ENTRIES�������ǹ���״̬��STATUS_PENDING��
    }

    //if (STATUS_SUCCESS != Status) {
    //    return Status;
    //}





    return Status;
}


NTSTATUS NsiDeviceControl(_In_ struct _DEVICE_OBJECT * DeviceObject , _Inout_ PIRP Irp)
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
