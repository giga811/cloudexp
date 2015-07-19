# -*- coding: utf-8 -*-
import requests as rq
import os, sys

NODES=["10.0.100." + str(i) for i in range(3, 23)]
# NODES = ["172.20.50.139", "172.20.50.140", "172.20.50.139", "172.20.50.140", "172.20.50.139"]
N = len(NODES)

def send_req(node, x):
    global NODES
    url = "http://" + NODES[node] + "/exp/exp1/req/" + str(x)
    rq.get(url)
    cmd = """curl {url}""".format(url=url)
    os.system(cmd)

    rq.get(url, stream=True)
    rq.head(url,allow_redirects=False)
    return "ok"

argvs = sys.argv
print send_req(0, int(argvs[1]))