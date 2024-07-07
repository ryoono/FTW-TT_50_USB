# FTW-TT_50_USB  

<span style="color: red; ">F</span>ake <span style="color: red; ">T</span>ype <span style="color: red; ">W</span>riter - <span style="color: red; ">T</span>etora <span style="color: red; ">T</span>ech lab _ <span style="color: red; ">5</span>.<span style="color: red; ">0</span>v _ <span style="color: red; ">USB</span> drive  
テトラ技研製 USB駆動 ニセタイプライター  

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
ここに構成(図)を記載する

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