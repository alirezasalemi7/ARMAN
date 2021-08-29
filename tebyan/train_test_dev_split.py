import pandas as pd
import numpy as np

random_state = 37
train_ratio = 0.85
dev_ratio = 0.075
test_ratio = 0.075

raw_dataframe = pd.read_csv("tebyan.csv").sample(frac=1.0)

train, validate, test = np.split(raw_dataframe.sample(frac=1, random_state=random_state), [int(train_ratio*len(raw_dataframe)), int((dev_ratio+train_ratio)*len(raw_dataframe))])
train.to_csv("final dataset/train.csv")
validate.to_csv("final dataset/dev.csv")
test.to_csv("final dataset/test.csv")

print(train)
print(validate)
print(test)