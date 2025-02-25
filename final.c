#include <8051.h>
#include "KeyPad4x4.h"
#include "LED_Display.h"

void main()
{
	int mult = 0;
	char carry = 0;
	char sym = 0;
	char res[16] = { 0,0,0,0,0,0,0,0 };
	char count[8] = { 0,0,0,0,0,0,0,0 };
	char num1[8] = { 0,0,0,0,0,0,0,0 };
	char num2[8] = { 0,0,0,0,0,0,0,0 };
	char num3[8] = { 0,0,0,0,0,0,0,0 };
	char htr[3][8];
	int k = 0;
	char key = 0;	// 讀key值
	char last_key = 0; 
	unsigned char nums[8] = { 0,0,0,0,0,0,0,0 }; // 輸出值

	while (1)
	{
		for(int i = 0; i < 8; i++) // 顯示 led
		{
			for (unsigned int i = 0; i < 100; i++);//不斷顯示
			LED_Display(i, nums[i]);
		}
		
		key = KeyPad4x4();	// 讀 keypad
		for (unsigned int i = 0; i < 100; i++); //延遲

		if (last_key != key) // debounce
		{
			if (key != 0xff) //-1(沒感應到下個按鈕)
			{
				if (key < 10)//數字鍵
				{
					for (int i = 0; i < 7; i++) nums[i] = nums[i + 1];// 往左移
					nums[7] = key;

				}
				if (key >= 10 && key <14)//加減乘除
				{
					for (int i = 0; i < 8; i++) num1[i] = nums[i];//num1存第一個值
					switch (key)
					{
					case 10:
						sym = '+';
						break;
					case 11:
						sym = '-';
						break;
					case 12:
						sym = '*';
						break;
					case 13:
						sym = '/';
						break;
					}

					for (int i = 0; i < 8; i++) nums[i] = 0;//nums存0準備讀第二個值
				}
				if (key == 14)//等於
				{
					int flag = 0;//檢查除數是否大於被除數
					int carry2 = 0;
					switch (sym)
					{
					case '+' :
						carry = 0;
						for (int i = 7; i >= 0; i--)
						{
							num1[i] = num1[i] + nums[i] + carry;//同個位元相加 存到num1
							if (num1[i] > 9)
							{
								carry = 1;//處理進位
								num1[i] -= 10;
							}
							else
							{
								carry = 0;
							}
						}
						break;
					case '-':
						for (int i = 7; i >= 0; i--)
						{
							if (num1[i] < nums[i])//如果同位元減數比被減數大
							{
								int j = i - 1;//被借位的位子
								while (num1[j] == 0)//借不到位就繼續借
								{
									num1[j] = 9;
									j--;
								}
								num1[j] -= 1;
								num1[i] += 10;
							}
							num1[i] = num1[i] - nums[i];//num1減num
						}
						break;
					case '*':
						for (int i = 0; i < 16; i++) res[i] = 0;//8x8最多16位數

						for (int i = 7; i >= 0; i--)//乘數
						{
							int j = 0;
							carry = 0;
							for (j = 7; j >= 0; j--)//被乘數
							{
								int pos = i + j + 1;//從最高位往下存
								res[pos] += num1[i] * nums[j] + carry;
								carry = res[pos] / 10;
								res[pos] %= 10;
							}
							res[i + j + 1] += carry;//最大那位的carry處理(乘法的最左邊)
						}

						//傳回num1
						for (int i = 0; i < 8; i++) num1[i] = res[i + 8];
						break;
					case '/':
						for (int i = 0; i < 8; i++) count[i] = 0;//存count
						for (int i = 0; i < 8; i++) num2[i] = 0;//存1
						for (int i = 0; i < 8; i++) num3[i] = nums[i];//存nums原本的值
						num2[7] = 1;//存1
						while (flag != 1)//做到除數大於被除數
						{
							flag = 1;
							carry = 0;
							for (int i = 7; i >= 0; i--)//count++(做家法)
							{
								count[i] = count[i] + num2[i] + carry;
								if (count[i] > 9)
								{
									carry = 1;//處理進位
									count[i] -= 10;
								}
								else
								{
									carry = 0;
								}
							}
							for (int i = 0; i < 8; i++)//檢查flag nums>num1跳出迴圈
							{
								if (num1[i] > nums[i])
								{
									flag = 0;
									break;
								}
								else if (num1[i] < nums[i])
								{
									flag = 1;
									break;
								}
							}
							carry2 = 0;
							for (int i = 7; i >= 0; i--)//除數加上除數
							{
								nums[i] = nums[i] + num3[i] + carry2;
								if (nums[i] > 9)
								{
									carry2 = 1;//處理進位
									nums[i] -= 10;
								}
								else
								{
									carry2 = 0;
								}
							}
						}
						//處理不整除
						//因為nums會大過num3一個除數 所以把他減回來一個除數 
						for (int i = 7; i >= 0; i--)
						{
							if (nums[i] < num3[i])
							{
								int j = i - 1;//被借位的位子
								while (nums[j] == 0)
								{
									nums[j] = 9;
									j--;
								}
								nums[j] -= 1;
								nums[i] += 10;
							}
							nums[i] = nums[i] - num3[i];
						}
						//處理不整除(檢查上一個for迴圈減玩之後的nums是否有等於num1 如果一樣則沒問題 不一樣則 count多算一次要減回來)
						for (int i = 7; i >= 0; i--)
						{
							if (num1[i] != nums[i])//如果不是整除
							{
								for (int i = 7; i >= 0; i--)
								{
									if (count[i] < num2[i])
									{
										int j = i - 1;//被借位的位子
										while (count[j] == 0)
										{
											count[j] = 9;
											j--;
										}
										count[j] -= 1;
										count[i] += 10;
									}
									count[i] = count[i] - num2[i];//結果減一
								}//count-1
								break;
							}
						}
						for (int i = 0; i < 8; i++) num1[i] = count[i];//回傳到num1
						break;
					}
					
					for (int i = 0; i < 8; i++) nums[i] = num1[i];//傳到nums才能顯示
					//處理紀錄
					
					for (int i = 0; i < 8; i++) htr[k%8][i] = num1[i];//按下等於後存到紀錄陣列
					k++;
				}
				if (key == 15)
				{
					k--;
					for (int i = 0; i < 8; i++) nums[i] = htr[k%8][i];//顯示紀錄
				}
			}
			last_key = key; // record the last key that was pressed for debounce
		}
	}
}
