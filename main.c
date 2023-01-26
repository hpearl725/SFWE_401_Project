#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

// programmed by Harrison Pearl and Logan Coddington

typedef struct card
{
    char suit;
    int value;
    // char assci art[][]; // optional
    struct card *next;
    struct card *prev;
} card;

typedef struct player_s
{
    char name[50];
    int P_money, P_num, P_bet;
    card *hand_HN_pointer; // points to head node for players hand
    struct player_s *next; // points to next plater
    struct plater_s *prev; // points to previous player
} player;

// global vars
struct card *head;
struct card *tail;
int playermoney;
struct card *dHand;
struct card *pHand;
int bet;
struct card *dHandHead;
struct card *dHandtail;

// functions list:
int betFun(int x);
card *ReadFile(char fnam[100]);                                                      // reads file
struct card *genNewCard(char suit, int value, struct card *prev, struct card *next); // generates new card
card *TailCard(char suit, int value, struct card *prev, struct card *next);          // adds card to end
void ShuffleDeck(card *HeadNode);
void SwapCards(card **HeadNode, int R1, int R2);
card deal(card *deck);
void print(card hand[], int round, int Pnum);
int total(card array[], int index);
void dub_or_loss(int total_player_hand_val, int total_dealer_hand_val);

int main()
{

    // int num_players; // the number of player in the game excluding dealer
    // char name_str[50];
    int total_deal_hand_val, total_player_hand_val; // total value of dealer and player hands
    char hit_or_stand = '\0';                       // character fort whether to hit or stay
    playermoney = 100;                              // amount of money the player has
    char fnam[100];                                 // file that user inputs for deck of cards
    char cont = 'y';
    char game_type = '\0';
    char shuffChar; // y/n char to shuffle linked list
    card dealer_cards[20];
    card player_cards[20];
    head = NULL;
    dHandHead = NULL;
    dHandtail = NULL;
    int Pnum_deal = 0;
    int Pnum_play = 0;

    // while loope so game can be run as many times as user wants
    while ((cont == 'y' || cont == 'Y') && (cont != 'q' || cont != 'Q'))
    {

        printf("enter the name of file containing the deck of cards:\n");
        scanf(" %s", fnam);

        head = (card *)malloc(sizeof(card));
        tail = (card *)malloc(sizeof(card));

        head = ReadFile(fnam);

        printf("do you want to shuffle the deck (y/n)?\n");
        scanf(" %c", &shuffChar);

        if (shuffChar == 'Y' || shuffChar == 'y')
        {
            ShuffleDeck(head);
        }

        // to add players must un-//
        // printf("enter number of players:\n");
        // scanf("%d",&num_players);

        /*for(int i = 0 ; i < num_players;i++){
            printf("Enter player %d's name:\n",i + 1);
            scanf("%s",name_str);
            //(call some function that creates the player, sends in i, and and name_str)
        }*/

        printf("Would you like to play (p) or test (t)?\n");
        scanf(" %c", &game_type); // scans to see what type of game the user would like to play

        if (game_type == 'p' || game_type == 'P')
        {

            ShuffleDeck(head);

            int i = 0;
            int x = 0;
            while (playermoney > 20 && (cont == 'y' || cont == 'Y'))
            {
                int bet = 0;
                int Pindex = 0;
                int Dindex = 0;
                Pnum_deal = 0;

                while (playermoney > 0 && (cont == 'y' || cont == 'Y'))
                {
                    player_cards[Pindex] = deal(head);
                    dealer_cards[Dindex] = deal(head);
                    Pindex++;
                    Dindex++;

                    printf("dealer:");
                    Pnum_deal = 0;
                    print(dealer_cards, Dindex, Pnum_deal);
                    printf("\n");
                    printf("player:");
                    Pnum_play = 1;
                    print(player_cards, Pindex, Pnum_play);
                    printf("\n");

                    bet = betFun(x);

                    total_deal_hand_val = total(dealer_cards, Dindex);
                    total_player_hand_val = total(player_cards, Pindex);

                    while (total_deal_hand_val != 14 && total_deal_hand_val <= 27)
                    {
                        total_deal_hand_val = total(dealer_cards, Dindex);
                        dealer_cards[Dindex] = deal(head);
                        Dindex++;
                        total_deal_hand_val = total(dealer_cards, Dindex);
                    }
                    printf("dealer:");
                    print(dealer_cards, Dindex, Pnum_deal);
                    printf("Player:");
                    print(player_cards, Pindex, Pnum_play);

                    if (total_deal_hand_val == 14)
                    {
                        printf("Dealer hit 14. you have to hit 31 to win.\n");
                        while ((total_player_hand_val <= 31))
                        { // deal player until either 31 or bust
                            hit_or_stand = 'h';
                            if (hit_or_stand == 'h' || hit_or_stand == 'H')
                            {
                                player_cards[Pindex] = deal(head);
                                Pindex++;
                                hit_or_stand = ' ';
                            }
                            total_player_hand_val = total(player_cards, Pindex);
                        }
                        if (total_player_hand_val == 31)
                        {
                            Pnum_deal = 1;
                            printf("dealer:");
                            print(dealer_cards, Dindex, Pnum_deal);
                            printf("Player:");
                            print(player_cards, Pindex, Pnum_play);
                            playermoney = playermoney + (2 * bet);
                            printf("Player Hit 31!,Player wins! player total: $%d\n", playermoney);
                            break;
                        }
                        else if (total_player_hand_val > 31)
                        {
                            Pnum_deal = 1;
                            printf("dealer:");
                            print(dealer_cards, Dindex, Pnum_deal);
                            printf("Player:");
                            print(player_cards, Pindex, Pnum_play);
                            printf("Bust! dealer wins, player total: $%d\n", playermoney);
                            break;
                        }
                    }

                    // dealer hits 31 // done
                    else if (total_deal_hand_val == 31)
                    {
                        Pnum_deal = 1;
                        printf("dealer:");
                        print(dealer_cards, Dindex, Pnum_deal);
                        printf("Player:");
                        print(player_cards, Pindex, Pnum_play);
                        printf("Dealer hits 31! Dealer wins, player total: $%d\n", playermoney);
                        break;
                    }

                    // player hits 14
                    else if ((total_player_hand_val == 14) && (total_deal_hand_val != 14 || total_deal_hand_val != 31))
                    {
                        Pnum_deal = 1;
                        printf("dealer:");
                        print(dealer_cards, Dindex, Pnum_deal);
                        printf("Player:");
                        print(player_cards, Pindex, Pnum_play);
                        playermoney = playermoney + (2 * bet);
                        printf("Player hit 14! Player wins! Player total $%d", playermoney);
                        break;
                    }

                    while (hit_or_stand != 's')
                    {
                        if (hit_or_stand == 'h' || hit_or_stand == 'H')
                        {
                            player_cards[Pindex] = deal(head);
                            Pindex++;
                            hit_or_stand = ' ';
                        }

                        if ((total_player_hand_val == 14) && (total_deal_hand_val != 14 || total_deal_hand_val != 31))
                        {
                            Pnum_deal = 1;
                            printf("dealer:");
                            print(dealer_cards, Dindex, Pnum_deal);
                            printf("Player:");
                            print(player_cards, Pindex, Pnum_play);
                            playermoney = playermoney + (2 * bet);
                            printf("Player hit 14! Player wins! Player total $%d\n", playermoney);
                            break;
                        }
                        else if (total_player_hand_val > 31)
                        {
                            printf("player bust\n");
                            break;
                        }
                    }

                    dub_or_loss(total_player_hand_val, total_deal_hand_val);
                }

                i++;

                printf("continue(y/n)?:\n");
                scanf(" %c", &cont);
            }
        }
    }

    return 0;
}

