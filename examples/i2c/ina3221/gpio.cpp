#include <wiringPi.h>
#include "gpio.h"

bool GPIO::setup = false;

// ***********************
// Constructeur
// Constructeur par defaut
// ***********************
GPIO::GPIO()
{
    if (!setup)
    {
        wiringPiSetupGpio();
        setup = true;
    }
}

// ******************************************************
// M�thode init en cas d'appel au constructeur par d�faut
// @param num�ro de pin
// ******************************************************
void GPIO::init(Pin pPinNumber)
{
    GPIO();
    pinNumber = pPinNumber;
}

// *********************************
// Constructeur
// M�morise le num�ro de pin utilis�
// @param num�ro de pin
// *********************************
GPIO::GPIO(Pin pPinNumber)
{
    GPIO();
    pinNumber = pPinNumber;
}

// **********************************
// Applique la direction sur une GPIO
// @param pDirection la direction
// **********************************
void GPIO::pinModePI(PinDirection pDirection)
{
    pinMode(pinNumber, pDirection);
}

// **************
// GPIO en entr�e
// **************
void GPIO::in()
{
    PinDirection input = In;
    pinMode(pinNumber, input);
}

// **************
// GPIO en sortie
// **************
void GPIO::out()
{
    PinDirection output = Out;
    pinMode(pinNumber, output);
}

// ***********************************************
// Active/d�sactive les r�sistance de pull down/up
// @param le type de r�sistance
// ***********************************************
void GPIO::pullUpDnControlPI(Pud pNiveau)
{
    pullUpDnControl(pinNumber, pNiveau);
}

// *******************************
// Active la r�sistance de pull up
// *******************************
void GPIO::pullUp()
{
    Pud pudUp = Up;
    pullUpDnControl(pinNumber, pudUp);
}

// *********************************
// Active la r�sistance de pull down
// *********************************
void GPIO::pullDn()
{
    Pud pudDown = Down;
    pullUpDnControl(pinNumber, pudDown);
}

// ***************************************
// D�sactive la r�sistance de pull up/down
// ***************************************
void GPIO::pullOff()
{
    Pud pudOff = Off;
    pullUpDnControl(pinNumber, pudOff);
}

// ****************************
// Applique un niveau � la GPIO
// @param le niveau
// ****************************
void GPIO::write(Level pValue)
{
    switch (pValue)
    {
        case LOW:
            off();
            break;
        case HIGH:
            on();
            break;
    }
}

// ********************************
// Applique un niveau bas � la GPIO
// ********************************
void GPIO::off()
{
    Level low = Low;
    digitalWrite(pinNumber, LOW);
}

// *********************************
// Applique un niveau haut � la GPIO
// *********************************
void GPIO::on()
{
    Level high = High;
    digitalWrite(pinNumber, HIGH);
}

// ****************************
// inverse le niveau de la GPIO
// ****************************
void GPIO::invertState()
{
    if (isOn())
    {
        off();
    }
    else
    {
        on();
    }
}

// ****************************
// Lecture du niveau de la GPIO
// @return le niveau
// ****************************
Level GPIO::read()
{
    if (digitalRead(pinNumber))
    {
        Level high = High;
        return high;
    }
    else
    {
        Level low = Low;
        return low;
    }
}

// *************************************
// Demande si la GPIO est au niveau haut
// @return true si oui sinon false
// *************************************
bool GPIO::isOn()
{
    Level high = High;
    if (digitalRead(pinNumber) == high)
    {
        return true;
    }
    return false;
}

// ************************************
// Demande si la GPIO est au niveau bas
// @return true si oui sinon false
// ************************************
bool GPIO::isOff()
{
    Level low = Low;
    if (digitalRead(pinNumber) == low)
    {
        return true;
    }
    return false;
}

// ***********************************************
// Active une fonction d'interruption sur un front
// @param le front
// @param un pointeur sur la fonction
// @return 0 si OK
// ***********************************************
int GPIO::fctInterrupt(FrontIntr pFront, interrupt intr)
{
    return wiringPiISR(pinNumber, pFront, *intr);
}

// ******************************************
// Demande si le destructeur sera appliqu�
// return destroyFlag, le flag de destructeur
// ******************************************
bool GPIO::isToDesactivate(void)
{
    return desactivateFlag;
}

// ****************************************
// Indique que le destructeur sera appliqu�
// ****************************************
void GPIO::toDesactivate(void)
{
    setToDesactivate(true);
}

// ***********************************************
// Indique que le destructeur ne sera pas appliqu�
// ***********************************************
void GPIO::noDesactivate(void)
{
    setToDesactivate(false);
}

// ************************************
// Positionne le flag de destructeur
// @param pValue le flag de destructeur
// ************************************
void GPIO::setToDesactivate(bool pValue)
{
    desactivateFlag = pValue;
}

// **********************************************************
// Destructeur
// Si le flag de destroy est activ�,
// met la GPIO en entr�e et d�sactive la r�sistance de rappel
// sinon, la GPIO reste en �tat m�me � la sortie du programme
// **********************************************************
GPIO::~GPIO()
{
    if (isToDesactivate())
    {
        Pud          pudOff = Off;
        PinDirection in     = In;
        pullUpDnControlPI(pudOff);
        pinModePI(in);
    }
}
