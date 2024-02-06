// Samuel Lazareanu 53036281
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define TAX 1.17

#define MUSHROOM_TOPPING_PRICE 12
#define OLIVE_TOOPING_PRICE 10
#define TOMATO_TOPPING_PRICE 9
#define PINEAPPLE_TOPPING_PRICE 14

#define BASIC_PIZZA_LENGTH 40
#define BASIC_PIZZA_WIDTH 50
#define BASIC_PIZZA_PRICE 70

#define REGULAR_DOUGH_PRICE 0
#define VEGAN_DOUGH_PRICE 5
#define WHOLE_WHEAT_DOUGH_PRICE 3
#define GLUTEN_FREE_DOUGH_PRICE 4

#define OLIVE_TOOPING_CHAR 'O'
#define MUSHROOM_TOOPING_CHAR 'M'
#define TOMATO_TOOPING_CHAR 'T'
#define PINEAPPLE_TOOPING_CHAR 'P'

#define ID_ARRAY_SIZE 10

// Pizza struct - Including all the Info needed for the pizza
struct Pizza {
	int length,		// The length of the pizza
		width,		// The width of the pizza
		oliveTopping,	// The Olive toppings of the pizza
		mushroomTopping,		// The Mushroom toppings of the pizza
		tomatoTopping,		// The Tomato toppings of the pizza
		pineappleTopping;		// The Pineapple toppings of the pizza
	double toppingsSum;			// The summary of toppings (if pizza is full - 1, else <1)
	char dough;			// The dough type of the pizza
};

void chooseDoughType(struct Pizza* pizza, int* doughTypePrice);
double addDoughToPrice(int doughTypePrice, double chosenToBasicPizzaRatio);
void printMenu();
void orderPizza(struct Pizza* pizza, double* pizzaPrice);
void printPizza(struct Pizza* pizza);
void getPizzaSize(struct Pizza* pizza);
void getOliveToppings(struct Pizza* pizza);
void getMushroomToppings(struct Pizza* pizza);
void getTomatoToppings(struct Pizza* pizza);
void getPineappleToppings(struct Pizza* pizza);
void dealWithChange(double change);
void dealWithRemainingBalance(double* change);
void addToToppingSummary(struct Pizza* pizza, int topping);
void printRowOfChar(char ch, int times);
bool checkID(int id);
int calculateNumberOfDigits(int num);
int roundNumber(int num);
void setPieces(char piece[], struct Pizza* pizza);
bool isDoughOK(struct Pizza* pizza);

