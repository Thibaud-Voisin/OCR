import random

def histo():
    hist = []
    for i in range(255):
        hist.append(0)
    f = open("20", "r")
    strr = f.read()
    for i in strr:
        hist[ord(i)] += 1
    for i in range( ord('z')):
        print(chr(i), hist[i])
    for i in range(ord(' ')-1):
        if(hist[i] >0 ):
            return False
    for i in range(ord(' ')-1):
        if(hist[i] >0 ):
            return False
    for i in range(ord('z')+1,255):
        if(hist[i] >0 ):
            return False
    return True


def generate():
    for i in range(100):
        strr=""
        for j in range(2000):
            k= random.randint(33,122)
            strr = strr+(chr(k))
        f = open(i, "w")
        f.write(strr)
        f.close()


def replace_guillemets():
    for i in range(20,45):
        f = open(str(i), "r")
        strr=f.read()
        f.close()
        strr_2 = ""
        for j in strr:
            if(j == '"'):
                strr_2 = strr_2 +"''"
            else:
                strr_2 = strr_2 + str(j)
        f = open(str(i), "w")
        f.write(strr_2)
        f.close()


replace_guillemets()
