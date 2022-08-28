#include "NUMPAD_ROW.h"

uint8_t Key_col[1]={0xff};

char scan_col(void){
	Key_col[0] = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6)<<3;
  Key_col[0] = Key_col[0] | (HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7)<<2);
  Key_col[0] = Key_col[0] | (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_8)<<1);
  Key_col[0] = Key_col[0] | (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_9));
	
	if(Key_col[0] != 0x0f){
		HAL_Delay(10);
		if(Key_col[0] != 0x0f){
      switch(Key_col[0]){
				case 0x07:         //0111 press the key on 1st col of the this row
					return 1;
        case 0x0b:         //1011 press the key on 2nd col of the this row
          return 2;
        case 0x0d:         //1101 press the key on 3rd col of the this row
          return 3;
        case 0x0e:         //1110 press the key on 4th col of the this row
          return 4;
        default :
          return 0;
       }
		}
		else return 0;
	}
	else return 0;
}

char get_key(void){
	char Key_Num=0;       //which key:1-16
  char key_col_num=0;   // which col
	
	KEY_ROW0_OUT_LOW;        
  if( (key_col_num=scan_col()) != 0 )
  { 
		while(scan_col() != 0){};  // in case of mistaken touch
    if(key_col_num == 1){
			Key_Num = '1';
		}
		else if(key_col_num == 2){
			Key_Num = '2';
		}
		else if(key_col_num == 3){
			Key_Num = '3';
		}
		else if(key_col_num == 4){
			Key_Num = 'A';
		}
  }
  KEY_ROW0_OUT_HIGH;
	
	KEY_ROW1_OUT_LOW;        
  if( (key_col_num=scan_col()) != 0 )
  { 
		while(scan_col() != 0){};  // in case of mistaken touch
    if(key_col_num == 1){
			Key_Num = '4';
		}
		else if(key_col_num == 2){
			Key_Num = '5';
		}
		else if(key_col_num == 3){
			Key_Num = '6';
		}
		else if(key_col_num == 4){
			Key_Num = 'B';
		}
  }
  KEY_ROW1_OUT_HIGH;
	
	KEY_ROW2_OUT_LOW;        
  if( (key_col_num=scan_col()) != 0 )
  { 
		while(scan_col() != 0){};  // in case of mistaken touch
    if(key_col_num == 1){
			Key_Num = '7';
		}
		else if(key_col_num == 2){
			Key_Num = '8';
		}
		else if(key_col_num == 3){
			Key_Num = '9';
		}
		else if(key_col_num == 4){
			Key_Num = 'C';
		}
  }
  KEY_ROW2_OUT_HIGH;
	
	KEY_ROW3_OUT_LOW;        
  if( (key_col_num=scan_col()) != 0 )
  { 
		while(scan_col() != 0){};  // in case of mistaken touch
    if(key_col_num == 1){
			Key_Num = '*';
		}
		else if(key_col_num == 2){
			Key_Num = '0';
		}
		else if(key_col_num == 3){
			Key_Num = '#';
		}
		else if(key_col_num == 4){
			Key_Num = 'D';
		}
  }
  KEY_ROW3_OUT_HIGH;
	
	return Key_Num;
}

/*void sid_manual(char password[]){
  int count_num = 0;
	while(strlen(password) < 9 ){
		char key_confirm;
		key_confirm = get_key();
		if( (0 < key_confirm) && (key_confirm != '*') && (key_confirm != '#') )
		{
			LCD_Clear(100,100,100,40,WHITE);
			LCD_DrawString(100,100,"You pressed:");
			LCD_DrawChar(100,120,key_confirm);
			//strcat(password,&key_confirm);
			password[count_num] = key_confirm;
			LCD_DrawString(100,140,"SID:");
			LCD_DrawString(100,160,password);
			count_num++;
		}
		else if (key_confirm == '*'){
			LCD_Clear(100,120,100,20,WHITE);
			LCD_Clear(100,160,100,20,WHITE);
			count_num--;
			password[count_num] = '\0';
			LCD_DrawString(100,160,password);
		}
		else if (key_confirm == '#'){
			LCD_Clear(100,160,100,20,WHITE);
			break;
		}
	}
	return;
}*/

