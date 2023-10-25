# pip install pyserial
import serial

# シリアル通信でキー押下情報を送信するクラス
class sendKeyInfo():

    # クラス定数
    ENTER_KEY       = 0
    NON_ENTER_KEY   = 1

    # ポート名を指定し、ポートとの通信を開始する
    def __init__( self, PortName):
        self.PortName = PortName
        self.ActPort = serial.Serial( self.PortName, 115200)

    # ポートを閉じる
    def __del__( self ):
        self.ActPort.close()

    # エンターキー押下情報を送信する
    def EnterKeyPush( self ):
        self.DataTransmission( self.ENTER_KEY )

    # エンターキー以外押下情報を送信する
    def NonEnterKeyPush( self ):
        self.DataTransmission( self.NON_ENTER_KEY )

    # Arduinoへデータの送信
    # num = 0~9の整数
    def DataTransmission( self, num):

        # 諸事情により、1度文字列にしてからcharに変換し、送信する
        sumStr = str(num)
        self.ActPort.write( bytes( sumStr, 'utf-8') )
