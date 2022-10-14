#pragma once

#include "Pch.h"

#define MY_CDO_DEVICE_TAG   'CTO' //�Լ��Ŀ����豸����չ��ı�־��
#define MY_NSI_DEVICE_TAG   'NSI' //�Լ��Ĺ���NSI���豸����չ���־��


typedef struct _DEVICE_EXTENSION
{
    ULONG DeviceTag; //������д��ֵ��NPFS,MSFS,CD0
    PDEVICE_OBJECT AttachedDevice;
    IO_REMOVE_LOCK RemoveLock;//��������֤�����õİ�ȫж�ػ��ơ�
} DEVICE_EXTENSION, * PDEVICE_EXTENSION;


//////////////////////////////////////////////////////////////////////////////////////////////////


NTSTATUS Detach(_In_ PDRIVER_OBJECT DriverObject);

NTSTATUS AttachDevice(_In_ PDRIVER_OBJECT DriverObject,
                      _In_ PCWSTR DeviceNameString,
                      _In_ PCWSTR FilterDeviceName,
                      _In_ ULONG Flag
);


//////////////////////////////////////////////////////////////////////////////////////////////////


class Attach
{

};

