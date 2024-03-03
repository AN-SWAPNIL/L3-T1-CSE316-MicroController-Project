import cv2
import numpy as np
import urllib.request
from pyfirmata import Arduino
import time
#
# board = Arduino('COM5')
# pin_4 = board.get_pin('d:4:o')
# pin_7 = board.get_pin('d:7:o')
# pin_8 = board.get_pin('d:8:o')

import serial.tools.list_ports

serialIns = serial.Serial()

# val = input("Select Port: ")

serialIns.baudrate = 9600
serialIns.port = 'COM3'
serialIns.open()


url = 'http://192.168.75.160/cam-hi.jpg'

cap = cv2.VideoCapture(url)
whT = 320
confThreshold = 0.5
nmsThreshold = 0.3
classesfile = 'coco.names'
classNames = []
with open(classesfile, 'rt') as f:
    classNames = f.read().rstrip('\n').split('\n')
# print(classNames)

modelConfig = 'yolov3.cfg'
modelWeights = 'yolov3.weights'
net = cv2.dnn.readNetFromDarknet(modelConfig, modelWeights)
net.setPreferableBackend(cv2.dnn.DNN_BACKEND_OPENCV)
net.setPreferableTarget(cv2.dnn.DNN_TARGET_CPU)

prevCommand = 'nothing'

def findObject(outputs, im, prevCommand):
    #print("called")
    hT, wT, cT = im.shape
    bbox = []
    classIds = []
    confs = []
    found_orange = False
    found_apple = False
    found_banana = False
    for output in outputs:
        for det in output:
            scores = det[5:]
            classId = np.argmax(scores)
            confidence = scores[classId]
            if confidence > confThreshold:
                w, h = int(det[2] * wT), int(det[3] * hT)
                x, y = int((det[0] * wT) - w / 2), int((det[1] * hT) - h / 2)
                bbox.append([x, y, w, h])
                classIds.append(classId)
                confs.append(float(confidence))
    # print(len(bbox))
    indices = cv2.dnn.NMSBoxes(bbox, confs, confThreshold, nmsThreshold)
    #print(indices)
    command = "nothing"
    for i in indices:
        #i = i[0]
        box = bbox[i]
        x, y, w, h = box[0], box[1], box[2], box[3]
        # if classNames[classIds[i]] == 'orange':
        #     found_orange = True
        # elif classNames[classIds[i]] == 'apple':
        #     found_apple = True
        # elif classNames[classIds[i]] == 'banana':
        #     found_banana = True

        if classNames[classIds[i]] == 'orange':
            cv2.rectangle(im, (x, y), (x + w, y + h), (255, 0, 255), 2)
            cv2.putText(im, f'{classNames[classIds[i]].upper()} {int(confs[i] * 100)}%', (x, y - 10),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 0, 255), 2)
            print('orange')
            # print(found_orange)
            # serialIns.write("orange".encode('utf-8'))
            # pin_4.write(0)
            # pin_7.write(0)
            # pin_8.write(1)
            command = 'orange'

        elif classNames[classIds[i]] == 'apple':
            cv2.rectangle(im, (x, y), (x + w, y + h), (255, 0, 255), 2)
            cv2.putText(im, f'{classNames[classIds[i]].upper()} {int(confs[i] * 100)}%', (x, y - 10),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 0, 255), 2)
            print('apple')
            command = 'apple'


        elif classNames[classIds[i]] == 'banana':
            cv2.rectangle(im, (x, y), (x + w, y + h), (255, 0, 255), 2)
            cv2.putText(im, f'{classNames[classIds[i]].upper()} {int(confs[i] * 100)}%', (x, y - 10),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 0, 255), 2)
            print('banana')
            command = 'banana'

    if command != prevCommand and command != 'nothing':
        print("Sending ", command)
        serialIns.write(command.encode('utf-8'))
        cv2.waitKey(3000)


    return command




while True:
    img_resp = urllib.request.urlopen(url)
    imgnp = np.array(bytearray(img_resp.read()), dtype=np.uint8)
    im = cv2.imdecode(imgnp, -1)
    sucess, img = cap.read()
    blob = cv2.dnn.blobFromImage(im, 1 / 255, (whT, whT), [0, 0, 0], 1, crop=False)
    net.setInput(blob)
    layernames = net.getLayerNames()
    #print(layernames)
    #print(len(layernames))
    #print(layernames[net.getUnconnectedOutLayers()[0] - 1])
    #print(layernames[net.getUnconnectedOutLayers()[1] - 1])
    #print(layernames[net.getUnconnectedOutLayers()[2] - 1])
    #outputNames = [layernames[i[0] - 1] for i in net.getUnconnectedOutLayers()]

    outputNames = [layernames[net.getUnconnectedOutLayers()[0] - 1],layernames[net.getUnconnectedOutLayers()[1] - 1],layernames[net.getUnconnectedOutLayers()[2] - 1]]
    #print(outputNames)
    #print(net.getUnconnectedOutLayers())
    outputs = net.forward(outputNames)
    # print(outputs[0].shape)
    # print(outputs[1].shape)
    # print(outputs[2].shape)
    # print(outputs[0][0])
    prevCommand = findObject(outputs, im, prevCommand)

    cv2.imshow('IMage', im)
    cv2.waitKey(100)