void main()
{
	// Samuel Lazareanu 53036281
	int counter,				// Counter - helper
		id = 0,				// The ID number of the user
		delivery,		// 1 - Y, 0 - N
		numOfPizzas;	// The number of pizzas the user wants to order
	double totalPrice = 0,			// The total price of the pizza
		priceWithTax = 0,			// The total price with tax
		pizzaPrice = 0,				// The price of the current pizza
		usersPayment,
		change;						// The change the user needs to receive - if needed
	struct Pizza pizza;

	printf(" Welcome to MTA-Pizza! \n\n");
	printf("*****\n *** \n  *  \n\n");

	printf("Please enter your ID number: \n");
	scanf("%d", &id);

	// Checks ID until its good
	while (!checkID(id))
	{
		printf("\nPlease enter your ID number: \n");
		scanf("%d", &id);
	}

	printMenu();

	printf("How many pizzas would you like to order? ");
	scanf("%d", &numOfPizzas);

	// Asking for the amount of pizzas until its good
	while (numOfPizzas <= 0)
	{
		printf("Invalid choice! Try again.\n");
		printf("How many pizzas would you like to order? ");
		scanf("%d", &numOfPizzas);
	}
	
	// Operating accordingly to the num of pizza the user wants to order
	if (numOfPizzas == 0)
		printf("\nThank you and goodbye.");
	else {
		for (counter = 1; counter <= numOfPizzas; counter++)
		{
			printf("\n*************************************************\n");
			printf("Pizza #%d: \n\n", counter);
			orderPizza(&pizza, &pizzaPrice);

			// Printing the details of the current pizza
			printf("\nPizza #%d details:\n", counter);
			printf("*******************\n");
			printf("Pizza size: %dx%d\n", pizza.length, pizza.width);
			printf("Pizza price (without tax): %.2lf\n", pizzaPrice);
			printPizza(&pizza);
			printf("\n");
			totalPrice += pizzaPrice;
			pizzaPrice = 0;
		}
		// THE NEXT PART WON'T EXECUTE IF THE USER DIDN'T WANT TO BUY A PIZZA
		printf("\n*************************************************\n");

		// Delivery or not + Adding to price
		printf("Do you want delivery for the price of 15 NIS? Enter 1 for delivery or 0 for pick-up: ");
		scanf("%d", &delivery);
		// Is the delivery input is invalid - keep askin until a valid one is entered
		while (delivery != 0 && delivery != 1) {
			printf("Invalid choice! Try again.\n");
			printf("Do you want delivery for the price of 15 NIS? Enter 1 for delivery or 0 for pick-up: ");
			scanf("%d", &delivery);
		}
		totalPrice += 15 * (double)delivery;

		priceWithTax = (int)(totalPrice * TAX);

		// Order details
		printf("\nYour order details: \n");
		printf("******************* \n");
		printf("ID number: %09d \n", id);
		printf("Number of pizzas: %d \n", numOfPizzas);
		if (delivery == 1)
			printf("Delivery\n");
		else
			printf("Pick-up\n");
		printf("Total price: %.2lf\n", totalPrice);
		printf("Total price with tax (rounded down): %d\n", (int)priceWithTax);

		printf("\nPlease enter your payment: ");
		scanf("%lf", &usersPayment);

		// Calculating and printing change
		change = usersPayment - priceWithTax;

		// If the user has a remaining payment to pay - it will keep asking for money
		while (change < 0)
		{
			dealWithRemainingBalance(&change);
		}
		if (change > 0) {
			dealWithChange(change);
			change = 0;
		}
	}
		printf("\nThank you for your order! \n");
}

// Ordering one pizza and updating the price of the pizza
void orderPizza(struct Pizza* pizza, double* pizzaPrice) {
	double chosenToBasicPizzaRatio;		// The ratio of the chosen pizza size to the basic pizza size
	int doughTypePrice = 0;			// The price of the chosen dough type
	bool exceededToppings = false;

	// The size of the pizza
	getPizzaSize(&*pizza);

	// Calculation the ratio of the chosen pizza size to basic pizza size - to use it in the calculations of the price
	chosenToBasicPizzaRatio = ((double)(pizza->length) * (double)(pizza->width)) / (double)(BASIC_PIZZA_LENGTH * BASIC_PIZZA_WIDTH);

	// The dough type of the pizza
	chooseDoughType(&*pizza, &doughTypePrice);
	while (!isDoughOK(&*pizza))
	{
		printf("Invalid choice! Try again. \n");
		chooseDoughType(&*pizza, &doughTypePrice);
	}
	*pizzaPrice += addDoughToPrice(doughTypePrice, chosenToBasicPizzaRatio);

	pizza->toppingsSum = 0;
	printf("\nPlease choose the toppings:\n");

	// Olive toppings
	printf("\nOlives (choose 0-3):\n");
	getOliveToppings(&*pizza);

	// Mushroom toppings
	if (pizza->toppingsSum < 1)
	{
		printf("\nMushrooms (choose 0-3):\n");
		getMushroomToppings(&*pizza);
		while (pizza->toppingsSum > 1) {
			printf("You have exceeded the maximum amount of toppings allowed on one pizza! Try again.\n");
			pizza->toppingsSum -= 1.0 / (pizza->mushroomTopping);
			getMushroomToppings(&*pizza);
		}
	}
	else
		pizza->mushroomTopping = 0;

	// Tomatos toppings
	if (pizza->toppingsSum < 1)
	{
		printf("\nTomatos (choose 0-3):\n");
		getTomatoToppings(&*pizza);
		while (pizza->toppingsSum > 1) {
			printf("You have exceeded the maximum amount of toppings allowed on one pizza! Try again.\n");
			pizza->toppingsSum -= 1.0 / (pizza->tomatoTopping);
			getTomatoToppings(&*pizza);
		}
	}
	else
		pizza->tomatoTopping = 0;

	// Pineapple toppings
	if (pizza->toppingsSum < 1)
	{
		printf("\nPineapple (choose 0-3):\n");
		getPineappleToppings(&*pizza);
		while (pizza->toppingsSum > 1) {
			printf("You have exceeded the maximum amount of toppings allowed on one pizza! Try again.\n");
			pizza->toppingsSum -= 1.0 / (pizza->pineappleTopping);
			getPineappleToppings(&*pizza);
		}
	}
	else
		pizza->pineappleTopping = 0;


	// Calculating the price of the pizza
	*pizzaPrice += chosenToBasicPizzaRatio * BASIC_PIZZA_PRICE;
	if (pizza->oliveTopping != 0)
		*pizzaPrice += chosenToBasicPizzaRatio * (1.0 / pizza->oliveTopping) * OLIVE_TOOPING_PRICE;
	if (pizza->mushroomTopping != 0)
		*pizzaPrice += chosenToBasicPizzaRatio * (1.0 / pizza->mushroomTopping) * MUSHROOM_TOPPING_PRICE;
	if (pizza->tomatoTopping != 0)
		*pizzaPrice += chosenToBasicPizzaRatio * (1.0 / pizza->tomatoTopping) * TOMATO_TOPPING_PRICE;
	if (pizza->pineappleTopping != 0)
		*pizzaPrice += chosenToBasicPizzaRatio * (1.0 / pizza->pineappleTopping) * PINEAPPLE_TOPPING_PRICE;
}

