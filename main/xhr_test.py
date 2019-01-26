import requests
import time
import numpy as np

# dictToSend = {'question':'what is the answer?'}
# res = requests.post('http://localhost:5000/task1', json=dictToSend)
# print 'response from server:',res.text
# dictFromServer = res.json()

def str_to_int16(str):
    return np.int16(int(str, 16))

def aY_to_x(aY):
    return np.float32(aY) / np.float32(0x4000)

def aX_to_y(aX):
    return -aY_to_x(aX)

# for arctan(y,x), use np.arctan2()

def radians_to_degrees(radVal):
    if radVal < 0:
        return np.uint16(180.0*(radVal/np.pi + 2))
    else:
        return np.uint16(180.0*radVal/np.pi)

def accelVal_to_degrees(aX, aY):
    return radians_to_degrees(
            np.arctan2(aX_to_y(str_to_int16(aX)),
                       aY_to_x(str_to_int16(aY))))

def normalize_degree_for_servo(degree):
    if degree > 270:
        return 0
    elif degree > 180:
        return 180
    else:
        return degree

def get_accel():
    res = requests.post('http://192.168.0.192:80/accel')
    my_list = res.text.split(",")
    #<debug>
    print res.text
    #</debug>
    return my_list

def set_servo(deg_val):
    res = requests.post('http://192.168.0.192:80/servo?deg_val=' + str(deg_val))
    #<debug>
    print res.text
    #</debug>

def run_task():
    while True:
        my_list = get_accel()
        deg_val = normalize_degree_for_servo(
            accelVal_to_degrees(my_list[0], my_list[1])
        )
        print deg_val
        set_servo(deg_val)
        time.sleep(0.1)
