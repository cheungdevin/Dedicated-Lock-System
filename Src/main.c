/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "string.h"
#include "NUMPAD_ROW.h"
#include "SD_op.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CRC_HandleTypeDef hcrc;

SD_HandleTypeDef hsd;

UART_HandleTypeDef huart1;

SRAM_HandleTypeDef hsram1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_FSMC_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_CRC_Init(void);
static void MX_SDIO_SD_Init(void);
/* USER CODE BEGIN PFP */
//void sdfile_read(void);
//void sdfile_parse(char txt[]);
void timer(void);
void unlock(void);
int withinTenMins(int timeslot);
void checkRoomStatus(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//unsigned char Cmd_ReadId[8] = {0x04,0x08,0xD0,0x20,0x00,0x00,0x00,0x03};
unsigned char Cmd_ReadId[8] = {0x01,0x08,0xA1,0x20,0x00,0x01,0x00,0x76};
unsigned char Cmd_ReadId2[8] = {0x01,0x08,0xA3,0x20,0x01,0x01,0x00,0x75};
/*char* filename = "booker.txt";
uint32_t bytesread;
char sdData[50];
char sdBooker[20] = "";
char sdSID[9] = "";
char sdStartTime[9] = "";
char icSID[9] = "";
char kpSID[9] = "";*/
unsigned char Rec_data[12];
int timer_sec = 55;
int timer_mins = 58;
int timer_hrs = 14; 
char sec_str[10];
char mins_str[10];
char hrs_str[10];		
char currentTime[20] = "";
char roomStatus[10] = "";
struct student users[4];
int currentTimeSlot = 0;
int countForPassword = 0;
char password [9]="";
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	
  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FSMC_Init();
  MX_USART1_UART_Init();
  MX_CRC_Init();
  MX_SDIO_SD_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
	LCD_INIT();
	LCD_DrawLine(8,8,8,288, BLACK);//left line 
	LCD_DrawLine(232,8,232,288, BLACK);//right line
	LCD_DrawLine(8,8,232,8, BLACK);//top line
	LCD_DrawLine(8,288,232,288, BLACK);//bottom line
	LCD_DrawLine(8,36,232,36, BLACK);//below Time:
	LCD_DrawLine(8,64,232,64, BLACK);//below Status:
	LCD_DrawLine(8,232,232,232, BLACK);//second bottom line
	LCD_DrawString(16,240, "Press [K2] to input SID");
	
	int roomStatus = 0;//0=open,1=reserved,2=token

	sdfile_read(users,4);
	for(int i=0; i<4; i++){
		sdfile_parse(users[i].sdData,users[i].sdBooker,users[i].sdSID,users[i].sdCID,users[i].sdStartTime,users[i].sdEndTime);
	}
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	

		LCD_DrawString(16, 44, "Room Status: " );
		char status_str[10] = "";
		if(strcmp(users[currentTimeSlot].sdBooker,"[OPEN]")==0){
			roomStatus=0;
		}
		if(roomStatus!=2 && withinTenMins(currentTimeSlot)==0){roomStatus=1;}
		if(roomStatus!=2 && withinTenMins(currentTimeSlot)==1){roomStatus=0;}
		if(roomStatus==0){
			strcpy(status_str, "OPEN    ");
		}
		if(roomStatus==1){
			strcpy(status_str, "RESERVED");
		}
		if(roomStatus==2){
			strcpy(status_str, "TAKEN   ");
		}
		LCD_DrawString(126, 44, status_str );
		LCD_DrawString(16, 72, "Time Slot: " );
		int j=100;
		for(int i=currentTimeSlot;i<4;i++){
			LCD_DrawString(16, j, users[i].sdStartTime );
			LCD_DrawString(100, j, users[i].sdBooker);
			j+=20;
		}
		
		HAL_UART_Transmit(&huart1,Cmd_ReadId,8,1000);
		HAL_Delay(100);//Must Have This Line
		HAL_UART_Receive(&huart1,Rec_data,12,900);
		timer();
		char icCID[100] = ""; 
		char temp [100] = "";
		
		for(int i=8;i<12;i++){
			sprintf(temp,"%x",Rec_data[i]);
			strcat(icCID,temp);
		}
		if(strcmp(icCID, "0000" ) !=0 ){
			if(roomStatus==0){
				LCD_DrawString(50,300,"[UNLOCK]");
				unlock();
			}
			if(roomStatus!=0){
				if(strcmp(icCID, users[currentTimeSlot].sdCID) ==0 ){  
					LCD_DrawString(50,300,"[UNLOCK]");
					unlock();
					roomStatus=2;
				}else{
					LCD_DrawString(50,300,"[INVALID]");
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 1);
					HAL_Delay(300);
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0);
				}
			}
			LCD_DrawString(50,300,"              ");
		}
		for(int j=0;j<12;j++){
			Rec_data[j] = 0x00; 
		}
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)){
			//LCD_DrawString(16,260,"Emter Here");
			LCD_DrawString(16,272,"--------");
			char manual_sid[9] = "";
			sid_manual(manual_sid);
			if(roomStatus==0){
				LCD_DrawString(50,300,"[UNLOCK]");
				unlock();
			}
			if(roomStatus!=0){
				if(strcmp(manual_sid, users[currentTimeSlot].sdSID) ==0 ){  
					LCD_DrawString(50,300,"[UNLOCK]");
					unlock();
					roomStatus=2;
				}else{
					LCD_DrawString(50,300,"[INVALID]");
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 1);
					HAL_Delay(300);
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0);
				}
			}
			LCD_DrawString(50,300,"              ");
		}
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

