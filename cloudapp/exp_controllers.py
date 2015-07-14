"""
2015/07/10
CloudApp
"""

# flask imports
from flask import Flask
from flask import request, render_template, send_from_directory
import json, os

# user imports
from cloudapp import app


# tests
@app.route('/test2')
def test2():
  return "test2"