card *ReadFile(char fnam[100])
{
    FILE *inp;
    struct card *temp = (struct card *)malloc(sizeof(struct card) * 104);
    temp->prev = NULL;
    temp->next = NULL;
    inp = fopen(fnam, "r");

    while (fscanf(inp, "%d", &temp->value) != EOF)
    {
        fscanf(inp, " %c", &temp->suit);
        if (temp->prev == NULL)
        {
            head = genNewCard(temp->suit, temp->value, temp->prev, temp->next);
            temp->prev = head;
        }
        else if (temp->prev != NULL)
        {
            tail = TailCard(temp->suit, temp->value, temp->prev, temp->next);
        }
    }
    return head;
}

// makes a new card
struct card *genNewCard(char suit, int value, struct card *prev, struct card *next)
{
    struct card *newCard = (struct card *)malloc(sizeof(struct card) * 104);
    // for head card
    if (prev == NULL && next == NULL)
    {
        newCard->suit = suit;
        newCard->value = value;
        newCard->prev = next;
        newCard->next = NULL;
    }
    // for temp card if if prev card was head card
    else if (prev == head)
    {
        newCard->suit = suit;
        newCard->value = value;
        newCard->prev = next;
        newCard->next = NULL;
    }
    // for all other cards
    else if (prev != head && next == NULL)
    {
        newCard->suit = suit;
        newCard->value = value;
        newCard->prev = next;
        newCard->next = NULL;
    }
    return newCard;
}
// function links cards
card *TailCard(char suit, int value, struct card *prev, struct card *next)
{
    struct card *temp = tail;
    struct card *newCard = genNewCard(suit, value, prev, next);
    // if the card after ther head card is NULL then the new card is assigned as the card after head
    if (head->next == NULL)
    {
        head->next = newCard;
    }
    // if there is no card after the current card the next card is the new card and the new cards previous card is previous current card
    if (temp->next == NULL)
    {
        temp->next = newCard;
        newCard->prev = temp;
    }
    return newCard;
}

