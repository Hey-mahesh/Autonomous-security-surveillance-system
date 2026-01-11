from ultralytics import YOLO
import cv2
import numpy as np
import paho.mqtt.client as mqtt
import json
import time

# ========== MQTT CONFIG ==========
BROKER = "dev.coppercloud.in"
PORT = 1883
TOPIC = "mahesh/vision/data"

client = mqtt.Client()
client.connect(BROKER, PORT, 60)

# ========== Load YOLO ==========
model = YOLO("yolov8n.pt")

# ========== Camera ==========
cap = cv2.VideoCapture(0)

if not cap.isOpened():
    print("Camera not opened")
    exit()

ret, prev = cap.read()
prev = cv2.flip(prev, 1)
prev_gray = cv2.cvtColor(prev, cv2.COLOR_BGR2GRAY)
prev_gray = cv2.GaussianBlur(prev_gray, (21,21), 0)

last_send = 0

while True:
    ret, frame = cap.read()
    if not ret:
        break

    # ---- Mirror webcam ----
    frame = cv2.flip(frame, 1)

    # ---------- MOTION ----------
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    blur = cv2.GaussianBlur(gray, (21,21), 0)

    delta = cv2.absdiff(prev_gray, blur)
    thresh = cv2.threshold(delta, 25, 255, cv2.THRESH_BINARY)[1]
    thresh = cv2.dilate(thresh, None, iterations=2)

    contours, _ = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    motion_area = 0
    for c in contours:
        motion_area += cv2.contourArea(c)

    motion = motion_area > 1500
    prev_gray = blur

    # ---------- YOLO ----------
    results = model(frame, verbose=False)[0]

    people = []
    for box in results.boxes:
        cls = int(box.cls[0])
        conf = float(box.conf[0])
        if results.names[cls] == "person":
            people.append(conf)

    people_count = len(people)
    person_conf = round(max(people),2) if people_count > 0 else 0.0
    person_detected = people_count > 0

    # ---------- DRAW YOLO ----------
    for box in results.boxes:
        cls = int(box.cls[0])
        name = results.names[cls]
        conf = float(box.conf[0])

        if name == "person":
            x1,y1,x2,y2 = map(int, box.xyxy[0])
            cv2.rectangle(frame,(x1,y1),(x2,y2),(0,255,0),2)
            cv2.putText(frame,f"Person {conf:.2f}",(x1,y1-10),
                        cv2.FONT_HERSHEY_SIMPLEX,0.6,(0,255,0),2)

    # ---------- DATA ----------
    data = {
        "person_detected": person_detected,
        "person_confidence": person_conf,
        "people_count": people_count,
        "motion": motion,
        "motion_area": int(motion_area)
    }

    # ---------- MQTT ----------
    if time.time() - last_send > 0.5:
        client.publish(TOPIC, json.dumps(data))
        last_send = time.time()

    # ---------- MULTI-LINE OVERLAY ----------
    y = 25
    for key, value in data.items():
        text = f"{key}: {value}"
        cv2.putText(frame, text, (10, y),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 0, 255), 2)
        y += 25

    cv2.imshow("AI Vision Node", frame)

    if cv2.waitKey(1) == 27:
        break

cap.release()
cv2.destroyAllWindows()
