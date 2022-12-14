if __name__ == '__main__':
    with open("send.txt",'a') as f:
        for i in range(0,10000):
            s = str(i)
            for j in range(len(str(i)),10):
                s += "#"
            f.write(s+"\n")

