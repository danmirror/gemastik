#--------------------------
#   author = Danu andrean
#   create = mey,19
#   title  = object tracking

#---------------------------------------
#   done
#
#-----------------------------------

import cv2
import numpy as np
import requests

import serial
ser = serial.Serial('/dev/ttyACM0', 115200)

def callback(x):
    pass

cap = cv2.VideoCapture(0)
cv2.namedWindow('image',cv2.WINDOW_NORMAL)


ilowH = 131
ilowS = 104
ilowV = 64
ihighH = 185
ihighS = 255
ihighV = 163

frame_w = 440
frame_h = 280

min_obj = 10*10
max_obj = frame_w*frame_h/1.5

#===================set variable ===================
count = 0

#===================================================
# create trackbars for color change

cv2.createTrackbar('lowH','image',ilowH,255,callback)
cv2.createTrackbar('lowS','image',ilowS,255,callback)
cv2.createTrackbar('lowV','image',ilowV,255,callback)
cv2.createTrackbar('highH','image',ihighH,255,callback)
cv2.createTrackbar('highS','image',ihighS,255,callback)
cv2.createTrackbar('highV','image',ihighV,255,callback)



kernelOpen=np.ones((10,10))
kernelClose=np.ones((20,20))
#font=cv2.cv.InitFont(cv2.cv.CV_FONT_HERSHEY_SIMPLEX,2,0.5,0,3,1)
def send(humadity,temp,soil,ph,status):

    data = "data1="+humadity+"&data2="+temp+"&data3="+soil+"&data4="+ph+"&data5="+status

    url = "http://127.0.0.1/connect.php?"+data

    payload = {}
    headers = {}

    response = requests.request("POST", url, headers=headers)

    print(response.text.encode('utf8'))

while True:
    
    val_send = '0'
    ret, frame=cap.read()
    frame = cv2.flip(frame,1)
    #frame=cv2.resize(frame,(340,220))

    ilowH = cv2.getTrackbarPos('lowH', 'image')
    ilowS = cv2.getTrackbarPos('lowS', 'image')
    ilowV = cv2.getTrackbarPos('lowV', 'image')
    ihighH = cv2.getTrackbarPos('highH', 'image')
    ihighS = cv2.getTrackbarPos('highS', 'image')
    ihighV = cv2.getTrackbarPos('highV', 'image')


    #convert BGR to HSV
    frameHSV= cv2.cvtColor(frame,cv2.COLOR_BGR2HSV)
    #----------------------------------------------------
    lower_hsv = np.array([ilowH, ilowS, ilowV])
    higher_hsv = np.array([ihighH, ihighS, ihighV])
    # create the Mask
    mask=cv2.inRange(frameHSV,lower_hsv,higher_hsv)
    #morphology
    maskOpen=cv2.morphologyEx(mask,cv2.MORPH_OPEN,kernelOpen)
    maskClose=cv2.morphologyEx(maskOpen,cv2.MORPH_CLOSE,kernelClose)

    maskFinal=maskClose
    conts,h=cv2.findContours(maskFinal.copy(),cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)

    #print conts
    for countour in conts:
        area = cv2.contourArea(countour)
        #print "area",area

    
        #cv2.drawContours(frame,conts,-1,(255,0,0),3)
        if ((area <max_obj) & (area >min_obj)):         
            objectFound = 1
            count +=1
            # print(count)
            cv2.drawContours(frame,conts,-1,(255,0,0),3)
            
            for i in range(len(conts)):
                x,y,w,h=cv2.boundingRect(conts[i])
                cv2.rectangle(frame,(x,y),(x+w,y+h),(255,0,0), 2)
                cv2.putText(frame,str(area),(x, y - 20), cv2.FONT_HERSHEY_COMPLEX, 1 ,(0,0,255), 2)
                if (count>=1):
                    val_send = '1';
                    cv2.putText(frame,'Object Found',(50,50), cv2.FONT_HERSHEY_COMPLEX, 2 ,(0,255,0), 2)        
        else:
            count =0
            val_send = '0';
            # for i in range(len(conts)):
            #     x,y,w,h=cv2.boundingRect(conts[i])
            #     cv2.rectangle(frame,(x,y),(x+w,y+h),(0,0,255), 2)
   
    inputFile = ser.readline()
    st =inputFile.decode()
    st = st[:-2]
    list_ = st.split (",")
    # send(list_[0],list_[1],list_[2],list_[4],val_send)
    print(list_)
    print(val_send)
    cv2.imshow("mask",mask)
    cv2.imshow("cam",frame)
    cv2.waitKey(10)
    #print ilowH, ilowS, ilowV,ihighH,ihighS,ihighV
    if(cv2.waitKey(1) & 0xFF == ord('q')):
        break


cv2.destroyAllWindows()
cap.release()