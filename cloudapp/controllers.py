# -*- coding: utf-8 -*-
"""
2015/07/10
CloudApp
"""

# flask imports
from flask import Flask
from flask import request, render_template, send_from_directory
import json, os

from cloudapp import app

# route for INDEX
@app.route("/")
def hello():
  return render_template('index.html')


# tests
@app.route('/test')
def test():
  return "test"
