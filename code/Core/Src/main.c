/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "myadc.h"
#include "key.h"
#include "led.h"
#include "pulsein.h"
#include "pwmout.h"

#include "string.h"
#include "stdio.h"
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

/* USER CODE BEGIN PV */
uint8_t page = 0;
uint8_t line = 0;

uint16_t CF;
uint8_t CD;
uint16_t DF;
uint8_t ST;
char run_time[20];
uint32_t real_time = 0;
uint32_t real_time_h = 0;
uint32_t real_time_m = 0;
uint32_t real_time_s = 0;

uint16_t recd_CF;
uint8_t recd_CD;
uint16_t recd_DF;
uint16_t recd_XF;
char recd_run_time[20] = "00H00M00S";
uint32_t recd_real_time = 0;
uint32_t recd_real_time_h = 0;
uint32_t recd_real_time_m = 0;
uint32_t recd_real_time_s = 0;

uint8_t DS = 1;//%
uint8_t DR = 80;//%
uint16_t FS = 100;//Hz
uint16_t FR = 2000;//Hz

uint8_t para_DS = 1;//%
uint8_t para_DR = 80;//%
uint16_t para_FS = 100;//Hz
uint16_t para_FR = 2000;//Hz

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void UI_Proc(void);
void KEY_Proc(void);
void ADC_Proc(void);
void LED_Proc(void);
void TIM_Proc(void);
void PWM_Proc(void);
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
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
    LCD_Init();
    LCD_SetBackColor(Black);
    LCD_SetTextColor(White);
    LCD_Clear(Black);
    
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
    
    HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
    
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      ADC_Proc();
      LED_Proc();
      KEY_Proc();
      UI_Proc();
      TIM_Proc();
      PWM_Proc();
      
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV3;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void UI_Proc(void){
    char buff[20];
    
    switch (page)
    {
    	case 0:
            LCD_DisplayStringLine(Line1, (u8*)"       PWM     ");
        
            sprintf(buff, "   CF=%dHz       ",CF);
            LCD_DisplayStringLine(Line3, (u8*)buff);
        
            sprintf(buff, "   CD=%d%%       ",CD);
            LCD_DisplayStringLine(Line4, (u8*)buff);
        
            sprintf(buff, "   DF=%dHz       ",DF);
            LCD_DisplayStringLine(Line5, (u8*)buff);
        
            sprintf(buff, "   ST=%s       ",ST == 0? "UNLOCK" :"UNLOCK");
            LCD_DisplayStringLine(Line6, (u8*)buff);
            
            sprintf(run_time,"%02dH%02dM%02dS",real_time_h,real_time_m,real_time_s);
        
            sprintf(buff, "   %s       ",run_time);
            LCD_DisplayStringLine(Line7, (u8*)buff);
        
    		break;
    	case 1:
            LCD_DisplayStringLine(Line1, (u8*)"       RECD     ");
        
            sprintf(buff, "   CF=%dHz       ",recd_CF);
            LCD_DisplayStringLine(Line3, (u8*)buff);
        
            sprintf(buff, "   CD=%d%%       ",recd_CD);
            LCD_DisplayStringLine(Line4, (u8*)buff);
        
            sprintf(buff, "   DF=%dHz       ",recd_DF);
            LCD_DisplayStringLine(Line5, (u8*)buff);
        
            sprintf(buff, "   XF=%d       ",recd_XF);
            LCD_DisplayStringLine(Line6, (u8*)buff);
  
            sprintf(buff, "   %s       ",recd_run_time);
            LCD_DisplayStringLine(Line7, (u8*)buff);
        
        
    		break;
    	default:
            LCD_DisplayStringLine(Line1, (u8*)"       PARA     ");
        
            sprintf(buff, "   DS=%d%%       ",para_DS);
            LCD_DisplayStringLine(Line3, (u8*)buff);
        
            sprintf(buff, "   DR=%d%%       ",para_DR);
            LCD_DisplayStringLine(Line4, (u8*)buff);
        
            sprintf(buff, "   FS=%dHz       ",para_FS);
            LCD_DisplayStringLine(Line5, (u8*)buff);
        
            sprintf(buff, "   FR=%dHz       ",para_FR);
            LCD_DisplayStringLine(Line6, (u8*)buff);
        
    		break;
    }
}
void KEY_Proc(void){
    KEY_Scan();
    uint8_t key = KEY_GetFlag();
    
    switch (page)
    {
    	case 0:
            if(key == 1) {
            
            }
            else if(key == 2) {
                ST ^= 1; 
            }
            else if(key == 3) {
            
            }
            else if(key == 4) {
            
            }
            else if(key == 5) {
                real_time = 0;
            }
            
    		break;
    	case 1:
            if(key == 1) {
            
            }
            else if(key == 2) {
            
            }
            else if(key == 3) {
            
            }
            else if(key == 4) {
            
            }
    		break;
    	default:
            if(key == 1) {
            
            }
            else if(key == 2) {
                if(++line == 4) line = 0;
            }
            else if(key == 3) {
                switch (line)
                {
                	case 0:
                        para_DS ++;
                		break;
                	case 1:
                        para_DR += 10;
                		break;
                	case 2:
                        para_FS += 100;
                		break;
                	case 3:
                        para_FR += 1000;
                		break;
                }
            }
            else if(key == 4) {
                switch (line)
                {
                	case 0:
                        para_DS --;
                		break;
                	case 1:
                        para_DR -= 10;
                		break;
                	case 2:
                        para_FS -= 100;
                		break;
                	case 3:
                        para_FR -= 1000;
                		break;
                }
            }
    		break;
    }
    if(key == 1) {
        if(para_DS >= para_DR || para_FS >= para_FR || para_DS < 1 || para_FS < 1 || para_DR > 100){
            para_DS = DS;
            para_FS = FS;
            para_FR = FR;
            para_DR = DR;
        }else {
            DS = para_DS;
            FS = para_FS;
            FR = para_FR;
            DR = para_DR;
        }
        LCD_Clear(Black);
        if(++page == 3) page = 0;
        line = 0;
        
    }
}

void ADC_Proc(void){
    ADC_Scan();
    if(!ST) {
        uint16_t VB = 4095 * FS / (FR - 1000);
        uint16_t n2 = vol.vol1 / VB;
        CF = 1000 + n2 * FS;
        
        uint16_t n1 = (DR - 10) / DS ;
        uint16_t VA = 4095 / (n1 + 1);
        uint16_t weiz = vol.vol2 / VA;
        
        CD = 10 + weiz * DS;
        if(CD > DR) CD = DR;
    }
}
void LED_Proc(void){
    if(page == 0) LED_Set(1);
    else LED_Reset(1);
    if(page == 1) LED_Set(2);
    else LED_Reset(2);
    if(page == 2) LED_Set(3);
    else LED_Reset(3);
    
    LED_Updata();
}
void TIM_Proc(void){
    real_time_s = real_time / 1000 % 60;
    real_time_m = real_time / 1000 / 60 % 60;
    real_time_h = real_time / 1000 / 60 / 60 % 24;
    
    
}
void PWM_Proc(void){
    static uint8_t flag = 0;
    pwm.pulse = CD;
    pwm.freq = CF;
    PWM_Updata();
    DF = pul.freq;
    
    uint16_t gas = CF > DF? (CF - DF) :(DF - CF); 
    if(gas > 1000 && flag == 0) {
        recd_CD = CD;
        recd_CF = CF;
        recd_DF = DF;
        recd_XF = gas; 
        strcpy(recd_run_time, run_time);
        flag = 1;
    }
    else if(gas <= 1000){
        flag = 0;
    }
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
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
