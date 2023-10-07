# FTW-TT_50_USB  

<span style="color: red; ">F</span>ace <span style="color: red; ">T</span>ype <span style="color: red; ">W</span>riter - <span style="color: red; ">T</span>etora <span style="color: red; ">T</span>ech lab _ <span style="color: red; ">5</span>.<span style="color: red; ">0</span>v _ <span style="color: red; ">USB</span> drive  
テトラ技研製 USB駆動 ニセタイプライター  

## 概要  
現代の消音キーボードに反抗すべく，タイプライターを叩いた時の音を楽しめるデバイスを作成する．  
タイプライター風キーボードも売ってはいるが，それを使っては味気ないので，外付けUSBデバイスとして実装する．  
当初，リレーで音を出そうと考えていたが，よくよく調べてみると二番煎じだった．  
そのため，急遽ソレノイドで音を出す仕様に変更した．．．  
[Silriumさん_USB KeyBoard RELAY](https://makezine.jp/blog/2015/08/mft2015_silrium.html)

## ハードウェア設計  
### 構成  
### 回路図  
足りない部品があるので，後で修正
* picマイコンのピン番号追記(現物合わせで決める)  
* Vddに電解コンデンサ追加  
* Vusb(？)みたいなピンに積層セラミックコンデンサ追加  
![schmatic](./資料/schematic.png)  

## ソフトウェア設計  
### PC側  
### デバイス側