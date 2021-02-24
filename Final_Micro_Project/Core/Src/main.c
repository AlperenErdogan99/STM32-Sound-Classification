/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "stdlib.h"
#include "ai_datatypes_defines.h"
#include "ai_platform.h"
#include "my_model.h"
#include "my_model_data.h"
#include"my_data.h"
#include "string.h"


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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CRC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */



	char buf[50];
	int buf_len = 0;
	ai_error ai_err;
	ai_i32 nbatch;

	float y_val_array[40], karar_dizisi[3];
	char sonuclar[30];


	// Chunk of memory used to hold intermediate values for neural network
	AI_ALIGNED(4) ai_u8 activations[AI_MY_MODEL_DATA_ACTIVATIONS_SIZE];

	// Giriş ve çıkış tensorları tanımlanmaktadır.
	AI_ALIGNED(4) ai_i8 in_data[AI_MY_MODEL_IN_1_SIZE_BYTES];
	AI_ALIGNED(4) ai_i8 out_data[AI_MY_MODEL_OUT_1_SIZE_BYTES];

	// Modelin pointeri oluşturulmaktadır.
	ai_handle my_model = AI_HANDLE_NULL;

	//initialize wrapper structs that hold pointers to data and info about the
	// data(tensor height, width, chaysels)
	ai_buffer ai_input[AI_MY_MODEL_IN_NUM] = AI_MY_MODEL_IN;
	ai_buffer ai_output[AI_MY_MODEL_OUT_NUM] = AI_MY_MODEL_OUT;

	//set working memory and get weights and biases from model
	ai_network_params ai_params = {

	AI_MY_MODEL_DATA_WEIGHTS(ai_my_model_data_weights_get()),
	AI_MY_MODEL_DATA_ACTIVATIONS(activations) };

	//set pointers wrapper structs to our data buffers
	ai_input[0].n_batches = 1;
	ai_input[0].data = AI_HANDLE_PTR(in_data);
	ai_output[0].n_batches = 1;
	ai_output[0].data = AI_HANDLE_PTR(out_data);

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
  MX_CRC_Init();
  /* USER CODE BEGIN 2 */

	//greetings!!
	buf_len = sprintf(buf, "\r\n\r\nSTM32 X-Cube-AI test \r\n");
	//buf değişkeninde verimiz saklanıyor

	//Modelimiz oluşturuluyor.
	ai_err = ai_my_model_create(&my_model, AI_MY_MODEL_DATA_CONFIG);
	if (ai_err.type != AI_ERROR_NONE) {
		buf_len = sprintf(buf, "Error: could not create NN instance\r\n");
		//buf değişkeninde verimiz saklanıyor
		while (1)
			;
	}

	//Modelin ön ayarlamaları yapılıyor.
	if (!ai_my_model_init(my_model, &ai_params)) {
		buf_len = sprintf(buf, "Error: could not initialize NN\r\n");
		//buf değişkeninde verimiz saklanıyor
		while (1)
			;

	}


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */




	while (1) {

		/*
		 * Buffer temizleniyor
		 */
		for (uint32_t i = 0; i < 40; i++) {
			y_val_array[i] = 0;

		}

		/*
		 * note: modele göndereceğimiz verilerimizi
		 * input bufferına alıyoruz.
		 */

		for (uint32_t i = 0; i < AI_MY_MODEL_IN_1_SIZE; i++) {
			((ai_float*) in_data)[i] = halil_ses_mfcc_1[i];

		}


		/*
		 * note: yüklediğimiz test verilerini modele gönderiyoruz.
		 * modeli bu veriler ile çalıştırıyoruz. Sonuçlarını kaydediyoruz.
		 * predict işlemini yapıyoruz yani aslında.
		 */
		nbatch = ai_my_model_run(my_model, &ai_input[0], &ai_output[0]);
		if (nbatch != 1) {
			buf_len = sprintf(buf, "Error: could not run inference\r\n");
			//buf değişkeninde verimiz saklanıyor
		}


		/*
		 * note: çıkış verimizi anlamlandırmak için
		 * out bufferındaki verileri farklı bir dizi
		 * içerisine alıyoruz. Ardından bu dizinin içerisindeki
		 * değerlere bakarak sınıflandırma sonucumuzu öğreniyoruz.
		 */
		/*
		 * sonucu yorumlama: y_val_array dizisinde 0 ile 9
		 * arasındaki indexlerinin değerlerini inceleriz.
		 * bunun sebebi softmax için 10 nöronlu bir
		 * çıkış layeri kullanmamızdır. 0 ile 9 arasındaki
		 * hangi index 1'e en yakın ise sınıflandırmanın
		 * sonucu ilgili indextir.
		 */
		for (uint32_t i = 0; i < 10; i++) {
			y_val_array[i] = ((float*) out_data)[i];

		}

		for (uint32_t i = 0 ; i<3 ; i++){

			karar_dizisi[i] = 1 - y_val_array[i];


		}

		memset(sonuclar, '0', 30);

		/*
		 * Modelin çıkışından alınan veriye göre sınıflandırma sonucu yorumlanır.
		 * Ses alperen sınıfına ait ise D13 Ledi yanar.
		 * Ses ayse sınıfına ait ise D12 Ledi yanar.
		 * Ses halil sınıfına ait ise D15 Ledi yanar.
		 */
		if ((karar_dizisi[0] < karar_dizisi[1]) & ( karar_dizisi[0] < karar_dizisi[2])){
			strcpy(sonuclar, "Alperen Bey Hosgeldiniz !!! ");
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, SET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, RESET);

		}else if((karar_dizisi[1] < karar_dizisi[0]) & ( karar_dizisi[1] < karar_dizisi[2])){
			strcpy(sonuclar, "Ayse Hosgeldin !!! ");
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, SET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, RESET);
		}else if ((karar_dizisi[2] < karar_dizisi[0]) & ( karar_dizisi[2] < karar_dizisi[1])){
			strcpy(sonuclar, "Halil Bey Hosgeldiniz !!! ");
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, SET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, RESET);
		}else {
			strcpy(sonuclar, "Sizi Taniyamadım !!! ");
		}

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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, AYSE_LED_Pin|ALPEREN_LED_Pin|HALIL_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : AYSE_LED_Pin ALPEREN_LED_Pin HALIL_LED_Pin */
  GPIO_InitStruct.Pin = AYSE_LED_Pin|ALPEREN_LED_Pin|HALIL_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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
	while (1) {
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
