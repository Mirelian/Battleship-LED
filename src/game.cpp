#include "header.h"

uint8_t nr_ships[2] = {0, 0};
bool ver[2] = {true, true};
bool currentPlayer = 1;
int winner = -1;

enum GamePhase gamePhase = PLACE_PHASE;

struct cursor P_cursor[2];
struct Ship P_ships[2][5];

unsigned char LEDS[2][8] = {{0}};
unsigned char Ships[2][8] = {{0}};

void inputP1()
{
    static uint16_t lastButtonValue = 0xFFFF;
    uint16_t adcValue = buttonPress(PC0);
    if (adcValue != lastButtonValue && adcValue != 0xFFFF)
    {
        lastButtonValue = adcValue;
        action(0, adcValue);
        return;
    }
}

void inputP2()
{
    static uint16_t lastButtonValue = 0xFFFF;
    uint16_t adcValue = buttonPress(PC1);
    if (adcValue != lastButtonValue)
    {
        lastButtonValue = adcValue;
        action(1, adcValue);
        return;
    }
}

void action(uint8_t player, uint16_t command)
{
    if (gamePhase == PLACE_PHASE && nr_ships[player] != 5)
    {
        int length = (8 + nr_ships[player]) / 3;
        switch (command)
        {
        case BTN1:
        {
            rotateShip(player, length);
            break;
        }
        case BTN2:
        {
            if (checkMovement(player, -1, 0))
                P_cursor[player].r -= 1;
            break;
        }
        case BTN3:
        {
            placeShip(player);
            break;
        }
        case BTN4:
        {
            if (ver[player])
            {
                if (checkMovement(player, 0, 1))
                    P_cursor[player].c += 1;
            }
            else
            {
                if (checkMovement(player, 0, length))
                    P_cursor[player].c += 1;
            }
            break;
        }
        case BTN5:
        {
            if (ver[player])
            {
                if (checkMovement(player, length, 0))
                    P_cursor[player].r += 1;
            }
            else
            {
                if (checkMovement(player, 1, 0))
                    P_cursor[player].r += 1;
            }
            break;
        }
        case BTN6:
        {
            if (checkMovement(player, 0, -1))
                P_cursor[player].c -= 1;
            break;
        }
        }
    }
    else if (gamePhase == BATTLE_PHASE)
    {
        if (currentPlayer == player)
        {
            switch (command)
            {
            case BTN2:
            {
                if (checkMovement(player, -1, 0))
                    P_cursor[player].r -= 1;
                break;
            }
            case BTN3:
            {
                checkHit(player);
                break;
            }
            case BTN4:
            {
                if (checkMovement(player, 0, 1))
                    P_cursor[player].c += 1;
                break;
            }
            case BTN5:
            {
                if (checkMovement(player, 1, 0))
                    P_cursor[player].r += 1;
                break;
            }
            case BTN6:
            {
                if (checkMovement(player, 0, -1))
                    P_cursor[player].c -= 1;
                break;
            }
            }
        }
        else if (command == BTN1)
        {
            P_cursor[player].viewSelf = !P_cursor[player].viewSelf;
        }
    }
}

_Bool checkMovement(uint8_t player, uint8_t r_dist, uint8_t c_dist)
{
    uint8_t row = P_cursor[player].r + r_dist;
    uint8_t col = P_cursor[player].c + c_dist;
    if (row > 7 || row < 0)
        return 0;
    if (col > 7 || col < 0)
    {
        return 1;
    }
    return 1;
}

_Bool checkShipPlacement(uint8_t player, uint8_t row, uint8_t col, uint8_t length)
{
    for (uint8_t i = 0; i < length; i++)
    {
        if (ver[player])
        {
            if (Ships[player][row + i] & (1 << col))
                return 0;
        }
        else if (Ships[player][row] & (1 << (col + i)))
            return 0;
    }
    return 1;
}

void placeShip(uint8_t player)
{
    uint8_t length = (8 + nr_ships[player]) / 3;
    if (!checkShipPlacement(player, P_cursor[player].r, P_cursor[player].c, length))
    {
        wrongSelection(buzz[player]);
        return;
    }
    P_ships[player][nr_ships[player]].row = P_cursor[player].r;
    P_ships[player][nr_ships[player]].col = P_cursor[player].c;
    P_ships[player][nr_ships[player]].length = length;
    P_ships[player][nr_ships[player]].hits = 0;
    nr_ships[player]++;

    for (uint8_t i = 0; i < length; i++)
    {
        if (ver[player])
        {
            Ships[player][P_cursor[player].r + i] |= (1 << P_cursor[player].c);
        }
        else
        {
            Ships[player][P_cursor[player].r] |= (1 << (P_cursor[player].c + i));
        }
    }

    P_cursor[player].r = 0;
    P_cursor[player].c = 0;

    if (nr_ships[player] == 5)
        P_cursor[player].active = !P_cursor[player].active;
}

void rotateShip(uint8_t player, uint8_t length)
{
    if ((!ver[player] && (P_cursor[player].r + length) > 8) || (ver[player] && (P_cursor[player].c + length) > 8))
        return;
    ver[player] = !ver[player];
}

