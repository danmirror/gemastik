
import serial
ser = serial.Serial('/dev/ttyACM0', 115200)

while True:
    # data =input("masukan nilai")
    # ser.write(data.encode()) 
    inputFile = ser.readline()
    st =inputFile.decode()
    st = st[:-2]
    list_ = st.split (",")
    print(list_)