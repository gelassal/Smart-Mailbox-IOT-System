import cv2
import os
import smtplib
import threading
import time
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.mime.base import MIMEBase
from email import encoders
from picamera2 import Picamera2

# Email setup
email_user = "smartlockbox1@gmail.com"
email_password = "qwhd sofk nnkx glha"  # Use an App Password if 2FA is enabled
email_send = "smartlockbox1@gmail.com"

# Function to send email notification asynchronously with an image attachment
def send_email_notification(image_path):
    msg = MIMEMultipart()
    msg["From"] = email_user
    msg["To"] = email_send
    msg["Subject"] = "Person Detected"

    body = "Hi there, a person has been detected by the Raspberry Pi camera!"
    msg.attach(MIMEText(body, "plain"))

    # Attach the image file
    with open(image_path, "rb") as attachment:
        part = MIMEBase("application", "octet-stream")
        part.set_payload(attachment.read())
        encoders.encode_base64(part)
        part.add_header("Content-Disposition", f"attachment; filename= {os.path.basename(image_path)}")
        msg.attach(part)

    text = msg.as_string()
    try:
        server = smtplib.SMTP("smtp.gmail.com", 587)
        server.starttls()
        server.login(email_user, email_password)
        server.sendmail(email_user, email_send, text)
        server.quit()
        print("Email sent: Person Detected with Image")
    except Exception as e:
        print(f"Failed to send email: {e}")

# Object detection setup
users = [os.getlogin()]
classNames = []
classFile = f"/home/{users[0]}/Desktop/Object_Detection_Files/coco.names"
with open(classFile, "rt") as f:
    classNames = f.read().rstrip("\n").split("\n")

configPath = f"/home/{users[0]}/Desktop/Object_Detection_Files/ssd_mobilenet_v3_large_coco_2020_01_14.pbtxt"
weightsPath = f"/home/{users[0]}/Desktop/Object_Detection_Files/frozen_inference_graph.pb"

# Load detection model
net = cv2.dnn_DetectionModel(weightsPath, configPath)
net.setInputSize(320, 320)
net.setInputScale(1.0 / 127.5)
net.setInputMean((127.5, 127.5, 127.5))
net.setInputSwapRB(True)

# Object detection function
def getObjects(img, thres, nms, draw=True, objects=[]):
    classIds, confs, bbox = net.detect(img, confThreshold=thres, nmsThreshold=nms)
    if len(objects) == 0: objects = classNames
    objectInfo = []
    if len(classIds) != 0:
        for classId, confidence, box in zip(classIds.flatten(), confs.flatten(), bbox):
            className = classNames[classId - 1]
            if className in objects:
                objectInfo.append([box, className, confidence])
                if draw:
                    cv2.rectangle(img, box, color=(0, 255, 0), thickness=2)
                    cv2.putText(img, className.upper(), (box[0] + 10, box[1] + 30),
                                cv2.FONT_HERSHEY_COMPLEX, 1, (0, 255, 0), 2)
                    cv2.putText(img, str(round(confidence * 100, 2)), (box[0] + 200, box[1] + 30),
                                cv2.FONT_HERSHEY_COMPLEX, 1, (0, 255, 0), 2)
    return img, objectInfo

# Main program
if __name__ == "__main__":
    # Start Pi camera
    picam2 = Picamera2()
    picam2.configure(picam2.create_preview_configuration(main={"format": 'XRGB8888', "size": (640, 480)}))
    picam2.start()

    last_email_time = 0  # Track the time of the last email sent
    email_interval = 60  # One minute interval in seconds

    # Start detection loop
    while True:
        img = picam2.capture_array("main")
        img = cv2.cvtColor(img, cv2.COLOR_BGRA2BGR)
        result, objectInfo = getObjects(img, 0.45, 0.2, objects=['person'])

        # Send email if a person is detected with confidence >= 0.75 and the time since last email is greater than interval
        current_time = time.time()
        for box, obj, confidence in objectInfo:
            if obj == "person" and confidence >= 0.73 and (current_time - last_email_time > email_interval):
                # Save the captured image
                image_path = "/home/nathan/Desktop/Object_Detection_Files/detected_person.jpg"
                cv2.imwrite(image_path, img)

                # Start a new thread to send the email asynchronously with the captured image
                email_thread = threading.Thread(target=send_email_notification, args=(image_path,))
                email_thread.start()
                last_email_time = current_time  # Update last email time
                break  # Only send one email per detection loop if conditions are met

        # Display the video output
        cv2.imshow("Output", img)
        if cv2.waitKey(50) == 27:  # Exit on 'Esc' key press
            picam2.stop()
            cv2.destroyAllWindows()
            break
