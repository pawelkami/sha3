#!/usr/bin/python
#-*- coding: utf-8 -*-

import sha3dll
import sha3cryptopp
import os
import datetime
import matplotlib.pyplot as plt
from operator import itemgetter
import matplotlib.patches as mpatches


statistics = {str(sha3dll) : [],
     str(sha3cryptopp) : []}

def computeSha3(module, filepath, type):
    hashCtx = module.HashCtx()
    hashCtx.setHashAlgo(type)
    hash = hashCtx.computeHash(filepath)
    return hash

def testSha3(module, type):
    os.chdir("./files/")
    for f in os.listdir("."):
        timeBegin = datetime.datetime.now()
        computeSha3(module, f, type)
        timeEnd = datetime.datetime.now()
        timeDelta = timeEnd - timeBegin
        miliseconds = (timeDelta.seconds * 1000000 + timeDelta.microseconds) / 1000
        global statistics
        statistics[str(module)].append({ "time" : miliseconds, "filesize" : os.stat(f).st_size / 1024})

    os.chdir("..")


def drawPlot(name):
    global statistics
    points1 = sorted(statistics[str(sha3dll)], key=itemgetter('filesize'))
    points2 = sorted(statistics[str(sha3cryptopp)], key=itemgetter('filesize'))

    line1x = []
    line1y = []
    for d in points1:
        line1x.append(d["filesize"])
        line1y.append(d["time"])

    line2x = []
    line2y = []
    for d in points2:
        line2x.append(d["filesize"])
        line2y.append(d["time"])

    plt.title(name)
    red_patch = mpatches.Patch(color='red', label='Implementowana biblioteka')
    blue_patch = mpatches.Patch(color='blue', label='Biblioteka wzorcowa (libcrypto++)')
    plt.legend(handles=[red_patch, blue_patch])

    plt.plot(line1x, line1y, 'r-', line2x, line2y, 'b-')
    plt.xlabel(u"Wielkość pliku [kB]")
    plt.ylabel(u"Czas [ms]")
    plt.savefig(name + ".png")
    plt.show()


testSha3(sha3dll, sha3dll.HashType.SHA3_224)
testSha3(sha3cryptopp, sha3cryptopp.HashType.SHA3_224)
drawPlot("SHA3-224")
statistics = {str(sha3dll) : [],
     str(sha3cryptopp) : []}

testSha3(sha3dll, sha3dll.HashType.SHA3_256)
testSha3(sha3cryptopp, sha3cryptopp.HashType.SHA3_256)
drawPlot("SHA3-256")
statistics = {str(sha3dll) : [],
     str(sha3cryptopp) : []}

testSha3(sha3dll, sha3dll.HashType.SHA3_384)
testSha3(sha3cryptopp, sha3cryptopp.HashType.SHA3_384)
drawPlot("SHA3-384")
statistics = {str(sha3dll) : [],
     str(sha3cryptopp) : []}

testSha3(sha3dll, sha3dll.HashType.SHA3_512)
testSha3(sha3cryptopp, sha3cryptopp.HashType.SHA3_512)
drawPlot("SHA3-512")
statistics = {str(sha3dll) : [],
     str(sha3cryptopp) : []}


