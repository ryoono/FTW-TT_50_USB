
#define MOVE_SINGLE   0 // 単発稼働
#define MOVE_CONTINUE 1 // 連続稼働

bool startFlag; // 稼働状態 false:停止 / true:起動
char startType; // 稼働タイプ
int  onTime;    // ソレノイドのON時間
int  offTime;   // ソレノイドのOFF時間

void setup() {

  startFlag = false;        // 停止に指定
  startType = MOVE_SINGLE;  // 単発に指定

  onTime  = 1000;
  offTime = 1000;

  pinMode( 13, OUTPUT);
  pinMode(  4, OUTPUT);
  digitalWrite( 13, LOW);   // 眩しいからLED消しておく
  digitalWrite(  4, LOW);

  Serial.begin( 9600 );
}

void loop() {

  // 何らかのデータを受信した場合の処理
  if ( Serial.available() > 0 ) {
    String dt = Serial.readStringUntil('\n');

    // ##################################
    // ####---- コマンド受信時の処理 ----####
    
    // ソレノイドのON時間を指定するコマンド受信時
    if( dt.indexOf("on") == 0 ){
      String num_s = dt.substring( 3, dt.length());
      
    }
    // ソレノイドのOFF時間を指定するコマンド受信時
    else if( dt.indexOf("off") == 0 ){
      String num_s = dt.substring( 4, dt.length());
    }
    // 連続稼働コマンド受信時
    else if( dt.compareTo("continue") == 0 ){
      startType = MOVE_CONTINUE;
    }
    // 単発稼働コマンド受信時
    else if( dt.compareTo("single") == 0 ){
      startType = MOVE_SINGLE;
    }
    // 開始コマンド受信時
    else if( dt.compareTo("start") == 0 ){
      startFlag = true;
    }
    // 停止コマンド受信時
    else if( dt.compareTo("s") == 0 ){
      startFlag = false;
    }
  }

  digitalWrite( 4, LOW);
  delay(40);
  digitalWrite( 4, HIGH);
  delay(40);
}
