
#define MOVE_SINGLE   0 // 単発稼働
#define MOVE_CONTINUE 1 // 連続稼働

#define SOL_PIN 4 // ソレノイド制御用ピン番号

bool startFlag; // 稼働状態 false:停止 / true:起動
char startType; // 稼働タイプ
int  onTime;    // ソレノイドのON時間[ms]
int  offTime;   // ソレノイドのOFF時間[ms]

void setup() {

  startFlag = false;        // 停止に指定
  startType = MOVE_SINGLE;  // 単発に指定

  onTime  = 1000; // 初期値は1000[ms]
  offTime = 1000;

  pinMode( SOL_PIN, OUTPUT);
  digitalWrite( SOL_PIN, LOW);

  Serial.begin( 9600 );
  Serial.println("コマンド詳細はREADME.mdを参照してください");
  Serial.println("例外処理など入れてないため，コマンドを間違えないように注意してください");
}

void loop() {

  // 何らかのデータを受信した場合の処理
  if ( Serial.available() > 0 ) {
    // 終端文字によってここは変更．
    // macだとCR+LFのため，CRまでで切る
    String command = Serial.readStringUntil('\r');

    // ##################################
    // ####---- コマンド受信時の処理 ----####
    
    // ソレノイドのON時間を指定するコマンド受信時
    if( command.indexOf("on ") == 0 ){
      String onTime_s = command.substring( 3, command.length());
      onTime = onTime_s.toInt();
    }
    // ソレノイドのOFF時間を指定するコマンド受信時
    else if( command.indexOf("off ") == 0 ){
      String offTime_s = command.substring( 4, command.length());
      offTime = offTime_s.toInt();
    }
    // 連続稼働コマンド受信時
    else if( command.compareTo("continue") == 0 ){
      startType = MOVE_CONTINUE;
    }
    // 単発稼働コマンド受信時
    else if( command.compareTo("single") == 0 ){
      startType = MOVE_SINGLE;
    }
    // 開始コマンド受信時
    else if( command.compareTo("start") == 0 ){
      startFlag = true;
    }
    // 停止コマンド受信時
    else if( command.compareTo("s") == 0 ){
      startFlag = false;
    }
  }

  // #################################
  // ####---- ソレノイド稼働処理 ----####
  
  // startFlagがtrueならばソレノイドを稼働する
  // startTypeが単発稼働ならばstartFlagをfalseにする
  if( startFlag == true ){
    
    digitalWrite( SOL_PIN, HIGH);
    delay(onTime);
    digitalWrite( SOL_PIN, LOW);
    delay(offTime); 
    
    if( startType != MOVE_CONTINUE ){
      startFlag = false;
    }
  }
}
