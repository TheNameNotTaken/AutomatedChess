/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "motorController.h"
#include "magnetController.h"
#include "board.h"
#include "display.h"
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
ADC_HandleTypeDef hadc1;

UART_HandleTypeDef hlpuart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_LPUART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char* mainMenu[16] = {"MAIN MENU","1.Choose Player Type\0", "2.Choose Piece Color\0","3.Board Configuration\0", "4.Start Game"};
char* colorSel[16] = {"1.Black\0", "2.White\0", "3.Main Menu"};
char* playerType[16] = {"1.Human\0", "2.CPU\0", "3.Main Menu"};
char* difficultyLevel[17] = {"CHOOSE DIFFICULTY\0", "1. level 1\0", "2. level 2\0", "3. level 3\0", "4. level 4\0", "5. level 5\0", "6. level 6\0", "7. level 7\0", "8. level 8\0"};
char* yourTurn[16] = {"Your Turn\0", "1.GO\0"};
char* chooseFirst[30] = {"Who's turn is it?", "1.Black", "2.White"};
char* invalid[16] = {"Invalid Move Try Again\0", "1.GO\0"};
char* gameOver[16] = {"Game Over\0","*White Won*\0" ,"1.Play Again(Main Menu)\0"};
char* configuration[16] = {"1.Default\0","2.Custom\0"};
char* opponentMove[16] = {"Opponent Move\0"};
char* pickPieces[16] = {"\0"};
char* askRow[32] = {"What row?", "1. 1", "2. 2", "3. 3", "4. 4", "5. 5", "6. 6", "7. 7", "8. 8"};
char* askCol[32] = {"What column?", "1. A", "2. B", "3. C", "4. D", "5. E", "6. F", "7. G", "8. H"};
char* askPiece[16] = {"What piece is right?", "1.P     2.p", "3.R     4.r", "5.N     6.n", "7.B     8.b", "9.Q     A.q", "B.K     C.k", "D.-"};
char* errorReadMove[30] = {"Error in reading state", "", "", "Press 1 to go back"};

//opponent Move
char move[5];

enum State {errorReadMoveState, pickPieceState, askPieceState, askColState, askRowState, validBoardState, whoFirst, startScreen, difficulty, color, type, yourMove, wrongMove, endGame, enemyMove, config , init, determineSide};
enum State currState = startScreen;
enum State pastState = init;

//a for ai h for human
char humanAi = 'a';
extern char playerColor;
extern char activeColor;
int defaultBoard = 0;

int selSquareRow = 0;
int selSquareCol = 0;

uint32_t keyValue(GPIO_TypeDef* colPorts[], GPIO_TypeDef* rowPorts[], uint16_t colPins[], uint16_t rowPins[]) {

	uint32_t keyNum = 16;

	for (int i = 0; i < 4; ++i) {
		//HAL_GPIO_WritePin(rowPorts[i], rowPins[i], GPIO_PIN_SET);
		HAL_GPIO_WritePin(colPorts[i], colPins[i], GPIO_PIN_RESET);
	}

	for (int i = 0; i < 4; ++i) {
		HAL_GPIO_WritePin(colPorts[i], colPins[i], GPIO_PIN_SET);


		for (int j = 0; j < 4; ++j) {
			if (HAL_GPIO_ReadPin(rowPorts[j], rowPins[j])) {
				if (i == 0) {
					if (j == 0) {
						keyNum = 1;
					} else if (j == 1) {
						keyNum = 4;
					} else if (j == 2) {
						keyNum = 7;
					} else {
						keyNum = 14; // *
					}

				} else if (i == 1) {
					if (j == 0) {
						keyNum = 2;
					} else if (j == 1) {
						keyNum = 5;
					} else if (j == 2) {
						keyNum = 8;
					} else {
						keyNum = 0;
					}
				} else if (i == 2) {
					if (j == 0) {
						keyNum = 3;
					} else if (j == 1) {
						keyNum = 6;
					} else if (j == 2) {
						keyNum = 9;
					} else {
						keyNum = 15; // #
					}
				} else {
					if (j == 0) {
						keyNum = 10; //A
					} else if (j == 1) {
					 	keyNum = 11; // B
					} else if (j == 2) {
						keyNum = 12; // C
					} else {
						keyNum = 13; // D
					}
				}

			}
		}

		HAL_GPIO_WritePin(colPorts[i], colPins[i], GPIO_PIN_RESET);
	}

	return keyNum;
}

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
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_LPUART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
//  struct coordinate coord = convertToCoord("A4");
//  coord = convertToCoord("P7");
  motorSetup();
  clearDisplay();
  makeMenu(5, mainMenu);
  setMagnet(STOP);
