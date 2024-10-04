//Ping Pong game

#include <stm32f031x6.h>
#include <stdbool.h>
#include "display.h"
#include "musical_notes.h"
#include "sound.h"
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 160
void initClock(void);
void initSysTick(void);
void SysTick_Handler(void);
void delay(volatile uint32_t dly);
void setupIO();
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py);
void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber);
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode);
int checkCollision(int, int, int, int, int, int, int);




volatile uint32_t milliseconds;

const uint16_t deco1[]=
{
0,0,0,0,4,4,4,4,4,0,0,0,0,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,65415,65415,65415,248,65415,0,0,0,0,0,0,0,65415,65415,65415,65415,65415,8068,0,0,0,0,0,0,65415,65415,65415,4096,4096,0,0,0,0,0,0,0,0,65415,65415,65415,0,0,0,0,0,0,0,0,0,7936,7936,7936,0,0,0,0,0,0,0,0,7936,7936,65535,7936,0,0,0,0,0,0,0,0,7936,7936,65535,7936,7936,7936,7936,0,0,0,0,0,7936,7936,65535,65535,65535,65535,7936,0,0,0,0,0,7936,7936,7936,7936,7936,7936,7936,0,0,0,0,0,7936,7936,7936,7936,0,0,0,0,0,0,0,0,0,7936,65535,7936,0,0,0,0,0,0,0,0,0,7936,65535,7936,0,0,0,0,0,0,0,0,0,7936,65535,7936,0,0,0,0,0,0,0,0,0,7940,7940,7940,7940,0,0,0,
};
const uint16_t deco2[]= 
	{
0,0,0,0,0,4,4,4,4,4,0,0,0,0,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,65415,65415,65415,248,65415,0,0,0,0,0,0,0,65415,65415,65415,65415,65415,8068,0,0,0,0,0,0,65415,65415,65415,4096,4096,0,0,0,0,0,0,0,0,65415,65415,65415,0,0,0,0,0,0,0,0,7936,7936,7936,0,0,0,0,0,0,0,0,7936,7936,65535,7936,0,0,0,0,0,0,0,0,7936,7936,65535,7936,7936,7936,7936,0,0,0,0,0,7936,7936,65535,65535,65535,65535,7936,0,0,0,0,0,7936,7936,7936,7936,7936,7936,7936,0,0,0,0,0,7936,7936,7936,7936,0,0,0,0,0,0,0,0,0,40224,7936,65535,7936,0,0,0,0,0,0,0,40224,40224,7936,65535,7936,0,0,0,0,0,0,65315,40224,40224,7936,65535,40224,0,0,0,0,0,0,0,65315,0,65315,65315,65315,65315,0,0,
	};
const uint16_t deco3[]= 
{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,4,4,4,4,4,4,0,0,0,0,7936,7936,4,4,4,4,4,4,7936,7936,0,0,65535,65535,4,4,4,4,4,4,65535,65535,0,0,7936,7936,4,4,4,4,4,4,7936,7936,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0,0,24327,24327,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};
const uint16_t dg1[]=
{
	0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,16142,16142,16142,0,0,0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,1994,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,
};