// Checking if the typed Dough char is valid
bool isDoughOK(struct Pizza* pizza)
{
	return (pizza->dough == 'r') || (pizza->dough == 'v') || (pizza->dough == 'w') || (pizza->dough == 'f');
}

// Choose a Dough type
void chooseDoughType(struct Pizza* pizza, int* doughTypePrice) {
	int doughType;
	printf("\nPlease enter the pizza's dough type: \n");
	printf("r - for regular \n");
	printf("v - for vegan \n");
	printf("w - for whole wheat \n");
	printf("f - for gluten-free \n");
	scanf(" %c", &pizza->dough);
	switch (pizza->dough) {
	case 'r':
		doughType = REGULAR_DOUGH_PRICE;
		break;
	case 'v':
		doughType = VEGAN_DOUGH_PRICE;
		break;
	case 'w':
		doughType = WHOLE_WHEAT_DOUGH_PRICE;
		break;
	case 'f':
		doughType = GLUTEN_FREE_DOUGH_PRICE;
		break;
	default:
		doughType = 0;
		break;
	}

	*doughTypePrice = doughType;
}

// Adding the dough to the price
double addDoughToPrice(int doughTypePrice, double chosenToBasicPizzaRatio) {
	return chosenToBasicPizzaRatio * (double)doughTypePrice;
}

// Prints the menu
void printMenu() {
	printf("\nOur menu: \n");
	printf("********* \n");
	printf("Basic pizza: %.2lf NIS for %dx%d size pizza \n\n", (double)BASIC_PIZZA_PRICE, BASIC_PIZZA_LENGTH, BASIC_PIZZA_WIDTH);
	printf("Toppings for basic size pizza: \n");
	printf("Olives: %d NIS \n", OLIVE_TOOPING_PRICE);
	printf("Mushrooms: %d NIS \n", MUSHROOM_TOPPING_PRICE);
	printf("Tomatoes: %d NIS \n", TOMATO_TOPPING_PRICE);
	printf("Pineapple: %d NIS \n\n", PINEAPPLE_TOPPING_PRICE);
	printf("Dough type for basic size pizza: \n");
	printf("Regular: %d NIS \n", REGULAR_DOUGH_PRICE);
	printf("Vegan: %d NIS \n", VEGAN_DOUGH_PRICE);
	printf("Whole wheat: %d NIS \n", WHOLE_WHEAT_DOUGH_PRICE);
	printf("Gluten free: %d NIS \n\n", GLUTEN_FREE_DOUGH_PRICE);
}

