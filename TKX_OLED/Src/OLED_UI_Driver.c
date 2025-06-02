#include "OLED_UI_Driver.h"

// /*
// 【文件说明】：[硬件抽象层]
// 此文件包含按键与编码器的驱动程序，如果需要移植此项目，请根据实际情况修改相关代码。
// 当你确保oled屏幕能够正常点亮，并且能够正确地运行基础功能时（如显示字符串等），就可以开始移植
// 有关按键与编码器等的驱动程序了。
// */


// /**
//  * @brief 定时器中断服务函数的初始化函数，用于产生20ms的定时器中断
//  * @param 无
//  * @return 无
//  */
// void Timer_Init(void)
// {
// 	/*开启时钟*/
// 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);			//开启TIM4的时钟
	
// 	/*配置时钟源*/
// 	TIM_InternalClockConfig(TIM4);		//选择TIM4为内部时钟，若不调用此函数，TIM默认也为内部时钟
	
// 	/*时基单元初始化*/
// 	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//定义结构体变量
// 	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
// 	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//计数器模式，选择向上计数
// 	TIM_TimeBaseInitStructure.TIM_Period = 200 - 1;				//计数周期，即ARR的值
// 	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;				//预分频器，即PSC的值
// 	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;			//重复计数器，高级定时器才会用到
// 	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);				//将结构体变量交给TIM_TimeBaseInit，配置TIM4的时基单元	
	
// 	/*中断输出配置*/
// 	TIM_ClearFlag(TIM4, TIM_FLAG_Update);						//清除定时器更新标志位
// 																//TIM_TimeBaseInit函数末尾，手动产生了更新事件
// 																//若不清除此标志位，则开启中断后，会立刻进入一次中断
// 																//如果不介意此问题，则不清除此标志位也可
	
// 	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);					//开启TIM4的更新中断
	
// 	/*NVIC中断分组*/
// 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				//配置NVIC为分组2
// 																//即抢占优先级范围：0~3，响应优先级范围：0~3
// 																//此分组配置在整个工程中仅需调用一次
// 																//若有多个中断，可以把此代码放在main函数内，while循环之前
// 																//若调用多次配置分组的代码，则后执行的配置会覆盖先执行的配置
	
// 	/*NVIC配置*/
// 	NVIC_InitTypeDef NVIC_InitStructure;						//定义结构体变量
// 	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;				//选择配置NVIC的TIM4线
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//指定NVIC线路使能
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//指定NVIC线路的抢占优先级为2
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//指定NVIC线路的响应优先级为1
// 	NVIC_Init(&NVIC_InitStructure);								//将结构体变量交给NVIC_Init，配置NVIC外设
	
// 	/*TIM使能*/
// 	TIM_Cmd(TIM4, ENABLE);			//使能TIM4，定时器开始运行
// }

/**
 * @brief 按键初始化函数，用于初始化按键GPIO
 * @param 无
 * @note GPIO被初始化为上拉输入模式（虽然在我的开发板上已经加上了上拉电阻，但是以防万一）
 * @return 无
 */
void Key_Init(void)
{

}



/**
 * @brief 编码器初始化函数，将定时器1配置为编码器模式
 * @param 无
 * @return 无
 */
void Encoder_Init(void)
{
	
}


/**
 * @brief 编码器使能函数
 * @param 无
 * @return 无
 */
void Encoder_Enable(void)
{

}

/**
 * @brief 编码器失能函数
 * @param 无
 * @return 无
 */
void Encoder_Disable(void)
{
	
}

/**
 * @brief 获取编码器的增量计数值（四倍频解码）
 * 
 * @details 该函数通过读取定时器TIM1的计数值，对编码器信号进行四倍频解码处理。
 *          使用静态变量累积计数，并通过除法和取模运算去除多余的增量部分，
 *          确保返回精确的增量值。主要用于电机控制、位置检测等应用场景。
 * 
 * @note   函数内部会自动清零定时器计数值，确保下次读取的准确性
 * 
 * @return int16_t 返回解码后的编码器增量值
 */
int16_t Encoder_Get(void)
{

    
    // 返回解码后的增量值
    return 0;
}



// /**
//   * @brief  微秒级延时
//   * @param  xus 延时时长，范围：0~233015
//   * @retval 无
//   */
// void Delay_us(uint32_t xus)
// {
// 	SysTick->LOAD = 72 * xus;				//设置定时器重装值
// 	SysTick->VAL = 0x00;					//清空当前计数值
// 	SysTick->CTRL = 0x00000005;				//设置时钟源为HCLK，启动定时器
// 	while(!(SysTick->CTRL & 0x00010000));	//等待计数到0
// 	SysTick->CTRL = 0x00000004;				//关闭定时器
// }

// /**
//   * @brief  毫秒级延时
//   * @param  xms 延时时长，范围：0~4294967295
//   * @retval 无
//   */
// void Delay_ms(uint32_t xms)
// {
// 	while(xms--)
// 	{
// 		Delay_us(1000);
// 	}
// }
 
// /**
//   * @brief  秒级延时
//   * @param  xs 延时时长，范围：0~4294967295
//   * @retval 无
//   */
// void Delay_s(uint32_t xs)
// {
// 	while(xs--)
// 	{
// 		Delay_ms(1000);
// 	}
// } 


