import urllib.request
import tqdm


for i in tqdm.tqdm(range(379900,479900)):
    try:
        print('https://article.tebyan.net/{i}'.format(i=i))
        fid=urllib.request.urlopen('https://article.tebyan.net/{i}'.format(i=i))
        if fid.getcode() == 200:
            webpage=fid.read().decode("utf8")
            with open("download/{i}.html".format(i=i),"w") as file:
                file.write(webpage)
    except:
        print("err")
        pass