const uint16_t my_char[] = 
{ 
	0,0,0,0,0,0,0,0,0,0,0,0,0,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,0,0,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,0,0,65535,65535,65535,65535,40224,40224,40224,40224,65535,65535,0,0,65535,65535,65535,40224,40224,40224,40224,40224,65535,65535,0,0,65535,65535,40224,40224,40224,40224,40224,40224,65535,65535,0,0,65535,65535,40224,40224,40224,16621,16621,16621,65535,65535,0,0,65535,65535,40224,40224,40224,16621,16621,16621,65535,65535,0,0,65535,65535,40224,40224,40224,16621,16621,16621,65535,65535,0,0,65535,65535,40224,40224,40224,40224,40224,40224,65535,65535,0,0,65535,65535,40224,40224,40224,40224,40224,40224,65535,65535,0,0,65535,65535,40224,40224,40224,40224,40224,40224,65535,65535,0,0,65535,65535,40224,65535,65535,65535,65535,40224,65535,65535,0,0,65535,65535,40224,65535,65535,65535,65535,40224,65535,65535,0,0,65535,65535,40224,65535,65535,65535,65535,40224,65535,65535,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

const char * menu = {"option1", "option2", "option3"};
int main()
{
	int hinverted = 0;
	int vinverted = 0;
	int toggle = 0;
	int hmoved = 0;
	int vmoved = 0;
	int ballSpeedX =0;
	int ballSpeedY =2;
	int Player1Score =0;
	int Player2Score =0;
	int ScoreLimit = 7;
	int topRectX =35;

	uint16_t x = 35;
	uint16_t y = 120;
	uint16_t topx = 35;
	uint16_t topy = 20;
	uint16_t oldx = x;
	uint16_t oldy = y;
	uint16_t rectWidth = 30;
	uint16_t rectHeight = 10;
	initClock();
	initSysTick();
	setupIO();
	initSound();
	doMenu(menu, 3, 10, 10);

	// bottom rectangle
	//fillRectangle(35, 140, 60, 10, RGBToWord(255, 255, 255));
	//top rectangle
	//fillRectangle(35, 10, 60, 10, RGBToWord(255, 255, 255));

	//fillCircle(90, 90, 5, RGBToWord(0, 0, 255));
	//drawLine(150, 150, 160, 160, RGBToWord(0, 255, 0));
	//printText("SCORE %D:%D",Player1Score PlayerScore2 60, 60, RGBToWord(255,0,0));
	//putImage(20,80,12,16,my_char,0,0);

	//printText("SCORE:",60, 5, RGBToWord(0xff,0xff,0), 0);
	
	printNumber(1, 60, 140, RGBToWord(0xff,0xff,0), 0);
	int ballX = 60;
	int ballY = 60;
	while(1)
	{
		fillCircle(ballX, ballY, 5, RGBToWord(0, 0, 255));
		hmoved = vmoved = 0;
		hinverted = vinverted = 0;
		if ((GPIOB->IDR & (1 << 4))==0) // right pressed
		{					
			if (x < 110)
			{
				x = x + 1;
				hmoved = 1;
				hinverted=0;
			}						
		}
		if ((GPIOB->IDR & (1 << 5))==0) // left pressed
		{			
			
			if (x > 10)
			{
				x = x - 1;
				hmoved = 1;
				hinverted=1;
			}			
		}
		/*if ( (GPIOA->IDR & (1 << 11)) == 0) // down pressed
		{
			if (y < 140)
			{
				topy = topy + 1;			
				vmoved = 1;
				vinverted = 0;
			}
		}*/
		/*if ( (GPIOA->IDR & (1 << 8)) == 0) // up pressed
		{			
			if (y > 16)
			{
				topy = topy - 1;
				vmoved = 1;
				vinverted = 1;
			}
		}*/

		if ((GPIOA->IDR & (1 << 11)) == 0) // Right button pressed
		{
			if (topRectX < 140)
			{
				// Clear the previous position of the top rectangle
				fillRectangle(topx, topy, rectWidth, rectHeight, RGBToWord(0, 0, 0));
				
				// Move the top rectangle to the right
				topx += 1;

				// Redraw the top rectangle at the new position
				fillRectangle(topx, topy, rectWidth, rectHeight, RGBToWord(255, 255, 255));
			}
		}

		

		
			if ((GPIOA->IDR & (1 << 8)) == 0) // Left button pressed
			{
				if (topRectX > 16) 
				{
					// Clear the previous position of the top rectangle
					fillRectangle(topx, topy, rectWidth, rectHeight, RGBToWord(0, 0, 0));
					
					// Move the top rectangle to the left
					topx-= 1;

					// Redraw the top rectangle at the new position
					fillRectangle(topx, topy, rectWidth, rectHeight, RGBToWord(255, 255, 255));
				}

			}
		

	
		if ((vmoved) || (hmoved))
		{
			// Only redraw if there has been some movement (reduces flicker)
			fillRectangle(oldx, oldy, rectWidth, rectHeight, 0);
			oldx = x;
			oldy = y;

			if (hmoved)
			{
				if (toggle)
				{
					fillRectangle(x, y, rectWidth, rectHeight, RGBToWord(255, 255, 255));  
					x = x+1;
				}
				else
				{
					fillRectangle(x, y, rectWidth, rectHeight, RGBToWord(255, 255, 255));  
					x = x-1;
				}        

				y = oldy;
				toggle = toggle ^ 1;
			}
			else
			{
				fillRectangle(x, y, rectWidth,  rectHeight, RGBToWord (255, 255, 255));  
			}
		


			
		}

	
	
		





		// Clear the old ball position
		fillCircle(ballX, ballY, 5, RGBToWord(0, 0, 0));

		// Update the ball's position
		ballX = ballX + ballSpeedX;
		ballY = ballY + ballSpeedY;


		
		//check collision for top rectangle
		if (checkCollision(ballX, ballY, 5, topx, topy, rectWidth, rectHeight)) 
		{
			ballSpeedY = +-ballSpeedY; // Reverse Y direction when touching the moving rectangle
			
		}
		// Check for collisions with the bottom rectangle
		if (checkCollision(ballX, ballY, 5, x, y, rectWidth, rectHeight)) 
		{
			ballSpeedY = -ballSpeedY; // Reverse Y direction when touching the bottom rectangle
			
		}
			
		
		
		
		
		//check if ball gets past bottom rectangle 
		if(ballY >= SCREEN_HEIGHT)
		{
			//spawn ball in middle to start next round
			ballX = SCREEN_WIDTH / 2;
			ballY = SCREEN_HEIGHT / 2;

			//make ball go opposite direction so its fair on
			//player who lost the round
			ballSpeedY =-ballSpeedY;

			//award point to winner of round and play a sound
			playNote(1000);
			Player1Score++;
		}

		
		//check if ball gets past top rectangle
		if(ballY <= 0)
		{
			//ballSpeedY =+ballSpeedY;
			ballX = SCREEN_WIDTH / 2;
			ballY = SCREEN_HEIGHT / 2;
			ballSpeedY =-ballSpeedY;
			playNote(1000);
			Player2Score++;

		}

		

		//switch statement to put socre on display for player 1
		switch(Player1Score)
		{
			case 1:
			{
				printNumber(1, 60, 10, RGBToWord(0xff,0xff,0), 0);
				break;
			}

			case 2:
			{
				printNumber(2, 60, 10, RGBToWord(0xff,0xff,0), 0);
				break;
			}

			case 3:
			{
				printNumber(3, 60, 10, RGBToWord(0xff,0xff,0), 0);
				break;
			}
			
			case 4:
			{
				printNumber(4, 60, 10, RGBToWord(0xff,0xff,0), 0);
				break;
			}

			case 5:
			{
				printNumber(5, 60, 10, RGBToWord(0xff,0xff,0), 0);
				break;

			}

			case 6:
			{
				printNumber(6, 60, 10, RGBToWord(0xff,0xff,0), 0);
				break;
			}

			case 7:
			{
				printNumber(7, 60, 10, RGBToWord(0xff,0xff,0), 0);
				ballSpeedX =0;
				ballSpeedY =0;
				printText("WINNER: PLAYER 1",10, 50, RGBToWord(0xff,0xff,0), 0);
				break;

				
			}

			default:
			{
				printNumber(0, 60, 10, RGBToWord(0xff,0xff,0), 0);
				break;
			}
			
			
		}

				//switch statement to put socre on display for player 2
				
		switch(Player2Score)
		{
			case 1:
			{
				printNumber(1, 60, 140, RGBToWord(0xff,0xff,0), 0);
				break;
			}

			case 2:
			{
				printNumber(2, 60, 140, RGBToWord(0xff,0xff,0), 0);
				break;
			}

			case 3:
			{
				printNumber(3, 60, 140, RGBToWord(0xff,0xff,0), 0);
				break;
			}
			
			case 4:
			{
				printNumber(4, 60, 140, RGBToWord(0xff,0xff,0), 0);
				break;
			}

			case 5:
			{
				printNumber(5, 60, 140, RGBToWord(0xff,0xff,0), 0);
				break;

			}

			case 6:
			{
				printNumber(6, 60, 140, RGBToWord(0xff,0xff,0), 0);
				break;
			}

			case 7:
			{
				printNumber(7, 60, 140, RGBToWord(0xff,0xff,0), 0);
				ballSpeedX =0;
				ballSpeedY =0;
				printText("WINNER: PLAYER 2",10, 50, RGBToWord(0xff,0xff,0), 0);
				break;

				
			}

			default:
			{
				printNumber(0, 60, 140, RGBToWord(0xff,0xff,0), 0);
				break;
			}
			
			
		}
		
		

		


		// Redraw the ball at its new position
		fillCircle(ballX, ballY, 5, RGBToWord(0, 0, 255));



		// Add a small delay to control the ball's speed
		delay(10); 
		playNote(0);
		playNote(0);





			
			

			
		
			

		

	
		

		

	

		

		
		
		

	
		

		
	

	
	}//end while


	
	
		

	

	
		
	return 0;

}//end main

	

// Function to check collision between the ball and a moving rectangle
int checkCollision(int ballX, int ballY, int ballRadius, int rectX, int rectY, int rectWidth, int rectHeight) 
{
    int ballLeft = ballX - ballRadius; // calculate left side of ball by subtracting x by the radius
    int ballRight = ballX + ballRadius; // calculate right side of ball by adding x point to the radius
    int ballTop = ballY - ballRadius; // calculate top side of ball by subtracting y point by the radius
    int ballBottom = ballY + ballRadius; // calculate bottom side of ball by adding y point to the radius

    int rectLeft = rectX;
    int rectRight = rectX + rectWidth;
    int rectTop = rectY;
    int rectBottom = rectY + rectHeight;

    // Check for collision between the ball and the moving rectangle
    if (ballRight >= rectLeft && ballLeft <= rectRight &&
        ballBottom >= rectTop && ballTop <= rectBottom) 
	{
        return 1; // Collision detected
    }

    return 0; // No collision
}


int doMenu(char *options[], int len, int x, int y)
{
	int i;
	int current =0;
	int done =0;


	while(done==0)
	{

	
		for(i=0; i<len; i++)
		{
			if(i==current )
			{
				printText(options[i], x, y + (i*8), RGBToWord(15,15,255), 0);
			}

			else
			{
				printText(options[i], x, y + (i*8), RGBToWord(255,255,255), 0);
			}

			if((GPIOA->IDR & (1 << 8)) == 0) // up pressed)
			{
				current = current -1;

				if(current <0)
				{
					current = len -1;
				}

			}

			if((GPIOA->IDR & (1 << 11)) == 0) // down pressed)
			{
				current = current +1;
				
				if(current >=len)
				{
					current = 0;
				}

			}

			if((GPIOA->IDR & (1 << 4)) == 0) // right pressed)
			{
				done =1;
			}

			if((GPIOA->IDR & (1 << 5)) == 0) // left pressed)
			{
				current =-1;
				done =1;
			}

			delay(50);


		
		}

	}
	
}




	

void initSysTick(void)
{
	SysTick->LOAD = 48000;
	SysTick->CTRL = 7;
	SysTick->VAL = 10;
	__asm(" cpsie i "); // enable interrupts
}
void SysTick_Handler(void)
{
	milliseconds++;
}
void initClock(void)
{
// This is potentially a dangerous function as it could
// result in a system with an invalid clock signal - result: a stuck system
        // Set the PLL up
        // First ensure PLL is disabled
        RCC->CR &= ~(1u<<24);
        while( (RCC->CR & (1 <<25))); // wait for PLL ready to be cleared
        
// Warning here: if system clock is greater than 24MHz then wait-state(s) need to be
// inserted into Flash memory interface
				
        FLASH->ACR |= (1 << 0);
        FLASH->ACR &=~((1u << 2) | (1u<<1));
        // Turn on FLASH prefetch buffer
        FLASH->ACR |= (1 << 4);
        // set PLL multiplier to 12 (yielding 48MHz)
        RCC->CFGR &= ~((1u<<21) | (1u<<20) | (1u<<19) | (1u<<18));
        RCC->CFGR |= ((1<<21) | (1<<19) ); 

        // Need to limit ADC clock to below 14MHz so will change ADC prescaler to 4
        RCC->CFGR |= (1<<14);

        // and turn the PLL back on again
        RCC->CR |= (1<<24);        
        // set PLL as system clock source 
        RCC->CFGR |= (1<<1);
}
void delay(volatile uint32_t dly)
{
	uint32_t end_time = dly + milliseconds;
	while(milliseconds != end_time)
		__asm(" wfi "); // sleep
}

void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber)
{
	Port->PUPDR = Port->PUPDR &~(3u << BitNumber*2); // clear pull-up resistor bits
	Port->PUPDR = Port->PUPDR | (1u << BitNumber*2); // set pull-up bit
}
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode)
{
	/*
	*/
	uint32_t mode_value = Port->MODER;
	Mode = Mode << (2 * BitNumber);
	mode_value = mode_value & ~(3u << (BitNumber * 2));
	mode_value = mode_value | Mode;
	Port->MODER = mode_value;
}
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py)
{
	// checks to see if point px,py is within the rectange defined by x,y,w,h
	uint16_t x2,y2;
	x2 = x1+w;
	y2 = y1+h;
	int rvalue = 0;
	if ( (px >= x1) && (px <= x2))
	{
		// ok, x constraint met
		if ( (py >= y1) && (py <= y2))
			rvalue = 1;
	}
	return rvalue;
}

void setupIO()
{
	RCC->AHBENR |= (1 << 18) + (1 << 17); // enable Ports A and B
	display_begin();
	pinMode(GPIOB,4,0);
	pinMode(GPIOB,5,0);
	pinMode(GPIOA,8,0);
	pinMode(GPIOA,11,0);
	enablePullUp(GPIOB,4);
	enablePullUp(GPIOB,5);
	enablePullUp(GPIOA,11);
	enablePullUp(GPIOA,8);
}