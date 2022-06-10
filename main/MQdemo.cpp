#include <MQCore/MQHeader.h>
#include <GlobalVar.h>
#include "Rollname.h"
using namespace MQ;
//MQ::Api		MQ �ṩ��api�����ӿ�
//MQ::Enum		MQ �¼�����
//MQ::Event		MQ ע���¼��ص�����
//MQ::Logging	MQ ��־
//MQ::Type		MQ ������ݷ�װ
//MQ::�ı�����	MQ �ı�����
vector<string> QQnum;
vector<string> IDname;
void processEvent(const Event::NormalEvent& e)
{
	MQEventCheck(e.eventType, Enum::MQEventEnum::��Ϣ����_����)
	{
		if (e.botQQ == e.activeQQ)return;
		//��־��� fun
		MQ::Api::FrameAPI::OutPut("fun");
		//��־��� [info]fun
		Logging::info("fun");
		//����
		MQ::Api::MessageAPI::SendMsg(e.botQQ, Enum::msgType::����, "", e.activeQQ, e.msg);
		//MQ::MessageAPI::SendMsg(e.botQQ, msgType::Ⱥ, e.sourceId, "", e.msg);
	}
}
/*
* ������
* ���ڸú�(MQ_REGISTER_EVENT)����������ע��ص�����
* �ھ�̬��Ա��ʼ�������ϵͳ�����¼�֮�󣬲���û������¼�����֮ǰ��ִ��,��������SDK��ע���¼��ص�
*/
MQ_REGISTER_EVENT
{
	Api::FrameAPI::OutPut("fun");
	InitQQ(QQnum);
	InitIDcard(IDname);
	Save_data(QQnum, IDname);
	if (EventContInit)return;
	//ע���¼��ص�����1,���ȼ�20000
	MQ::Event::reg_Event(processEvent, 20000);
	//ע���¼��ص�����2,���ȼ�15000
	MQ::Event::reg_Event([](const Event::NormalEvent& e) {
		MQEventCheck(e.eventType, Enum::MQEventEnum::��Ϣ����_����)
		{
			if (e.botQQ == e.activeQQ)return;
			//��־��� lambda
			Api::FrameAPI::OutPut("lambda");
			Api::FrameAPI::OutPut(e.activeQQ );
			Api::FrameAPI::OutPut(e.passiveQQ);
			Api::FrameAPI::OutPut(e.rawMsg);
			//����QQ
			Api::MessageAPI::SendMsg(e.botQQ, Enum::msgType::����, "", e.activeQQ, MQ::�ı�����::����QQ() + ":" + e.msg);
			//��ֹ�������¼��ص�����ִ��,�����ȼ��ص�������Ȩ�ܾ�
			e.eventBlock();
		}
		MQEventCheck(e.eventType, Enum::MQEventEnum::��Ϣ����_Ⱥ)
		{
			if (e.botQQ == e.activeQQ)return;

			if (e.msg=="#ROLL")
			{
				Api::FrameAPI::OutPut("success");
				InitQQ(QQnum);
				InitIDcard(IDname);
				Save_data(QQnum, IDname);
			}
			//Api::FrameAPI::OutPut(e.botQQ);
			//Api::FrameAPI::OutPut(e.sourceId);
			Api::FrameAPI::OutPut(e.activeQQ);
			Api::FrameAPI::OutPut(e.passiveQQ);
			Api::FrameAPI::OutPut(e.msg);
			//Api::FrameAPI::OutPut(e.msgNum);
			//Api::FrameAPI::OutPut(e.msgId);
			//Api::MessageAPI::SendMsg(e.botQQ, Enum::msgType::Ⱥ, "", e.activeQQ, MQ::�ı�����::����QQ() + ":" + e.msg);
			e.eventBlock();
		}
	}, 15000);

	//ע���¼��ص�����3,���ȼ�10000
	Event::reg_Event([](const Event::NormalEvent& e) {
		MQEventCheck(e.eventType, Enum::MQEventEnum::��Ϣ����_���������)
		{
			static bool containInit = false;
			if (containInit)
			{
				return;
			}
			Api::FrameAPI::OutPut("fun");
			InitQQ(QQnum);
			InitIDcard(IDname);
			Save_data(QQnum, IDname);
		}
		}, 10000);
	//ע�����ô���,���ȼ�ΪĬ��30000
	Event::reg_Setting([](const auto& e) {
		MessageBoxA(nullptr, "Text", "Caption", 0);
		});
	EventContInit = true;
}
