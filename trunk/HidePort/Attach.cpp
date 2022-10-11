#include "Attach.h"


NTSTATUS Detach(_In_ PDRIVER_OBJECT DriverObject)
/*
���ܣ��Ա����������е�AttachDevice����DetachDevice������
*/
{
    NTSTATUS Status = STATUS_SUCCESS;
    PDEVICE_OBJECT PreDeviceObject, CurrentDeviceObject;
    PDEVICE_EXTENSION DevExt;

    PAGED_CODE();

    PreDeviceObject = DriverObject->DeviceObject;
    while (PreDeviceObject != NULL) {
        DevExt = (PDEVICE_EXTENSION)PreDeviceObject->DeviceExtension;
        if (DevExt->AttachedDevice) { //ȷ���������������豸������PFILTER_DEVICE_EXTENSION,û�й��صİѴ�ֵ����Ϊ0.
            IoDetachDevice(DevExt->AttachedDevice);
        }

        CurrentDeviceObject = PreDeviceObject;
        PreDeviceObject = CurrentDeviceObject->NextDevice;

        if (DevExt->AttachedDevice) {
            IoDeleteDevice(CurrentDeviceObject);
        }
    }

    return Status;
}


NTSTATUS AttachDevice(_In_ PDRIVER_OBJECT DriverObject, 
                      _In_ PCWSTR DeviceNameString, 
                      _In_ PCWSTR FilterDeviceName, 
                      _In_ ULONG Flag
)
/*
����:����һ��(����)�豸�����ص�ָ�����豸��.

����˵��:
DeviceNameString �����ص��豸������.
FilterDeviceName �������Լ������豸.
Flag �����Ĺ����豸�ı�־,���豸�������չ�ṹ����.
*/
{
    NTSTATUS Status = STATUS_SUCCESS;
    UNICODE_STRING DeviceName = {0};
    PDEVICE_OBJECT FilterDeviceObject = NULL;
    UNICODE_STRING ObjectName = {0};
    PFILE_OBJECT FileObject = NULL;
    PDEVICE_OBJECT DeviceObject = NULL;
    PDEVICE_EXTENSION DevExt = NULL;
    PDEVICE_OBJECT  AttachedDevice = NULL;

    RtlInitUnicodeString(&ObjectName, DeviceNameString);
    RtlInitUnicodeString(&DeviceName, FilterDeviceName);

    do {
        //Status = SetFastIoDispatch(&ObjectName);
        //if (Status != STATUS_SUCCESS) {
        //    break;
        //}

        Status = IoGetDeviceObjectPointer(&ObjectName, FILE_ALL_ACCESS, &FileObject, &DeviceObject);
        if (Status != STATUS_SUCCESS) {//INF�������������������⼸���豸û������STATUS_OBJECT_NAME_NOT_FOUND����
            PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_ERROR_LEVEL, "Error: Status:%#x", Status);
            break;
        }

        ObDereferenceObject(FileObject);

        Status = IoCreateDevice(DriverObject, 
                                sizeof(DEVICE_EXTENSION), 
                                &DeviceName, 
                                DeviceObject->DeviceType, 
                                0, 
                                FALSE, 
                                &FilterDeviceObject);//���Կ��Ǳ������ֵ��ȫ�ֱ�����
        if (!NT_SUCCESS(Status)) {
            PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_ERROR_LEVEL, "Error: Status:%#x", Status);
            break;
        }

        ClearFlag(FilterDeviceObject->Flags, DO_DEVICE_INITIALIZING); //filterDeviceObject->Flags &= ~0x00000080;

        Status = IoAttachDeviceToDeviceStackSafe(FilterDeviceObject, DeviceObject, &AttachedDevice);//���ظ���ǰ�Ķ����豸��
        if (!NT_SUCCESS(Status)) {
            PrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_ERROR_LEVEL, "Error: Status:%#x", Status);
            IoDeleteDevice(FilterDeviceObject);
            break;
        }

        DevExt = (PDEVICE_EXTENSION)FilterDeviceObject->DeviceExtension;
        DevExt->AttachedDevice = AttachedDevice;//���Կ��Ǳ������ֵ��ȫ�ֱ���������DetachDevice��
        DevExt->DeviceTag = Flag;
    } while (FALSE);

    return Status;
}
