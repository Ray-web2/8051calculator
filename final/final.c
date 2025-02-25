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
	char key = 0;	// Ūkey��
	char last_key = 0; 
	unsigned char nums[8] = { 0,0,0,0,0,0,0,0 }; // ��X��

	while (1)
	{
		for(int i = 0; i < 8; i++) // ��� led
		{
			for (unsigned int i = 0; i < 100; i++);//���_���
			LED_Display(i, nums[i]);
		}
		
		key = KeyPad4x4();	// Ū keypad
		for (unsigned int i = 0; i < 100; i++); //����

		if (last_key != key) // debounce
		{
			if (key != 0xff) //-1(�S�P����U�ӫ��s)
			{
				if (key < 10)//�Ʀr��
				{
					for (int i = 0; i < 7; i++) nums[i] = nums[i + 1];// ������
					nums[7] = key;

				}
				if (key >= 10 && key <14)//�[���
				{
					for (int i = 0; i < 8; i++) num1[i] = nums[i];//num1�s�Ĥ@�ӭ�
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

					for (int i = 0; i < 8; i++) nums[i] = 0;//nums�s0�ǳ�Ū�ĤG�ӭ�
				}
				if (key == 14)//����
				{
					int flag = 0;//�ˬd���ƬO�_�j��Q����
					int carry2 = 0;
					switch (sym)
					{
					case '+' :
						carry = 0;
						for (int i = 7; i >= 0; i--)
						{
							num1[i] = num1[i] + nums[i] + carry;//�P�Ӧ줸�ۥ[ �s��num1
							if (num1[i] > 9)
							{
								carry = 1;//�B�z�i��
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
							if (num1[i] < nums[i])//�p�G�P�줸��Ƥ�Q��Ƥj
							{
								int j = i - 1;//�Q�ɦ쪺��l
								while (num1[j] == 0)//�ɤ����N�~���
								{
									num1[j] = 9;
									j--;
								}
								num1[j] -= 1;
								num1[i] += 10;
							}
							num1[i] = num1[i] - nums[i];//num1��num
						}
						break;
					case '*':
						for (int i = 0; i < 16; i++) res[i] = 0;//8x8�̦h16���

						for (int i = 7; i >= 0; i--)//����
						{
							int j = 0;
							carry = 0;
							for (j = 7; j >= 0; j--)//�Q����
							{
								int pos = i + j + 1;//�q�̰��쩹�U�s
								res[pos] += num1[i] * nums[j] + carry;
								carry = res[pos] / 10;
								res[pos] %= 10;
							}
							res[i + j + 1] += carry;//�̤j���쪺carry�B�z(���k���̥���)
						}

						//�Ǧ^num1
						for (int i = 0; i < 8; i++) num1[i] = res[i + 8];
						break;
					case '/':
						for (int i = 0; i < 8; i++) count[i] = 0;//�scount
						for (int i = 0; i < 8; i++) num2[i] = 0;//�s1
						for (int i = 0; i < 8; i++) num3[i] = nums[i];//�snums�쥻����
						num2[7] = 1;//�s1
						while (flag != 1)//���찣�Ƥj��Q����
						{
							flag = 1;
							carry = 0;
							for (int i = 7; i >= 0; i--)//count++(���a�k)
							{
								count[i] = count[i] + num2[i] + carry;
								if (count[i] > 9)
								{
									carry = 1;//�B�z�i��
									count[i] -= 10;
								}
								else
								{
									carry = 0;
								}
							}
							for (int i = 0; i < 8; i++)//�ˬdflag nums>num1���X�j��
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
							for (int i = 7; i >= 0; i--)//���ƥ[�W����
							{
								nums[i] = nums[i] + num3[i] + carry2;
								if (nums[i] > 9)
								{
									carry2 = 1;//�B�z�i��
									nums[i] -= 10;
								}
								else
								{
									carry2 = 0;
								}
							}
						}
						//�B�z���㰣
						//�]��nums�|�j�Lnum3�@�Ӱ��� �ҥH��L��^�Ӥ@�Ӱ��� 
						for (int i = 7; i >= 0; i--)
						{
							if (nums[i] < num3[i])
							{
								int j = i - 1;//�Q�ɦ쪺��l
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
						//�B�z���㰣(�ˬd�W�@��for�j�����᪺nums�O�_������num1 �p�G�@�˫h�S���D ���@�˫h count�h��@���n��^��)
						for (int i = 7; i >= 0; i--)
						{
							if (num1[i] != nums[i])//�p�G���O�㰣
							{
								for (int i = 7; i >= 0; i--)
								{
									if (count[i] < num2[i])
									{
										int j = i - 1;//�Q�ɦ쪺��l
										while (count[j] == 0)
										{
											count[j] = 9;
											j--;
										}
										count[j] -= 1;
										count[i] += 10;
									}
									count[i] = count[i] - num2[i];//���G��@
								}//count-1
								break;
							}
						}
						for (int i = 0; i < 8; i++) num1[i] = count[i];//�^�Ǩ�num1
						break;
					}
					
					for (int i = 0; i < 8; i++) nums[i] = num1[i];//�Ǩ�nums�~�����
					//�B�z����
					
					for (int i = 0; i < 8; i++) htr[k%8][i] = num1[i];//���U�����s������}�C
					k++;
				}
				if (key == 15)
				{
					k--;
					for (int i = 0; i < 8; i++) nums[i] = htr[k%8][i];//��ܬ���
				}
			}
			last_key = key; // record the last key that was pressed for debounce
		}
	}
}