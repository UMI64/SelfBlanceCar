#ifndef __CAR_H
#define __CAR_H
#include <stm32f10x.h>
struct BufferStruct
{
	void * pBuffer;
	uint16_t Buffer_Size;
	uint16_t Buffer_Long;
	uint16_t Address_Count;
	bool Reading;
};
extern struct BufferStruct Buffer_1;
extern struct BufferStruct Buffer_2;
class PCM5102
{
	public:
	PCM5102();
	void Init(BufferStruct * Buffer1,BufferStruct * Buffer2);
	void Buffer_1_Set(void* setpBuffer,uint16_t Size,uint16_t Long);
	void Buffer_2_Set(void* setpBuffer,uint16_t Size,uint16_t Long);
	void Play();
	void Stop();
	void End();
	BufferStruct *Buffer_1;
	BufferStruct *Buffer_2;	
	private:
	uint16_t DIN=GPIO_Pin_15;//����
	uint16_t BCK=GPIO_Pin_12;//����
	uint16_t SCK=GPIO_Pin_13;//ʱ��
	uint16_t XMT=GPIO_Pin_14;//���� ��(��)
	uint16_t DMP=GPIO_Pin_10;//44.1kHz�����ʵ�ȥ���ؿ��� ��(��)
	uint16_t FLT=GPIO_Pin_11;//�˲�ѡ�� Normal latency (Low) / Low latency (High)
};
#endif