#include "MajorFunction.h"
#include "Cdo.h"


//////////////////////////////////////////////////////////////////////////////////////////////////


_Function_class_(DRIVER_DISPATCH)
_IRQL_requires_max_(DISPATCH_LEVEL)
_IRQL_requires_same_
NTSTATUS GlobalMajorFunction(_In_ struct _DEVICE_OBJECT * DeviceObject, _Inout_ struct _IRP * Irp)
{
    PDEVICE_EXTENSION DevExt = (PDEVICE_EXTENSION)DeviceObject->DeviceExtension;
    NTSTATUS Status = STATUS_SUCCESS;

    PAGED_CODE();

    switch (DevExt->DeviceTag) {
    case MY_NSI_DEVICE_TAG:
        Status = NsiMajorFunction(DeviceObject, Irp);
        break;
    case MY_CDO_DEVICE_TAG:
        return ControlDeviceObjectMajorFunction(DeviceObject, Irp);//ֱ�ӷ��أ����������档
        break;
    default:
        ASSERTMSG((PSTR)"�Ƿ����豸����", FALSE);
        break;
    }
    
    return Status;
}
