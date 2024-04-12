import cv2
import imutils
import time
import serial
import datetime
######################
# initialize the serial connection with the arduino
ser = serial.Serial('COM8', 9600, timeout=0.001) # channel number, and the baud rate
if not ser.isOpen():
    ser.open()
######################
# define color ranges; adjust that to the actual ball color using an RGB map and convert the RGB to HSV formatt
whiteLower = (164, 230, 190)
whiteUpper = (190, 255, 230)

orangeLower = (10, 230, 230)
orangeUpper = (30, 255, 255)
######################
# define the color of the ball to be collected
# ideally, this would be sent through the application
# 1 for orange, 0 for white
color = 1
######################
# assign the color to be detected
if color == 1:
    positiveColorLower = orangeLower
    positiveColorUpper = orangeUpper
    negativeColorLower = whiteLower
    negativeColorUpper = whiteUpper
else:
    positiveColorLower = whiteLower
    positiveColorUpper = whiteUpper
    negativeColorLower = orangeLower
    negativeColorUpper = orangeUpper
######################
# initialize the camera
cap = cv2.VideoCapture(1)
state = -1
current_time = datetime.datetime.now()
last_time = datetime.datetime.now()
last_timeFlag = 0
while True:
    ret, frame = cap.read()
    if ret == False:
        break
    blurred = cv2.GaussianBlur(frame, (11, 11), 0)
    width, height = frame.shape[:2]
    hsvImage = cv2.cvtColor(blurred, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsvImage, positiveColorLower, positiveColorUpper) # inRange function specifies the lower and upper ranges of color we want to detect
    mask = cv2.erode(mask, None, iterations=2)
    mask = cv2.dilate(mask, None, iterations=2)
    cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    cnts = imutils.grab_contours(cnts)
    center = None
    if len(cnts) > 0:
        c = max(cnts, key=cv2.contourArea)
        ((x, y), radius) = cv2.minEnclosingCircle(c)
        print(x)
        print(y)
        M = cv2.moments(c)
        center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))
        # To see the centroid clearly
        if radius > 10:#  and x < 400 and y <  135:
            cv2.circle(frame, (int(x), int(y)), int(radius), (0, 255, 255), 5)
            cv2.imwrite("frame.png", cv2.resize(frame, (int(height / 2), int(width / 2))))
            cv2.circle(frame, center, 5, (0, 0, 255), -1)
        ######################
            print('positive ball detected')
            state = 1
            message = str(str(state)) + '\n' + '\r'
            ser.write(message.encode())
        ######################
    else:
        blurred = cv2.GaussianBlur(frame, (11, 11), 0)
        width, height = frame.shape[:2]
        hsvImage = cv2.cvtColor(blurred, cv2.COLOR_BGR2HSV)
        mask = cv2.inRange(hsvImage, negativeColorLower, negativeColorUpper)
        mask = cv2.erode(mask, None, iterations=2)
        mask = cv2.dilate(mask, None, iterations=2)
        cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        cnts = imutils.grab_contours(cnts)
        center = None
        if len(cnts) > 0:
            c = max(cnts, key=cv2.contourArea)
            ((x, y), radius) = cv2.minEnclosingCircle(c)
            M = cv2.moments(c)
            center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))

            # To see the centroid clearly
            if radius > 10:# and x < 500 and y < 135:
                cv2.circle(frame, (int(x), int(y)), int(radius), (0, 255, 255), 5)
                cv2.imwrite("frame.png", cv2.resize(frame, (int(height / 2), int(width / 2))))
                cv2.circle(frame, center, 5, (0, 0, 255), -1)
                print('negative ball detected')
                state = 0
                message = str(str(state)) + '\n' + '\r'
                ser.write(message.encode())
    cv2.imshow("Frame", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        ser.close()
        break

cap.release()
cv2.destroyAllWindows()
