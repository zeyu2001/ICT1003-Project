from flask import Flask
from flask import render_template
from flask import request
app = Flask(__name__)

@app.route('/', methods=["GET","POST"])
def index():
    user = {'username': 'Kai Huan', 'steps': None, 'temp': None}
    if request.method == "POST":
        json_data = request.get_json(force=True)
        user['steps'] = json_data['steps']
        user['temp'] = json_data['temp']
    return render_template('index.html', title='Home', user=user)