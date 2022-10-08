#include "Nsi.h"


//////////////////////////////////////////////////////////////////////////////////////////////////


NTSTATUS MyNsippEnumerateObjectsAllParameters(PVOID InputBuffer,
                                              SIZE_T Length,
                                              KPROCESSOR_MODE RequestorMode,
                                              PULONG_PTR Information
)
/*
���ܣ����ض˿ڡ�

������ժ����Ҫָ��ժ������Ϣ��

�ص㣺InputBuffer�Ľṹ���͡�

�۸�ʱ����IoCallDriver֮��
*/
{
    NTSTATUS Status = STATUS_SUCCESS;

    UNREFERENCED_PARAMETER(InputBuffer);
    UNREFERENCED_PARAMETER(Length);
    UNREFERENCED_PARAMETER(RequestorMode);
    UNREFERENCED_PARAMETER(Information);

    return Status;
}


NTSTATUS NsiDeviceControl(_In_ PIO_STACK_LOCATION IrpStack, _Inout_ PIRP Irp)
/*
�����ο���nsiproxy!NsippDispatch.

���и������У�NsippDispatchDeviceControl��
*/
{
    NTSTATUS Status = STATUS_SUCCESS;
    PIO_STACK_LOCATION  irpSp;// Pointer to current stack location
    ULONG               InputBufferLength; // Input buffer length
    ULONG               outBufLength; // Output buffer length
    PCHAR               inBuf, outBuf; // pointer to Input and output buffer

    UNREFERENCED_PARAMETER(IrpStack);

    irpSp = IoGetCurrentIrpStackLocation(Irp);
    InputBufferLength = irpSp->Parameters.DeviceIoControl.InputBufferLength;
    outBufLength = irpSp->Parameters.DeviceIoControl.OutputBufferLength;
    PVOID Type3InputBuffer = irpSp->Parameters.DeviceIoControl.Type3InputBuffer;
    ULONG IoControlCode = irpSp->Parameters.DeviceIoControl.IoControlCode;
    inBuf = (PCHAR)Irp->AssociatedIrp.SystemBuffer;
    outBuf = (PCHAR)Irp->AssociatedIrp.SystemBuffer;
    KPROCESSOR_MODE RequestorMode = Irp->RequestorMode;

    switch (IoControlCode) {
    case 0x12001Bu://NsippEnumerateObjectsAllParameters
        MyNsippEnumerateObjectsAllParameters(Type3InputBuffer,
                                             (SIZE_T)InputBufferLength,
                                             RequestorMode,
                                             &Irp->IoStatus.Information);
        break;
    case 0x120007u://NsippGetParameter

        break;
    case 0x12000Fu://NsippGetAllParameters

        break;
    case 0x12000Bu://NsippSetParameter

        break;
    case 0x120013u://NsippSetAllParameters

        break;
    case 0x12001Fu://NsippIsCallerNsiService + NsippRegisterChangeNotification

        break;
    case 0x120023u://NsippIsCallerNsiService + NsippDeregisterChangeNotification

        break;
    case 0x12003Fu://NsippRequestChangeNotification

        break;
    case 0x120040u://NsippCancelChangeNotification

        break;
    case 0x120047u://NsippEnumerateObjectsAllPersistentParametersWithMask

        break;
    case 0x12004Bu://NsippGetAllPersistentParametersWithMask

        break;
    case 0x12004Fu://NsippSetAllPersistentParametersWithMask

        break;
    default:
        PrintEx(DPFLTR_DEFAULT_ID, DPFLTR_WARNING_LEVEL,
                "Warning: MajorFunction: %d, IrpName: %s, IoControlCode:%d", 
                IrpStack->MajorFunction, FltGetIrpName(IrpStack->MajorFunction), IoControlCode);
        break;
    }

    return Status;
}


NTSTATUS NsiMajorFunction(_In_ PIO_STACK_LOCATION IrpStack, _Inout_ PIRP Irp)
/*
�����ο���nsiproxy!NsippDispatch.
*/
{
    NTSTATUS Status = STATUS_SUCCESS;

    UNREFERENCED_PARAMETER(Irp);

    switch (IrpStack->MajorFunction) {
    case IRP_MJ_CREATE:

        break;
    case IRP_MJ_DEVICE_CONTROL:
        Status = NsiDeviceControl(IrpStack, Irp);
        break;
    case IRP_MJ_INTERNAL_DEVICE_CONTROL:
        Status = NsiDeviceControl(IrpStack, Irp);
        break;
    case IRP_MJ_CLEANUP:

        break;
    case IRP_MJ_CLOSE:

        break;
    default:
        PrintEx(DPFLTR_DEFAULT_ID, DPFLTR_WARNING_LEVEL, "Warning: MajorFunction: %d, IrpName: %s", 
                IrpStack->MajorFunction, FltGetIrpName(IrpStack->MajorFunction));
        break;
    }

    return Status;
}
