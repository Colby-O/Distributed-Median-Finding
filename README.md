###Distributed Median Finding

This program finds the median of data seperated in mutliple textfiles using a distribute algorthum. The program can be ran using the following command
```
make CHILD_COUNT = n
./findmedian
```
where n is the number of files you have. You can run the program with the following
```
make
./findmedian
```
Where CHILD_COUNT will default to 5. The data for this project must be placed in the data folder and have the following named in the following manner.
<center>
input_n.txt
</center>
Where n ranges from 0 to CHILD_COUNT - 1. 
### Example 
The project is setup to run the following example
<b>Input:</b>
<center>
input_0.txt : 1 2 3 4 5
input_1.txt : 6 7 8 9 10
input_2.txt : 11 12 13 14 15
input_3.txt : 16 17 18 19 20
input_4.txt : 21 22 23 24 25
</center>
<b>Output:</b>
<center>
![QnJMUW](https://user-images.githubusercontent.com/60577496/156433755-84b3a20d-20f7-4ec5-ae29-05309a2da154.png)
</center>
