if __name__ == '__main__':
    with open("send.txt",'a') as f:
        for i in range(0,1000):
            f.write(str(i)+"##########\n")

