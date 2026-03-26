from flask import Flask, request, jsonify, make_response
from flask_cors import CORS
import serial

app = Flask(__name__)

# configure the serial prot 
ser = serial.Serial(
    port='/dev/serial0', 
    baudrate=19200,
    timeout=1
)

CORS(app, resources={r"/*": {"origins": ["http://bean2-env.eba-mytp8mwb.us-east-1.elasticbeanstalk.com", "http://localhost:3000"]}})

#Define a route for a GET request
@app.route('/', methods=['GET'])
def home():
    return jsonify({}), 200

@app.route('/data/<string:parameter>', methods=['GET'])
def get_data(parameter):
    # Use the path parameter in your logic
    if parameter == "1":
        send_data("1")
    elif parameter == "2":
        send_data("2")
    data = {
        "message": f"You provided the parameter: {parameter}",
        "data": parameter
    }
    return jsonify({}), 200

def send_data(data):
    ser.write(data.encode('utf-8'))
    print(f"sent: {data}")

def recieve_data():
    return "1"

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
