/*
**
**                           Main.c
**
**
**********************************************************************/
/*
   Last committed:     $Revision: 00 $
   Last changed by:    $Author:logic$
   Last changed date:  $Date:20180802  $
   ID:                 $Id:  $

**********************************************************************/
#include "stm32f10x_conf.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "pwm_output.h"
#include "procto.h"

#define LED_OFF GPIO_SetBits(GPIOC,GPIO_Pin_13)
#define LED_ON GPIO_ResetBits(GPIOC,GPIO_Pin_13)
#define KEY0  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
void led_gpio_init();
void key_gpio_init();
u8 keyscan(void);
u8 pwm_messeage[12]={0};
volatile u16 ARR[4]={0};

#define START_TASK_PRIO		1
#define START_STK_SIZE 		60
TaskHandle_t StartTask_Handler;
void start_task(void *pvParameters);


#define KEY_TASK_PRIO		2
#define KEY_STK_SIZE 		60
TaskHandle_t KeyTask_Handler;
void key_task(void *pvParameters);

#define LED0_TASK_PRIO		3
#define LED0_STK_SIZE 		60
TaskHandle_t LED0Task_Handler;
void led0_task(void *pvParameters);


//�������ȼ�
#define TASK1_TASK_PRIO		5
//�����ջ��С
#define TASK1_STK_SIZE 		180
//������
TaskHandle_t Task1Task_Handler;
//������
void task1_task(void *pvParameters);
QueueHandle_t Message_Queue;	//��Ϣ���о��
u8 keycount=0;

void uart_init();
void uart_putchar(u8 ch)
{

    USART_ClearFlag(USART1,USART_FLAG_TC);
    USART_SendData(USART1,ch);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET)
        ;
}
void uart_sendbuffer(u8 *ch,u8 len)
{
 for (int i=0;i<len;i++)
 {
     USART_ClearFlag(USART1,USART_FLAG_TC);
     USART_SendData(USART1,ch[i]);
     while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
 }
}
int main(void)
{

 //  __set_PRIMASK(1);�ر������ж�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4
	rcc_init();
	led_gpio_init();
	key_gpio_init();
	uart_init();
	pwm_gpio_init();
    //��һ������pwm
    TIMx_PWM_Config(TIM1,4999,2500);  //1600hz a11
    TIMx_PWM_Config(TIM2,5713,2857);  //1400hz a3
    TIMx_PWM_Config(TIM3,6665,3333);  //1200hz b1
    TIMx_PWM_Config(TIM4,7999,2000);  //1000hz b9

	//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������
    vTaskStartScheduler();          //�����������
    while(1);
}

//��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
     Message_Queue=xQueueCreate(42,sizeof(u8)); //������ϢMessage_Queue,��������Ǵ��ڽ��ջ���������
        //����TASK1����
    xTaskCreate((TaskFunction_t )task1_task,
                (const char*    )"task1_task",
                (uint16_t       )TASK1_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK1_TASK_PRIO,
                (TaskHandle_t*  )&Task1Task_Handler);
     //����KEY����
	xTaskCreate((TaskFunction_t )key_task,
                (const char*    )"key_task",
                (uint16_t       )KEY_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )KEY_TASK_PRIO,
                (TaskHandle_t*  )&KeyTask_Handler);
    //����LED0����
    xTaskCreate((TaskFunction_t )led0_task,
                (const char*    )"led0_task",
                (uint16_t       )LED0_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED0_TASK_PRIO,
                (TaskHandle_t*  )&LED0Task_Handler);
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}
//key������
void key_task(void *pvParameters)
{
    uart_putchar(0x13);
	while(1)
	{

        keycount+=keyscan();
		if(keycount==1)
        {
            vTaskSuspend(LED0Task_Handler);//��������
        }
		if(keycount==3)
        {
            vTaskResume(LED0Task_Handler);	//�ָ�����1
            keycount=0;
        }
		vTaskDelay(10);			//��ʱ10ms
	}
}
//LED0������
void led0_task(void *pvParameters)
{
    uart_putchar(0x15);
    while(1)
    {

        LED_OFF;
        vTaskDelay(500);
        LED_ON;
        vTaskDelay(500);
    }
}

//task1���������������ڽ���Э�������pwm
void task1_task(void *pvParameters)
{
    BaseType_t xResult;
    u8 ucmesseage_counter=0;
    u16 crc_num=0;
    u8 crc_hi=0;
    u8 crc_low=0;
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS(300); /* �������ȴ�ʱ��Ϊ 3s�����ڽ�����Ϣ */

    while(1)
    {
         // taskENTER_CRITICAL();
        xResult = xQueueReceive(Message_Queue,/* ��Ϣ���о�� */
                                (void *)&pwm_messeage[ucmesseage_counter], /* �洢���յ������ݵ����� pwm_message �� */
                                (TickType_t)xMaxBlockTime);/* ��������ʱ�� */
           if(xResult==pdPASS)
           {

                  ucmesseage_counter++;
                  //process the message;
                  if(ucmesseage_counter>11)
                  {
                      taskENTER_CRITICAL();
                       ucmesseage_counter=0;
                      crc_num=crc16(pwm_messeage,10);
                      crc_hi=crc_num>>8;
                      crc_low=crc_num;
                      if(crc_low==pwm_messeage[11]&&crc_hi==pwm_messeage[10])
                      {


                        uart_sendbuffer(pwm_messeage,sizeof(pwm_messeage));
                        ARR[0]=(pwm_messeage[2]<<8)|(pwm_messeage[3]);
                        ARR[1]=(pwm_messeage[4]<<8)|(pwm_messeage[5]);
                        ARR[2]=(pwm_messeage[6]<<8)|(pwm_messeage[7]);
                        ARR[3]=(pwm_messeage[8]<<8)|(pwm_messeage[9]);
                        TIMx_PWM_Config(TIM1,ARR[0]-1,ARR[0]>>1);
                        TIMx_PWM_Config(TIM2,ARR[1]-1,ARR[1]>>1);
                        TIMx_PWM_Config(TIM3,ARR[2]-1,ARR[2]>>1);
                        TIMx_PWM_Config(TIM4,ARR[3]-1,ARR[3]>>1);
                      }
                      taskEXIT_CRITICAL();

                  }
           }

        //  taskEXIT_CRITICAL();
    }


}


void led_gpio_init()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_SetBits(GPIOC, GPIO_Pin_13 );
}

void key_gpio_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA, GPIO_Pin_0 );
}
u8 keyscan(void)
{
    if(KEY0==1)
    {
        vTaskDelay(10);
        if(KEY0==1)
        {
            while(KEY0==1);
            return 1;//keyon
        }
        else
            return 0;//keyoff
    }
    else
        return 0;
}
void uart_init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate=9600; //������
    USART_InitStructure.USART_WordLength=USART_WordLength_8b; //����λ��
    USART_InitStructure.USART_StopBits=USART_StopBits_1; //ֹͣλ
    USART_InitStructure.USART_Parity=USART_Parity_No; //����żУ��
    USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    //��Ӳ��������
    USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
    //�շ���ʹ��
    USART_Init(USART1,&USART_InitStructure);

    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
    USART_Cmd(USART1,ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}
void USART1_IRQHandler(void)
{
    u8 dat;
    BaseType_t xHigherPriorityTaskWoken;
   if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)
   {
      USART_ClearITPendingBit(USART1, USART_IT_RXNE);
      dat=USART_ReceiveData(USART1);
      xQueueSendFromISR(Message_Queue,(void *)&dat,&xHigherPriorityTaskWoken);

   }
}
