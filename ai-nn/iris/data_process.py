#!/usr/bin/python3
import sys
import pandas as pd
import numpy as np
innode=4
out=3
df=pd.read_csv("iris.data",header=None)
for i in range(len(df.iloc[:,-1].unique())):
    df.insert(len(df. columns)-1,len(df. columns),0)
i=len(df.columns)
for index, row in df.iterrows():
    if (df.at[index,4]=="Iris-setosa"):
        df.at[index,i-1]=1
    if (df.at[index,4]=="Iris-versicolor"):
        df.at[index,i-2]=1
    if (df.at[index,4]=="Iris-virginica"):
        df.at[index,i-3]=1
df.pop(4)
i=len(df.columns)
df=(df-df.min())/(df.max()-df.min())
df=df.round(3)
df[np.arange(i-2,i+1)] = df[np.arange(i-2,i+1)].astype(int)
train=df.sample(frac = 0.7)
df=df.drop(train.index,axis=0)
testfile=open("test.csv","w")
testfile.write(str(len(df.index))+" "+str(innode)+" "+str(out)+"\n")
df.to_csv("test.csv",header=False,index=False,mode='a')
testfile=open("train.csv","w")
testfile.write(str(len(train.index))+" "+str(innode)+" "+str(out)+"\n")
train.to_csv("train.csv",header=False,index=False,mode='a')