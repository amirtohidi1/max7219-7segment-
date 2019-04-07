
int MAX7219_DIN[] = { 23 , 25 , 27 , 29,43};
int MAX7219_CS[]  = {31, 33 , 35, 37,41};
int MAX7219_CLK[] = {22, 22, 22, 39,45};

byte xx[] = {0x00 , 0x01 , 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};

char te[5][8] = {
  {'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c'},
  {'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c'},
  {'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c'},
  {'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c'},
  {'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c'}
};

int tedad = 5;
void initialise()
{
  for (int i = 0 ; i < tedad ; i++)
  {
    pinMode(MAX7219_DIN[i], OUTPUT);
  }

  for (int i = 0 ; i < tedad ; i++)
  {
    pinMode(MAX7219_CS[i], OUTPUT);
    pinMode(MAX7219_CLK[i], OUTPUT);
  }

  

  for (int i = 0 ; i < tedad ; i++)
  {
    digitalWrite(MAX7219_CS[i], HIGH);

    while (digitalRead(MAX7219_CS[i]) != HIGH)
    {}
  }

}

void output(byte address, byte data , int pin_num)
{
  digitalWrite(MAX7219_CS[pin_num], LOW);
  while (digitalRead(MAX7219_CS[pin_num]) != LOW)
  {
  }
  shiftOut(MAX7219_DIN[pin_num], MAX7219_CLK[pin_num], MSBFIRST, address);
  shiftOut(MAX7219_DIN[pin_num], MAX7219_CLK[pin_num], MSBFIRST, data);

  digitalWrite(MAX7219_CS[pin_num], HIGH);

  while (digitalRead(MAX7219_CS[pin_num]) != HIGH)
  {
  }
}

void setup() {
  Serial.begin(9600);

  // put your setup code here, to run once:
  initialise();

  for (int i = 0 ; i < tedad ; i++)
  {
    initMax7219(i);
    showNUMBER(99990+i, i);
    delay(2000);
  }

}

void initMax7219(int pin_num)
{
  output(0x0f, 0x00, pin_num); //display test register - test mode off
  output(0x0c, 0x01, pin_num); //shutdown register - normal operation
  output(0x0b, 0x07, pin_num); //scan limit register - display digits 0 thru 7
  output(0x0a, 0x0f, pin_num); //intensity register - max brightness
  output(0x09, 0xff, pin_num); //decode mode register - CodeB decode all digits

}

long kk = 0;
void loop() {

  showNUMBER(kk , 0);
  showNUMBER(kk , 1);
  showNUMBER(kk , 2);
  showNUMBER(kk , 3);
  showNUMBER(kk , 4);
  kk++;
  delay(1000);
}



void showNUMBER(long number , int pin_num)
{

  String str = make_str((String)number);
  output(0x0c, 0x1, pin_num); //shutdown register - normal operation
  output(0x0b, 0x07, pin_num);


  for (int i = 0 ; i < 8 ; i++) {
    if (str.charAt(i) == te[pin_num][i])
    {
      
    }
    else
    {
      if (str.charAt(i) == ' ')
      {
        output(xx[8 - i], 0b01111111 , pin_num); //digit 7 (leftmost digit) data
      }
      else
      {
        output(xx[8 - i], xx[((String)str.charAt(i)).toInt()] , pin_num); //digit 7 (leftmost digit) data
      }
      te[pin_num][i] = str.charAt(i);
    }

  }

  output(0x0c, 0x0, pin_num); //shutdown register - normal operation
  output(0x0c, 0x1, pin_num);
}

String make_str(String str) {
  int len = 8 - str.length();
  for (int i = 0; i < (len); i++) {
    str = ' ' + str;
  }
  return str;
}
