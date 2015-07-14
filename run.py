#!flask/bin/python

"""Run cloudapp"""

from cloudapp import app
app.run(host="0.0.0.0", debug=True)

