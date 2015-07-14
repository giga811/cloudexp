"""
2015/07/10
CloudApp
"""

# flask imports
from flask import Flask

# configuration
DEBUG = True
SECRET_KEY = 'cloudapp'

# create our little application :)
app = Flask(__name__)
app.config.from_object(__name__)

from cloudapp import controllers, exp_controllers