// Sets the pizza size given by the user
void getPizzaSize(struct Pizza* pizza) {
	printf("Please enter your pizza's length (cm): ");
	scanf("%d", &pizza->length);
	// If the input is invalid - keep asking until a valid one is entered
	while ((pizza->length % 2 != 0) || (pizza->length < 10) || (pizza->length > 40)) {
		printf("Invalid length! Try again.\n");
		printf("Please enter your pizza's length (cm): ");
		scanf("%d", &pizza->length);
	}

	printf("Please enter your pizza's width (cm): ");
	scanf("%d", &pizza->width);
	// If the input is invalid - keep asking until a valid one is entered
	while ((pizza->width % 2 != 0) || (pizza->width < 10) || (pizza->width > 80)) {
		printf("Invalid width! Try again.\n");
		printf("Please enter your pizza's width (cm): ");
		scanf("%d", &pizza->width);
	}
}

// Sets the olive toppings given by the user
void getOliveToppings(struct Pizza* pizza) {
	printf("0. None\n");
	printf("1. Whole pizza\n");
	printf("2. Half pizza \n");
	printf("3. Quarter pizza \n");
	scanf("%d", &pizza->oliveTopping);
	if ((pizza->oliveTopping < 0) || (pizza->oliveTopping > 3)) {
		printf("Invalid choice! Current topping not added. \n");
		pizza->oliveTopping = 0;
	}
	// For convenience - if the user chooses 3 for the toppings - changing it to 4 so I can devide 1 by it and get the ratio of toppin on the pizza
	if (pizza->oliveTopping == 3)
		pizza->oliveTopping = 4;

	if (pizza->oliveTopping != 0)
		pizza->toppingsSum += 1.0 / (pizza->oliveTopping);
}

// Sets the mushroom toppings given by the user
void getMushroomToppings(struct Pizza* pizza) {
	printf("0. None\n");
	printf("1. Whole pizza\n");
	printf("2. Half pizza \n");
	printf("3. Quarter pizza \n");
	scanf("%d", &pizza->mushroomTopping);
	if ((pizza->mushroomTopping < 0) || (pizza->mushroomTopping > 3)) {
		printf("Invalid choice! Current topping not added. \n");
		pizza->mushroomTopping = 0;
	}
	if (pizza->mushroomTopping == 3)
		pizza->mushroomTopping = 4;

	if (pizza->mushroomTopping != 0)
		pizza->toppingsSum += 1.0 / (pizza->mushroomTopping);
}

// Sets the tomato toppings given by the user
void getTomatoToppings(struct Pizza* pizza) {
	printf("0. None\n");
	printf("1. Whole pizza\n");
	printf("2. Half pizza \n");
	printf("3. Quarter pizza \n");
	scanf("%d", &pizza->tomatoTopping);
	if ((pizza->tomatoTopping < 0) || (pizza->tomatoTopping > 3)) {
		printf("Invalid choice! Current topping not added. \n");
		pizza->tomatoTopping = 0;
	}
	if (pizza->tomatoTopping == 3)
		pizza->tomatoTopping = 4;

	if (pizza->tomatoTopping != 0)
		pizza->toppingsSum += 1.0 / (pizza->tomatoTopping);
}

// Sets the pineapple toppings given by the user
void getPineappleToppings(struct Pizza* pizza) {
	printf("0. None\n");
	printf("1. Whole pizza\n");
	printf("2. Half pizza \n");
	printf("3. Quarter pizza \n");
	scanf("%d", &pizza->pineappleTopping);
	if ((pizza->pineappleTopping < 0) || (pizza->pineappleTopping > 3)) {
		printf("Invalid choice! Current topping not added. \n");
		pizza->pineappleTopping = 0;
	}
	if (pizza->pineappleTopping == 3)
		pizza->pineappleTopping = 4;

	if (pizza->pineappleTopping != 0)
		pizza->toppingsSum += 1.0 / (pizza->pineappleTopping);
}

