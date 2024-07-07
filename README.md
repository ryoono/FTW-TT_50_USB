# FTW-TT_50_USB  

<span style="color: red; ">F</span>ake <span style="color: red; ">T</span>ype <span style="color: red; ">W</span>riter - <span style="color: red; ">T</span>etora <span style="color: red; ">T</span>ech lab _ <span style="color: red; ">5</span>.<span style="color: red; ">0</span>v _ <span style="color: red; ">USB</span> drive  
テトラ技研製 USB駆動 ニセタイプライター  

![外観](./資料/PXL_20240114_082543204.TS_exported_4065~2.jpg)

**[紹介動画](https://youtu.be/UQrdH8_pX44?si=VVSs2FK_6RHxstvM)**

## 背景  
現代の消音キーボードに反抗すべく、タイプライターを叩いた時の音を楽しめるデバイスを作成する。  
タイプライター風キーボードも売ってはいるが、それを使っては味気ないので、外付けUSBデバイスとして実装する。  
当初、リレーで音を出そうと考えていたが、よくよく調べてみると二番煎じだった。  
そのため、急遽ソレノイドで音を出す仕様に変更した...   
[Silriumさん_USB KeyBoard RELAY](https://makezine.jp/blog/2015/08/mft2015_silrium.html)

## 概要  
キータイプ時にソレノイドを動作させ、タイプライターの音を表現する。  
音は2種類用意する。

| キー種類     | 音           | 備考                   | 
| ------------ | ------------ | ---------------------- | 
| 一般キー     | メカニカル音 | エンターキー以外のキー | 
| エンターキー | ベル音       |                        |  

メカニカル音はソレノイドの動作音で再現する。  
ベル音はタイプライターの文字送り催促ベルを指しており、自転車のベルで再現する。  

## ハードウェア設計  
### 構成  
一般キー押下時の音を再現するために、ソレノイドを接続する。  
タイピングの間隔が80msを下回ると、ソレノイドのデッドタイムとかぶるため、一般キー用のソレノイドを2つ接続して対策する。  
エンターキー押下時の音は、ベルを鳴らすことで対応する。  
なお、エンターキーの押下間隔は80msを下回らないと仮定して、ソレノイドは1つのみ接続する。  
![ハードウェア構成](./資料/ハードウェア構成.png) 

### 回路図  
小型化を狙いpicマイコンで実装しようとしたが、時間が無いのでArduinoを使用する。  
Arduino Microを使用して設計したが、電源回りが弱いようで5Vピンから5V出力されないため、Nanoを使用する。  
使用するソレノイドには、ON/OFF 40[ms]ずつ計80[ms]のデッドタイムがあり、一般キー押下タイミングによっては、反応が追い付かない。  
そのため、ソレノイドを2個設置し、40[ms]のデッドタイムとすることで対応する。  
なお、2個以上のソレノイドを同時に動作させることはUSB2.0の規格上、電流が大きすぎるため同時動作ソレノイドは1つまでとする。  
![schmatic](./資料/schematic_ArduinoNano.jpg)  

## ソフトウェア設計  
### PC側  
```mermaid
---
title: Fig. PC側処理_フローチャート
---

flowchart TD
    start1([開始])
    node0[Arduinoとの通信開始]
    node1[キーイベント監視開始]
    end1([終了])

    start2([キーイベント発生])
    node2[押下キー取得]
    switch1{押下キー}
    node3[Arduinoに'0'を送信]
    node4[Arduinoに'1'を送信]
    end2([終了])

    start1 --> node0
    node0 --> node1
    node1 --> end1

    start2 --> node2
    node2 --> switch1
    switch1 -- エンターキー --> node3
    switch1 -- エンターキー以外 --> node4
    node3 --> end2
    node4 --> end2
```

### デバイス側  
なぜ関数化しなかったのかが悔やまれるが、展示ギリギリだったので許してほしい。  

```mermaid
---
title: Fig. Arduino側処理_フローチャート
---

flowchart TD
    start1([開始])
    node0[シリアル受信チェック]
    switch0{受信有無}
    switch1{打鍵タイプ}
    switch2{受信キー}
    switch2_1{受信キー}
    node1[受信キーBuff = エンターキー]
    switch3{受信キーBuff}
    node2[受信キーBuff = 一般キー]
    node2_1[受信キーBuff = 一般キー]
    switch4{ソレノイド状態}
    end1([終了])

    start1 --> node0
    node0 --> switch0
    switch0 -- あり --> switch1
    switch1 -- 英語(1) --> switch2
    switch2 -- エンターキー('0') --> node1
    switch2 -- 一般キー('1') --> switch3
    switch3 -- なし(0) --> node2
    switch1 -- 日本語(0) --> switch2_1
    switch2_1 -- エンターキー('0') | 一般キー('1') --> node2_1
    node1 --> switch4
    node2 --> switch4
    switch2 -- それ以外--> switch4
    switch3 -- あり(!=0) --> switch4
    node2_1 --> switch4
    switch2_1 -- それ以外 --> switch4
    switch0 -- なし --> switch4

    switch10{受信キーBuff}
    node10[ソレノイドON時間設定 = 35ms]
    node11[ON中ソレノイドID = 3:エンターキー]
    node12[受信キーBuff初期化]

    node13[ソレノイドON時間設定 = 40ms]
    switch11{一般キーONソレノイド前回値}
    node14[ON中ソレノイドID = 1:一般キー]
    node14[ON中ソレノイドID = 2:一般キー]
    node15[受信キーBuff初期化]

    switch4 -- ON中(0以外) --> switch10
    switch10 -- エンターキー(1) --> node10
    node10 --> node11
    node11 --> node12
    switch10 -- 一般キー(0) --> node13
    node13 --> switch11
    switch11 -- 1 --> node14
    switch11 -- 2 --> node15

    switch20{ソレノイドON時間設定}
    switch21{ON中ソレノイドID}
    node20[ソレノイド1のみON]
    node21[ソレノイド2のみON]
    node22[ソレノイド3のみON]
    node23[ソレノイド全OFF]

    node30[1ms停止]

    node12 --> switch20
    node14 --> switch20
    node15 --> switch20
    switch20 -- 0以外 --> switch21
    switch21 -- 1:一般キー --> node20
    switch21 -- 2:一般キー --> node21
    switch21 -- 3:エンターキー --> node22
    switch20 -- 0 --> node23

    node20 --> node30
    node21 --> node30
    node22 --> node30
    node23 --> node30

    node30 --> end1
```