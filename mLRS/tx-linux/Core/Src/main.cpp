/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "../Inc/main.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <utility>
#include <cassert>

/* The systick source */
volatile uint32_t doSysTask;
volatile uint32_t uwTick;
class tThTimer
{
  bool tht_term { false };
  std::thread tht_thread;

  void thread_func()
    {
      while(!tht_term)
      {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        uwTick += 1;
        doSysTask++;
      }
    }
public:
  tThTimer()
  : tht_thread(&tThTimer::thread_func, this)
    { }

    ~tThTimer()
      {
        tht_term = true;
        tht_thread.join();
      }
  };


int main_main();

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  tThTimer tmr;

  return main_main();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  std::cerr << "Critical failure, terminating" << std::endl;
  assert(false);
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
