2015/07/10
Cloud LB Experiment

Setup Memo

0. On openstack
* Made "Cloud-experiment" tenant. admin and sail are users.
* made cloudexp-network, cloudexp-network-subnet, cloudexp-router
* Started ubuntu14.04 server

1. On ubuntu14.04
* python -V
Python 2.7.6

* sudo apt-get install update && python-pip (python-dev-all) git

2. Made repository for app
git init
git add ./
git commit
git remote add origin https://github.com/giga811/cloudexp.git
git push -u origin master

3. Parameter setting
seems jmeter can generate the parameter

4. App

/exp/nameget
/exp/nameset/<string:exp_name>
/exp/exp1/req/<int:x>

calculation is pi calculator
http://stackoverflow.com/questions/28284996/python-pi-calculation
```
for i in range(x):
    calc_pi(100)
```
Result(seems almost order(N))
```
 ➜  ~  time curl localhost:5000/exp/exp1/req/1
1curl localhost:5000/exp/exp1/req/1  0.01s user 0.00s system 15% cpu 0.051 total
 ➜  ~  time curl localhost:5000/exp/exp1/req/10
10curl localhost:5000/exp/exp1/req/10  0.01s user 0.00s system 3% cpu 0.269 total
 ➜  ~  time curl localhost:5000/exp/exp1/req/100
100curl localhost:5000/exp/exp1/req/100  0.00s user 0.00s system 0% cpu 2.482 total
 ➜  ~  time curl localhost:5000/exp/exp1/req/1000
1000curl localhost:5000/exp/exp1/req/1000  0.00s user 0.00s system 0% cpu 28.026 total
 ➜  ~  time curl localhost:5000/exp/exp1/req/10000
10000curl localhost:5000/exp/exp1/req/10000  0.01s user 0.01s system 0% cpu 4:07.89 total
```

* save_log function with os command
```
cmd = """echo "`date +"%D %T"`,{x}" >> log/{exp_name}.log""".format(x=x, exp_name=exp_name)
    os.system(cmd)
```

5. setting up nginx gunicorn on node
https://www.digitalocean.com/community/tutorials/how-to-serve-flask-applications-with-gunicorn-and-nginx-on-ubuntu-14-04

* sudo apt-get install python-pip python-dev nginx
* created 3 files

** wsgi.py
```
# -*- coding: utf-8 -*-
from cloudapp import app

if __name__ == "__main__":
  app.run()

```

** /etc/init/cloudapp.conf
```
description "Gunicorn application server running myproject"

start on runlevel [2345]
stop on runlevel [!2345]

respawn
setuid user
setgid www-data

env PATH=/home/user/myproject/myprojectenv/bin
chdir /home/user/myproject
exec gunicorn --workers 3 --bind unix:myproject.sock -m 007 wsgi
```

** /etc/nginx/sites-available/cloudapp-virtualhost.conf
sudo ln -s /etc/nginx/sites-available/cloudapp-virtualhost.conf /etc/nginx/sites-enabled
sudo nginx -t
sudo service nginx restart
```
server {
    listen 80 default_server;
    server_name _;

    location / {
        include proxy_params;
        proxy_pass http://unix:/home/ubuntu/cloudexp/cloudapp.sock;
    }
}
```
had to change /etc/nginx/sites-enabled/default
removed its default_server attribute

* run gunicorn
gunicorn --bind 0.0.0.0:8000 wsgi:app
curl localhost:8000

* start init script
sudo start|stop|restart cloudapp

6. lbaas setup

assigning floating ip for lb is done in "Access and Security" -> "Floating IP" section