void ShuffleDeck(card *HeadNode)
{

    srand(time(NULL));

    for (int i = 0; i < 1000; i++)
    {

        int R1 = rand() % 103;
        int R2 = rand() % 103;
        if (R2 < R1)
        {
            SwapCards(&HeadNode, R1, R2);
        }
    }
}

void SwapCards(card **HeadNode, int R1, int R2)
{
    card *tempcard1 = NULL;
    card *tempcard2 = NULL;
    card temp1;
    tempcard1 = *HeadNode;
    tempcard2 = *HeadNode;

    if (R1 != R2)
    {
        for (int i = 0; i < R1; i++)
        {
            tempcard1 = tempcard1->next;
        }
        temp1.value = tempcard1->value;
        temp1.suit = tempcard1->suit;

        for (int i = 0; i < R2; i++)
        {
            tempcard2 = tempcard2->next;
        }
        tempcard1->value = tempcard2->value;
        tempcard1->suit = tempcard2->suit;

        tempcard2->value = temp1.value;
        tempcard2->suit = temp1.suit;
    }
}

card deal(card *deck)
{
    head = head->next;

    free(head->prev);

    return *deck;
}

int betFun(int x)
{
    while (1)
    {
        printf("How much do you want to bet?: \n");
        scanf(" %d", &x);

        if (playermoney - x >= 0)
        {
            playermoney = playermoney - x;
            break;
        }
        else if (x > playermoney)
        {
            printf("You don't have enough money to place that bet. Try again");
        }
    }
    return x;
}

void print(card hand[], int round, int Pnum)
{
    int i;
    int num = 0;

    if (Pnum == 0)
    {
        num = 1;
        printf("\u25FE");
    }

    for (i = num; i < round; i++)
    {

        if (hand[i].value > 10)
        {
            if (hand[i].value == 11)
            {
                printf("J");
            }
            if (hand[i].value == 12)
            {
                printf("Q");
            }
            if (hand[i].value == 13)
            {
                printf("K");
            }
        }

        else if (hand[i].value <= 10)
        {
            printf("%d", hand[i].value);
        }

        if (hand[i].suit == 'H')
        {
            printf("\u2665 ");
        }
        else if (hand[i].suit == 'C')
        {
            printf("\u2663 ");
        }
        else if (hand[i].suit == 'D')
        {
            printf("\u2662 ");
        }
        else if (hand[i].suit == 'S')
        {
            printf("\u2660 ");
        }
    }
    printf("\n");
}

// delete

int total(card array[], int index)
{
    int HandTot = 0;

    for (int i = 0; i < index; i++)
    {
        if (array[i].value > 10)
        {
            HandTot += 10;
        }
        else
        {
            HandTot += array[i].value;
        }
    }

    return HandTot;
}
void dub_or_loss(int total_player_hand_val, int total_dealer_hand_val)
{

    // int playermoney;
    //    int bet;

    // player wins, dealer loses
    if ((total_player_hand_val >= total_dealer_hand_val) && (total_player_hand_val != total_dealer_hand_val))
    {
        playermoney = playermoney + 2 * (bet);
        printf("Congrats! You beat the dealer");
    }

    // player losses, dealer wins

    else if ((total_player_hand_val <= total_dealer_hand_val) && (total_player_hand_val != total_dealer_hand_val))
    {
        playermoney = playermoney;
        printf("You lost. Better luck next hand");
    }

    // player and dealer tie
    else
        playermoney = playermoney + bet;
}