//  moveToCoord("A1");
//  moveToCoord("A8");
//  moveToCoord("H8");
//  moveToCoord("H1");
//  moveToCoord("A1");
//  executeInstruction("H1G8", BLACK, QUEEN);
//  moveToCoord("H1");
  calibrate_hall_sensors(board_defaults_calculated);
  print_board(board_defaults_calculated);
////  char* arr[16] = {"Jazib\0", "Jabibi\0","Ahmad\0"};
////  makeMenu(3, arr);
//  struct coordinate location = {2140, 1805};
//  moveToExactCoord(location);
//  location.x=0;
//  location.y=0;
//  moveToExactCoord(location);
  while (1)
  {
	  while(1){
//		blackwhite_readCurrentBoard(board);
		custom_readCurrentBoard(board);
		print_board_char(board);
//		setMagnet(BLACK);
//		setMagnet(WHITE);
//		setMagnet(STOP);
	  }
	 uint32_t keyNum = 16;
	//1:, r2, pd8
	//2:  r3, pf15
	//3:  c1, pf14
	//4:   r4, pe13
	//5:   c2, pe11
	//6:    c3 , pe9
	//7:   c4, pf13
	//8:   r1 , pd14


	//receive key pad input
	GPIO_TypeDef* colPorts[4] = {GPIOF, GPIOE, GPIOE, GPIOF};
	GPIO_TypeDef* rowPorts[4] = {GPIOD, GPIOD, GPIOF, GPIOE};
	uint16_t colPins[4] = {GPIO_PIN_14, GPIO_PIN_11, GPIO_PIN_9, GPIO_PIN_13};
	uint16_t rowPins[4] = {GPIO_PIN_9, GPIO_PIN_8, GPIO_PIN_15, GPIO_PIN_13};
	keyNum = keyValue(colPorts, rowPorts, colPins, rowPins);


//	if(keyNum == 2){
//		stepForward();
//	}
//	if(keyNum == 4){
//		stepLeft();
//	}
//	if(keyNum == 6){
//		stepRight();
//	}
//	if(keyNum == 5){
//		stepBack();
//	}

	pastState = currState;
	if(currState == startScreen){
		if(keyNum == 1){
			currState = type;
		}
		else if(keyNum == 2){
			currState = color;
		}
		else if(keyNum == 3){
			currState = config;
		}
		else if(keyNum == 4){
			if(defaultBoard == 0){
				sendInformation("default setup\r");
			}
			else{
				sendInformation("setup\r");
				char info[100];
				char FEN[80] = "\0";
				boardToFEN(board, FEN);
				sprintf(info, "%s\r", FEN);
				sendInformation(info);
			}
			//based on information of whose turn it is display either opponent or your move
			//easy to figure out if default configuration is setup
			//otherwise if custom then have to wait for XBEE response or from the reading from the board to determine which side to display
			//update variable regarding if its your Move or  enemy Move
			if(activeColor == playerColor){
				currState = yourMove ;
			}
			else{
				currState = enemyMove;
			}
		}
	}
	else if(currState == color)
	{
		if(keyNum == 1){
			playerColor = 'b';
			currState = startScreen;
		}
		else if(keyNum == 2)
		{
			playerColor = 'w';
			currState = startScreen;
		}
		else if(keyNum == 3)
		{
			currState = startScreen;
		}
	}
	else if(currState == type){
		if(keyNum == 1){
			humanAi = 'h';
			currState = startScreen;
		}
		else if(keyNum == 2){
			humanAi = 'a';
			currState = difficulty;
		}
		else if(keyNum == 3){
			currState = startScreen;
		}
	}
	else if(currState == difficulty){
		if(keyNum >= 1 && keyNum <= 8){
			sendInformation("difficulty\r");
			char num[3];
			sprintf(num, "%d\r", keyNum);
			sendInformation(num);
			currState = startScreen;
		}
	}
	else if(currState == config){
		if(keyNum == 1){
			defaultBoard = 0;
			currState = startScreen;
		}
		else if(keyNum == 2){
			defaultBoard = 1;
			char nextBoard[8][8];
			custom_readCurrentBoard(nextBoard);
			updateBoard(nextBoard);
			currState = validBoardState;
		}
	}
	else if(currState == validBoardState){
		//YES
		if(keyNum == 1){
			currState = whoFirst;
		}
		//NO
		else if(keyNum == 2){
			currState = askColState;
		}
	}
	else if(currState == askRowState){
		if(keyNum >= 1 && keyNum <= 8) currState = askPieceState;
		if(keyNum == 1)selSquareRow = 7;
		if(keyNum == 2)selSquareRow = 6;
		if(keyNum == 3)selSquareRow = 5;
		if(keyNum == 4)selSquareRow = 4;
		if(keyNum == 5)selSquareRow = 3;
		if(keyNum == 6)selSquareRow = 2;
		if(keyNum == 7)selSquareRow = 1;
		if(keyNum == 8)selSquareRow = 0;
	}
	else if(currState == askColState){
		if(keyNum >= 1 && keyNum <= 8) currState = askRowState;
		if(keyNum == 1)selSquareCol = 0;
		if(keyNum == 2)selSquareCol = 1;
		if(keyNum == 3)selSquareCol = 2;
		if(keyNum == 4)selSquareCol = 3;
		if(keyNum == 5)selSquareCol = 4;
		if(keyNum == 6)selSquareCol = 5;
		if(keyNum == 7)selSquareCol = 6;
		if(keyNum == 8)selSquareCol = 7;
	}
	else if(currState == askPieceState){
		if(keyNum >= 1 && keyNum <= 13) currState = validBoardState;
		if(keyNum == 1)board[selSquareRow][selSquareCol] = 'P';
		if(keyNum == 2)board[selSquareRow][selSquareCol] = 'p';
		if(keyNum == 3)board[selSquareRow][selSquareCol] = 'R';
		if(keyNum == 4)board[selSquareRow][selSquareCol] = 'r';
		if(keyNum == 5)board[selSquareRow][selSquareCol] = 'N';
		if(keyNum == 6)board[selSquareRow][selSquareCol] = 'n';
		if(keyNum == 7)board[selSquareRow][selSquareCol] = 'B';
		if(keyNum == 8)board[selSquareRow][selSquareCol] = 'b';
		if(keyNum == 9)board[selSquareRow][selSquareCol] = 'Q';
		if(keyNum == 10)board[selSquareRow][selSquareCol] = 'q';
		if(keyNum == 11)board[selSquareRow][selSquareCol] = 'K';
		if(keyNum == 12)board[selSquareRow][selSquareCol] = 'k';
		if(keyNum == 13)board[selSquareRow][selSquareCol] = '-';
	}
	else if(currState == whoFirst){
		if(keyNum == 1){
			activeColor = 'b';
			currState = startScreen;
		}
		else if(keyNum == 2){
			activeColor = 'w';
			currState = startScreen;
		}
	}
	else if(currState == yourMove){
		if(keyNum == 1){
			blackwhite_readCurrentBoard(newBoard);
			char move[5];
			findMoveFromBoards(newBoard, move);
			if(move[0] < 'A' || move[0] > 'H' || move[1] < '1' || move[1] > '8'
			|| move[2] < 'A' || move[2] > 'H' || move[3] < '1' || move[3] > '8'){
				currState = errorReadMoveState;
			}
			else{
				sendInformation("make move\r");
				char sentMove[6];
				sprintf(sentMove, "%s\r", move);
				sendInformation(sentMove);
				moveOnBoard(move);
				//check the board to determine ivalid move
				//if invalid move
		//			currState = wrongMove;
				//else if gameOver(store infroamtion in some variable regarding who won)
	//			currState = endGame;
				//else opponents turn
				currState = enemyMove;
				if(activeColor == 'w') activeColor = 'b';
				else if(activeColor == 'b') activeColor = 'w';
			}
		}
	}
	else if(currState == errorReadMoveState){
		if(keyNum == 1){
			currState = yourMove;
		}
	}
	else if(currState == wrongMove){
		if(keyNum == 1){
			currState = wrongMove;
		}
	}
	else if(currState == endGame){
		if(keyNum == 1){
			currState = startScreen;
		}
	}
	else if(currState == enemyMove){
		if(humanAi == 'a'){
			sendInformationGetData("computer move\r", move, 5, 2500);
		}
		else if(humanAi == 'h'){
			sendInformationGetData("human move\r", move, 5, 10000);
		}
		checkMoveForPickup(move);
		if(numToPickUp > 0){
			currState = pickPieceState;
		}
		else{
			int curColor = (activeColor == 'b') ? BLACK : WHITE;
			executeInstruction(move, curColor);
			moveOnBoard(move);
			if(activeColor == 'w') activeColor = 'b';
			else if(activeColor == 'b') activeColor = 'w';
			currState = yourMove;
			//assume for now that the move wont be invalid but could add a check here where set currstate to invalid move after checking opponent move
			//use xbee data to determine which piece to physically move to allow the motor to work, and if there is a pieve to move change currState
//			currState = pickPieces;
			//othewise  call function to  move the motor
			//once motor is movedd check if game ends and if game ends change currstate to end state after updating variable which piece won
//			currState = endGame;
			//otherwise
//			currState = yourMove;
		}
//		else if(humanAi == 'h'){
//			if(sendInformation("human move\r")){
////				getInformation(4, 50000);
//			}
//		}

	}
	else if(currState == pickPieceState){
		if(keyNum == 1){
			//TODO get knight info here
			executeInstruction(move, activeColor);
			currState = yourMove;
			numToPickUp = 0;
			toPickUp[0][0] = '\0';
			toPickUp[1][0] = '\0';
			toPickUp[2][0] = '\0';
			toPickUp[3][0] = '\0';
			if(activeColor == 'w') activeColor = 'b';
			else if(activeColor == 'b') activeColor = 'w';
		}
	}

	//move the motor
	//read Xvee data
	//read the board





	if(pastState != currState){
		if(currState == startScreen){
			makeMenu(5, mainMenu);
		}
		else if(currState == color){
			makeMenu(3, colorSel);
		}
		else if(currState == type){
			makeMenu(3, playerType);
		}
		else if(currState == difficulty){
			makeMenu(9, difficultyLevel);
		}
		else if(currState == yourMove){
			 makeMenu(2, yourTurn);
		}
		else if(currState == errorReadMoveState){
			makeMenu(4, errorReadMove);
		}
		else if(currState == pickPieceState){
			char* trigger[7];
			trigger[0] = "Pick up pieces at:";
			trigger[1] = toPickUp[0];
			trigger[2] = toPickUp[1];
			trigger[3] = toPickUp[2];
			trigger[4] = toPickUp[3];
			trigger[5] = "Press 1 to confirm";
			makeMenu(6, trigger);
		}
		else if(currState == wrongMove){
				makeMenu(2, invalid);
		}
		else if(currState == endGame ){
				makeMenu(2, gameOver);
		}
		else if(currState == enemyMove){
				makeMenu(2, opponentMove);
		}
		else if(currState == config){
			makeMenu(2, configuration);
		}
		else if(currState == validBoardState){
			displayChess(board);
			HAL_Delay(1000);
		}
		else if(currState == askRowState){
			makeMenu(9, askRow);
		}
		else if(currState == askColState){
			makeMenu(9, askCol);
		}
		else if(currState == askPieceState){
			makeMenu(8, askPiece);
		}
		else if(currState == whoFirst){
			makeMenu(3, chooseFirst);
		}
	}


	}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV32;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief LPUART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPUART1_UART_Init(void)
{

  /* USER CODE BEGIN LPUART1_Init 0 */

  /* USER CODE END LPUART1_Init 0 */

  /* USER CODE BEGIN LPUART1_Init 1 */

  /* USER CODE END LPUART1_Init 1 */
  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 115200;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  hlpuart1.FifoMode = UART_FIFOMODE_DISABLE;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&hlpuart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&hlpuart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPUART1_Init 2 */

  /* USER CODE END LPUART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

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
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  HAL_PWREx_EnableVddIO2();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_6
                          |GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_12|GPIO_PIN_13
                          |GPIO_PIN_15|GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9|GPIO_PIN_11|GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6
                          |GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PE2 PE3 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF13_SAI1;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PF0 PF1 PF2 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PF3 PF5 PF13 PF14 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_13|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : PF7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF13_SAI1;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PC1 PC3 PC4 PC6
                           PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_6
                          |GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA5 PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB2 PB12 PB13
                           PB15 PB4 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_12|GPIO_PIN_13
                          |GPIO_PIN_15|GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PF15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PE7 PE8 PE10 PE12 */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_10|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PE9 PE11 PE0 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_11|GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PE13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PE14 PE15 */
  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF3_TIM1_COMP1;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PB14 */
  GPIO_InitStruct.Pin = GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF14_TIM15;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PD8 PD9 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PC7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PC8 PC9 PC10 PC11 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_SDMMC1;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA8 PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PD0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PD2 PD4 PD5 PD6
                           PD7 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6
                          |GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PB3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
PUTCHAR_PROTOTYPE
{
  HAL_UART_Transmit(&hlpuart1, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
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
