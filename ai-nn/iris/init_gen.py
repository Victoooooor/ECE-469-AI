  
#!/usr/bin/python3
from random import randint

in_node = 4
hid_node = 6
out_node =3

outfile=open('my.init','w')
outfile.write(str(in_node)+" "+str(hid_node)+" "+str(out_node)+"\n")
temp =[in_node+1,hid_node+1,out_node]
for i, j in enumerate(temp[1:]):
    for k in range(j):
        buf=''
        for m in range(temp[i]):
            buf+=str(randint(0 ,999)/1000)+' '
        outfile.write(buf+'\n')