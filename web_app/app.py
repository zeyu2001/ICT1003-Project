from flask import Flask
from flask import render_template
from flask import request
app = Flask(__name__)

@app.route('/', methods=["GET","POST"])
def index():
    user = {'username': 'Kai Huan', 'steps': None, 'temp': None}
    if request.method == "POST":
        user['steps'] = request.form['steps']
        user['temp'] = request.form['temp']
    return render_template('index.html', title='Home', user=user)