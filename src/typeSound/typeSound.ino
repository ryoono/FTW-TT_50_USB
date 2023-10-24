
#define KEY_NULL      'a'
#define ENTER_KEY     '0'
#define NON_ENTER_KEY '1'

#define SOL_PIN       4
#define SOL_BELL_PIN  5
#define LED_PIN       13

#define SOL_ON_TIME   40  // ソレノイドのON時間[msec]
#define SOL_OFF_TIME  40  // ソレノイドのOFF時間[msec]

bool isSerialReq;
char SerialReq;
bool isLedOn;

void setup() {

  isSerialReq = false;
  SerialReq = KEY_NULL;
  
  Serial.begin( 9600 );
  pinMode(      SOL_PIN, OUTPUT);
  pinMode( SOL_BELL_PIN, OUTPUT);
  pinMode(      LED_PIN, OUTPUT);

  digitalWrite(      SOL_PIN, LOW);
  digitalWrite( SOL_BELL_PIN, LOW);
  digitalWrite(      LED_PIN, LOW);
  isLedOn = false;
}

void loop() { 

  // シリアル受信した場合
  while( Serial.available() > 0 ){
    char buff;
    buff = Serial.read();

    Serial.println( buff );

    // エンターキーを優先する
    if( buff == ENTER_KEY ){
      SerialReq = buff;
      isSerialReq = true;
    }
    else if( (buff == NON_ENTER_KEY) && (SerialReq == KEY_NULL) ){
      SerialReq = buff;
      isSerialReq = true;
    }
  }

  // 有効データ受け取り状態ならば実行
  if( isSerialReq == true ){
    isSerialReq = false;

    if( SerialReq == ENTER_KEY ){
        digitalWrite( SOL_BELL_PIN, HIGH);
        delay( SOL_ON_TIME );
        digitalWrite( SOL_BELL_PIN, LOW);
    }
    else if( SerialReq == NON_ENTER_KEY ){
        digitalWrite( SOL_PIN, HIGH);
        delay( SOL_ON_TIME );
        digitalWrite( SOL_PIN, LOW);
    }

    SerialReq = KEY_NULL;
  }

//  isLedOn = !isLedOn;
//  if( isLedOn ) digitalWrite( LED_PIN, HIGH);
//  else          digitalWrite( LED_PIN,  LOW);
  
  // 1msecごとに実行する
  delay( SOL_OFF_TIME );
}
