from manage import db, app

class User(db.Model):
    __tablename__ = 'users'

    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(64), index=True, unique=True)
    steps = db.Column(db.Integer, index=True, unique=True)
    temp = db.Column(db.Integer, index=True, unique=True)

    def __repr__(self):
        return self.name