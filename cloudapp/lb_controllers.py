# -*- coding: utf-8 -*-
"""
2015/07/10
CloudApp

updated 2015/12/24
"""

# flask imports
from flask import Flask
from flask import request, render_template, send_from_directory
import json, os

# user imports
import math, random
from cloudapp import app
from exp_controllers import calculation
import requests as rq

# lb variables

NODES=["10.0.100." + str(i) for i in range(3, 13)]

# NODES = ["172.20.50.139", "172.20.50.140"] * 10
N = len(NODES)

# edges between self and other
#
EDGES = [[i for i in range(N)] for i in range(N)]
for i in range(N):
    EDGES[i].pop(i)

WALK = int(math.log(N) / math.log(2))

def random_sample(start_node):
    global EDGES, N, WALK
    w = WALK
    end_node = start_node
    while w > 0:
        edge = EDGES[end_node]
        end_node = random.choice(edge)
        w -= 1
    return end_node

def deledge(node1, node2):
    # print "deledge", node1, node2
    global EDGES, N, WALK
    if node1 == node2:
        return
    if node2 in EDGES[node1]:
        EDGES[node1].remove(node2)
    if node1 in EDGES[node2]:
        EDGES[node2].remove(node1)

def addedge(node1, node2):
    # print "addedge", node1, node2
    global EDGES, N, WALK
    if node1 == node2:
        return
    if node2 not in EDGES[node1]:
        EDGES[node1].append(node2)
    if node1 not in EDGES[node2]:
        EDGES[node2].append(node1)

# increment n edge to this_node
def increment(this_node, n):
    global EDGES, N, WALK
    edge = EDGES[this_node]
    n = N - len(edge) - 1
    for _ in range(n):
        sample_node = this_node
        while sample_node == this_node:
            sample_node = random_sample(this_node)
        addedge(this_node, sample_node)


def decrement(this_node, n):
    global EDGES, N, WALK
    edge = EDGES[this_node]
    n = min(n, len(edge))
    if n == len(edge):
        n -= 1
    for _ in range(n):
        sample_node = this_node
        while sample_node == this_node:
            sample_node = random_sample(this_node)
        deledge(this_node, sample_node)

def send_req(node, x):
    global NODES
    url = "http://" + NODES[node] + "/exp/exp1/req/" + str(x)
    rq.head(url,allow_redirects=False)
    return "ok"


@app.route('/brs/<int:x>')
def brs(x):
    global NODES, EDGES, WALK, N

    start_node = random.choice(range(N))

    sample_node = random_sample(start_node)

    load_n = int(float(x) / 200 * N)

    # print start_node, sample_node, load_n
    # print EDGES
    decrement(sample_node, load_n)
    # print EDGES

    send_req(sample_node, x)

    increment(sample_node, load_n)
    # print EDGES

    return str(x)

rr_index = 0
@app.route('/rr/<int:x>')
def rr(x):
    global NODES, rr_index, N
    rr_index += 1
    if rr_index > N:
        rr_index = 0
    send_req(rr_index, x)
    return str(x)
