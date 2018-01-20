#ifndef __PARAM_H
#define __PARAM_H

#define HISTORY_START_SECTOR 64//ǰ��00-63��������������
#define HISTORY_START_ADDR 0
#define HISTORY_END_SECTOR 512//�����������ַ�������ڣ���ô�ͷ��ʼ
#define HISTORY_END_ADDR 0

#define MAX_CARD_NUM 500
#define MAX_FINGER_NUM 500
#define MAX_MULTIPLE_CARD_NUM 10
#define MAX_THREAT_CARD_NUM 3

#define MAX_OPEN_CFG_NUM 8
//����
typedef struct
{
    uint8_t reader_switcher;//��ͷ��̵�����Ӧ��ϵ����4λ-�̵���a����4λ-�̵���b��0��ʾ�޶�Ӧ��ϵ��1��
    uint8_t button_switcher;//������̵�����Ӧ��ϵ����4λ-�̵���a����4λ-�̵���b
}SingleRelation_T; 

//������
typedef struct
{
    SingleRelation_T relationA;//��ϵA����ͷA/����A��
    SingleRelation_T relationB;//��ϵB����ͷB/����B��
}Relation_T;

//��������
typedef struct 
{   
    uint16_t local_port;// ��ʼ��һ�����ض˿�
    uint16_t server_port;// ����Զ�̷������˿�
    uint8_t local_ip[4];
    uint8_t server_ip[4];// ����Զ�̷�����IP��ַ,����[192][168][1][100]
    uint8_t mcuID[4];//оƬ��ΨһID //����mac��ַ
}NetCfg_T;

//ϵͳ����
typedef struct
{
    uint8_t openTime;//����ʱ��
    uint8_t waitTime;//�ȴ�ʱ��  
    //���ؿ����޶�����4λ��ʾ��������2-10������4λ��ʾ���û�����
    uint8_t multipleOpenCfg[MAX_OPEN_CFG_NUM];//���ֿ��Ź����Ƿ�����0-������1-�׿���2-���ؿ���3-��������4-�������룬5-в�ȿ���6-в���룬7-ָ��ģ��(ָ��id�Ͱ�������id)      
}SystemCfg_T;

//��ʷ��¼����һ��д���ַ
typedef struct 
{
    //��һ���ֽڴ��8λ���ڶ����ֽڴ��8λ
    uint8_t nextStartSector[2];//��һ��Ҫд����ʷ��¼������
    uint8_t nextStartAddr[2];//��һ��Ҫд����ʷ��¼�ĵ�ַ(��������)
}NextStartAddr_T;

//��¼���ŵĽṹ��
typedef struct
{
    //9��
    uint8_t firstCardID[3];//�׿�  
    uint8_t superCardID[3];//������
    uint8_t superPasswordID[3];//��������
    uint8_t threatCardID[MAX_THREAT_CARD_NUM*3];//в�ȿ�,3��
    uint8_t threatPasswordID[3];//в����
    uint8_t keyBoardID[3];//��������ID
    uint8_t multipleCardID[MAX_MULTIPLE_CARD_NUM*3];//���ؿ�,10�� 
    uint8_t fingerID[MAX_FINGER_NUM*3];//ָ��ID
    uint8_t generalCardID[MAX_CARD_NUM*3];//��ͨ��
}MultipleCardID_T;

typedef struct
{
    void (*updateRelation)(Relation_T *relation, enum ReaderOrButton_Enum type);
    void (*updateNetCfg)(NetCfg_T *netCfg, enum NetCfg_Enum type);
    void (*updateSystemCfg)(SystemCfg_T *systemCfg, enum OpenCfg_Enum type);
    void (*updateNextStartAddr)(NextStartAddr_T *nextStartAddr);
    void (*updateMultipleCardID)(uint8_t *data, uint16_t len, enum ID_Enum type);
    Relation_T relation;
    NetCfg_T netCfg;
    SystemCfg_T systemCfg;
    NextStartAddr_T nextStartAddr;
    MultipleCardID_T multipleCardID;
}Param_T;

/* ���ⲿ���õĺ������� */
 
extern Param_T g_tParam;
void paramInit(void);//��IIC�ж�ȡ����
void resetParam(void);//���ò���Ϊ0xFF
uint8_t searchID(uint8_t *id);//���id�Ƿ�������ܷ���
#endif