/**
  * @brief SDIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDIO_SD_Init(void)
{

  /* USER CODE BEGIN SDIO_Init 0 */

  /* USER CODE END SDIO_Init 0 */

  /* USER CODE BEGIN SDIO_Init 1 */

  /* USER CODE END SDIO_Init 1 */
  hsd.Instance = SDIO;
  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
  hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
  hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
  hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_ENABLE;
  hsd.Init.ClockDiv = 4;
  /* USER CODE BEGIN SDIO_Init 2 */

  /* USER CODE END SDIO_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, row1_Pin|row2_Pin|row3_Pin|row4_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA4 PA5 PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : row1_Pin row2_Pin row3_Pin row4_Pin */
  GPIO_InitStruct.Pin = row1_Pin|row2_Pin|row3_Pin|row4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PD12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : col1_Pin col2_Pin */
  GPIO_InitStruct.Pin = col1_Pin|col2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : col3_Pin col4_Pin */
  GPIO_InitStruct.Pin = col3_Pin|col4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PE1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

}

/* FSMC initialization function */
static void MX_FSMC_Init(void)
{

  /* USER CODE BEGIN FSMC_Init 0 */

  /* USER CODE END FSMC_Init 0 */

  FSMC_NORSRAM_TimingTypeDef Timing = {0};

  /* USER CODE BEGIN FSMC_Init 1 */

  /* USER CODE END FSMC_Init 1 */

  /** Perform the SRAM1 memory initialization sequence
  */
  hsram1.Instance = FSMC_NORSRAM_DEVICE;
  hsram1.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram1.Init */
  hsram1.Init.NSBank = FSMC_NORSRAM_BANK1;
  hsram1.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
  hsram1.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
  hsram1.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
  hsram1.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
  hsram1.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram1.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
  hsram1.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
  hsram1.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
  hsram1.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
  hsram1.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
  hsram1.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram1.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
  /* Timing */
  Timing.AddressSetupTime = 15;
  Timing.AddressHoldTime = 15;
  Timing.DataSetupTime = 255;
  Timing.BusTurnAroundDuration = 15;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FSMC_ACCESS_MODE_A;
  /* ExtTiming */

  if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
  {
    Error_Handler( );
  }

  /** Disconnect NADV
  */

  __HAL_AFIO_FSMCNADV_DISCONNECTED();

  /* USER CODE BEGIN FSMC_Init 2 */

  /* USER CODE END FSMC_Init 2 */
}

/* USER CODE BEGIN 4 */

void timer(void){
	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)){
			timer_mins+=9;
	}
	timer_sec+=1;
	if(timer_sec>59){
		timer_mins+=1;
		timer_sec = 0;
	}
	if(timer_mins>59){
		timer_hrs+=1;
		timer_mins = 0;
	}
	sprintf(sec_str,"%02d",timer_sec);
	sprintf(mins_str,"%02d",timer_mins);
	strcat(mins_str,":"); 
	sprintf(hrs_str,"%02d",timer_hrs);
	strcat(hrs_str,":"); 
	strcpy(currentTime,"");
	strcat(currentTime,hrs_str);
	strcat(currentTime,mins_str); 
	strcat(currentTime,sec_str); 

	LCD_DrawString(16, 16, "Current Time: " );
	LCD_DrawString(126, 16, currentTime );
	int nextslot = currentTimeSlot+1;
	if(withinTenMins(nextslot)==0){
		currentTimeSlot+=1;
		LCD_Clear(16,100,215,100,WHITE);
	}
		
}

void unlock(void){
	//buzzer=================================
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 1);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 1);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0);
	//buzzer=================================
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,1);
	HAL_Delay(1000);
	timer();
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,0);
}

int withinTenMins(int timeslot){
	char string[50] = "";
	char temp[50] = "";
	int sdHrs = 0;
	int sdMins = 0;
	strcpy(string,users[timeslot].sdStartTime);
	char * token = strtok(string, ":");
	sprintf(temp,"%s",token);
	sdHrs = atoi(temp);
	token = strtok(NULL, ":");
	sprintf(temp,"%s",token);
	sdMins = atoi(temp);
	if(sdHrs == timer_hrs){
		if(timer_mins>=sdMins && timer_mins<= sdMins+10 ){
			return 0;
		}
	}
  return 1;
}

void sid_manual(char password[]){
  int count_num = 0;
	while(strlen(password) < 9 ){
		char key_confirm;
		key_confirm = get_key();
		if( (0 < key_confirm) && (key_confirm != '*') && (key_confirm != '#') )
		{
			LCD_Clear(16,260,200,26,WHITE);
			//LCD_DrawString(100,100,"You pressed:");
			//LCD_DrawChar(100,120,key_confirm);
			//strcat(password,&key_confirm);
			password[count_num] = key_confirm;
			//LCD_DrawString(100,140,"SID:");
			LCD_DrawString(16,260,password);
			LCD_DrawString(16,272,"--------");
			count_num++;
		}
		else if (key_confirm == '*'){
			LCD_Clear(16,260,200,26,WHITE);
			count_num--;
			password[count_num] = '\0';
			LCD_DrawString(16,260,password);
			LCD_DrawString(16,272,"--------");
		}
		else if (key_confirm == '#'){
			LCD_Clear(16,260,200,26,WHITE);
			break;
		}
		HAL_Delay(100);
		countForPassword += 1;
		if(countForPassword==10){
			timer();
			countForPassword=0;
		}
	}
	LCD_Clear(16,260,200,26,WHITE);
	return;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
