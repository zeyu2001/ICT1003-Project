from flask import Flask
from flask import render_template
from flask import request
from flask_sqlalchemy import SQLAlchemy

app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///app.db'

db = SQLAlchemy(app)

from models import User

@app.route('/', methods=["GET","POST"])
def index():

    # For demonstration purposes, we only have one user
    user = User.query.filter_by(id=1).first()
    if not user:
        u = User(id = 1, name = "Kai Huan", steps = 0, temp = 0)
        db.session.add(u)
        db.session.commit()

    if request.method == "POST":
        json_data = request.get_json(force=True)
        name = json_data['name']
        steps = json_data['steps']
        temp = json_data['temp']

        user = User.query.filter_by(id=1).first()
        user.name, user.steps, user.temp = name, steps, temp
        db.session.commit()

    return render_template('index.html', title='Home', user=user)