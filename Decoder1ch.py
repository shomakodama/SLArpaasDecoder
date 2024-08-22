import numpy as np
# import ROOT

with open("data_linearity/100ev_25mV_500ns_ch0.dat") as f:
    for line in f:
        if(line == "\n"):
            continue
        x = int(line, 16)
        if(x == 0xFFFFFFFF):
            print("header")
            continue

        if(x == 0xEEEEEEEE):
            print("footer")
            continue

        if(x != 0xFFFFFFFF or x != 0xEEEEEEEE):

            x1 = x&0xFFFF       # first half (lower 16 bits)
            x2 = (x>>16)&0xFFFF # second half (upper 16 bits)

            print(x)
            print(" > ", x1)
            print(" > ", x2)

f.close()