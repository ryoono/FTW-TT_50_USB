
#define KEY_NULL      '-'
#define ENTER_KEY     '0'
#define NON_ENTER_KEY '1'

#define SOL_PIN_1     4
#define SOL_PIN_2     5
#define SOL_BELL_PIN  6
#define LED_PIN       13

#define SOL_ON_TIME   40  // ソレノイドのON時間[msec]
#define SOL_OFF_TIME  40  // ソレノイドのOFF時間[msec]

bool isLedOn;

// 0:OFF, 1:SOL_PIN_1 ON, 2:SOL_PIN_2 ON, 3:SOL_BELL_PIN ON
unsigned char solOnState;
unsigned char solOnStateBuf;  // 前回値
unsigned char solPinCnt;      // 一般キー用ソレノイド動作回数
int solOnRestTime;            // ソレノイドON時間計測用タイマ
// 0:無し, 1:一般キー, 2:エンターキー
unsigned char solOnBuff;

void setup() {

  solOnState = 0;
  solOnStateBuf = solOnState;
  solPinCnt = 0;
  solOnRestTime = 0;
  solOnBuff = 0;
  
  Serial.begin( 115200 );
  pinMode(    SOL_PIN_1, OUTPUT);
  pinMode(    SOL_PIN_2, OUTPUT);
  pinMode( SOL_BELL_PIN, OUTPUT);
  pinMode(      LED_PIN, OUTPUT);

  digitalWrite(    SOL_PIN_1, LOW);
  digitalWrite(    SOL_PIN_2, LOW);
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
      solOnBuff = 1;
    }
    // 一般キー
    else if( (buff == NON_ENTER_KEY) && (solOnBuff == 0) ){
      solOnBuff = 2;
    }
  }

  // 全部のソレノイドが解放されている場合に実行
  if( !solOnState ){
    // エンターキーをセット
    // エンターキーを連続で叩くことは無いと想定しているため，
    // 連続で叩かれると最初の1回しかベルが鳴らない
    if( solOnBuff == 1 ){
      solOnState = 3;
      solOnBuff = 0;
    }
    // 一般キーをセット
    // 40msecのOFFタイムを減らすために，2つのソレノイドを交互に動作させている
    else if( solOnBuff == 2 ){
      solOnState = (solPinCnt++ % 2) + 1;
      solOnBuff = 0;
    }
  }

  // ソレノイドON時間タイマセット
  // 初回1回目のみセット
  if( (solOnStateBuf == 0) && (solOnState != 0) ){
    solOnRestTime = SOL_ON_TIME;
    isLedOn = !isLedOn;
  }

  if( solOnRestTime != 0 ){
    if( solOnState == 1 ){
      digitalWrite(    SOL_PIN_2,  LOW);
      digitalWrite( SOL_BELL_PIN,  LOW);
      digitalWrite(    SOL_PIN_1, HIGH);
    }
    else if( solOnState == 2 ){
      digitalWrite(    SOL_PIN_1,  LOW);
      digitalWrite( SOL_BELL_PIN,  LOW);
      digitalWrite(    SOL_PIN_2, HIGH);
    }
    else if( solOnState == 3 ){
      digitalWrite(    SOL_PIN_1,  LOW);
      digitalWrite(    SOL_PIN_2,  LOW);
      digitalWrite( SOL_BELL_PIN, HIGH);
    }

    // カウンタの更新
    if( !(--solOnRestTime) ){
      solOnState = 0;
    }
  }
  else{
    digitalWrite(    SOL_PIN_1, LOW);
    digitalWrite(    SOL_PIN_2, LOW);
    digitalWrite( SOL_BELL_PIN, LOW);
  }

  // 前回値更新
  solOnStateBuf = solOnState;

  // LED点灯(ソレノイド動作ごとに点滅)
  if( isLedOn ) digitalWrite( LED_PIN, HIGH);
  else          digitalWrite( LED_PIN,  LOW);
  
  // 1msecごとに実行する
  delay( 1 );
}
