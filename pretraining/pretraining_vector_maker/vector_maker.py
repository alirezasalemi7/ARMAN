from gensim.models import FastText
import struct
import math
import os

model = FastText.load_fasttext_format("cc.fa.300.bin")

with open("hash2index.txt","w") as file:
    for (i,j) in model.wv.hash2index.items():
        file.write(str(i) + " " + str(j)+"\n")

with open("ngram_vectors.txt","w") as file:
    for row in model.wv.vectors_ngrams:
        for item in row:
            file.write(str(item) + " ")
        file.write("\n")

with open("ngram.vec","wb") as file,open("ngram_vectors.txt") as input:
    for line in input:
        vec = line.split()
        vec = [float(item) for item in vec]
        mag_vec = [item*item for item in vec]
        mag = math.sqrt(sum(mag_vec))
        vec = [item/mag for item in vec]
        for i in vec:
            file.write(struct.pack("<f",i))

words = ['']*len(model.wv.vocab)
for key,val in model.wv.vocab.items():
    words[val.index] = key

with open("words.txt","w") as file:
    file.write(str(model.wv.bucket)+" "+str(model.wv.vector_size)+"\n")
    for word in words:
        file.write(word+"\n")

with open("vectors.txt","w") as file:
    for row in model.wv.vectors:
        for item in row:
            file.write(str(item) + " ")
        file.write("\n")

with open("vectors.vec","wb") as file,open("vectors.txt") as input:
    for line in input:
        vec = line.split()
        vec = [float(item) for item in vec]
        mag_vec = [item*item for item in vec]
        mag = math.sqrt(sum(mag_vec))
        vec = [item/mag for item in vec]
        for i in vec:
            file.write(struct.pack("<f",i))

os.remove("./vectors.txt")
os.remove("./ngram_vectors.txt")
