# バックグラウンドでキー押下監視
# エンターキー：1
# エンターキー以外：2

# 参考ページ
# https://ameblo.jp/kabapython/entry-12786950485.html

from pynput import keyboard

def on_press(key):
   try:
      if key == keyboard.Key.enter:
         print("1")
      else:
         print("2")
         # print('Alphanumeric key pressed: {0} '.format(
         #    key.char))
   except AttributeError:
      print('special key pressed: {0}'.format(key))

def on_release(key):
   pass
   #  print('Key released: {0}'.format(
   #      key))
   #  if key == keyboard.Key.esc:
   #      # Stop listener
   #      return False

# Collect events until released
with keyboard.Listener( on_press=on_press, on_release=on_release) as listener:
   listener.join()   
