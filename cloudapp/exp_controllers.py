# -*- coding: utf-8 -*-
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

# global variables
## experiment name
exp_name = "exp_default"


# tests
@app.route('/test2')
def test2():
  return "test2"

# set exp_name
@app.route('/exp/nameset/<string:exp_name_in>')
def exp_nameset(exp_name_in):
    global exp_name
    exp_name = exp_name_in
    return "ok"

# get exp_name
@app.route('/exp/nameget')
def exp_nameget():
    global exp_name
    return exp_name

# save_log
def save_log(x):
    # write to file
    global exp_name
    cmd = """echo "`date +"%D %T"`,{x}" >> log/{exp_name}.log""".format(x=x, exp_name=exp_name)
    os.system(cmd)
    return True

# CALCULATION using x
# O(N)
#
# Pi calc from http://stackoverflow.com/questions/28284996/python-pi-calculation
#
from math import factorial
from decimal import Decimal, getcontext

getcontext().prec=100

def calc_pi(n):
    t= Decimal(0)
    pi = Decimal(0)
    deno= Decimal(0)
    k = 0
    for k in range(n):
        t = ((-1)**k)*(factorial(6*k))*(13591409+545140134*k)
        deno = factorial(3*k)*(factorial(k)**3)*(640320**(3*k))
        pi += Decimal(t)/Decimal(deno)
    pi = pi * Decimal(12)/Decimal(640320**Decimal(1.5))
    pi = 1/pi
    return pi

def calculation(x):
    for i in range(x):
        calc_pi(100)
    return x



@app.route('/exp/exp1/req/<int:x>')
def exp_exp1(x):
    # generate x, x is from jmeter
    # x = gen_var()

    # calling save log functions
    save_state = save_log(x)

    # calculation using x
    result = calculation(x)

    message = """Calculation {x}\n""".format(x=x)
    return str(x)

