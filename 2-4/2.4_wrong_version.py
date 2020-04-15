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
'''去除文本中的空格、空行与符号'''
m=3 #每组的个数
n=int(len(new_contents)/m) #最大组数
key_point={}#字典1：存放m元字母组以及该字母组出现的频率(后改为概率)
m_group={}#字典2：存放每个字母组的熵以及该字母组出现的概率
i=1
print('the length of the new_contents is: '+str(len(new_contents)))
print('the maxinum of the group number is: '+str(n))
for i in range(0,len(new_contents),3):
    temp=new_contents[i:i+3]
    if temp in key_point.keys():
        key_point[temp]+=1
    else:key_point[temp]=1
    i+=1
l=len(key_point)
entropy=0#最终获得的总熵，随机变量为m元组
m_letter_entropy=0#每个m元组的熵，随机变量为单个字母
print(key_point)
print('the length of the key_point is: '+str(l))
for key,value in key_point.items():
    key_point[key]=value/l
'''频率转化为概率'''
for key,value in key_point.items():
    letters={}
    for letter in key:
        if letter in letters.keys():
            letters[letter]+=1
        else:letters[letter]=1
    for letter,frequency in letters.items():
        letters[letter]=frequency/m
    '''letters是一个字典，用来临时存储key里的字母letter和概率frequency'''
    for letter,frequency in letters.items():
        m_letter_entropy += -(letters[letter] * (math.log(letters[letter], 2)))
    m_group[str(m_letter_entropy)]=value
    '''计算了每个m元字母组的熵，并把它们存储在字典m_group中'''
for key,value in m_group.items():
    entropy += float(key)*value
avergae_entropy=entropy/m
print('entropy is:'+str(entropy))
print('average entropy is:'+str(avergae_entropy))
