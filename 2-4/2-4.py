'''联合概率计算自然语言的熵'''
import string
import math
filename='hamlet2.txt'
with open(filename) as file_object:
    contents=file_object.read()
    new_contents=''
    for i in contents:
        if i in string.punctuation:
            contents=contents.replace(i,'')
    for i in contents:
        new_contents += i.strip()
m=3  #每组的个数
n=int(len(new_contents)/m) #最大组数
key_point={}
i=1
print('the length of the new_contents is: '+str(len(new_contents)))
print('the maxinum of the group number is: '+str(n))
while i<=n:
    temp=new_contents[m*i-m:m*i]
    if temp in key_point.keys():
        key_point[temp]+=1
    else:key_point[temp]=1
    i+=1
l=len(key_point)
entropy=0
print(key_point)
print('the length of the key_point is: '+str(l))
for key,value in key_point.items():
    key_point[key]=value/n
for key,value in key_point.items():
    entropy+=-(key_point[key]*(math.log(key_point[key],2)))
avergae_entropy=entropy/m
print('entropy is:'+str(entropy))
print('average entropy is:'+str(avergae_entropy))