// Adding the topping to the summary of toppings on the pizza
void addToToppingSummary(struct Pizza* pizza, int topping) {
	if (topping != 0)
		pizza->toppingsSum += 1.0 / (double)topping;
}

// Dealing with change - calculating the change and printing it in coins
void dealWithChange(double change)
{
	// The change the user needs to get - in tens, fives, twos and ones.
	int tensChange,
		fivesChange, 
		twosChange, 
		onesChange;	

	printf("Your change is %d NIS using: \n", (int)change);

	// Calculatin the coins
	tensChange = (int)((int)change - ((int)change % 10)) / 10;
	change -= tensChange * 10.0;
	fivesChange = ((int)change / 5);
	change -= fivesChange * 5.0;
	twosChange = ((int)change / 2);
	change -= twosChange * 2.0;
	onesChange = ((int)change / 1);

	// Printing the change in coins
	if (tensChange != 0)
		printf("%d coin(s) of ten \n", tensChange);
	if (fivesChange != 0)
		printf("%d coin(s) of five \n", fivesChange);
	if (twosChange != 0)
		printf("%d coin(s) of two \n", twosChange);
	if (onesChange != 0)
		printf("%d coin(s) of one ", onesChange);
}

// dealing with reamining balance - calculating it and asking for another payment
void dealWithRemainingBalance(double* change)
{
	double usersPayment;
	while (*change < 0)
	{
		printf("Your remaining balance is: %d", ((int)*change * (-1)));
		printf("\nPlease enter your payment: ");
		scanf("%lf", &usersPayment);
		*change += usersPayment;
	}
}

// Prints the pizza
void printPizza(struct Pizza* pizza) {
	int i,
		length = pizza->length,
		width = pizza->width;
	bool finished = false;
	char piece[5] = { 0,0,0,0,0 };

	setPieces(piece, *&pizza);

	printRowOfChar(pizza->dough, width);

	for (i = 0; i < (length - 2) / 2; i++)
	{
		printf("\n%c", pizza->dough);
		printRowOfChar(piece[4], (width - 2) / 2);
		printRowOfChar(piece[1], (width - 2) / 2);
		printf("%c", pizza->dough);
	}
	for (i = 0; i < (length - 2) / 2; i++)
	{
		printf("\n%c", pizza->dough);
		printRowOfChar(piece[3], (width - 2) / 2);
		printRowOfChar(piece[2], (width - 2) / 2);
		printf("%c", pizza->dough);
	}
	printf("\n");
	printRowOfChar(pizza->dough, width);
}

// Prints a row in length 'times' of char 'ch'
void printRowOfChar(char ch, int times) {
	while (times > 0) {
		printf("%c", ch);
		times--;
	}
}

// Checks the ID is valid
bool checkID(int id) {
	int digits = calculateNumberOfDigits(id),
		currentDigit,
		checkDigit,
		multiplyBy = 2,
		sum = 0;
	int idDigits[ID_ARRAY_SIZE];

	if (digits > 9 || id == 0) {
		printf("Invalid ID number! Try again.");
		return false;
	}
	else {
		checkDigit = id % 10;
		id /= 10;
		while (id > 0) {
			currentDigit = id % 10;
			if (currentDigit * multiplyBy >= 10)
				sum += (currentDigit*multiplyBy) % 10 + ((currentDigit*multiplyBy / 10) % 10);
			else
				sum += currentDigit*multiplyBy;

			if (multiplyBy == 2)
				multiplyBy = 1;
			else
				multiplyBy = 2;

			id /= 10;
		}
		if (roundNumber(sum) - sum != checkDigit)
		{
			printf("Invalid check digit! Try again.");
			return false;
		}
		else
			return true;
	}
}

// Calculates the number of digits in a number
int calculateNumberOfDigits(int num) {
	int counter = 0;

	while (num > 0) {
		num /= 10;
		counter++;
	}

	return counter;
}

// Rounds the number to the upper tenth
int roundNumber(int num) {
	int digits = calculateNumberOfDigits(num);
	if (num % 10 == 0)
		return num;
	if (num > 0 && num < 10)
		return 10;
	digits--;
	num = num / (pow(10, digits));
	return (num + 1) * 10;
}

