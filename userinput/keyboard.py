from pymouse import PyMouse
from pykeyboard import PyKeyboard
import time
m = PyMouse()
k = PyKeyboard()
# m.drag(400, 400)

# for i in range(x_dim):
# 	for j in range(y_dim):
# 		time.sleep(0.1)
# 		m.move(i, j)
# m.scroll(vertical=1)
# time.sleep(1)
# m.scroll(vertical=1)
# m.click(x_dim/2, y_dim/2, 1)
# k.type_string('Hello, World!')
# # pressing a key
# k.press_key('H')
# # which you then follow with a release of the key
# k.release_key('H')
# # or you can 'tap' a key which does both
# k.tap_key('e')
# # note that that tap_key does support a way of repeating keystrokes with a interval time between each
# k.tap_key('l',n=2,interval=5)
# # and you can send a string if needed too
# k.type_string('o World!')

def alt_tab():
	k.press_key(k.alt_key)
	k.tap_key(k.tab_key)
	k.release_key(k.alt_key)
	time.sleep(1)
def alt_f4():
	k.press_key(k.alt_key)
	k.tap_key(k.function_keys[4])
	k.release_key(k.alt_key)

# k.tap_key('space')
# k.tap_key(k.page_down_key)
# time.sleep(2)
# k.tap_key(k.page_up_key)
x_dim, y_dim = m.screen_size()
alt_tab()
k.tap_key(k.page_up_key)
time.sleep(0.5)
k.tap_key(k.page_up_key)
time.sleep(1)
print x_dim, y_dim
# m.click(x_dim / 2, y_dim / 2)
time.sleep(0.5)
m.scroll(vertical=10)
time.sleep(0.5)
m.scroll(vertical=-10)
time.sleep(1)