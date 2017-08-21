#ifndef __HLPUB_DEFINE_H__
#define __HLPUB_DEFINE_H__



/************************************************************/
//		常量定义部分										//
/************************************************************/
const int HL_CAL_OPT_KEYWORD = 0x1;	//计算关键词附加标识
const int HL_CAL_OPT_FINGER = 0x2;	//计算文章语义指纹标识
const int HL_CAL_OPT_POS = 0x4;//计算词性标识
const int HL_CAL_OPT_SEARCH = 0x8;//输出面向检索的分词结果


/************************************************************/
//		词性定义部分										//
/************************************************************/

#define NATURE_D_A	0x40000000	//	形容词 形语素
#define NATURE_D_B	0x20000000	//	区别词 区别语素
#define NATURE_D_C	0x10000000	//	连词 连语素
#define NATURE_D_D	0x08000000	//	副词 副语素
#define NATURE_D_E	0x04000000	//	叹词 叹语素
#define NATURE_D_F	0x02000000	//	方位词 方位语素
#define NATURE_D_I	0x01000000	//	成语
#define NATURE_D_L	0x00800000	//	习语
#define NATURE_A_M	0x00400000	//	数词 数语素
#define NATURE_D_MQ	0x00200000	//	数量词
#define NATURE_D_N	0x00100000	//	名词 名语素
#define NATURE_D_O	0x00080000	//	拟声词
#define NATURE_D_P	0x00040000	//	介词
#define NATURE_A_Q	0x00020000	//	量词 量语素
#define NATURE_D_R	0x00010000	//	代词 代语素
#define NATURE_D_S	0x00008000	//	处所词
#define NATURE_D_T	0x00004000	//	时间词
#define NATURE_D_U	0x00002000	//	助词 助语素
#define NATURE_D_V	0x00001000	//	动词 动语素
#define NATURE_D_W	0x00000800	//	标点符号
#define NATURE_D_X	0x00000400	//	非语素字
#define NATURE_D_Y	0x00000200	//	语气词 语气语素
#define NATURE_D_Z	0x00000100	//	状态词
#define NATURE_A_NR	0x00000080	//	人名
#define NATURE_A_NS	0x00000040	//	地名
#define NATURE_A_NT	0x00000020	//	机构团体
#define NATURE_A_NX	0x00000010	//	外文字符
#define NATURE_A_NZ	0x00000008	//	其他专名
#define NATURE_D_H	0x00000004	//	前接成分
#define NATURE_D_K	0x00000002	//	后接成分


struct SHLSegWord
{
	char	*s_szWord; //字符串
	DWORD	s_dwPOS;  //词性标志
	float   s_fWeight ;//关键词权重，如果不是关键词，权重为0

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

