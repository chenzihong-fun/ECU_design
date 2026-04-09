#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "ui.h"
extern void test_all();

//启动初始化任务
#define START_TASK_PRIO      			10
#define START_STK_SIZE  				64 /*单位是字，一个字是4个字节，半字是2个字节*/
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
static void start_task(void *pvParameters);


//测试任务
#define TEST_TASK_PRIO      			8
#define TEST_STK_SIZE  					256 /*单位是字，一个字是4个字节，半字是2个字节*/
//任务句柄
TaskHandle_t TestTask_Handler;
//任务函数
static void test_task(void *pvParameters);



int main(void)
{
    board_lowlevel_init();
    device_init();
    xTaskCreate(start_task,"start_task",START_STK_SIZE,NULL,START_TASK_PRIO,&StartTask_Handler);/*创建起始任务*/
    vTaskStartScheduler(); 	/*开启多任务调度*/
	while(1)
		{

		};
}

static void start_task(void *pvParameters)
{
	xTaskCreate(test_task,"test_task",TEST_STK_SIZE,NULL,TEST_TASK_PRIO,&TestTask_Handler);/*创建测试任务*/
	ui_task_init();   //ui任务优先级为9
	vTaskDelete(NULL);
}

static void test_task(void * Parameters)
{
	while (1)
	{
		test_all();
        vTaskDelay(pdMS_TO_TICKS(10));
	}
}





