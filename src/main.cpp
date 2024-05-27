#include "header.h"

int main()
{
	SET(DDRB, csPin); // Set the chip select pin as an output
	SETT(DDRD, buzz[0], buzz[1]);

	twi_init();
	lcd_init();
	adc_init();

	timer2_init();
	timer1_init();

	sei();

	SPI.begin(); // Initialize SPI
	SPI.setDataMode(SPI_MODE0);
	SPI.setClockDivider(SPI_CLOCK_DIV128); // Slow down the clock for reliability

	initMax7219();

	for (uint8_t player = 0; player < 2; player++)
	{
		P_cursor[player].r = 0;
		P_cursor[player].c = 0;
		P_cursor[player].viewSelf = true;
		P_cursor[player].active = true;
	}

	lcd_print((char *)"  Place Phase!");
	lcd_set_cursor(1, 0);
	lcd_print((char *)"P1: ");
	lcd_send_data('0' + nr_ships[0]);
	lcd_set_cursor(1, 11);
	lcd_print((char *)"P2: ");
	lcd_send_data('0' + nr_ships[1]);

	while (1)
	{
		if (gamePhase == PLACE_PHASE)
		{
			if (nr_ships[0] >= 5 && nr_ships[1] >= 5)
			{
				gamePhase = BATTLE_PHASE;
				lcd_send_command(0x01); // clear display
				lcd_send_command(0x02); // return home
				lcd_print((char *)"   Start Game!");
				gameStart();

				delay(500);
				updatePlayerTurn();
			}
			else
			{
				lcd_set_cursor(1, 4);
				lcd_send_data('0' + nr_ships[0]);
				lcd_set_cursor(1, 15);
				lcd_send_data('0' + nr_ships[1]);
			}
			displayPlacePhase();
		}
		else
		{
			displayBattlePhase();
		}
		if (winner == -1)
		{
			inputP1();
			inputP2();
		}
	}

	return 0;
}
