# キー押下を監視する
# エンターキーかエンターキー以外かを判断し
# PCに接続しているArduinoに送信する

# 参考ページ
# https://ameblo.jp/kabapython/entry-12786950485.html

import sendKeyInfo
from pynput import keyboard

# 参考ページ
# https://ameblo.jp/kabapython/entry-12786950485.html

# キーを押した時のイベント
def on_press(key):
    try:
        # at1.NonEnterKeyPush()
        # エンターキーの場合
        if key == keyboard.Key.enter:
            at1.EnterKeyPush()
        
        # エンターキー以外の場合
        else:
            at1.NonEnterKeyPush()
        
    except AttributeError:
        print('special key pressed: {0}'.format(key))

# キーを離した時のイベント
def on_release(key):
   pass

# Arduino用のクラスを作成する
# 引数：ポート名
# デバイス1つで動aasd作させる場合は、at2を削除してください
# with sendKeyInfo.sendKeyInfo("/dev/tty.usbmodem14101") as at1:
at1 = sendKeyInfo.sendKeyInfo("/dev/tty.usbserial-1420")

    # Collect events until released
with keyboard.Listener( on_press=on_press, on_release=on_release) as listener:
    listener.join()