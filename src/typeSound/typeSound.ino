
#define KEY_NULL      '-'
#define ENTER_KEY     '0'
#define NON_ENTER_KEY '1'

#define SOL_PIN_1     5
#define SOL_PIN_2     6
#define SOL_BELL_PIN  4
#define SW_PIN        A4
#define LED_PIN       13

#define SOL_ON_TIME         40  // ソレノイドのON時間(一般キー)[msec]
#define SOL_ENTER_ON_TIME   35  // ソレノイドのON時間(エンターキー)[msec]
#define SOL_OFF_TIME        40  // ソレノイドのOFF時間[msec]

#define SW_EN HIGH  // スイッチ状態_英語
#define SW_JP  LOW  // スイッチ状態_日本

bool ledState;
int  swState; // SWの状態

// 0:OFF, 1:SOL_PIN_1 ON, 2:SOL_PIN_2 ON, 3:SOL_BELL_PIN ON
unsigned char solOnState;
unsigned char solPinCnt;      // 一般キー用ソレノイド動作回数
int solOnRestTime;            // ソレノイドON時間計測用タイマ
// 0:無し, 1:エンターキー, 2:一般キー
unsigned char solOnBuff;

void setup() {

  solOnState = 0;
  solPinCnt = 0;
  solOnRestTime = 0;
  solOnBuff = 0;
  
  pinMode(    SOL_PIN_1, OUTPUT);
  pinMode(    SOL_PIN_2, OUTPUT);
  pinMode( SOL_BELL_PIN, OUTPUT);
  pinMode(      LED_PIN, OUTPUT);
  pinMode(            1,  INPUT); // Vccが当たりそうだから入力ピンにしておく
  pinMode(       SW_PIN, INPUT_PULLUP);

  digitalWrite(    SOL_PIN_1, LOW);
  digitalWrite(    SOL_PIN_2, LOW);
  digitalWrite( SOL_BELL_PIN, LOW);
  digitalWrite(      LED_PIN, LOW);
  
  ledState = true;
  swState = digitalRead( SW_PIN );

  Serial.begin( 115200 );
}

void loop() { 

  swState = digitalRead( SW_PIN );
  
  // シリアル受信した場合
  while( Serial.available() > 0 ){
    char buff;
    buff = Serial.read();

    Serial.println( buff );

    if( swState == SW_EN ){
      // エンターキーを優先する
      if( buff == ENTER_KEY ){
        solOnBuff = 1;
      }
      // 一般キー
      else if( (buff == NON_ENTER_KEY) && (solOnBuff == 0) ){
        solOnBuff = 2;
      }
    }
    else{
      // 一般キー
      if( (buff == ENTER_KEY) || (buff == NON_ENTER_KEY) ){
        solOnBuff = 2;
      }
    }
  }

  // 全部のソレノイドが解放されている場合に実行
  if( !solOnState ){
    // エンターキーをセット
    // エンターキーを連続で叩くことは無いと想定しているため，
    // 連続で叩かれると最初の1回しかベルが鳴らない
    if( solOnBuff == 1 ){
      solOnRestTime = SOL_ENTER_ON_TIME;
      solOnState = 3;
      solOnBuff = 0;
      ledState = !ledState;
    }
    // 一般キーをセット
    // 40msecのOFFタイムを減らすために，2つのソレノイドを交互に動作させている
    else if( solOnBuff == 2 ){
      solOnRestTime = SOL_ON_TIME;
      solOnState = (solPinCnt++ % 2) + 1;
      solOnBuff = 0;
      ledState = !ledState;
    }
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

  // LED点灯(ソレノイド動作ごとに点滅)
  if( ledState ) digitalWrite( LED_PIN, HIGH);
  else          digitalWrite( LED_PIN,  LOW);
  
  // 1msecごとに実行する
  delay( 1 );
}