// Sets the toppings in an array of the pieces of the pizza
void setPieces(char piece[], struct Pizza* pizza)
{
	bool finished = false;
	int i;
	double oliveTopping = 0,			// I don't want to ruin the data on the pizza so I created a variable for each one to play with
		mushroomTopping = 0,
		tomatoTopping = 0,
		pineappleTopping = 0;
	/*
	switch (pizza->oliveTopping)
	{
	case 1:
		for (i = 1; i < 5; i++)
			*piece[i] = OLIVE_TOOPING_CHAR;
		finished = true;
		break;
	case 2:
		*piece[1] = OLIVE_TOOPING_CHAR;
		*piece[2] = OLIVE_TOOPING_CHAR;

		if (pizza->mushroomTopping == 2)
		{
			*piece[3] = MUSHROOM_TOOPING_CHAR;
			*piece[4] = MUSHROOM_TOOPING_CHAR;
			finished = true;
		}
		else
		{
			if (pizza->mushroomTopping == 4)
			{
				*piece[3] = MUSHROOM_TOOPING_CHAR;
				if (pizza->tomatoTopping == 4)
				{
					*piece[4] = MUSHROOM_TOOPING_CHAR;
					finished = true;
				}
				else if (pizza->pineappleTopping == 4)
				{
					*piece[4] = PINEAPPLE_TOOPING_CHAR;
					finished = true;
				}
				else
				{
					*piece[4] = 0;
					finished;
				}
			}
			else
			{
				if (pizza->tomatoTopping == 4)
				{
					*piece[3] = TOMATO_TOOPING_CHAR;
					if (pizza->pineappleTopping == 4)
					{
						*piece[4] = PINEAPPLE_TOOPING_CHAR;
						finished = true;
					}
					else
					{
						*piece[4] = 0;
						finished;
					}
				}
				else
				{
					if (pizza->pineappleTopping == 4)
					{
						*piece[3] = PINEAPPLE_TOOPING_CHAR;
						*piece[4] = 0;
						finished;
					}
				}
			}
		}
		break;
	case 4:
		*piece[1] = OLIVE_TOOPING_CHAR;
		if (pizza->mushroomTopping == 2)
		{
			*piece[2] = MUSHROOM_TOOPING_CHAR;
			*piece[3] = MUSHROOM_TOOPING_CHAR;
			if (pizza->tomatoTopping == 4)
				*piece[4] = TOMATO_TOOPING_CHAR;
			else if (pizza->pineappleTopping == 4)
				*piece[4] = PINEAPPLE_TOOPING_CHAR;
			else
				*piece[4] = 0;
		}

		break;
	default:
		break;
	}
	*/

	if (pizza->oliveTopping != 0)
		oliveTopping = 1.0 / pizza->oliveTopping;
	if (pizza->mushroomTopping != 0)
		mushroomTopping = 1.0 / pizza->mushroomTopping;
	if (pizza->tomatoTopping != 0)
		tomatoTopping = 1.0 / pizza->tomatoTopping;
	if (pizza->pineappleTopping != 0)
		pineappleTopping = 1.0 / pizza->pineappleTopping;

	for (i = 1; i < 5; i++)
	{
		if (oliveTopping > 0 && oliveTopping <= 1) {
			piece[i] = OLIVE_TOOPING_CHAR;
			oliveTopping -= 0.25;
		}
		else if (mushroomTopping > 0 && mushroomTopping <= 1)
		{
			piece[i] = MUSHROOM_TOOPING_CHAR;
			mushroomTopping -= 0.25;
		}
		else if (tomatoTopping > 0 && tomatoTopping <= 1)
		{
			piece[i] = TOMATO_TOOPING_CHAR;
			tomatoTopping -= 0.25;
		}
		else if (pineappleTopping > 0 && pineappleTopping <= 1)
		{
			piece[i] = PINEAPPLE_TOOPING_CHAR;
			pineappleTopping -= 0.25;
		}
		else
			piece[i] = ' ';
	}
}