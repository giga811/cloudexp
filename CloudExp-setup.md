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
