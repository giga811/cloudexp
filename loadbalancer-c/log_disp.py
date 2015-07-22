# -*- coding: utf-8 -*-
def main():

    # inputs
    N = int(raw_input())
    M = int(raw_input())

    # log list
    S = []
    for i in range(N):
        S.append( str(raw_input()) )

    lb = [0] * M
    rr = [0] * M
    i = 0

    lsum = 0
    for s in S:
        n, l = map(int, s.split(','))
        lb[n] += l
        rr[i] += l
        lsum += l
        i += 1
        if i == M:
            i = 0
    print "Average is ", lsum/M

    for i in range(len(lb)):
        print "Node {i} had {l} load, rr {rr} load".format(i=i, l=lb[i], rr=rr[i])


    return 0


if __name__ == "__main__":
    main()
