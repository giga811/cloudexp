"""
2015/07/10
CloudApp
"""

# flask imports
from flask import Flask
from flask import request, render_template, send_from_directory
import json, os

# configuration
DEBUG = True
SECRET_KEY = 'cloudapp'

# create our little application :)
app = Flask(__name__)
app.config.from_object(__name__)


# route for INDEX
@app.route("/")
def hello():
  return render_template('index.html')

# tests
@app.route('/test')
def test():
  return "test"

if __name__ == '__main__':
    app.run(debug=True, host="0.0.0.0")