void updatePlayerTurn()
{
    P_cursor[0].viewSelf = (currentPlayer == 0);
    P_cursor[0].active = (currentPlayer == 1);
    P_cursor[1].viewSelf = (currentPlayer == 1);
    P_cursor[1].active = (currentPlayer == 0);
    currentPlayer = !currentPlayer;

    if (currentPlayer == 0)
    {
        lcd_send_command(0x01); // clear display
        lcd_send_command(0x02); // return home
        lcd_print((char *)"P1 Turn | ");
        lcd_set_cursor(1, 0);
        lcd_print((char *)"P2 ships left: ");
        lcd_send_data('0' + nr_ships[1]);
    }
    else
    {
        lcd_send_command(0x01); // clear display
        lcd_send_command(0x02); // return home
        lcd_print((char *)"P2 Turn | ");
        lcd_set_cursor(1, 0);
        lcd_print((char *)"P1 ships left: ");
        lcd_send_data('0' + nr_ships[0]);
    }
}

void checkHit(uint8_t player)
{
    uint8_t enemy = 1 - player;

    if (LEDS[enemy][P_cursor[player].r] & (1 << P_cursor[player].c))
    {
        wrongSelection(buzz[player]);
        return;
    }

    _Bool hitt = 0;

    for (int i = 0; i < 5; i++)
    {
        struct Ship *ship = &P_ships[enemy][i];
        if (ship->hits < ship->length)
        {
            for (int j = 0; j < ship->length; j++)
            {
                int shipRow = ship->row + (ship->vertical ? j : 0);
                int shipCol = ship->col + (ship->vertical ? 0 : j);
                if (shipRow == P_cursor[player].r && shipCol == P_cursor[player].c)
                {
                    hitt = 1;
                    ship->hits++;
                    if (ship->hits == ship->length)
                    {
                        shipSunken(buzz[player]);
                        nr_ships[enemy]--;
                        lcd_set_cursor(0, 10);
                        lcd_print((char *)"SUNKEN");
                        lcd_set_cursor(1, 15);
                        lcd_send_data('0' + nr_ships[enemy]);
                        delay(500);
                    }
                    else
                    {
                        shipHit(buzz[player]);
                        lcd_set_cursor(0, 10);
                        lcd_print((char *)"Hit!!!");
                        delay(500);
                    }
                    break;
                }
            }
            if (hitt)
                break;
        }
    }

    if (!hitt)
    {
        miss(buzz[player]);
        lcd_set_cursor(0, 10);
        lcd_print((char *)"Miss!?");
        delay(1000);
        updatePlayerTurn();
    }

    if (nr_ships[enemy] == 0)
    {
        victory(buzz[player]);
        lcd_send_command(0x01); // clear display
        lcd_send_command(0x02); // return home
        if (player == 0)
        {
            lcd_print((char *)"P1 WINS!!!!!!!!!");
        }
        else
        {
            lcd_print((char *)"P2 WINS!!!!!!!!!");
        }
        lcd_set_cursor(1, 0);
        if (nr_ships[player] == 5)
            lcd_print((char *)"FLAWLESS VICTORY");
        else
        {
            lcd_print((char *)"Ships left: ");
            lcd_send_data('0' + nr_ships[player]);
        }

        winner = player;
    }

    LEDS[enemy][P_cursor[player].r] |= (1 << P_cursor[player].c);
}

void displayPlacePhase()
{
    for (uint8_t row = 0; row < 8; row++)
    {
        unsigned char displayData[2] = {Ships[0][row], Ships[1][row]};

        for (uint8_t player = 0; player < 2; player++)
        {
            if (P_cursor[player].active)
            {
                uint8_t length = (8 + nr_ships[player]) / 3;
                if (ver[player])
                {
                    if (P_cursor[player].r <= row && P_cursor[player].r + length - 1 >= row)
                        displayData[player] = View_Placement(Ships[player][row], (1 << P_cursor[player].c));
                }
                else
                {
                    if (row == P_cursor[player].r)
                    {
                        unsigned char temp = 0x00;
                        for (uint8_t j = 0; j < length; j++)
                            temp |= (1 << (P_cursor[player].c + j));
                        displayData[player] = View_Placement(Ships[player][row], temp);
                    }
                    else
                        displayData[player] = Ships[player][row];
                }
            }
        }

        sendToMaxAll(row + 1, displayData[0], displayData[1]);
    }
}

void displayBattlePhase()
{
    for (uint8_t row = 0; row < 8; row++)
    {
        unsigned char displayData[2];

        for (uint8_t player = 0; player < 2; player++)
        {
            if (P_cursor[player].viewSelf)
            {
                displayData[player] = View_Self(LEDS[player][row], Ships[player][row]);
                if (P_cursor[1 - player].r == row && P_cursor[1 - player].active)
                {
                    displayData[player] ^= (f2 << P_cursor[1 - player].c);
                }
            }
            else
            {
                displayData[player] = View_Enemy(LEDS[1 - player][row], Ships[1 - player][row]);
                if (P_cursor[player].r == row && P_cursor[player].active)
                {
                    displayData[player] ^= (f2 << P_cursor[player].c);
                }
            }
        }

        sendToMaxAll(row + 1, displayData[0], displayData[1]);
    }
}
