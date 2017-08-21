#ifndef __HLPUB_DEFINE_H__
#define __HLPUB_DEFINE_H__



/************************************************************/
//		�������岿��										//
/************************************************************/
const int HL_CAL_OPT_KEYWORD = 0x1;	//����ؼ��ʸ��ӱ�ʶ
const int HL_CAL_OPT_FINGER = 0x2;	//������������ָ�Ʊ�ʶ
const int HL_CAL_OPT_POS = 0x4;//������Ա�ʶ
const int HL_CAL_OPT_SEARCH = 0x8;//�����������ķִʽ��


/************************************************************/
//		���Զ��岿��										//
/************************************************************/

#define NATURE_D_A	0x40000000	//	���ݴ� ������
#define NATURE_D_B	0x20000000	//	����� ��������
#define NATURE_D_C	0x10000000	//	���� ������
#define NATURE_D_D	0x08000000	//	���� ������
#define NATURE_D_E	0x04000000	//	̾�� ̾����
#define NATURE_D_F	0x02000000	//	��λ�� ��λ����
#define NATURE_D_I	0x01000000	//	����
#define NATURE_D_L	0x00800000	//	ϰ��
#define NATURE_A_M	0x00400000	//	���� ������
#define NATURE_D_MQ	0x00200000	//	������
#define NATURE_D_N	0x00100000	//	���� ������
#define NATURE_D_O	0x00080000	//	������
#define NATURE_D_P	0x00040000	//	���
#define NATURE_A_Q	0x00020000	//	���� ������
#define NATURE_D_R	0x00010000	//	���� ������
#define NATURE_D_S	0x00008000	//	������
#define NATURE_D_T	0x00004000	//	ʱ���
#define NATURE_D_U	0x00002000	//	���� ������
#define NATURE_D_V	0x00001000	//	���� ������
#define NATURE_D_W	0x00000800	//	������
#define NATURE_D_X	0x00000400	//	��������
#define NATURE_D_Y	0x00000200	//	������ ��������
#define NATURE_D_Z	0x00000100	//	״̬��
#define NATURE_A_NR	0x00000080	//	����
#define NATURE_A_NS	0x00000040	//	����
#define NATURE_A_NT	0x00000020	//	��������
#define NATURE_A_NX	0x00000010	//	�����ַ�
#define NATURE_A_NZ	0x00000008	//	����ר��
#define NATURE_D_H	0x00000004	//	ǰ�ӳɷ�
#define NATURE_D_K	0x00000002	//	��ӳɷ�


struct SHLSegWord
{
	char	*s_szWord; //�ַ���
	DWORD	s_dwPOS;  //���Ա�־
	float   s_fWeight ;//�ؼ���Ȩ�أ�������ǹؼ��ʣ�Ȩ��Ϊ0

	SHLSegWord()
	{
		Reset();
	};
	void Reset()
	{
		s_szWord = NULL ;
		s_dwPOS = 0 ;
		s_fWeight = 0;
	};
};

#endif//__HLPUB_DEFINE_H__

