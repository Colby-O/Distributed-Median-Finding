### Distributed Median Finding

This program finds the median of data seperated in mutliple text files with a distribute algorithm using multiple processes. The program can be ran using the following command:

```
make CHILD_COUNT = n
./findmedian
```

where n is the number of files you have. You can also run the program with the following:

```
make
./findmedian
```

Where CHILD_COUNT will default to 5. The data for this project must be placed in the data folder and be named in the following manner.
<br>
<br>
input_n.txt
<br>
<br>
Where n ranges from 0 to CHILD_COUNT - 1. 
### Example 
The project is setup to run the following example below with 5 files and hence 5 child processes.
#### Input:

input_0.txt : 1 2 3 4 5 <br>
input_1.txt : 6 7 8 9 10 <br>
input_2.txt : 11 12 13 14 15 <br>
input_3.txt : 16 17 18 19 20 <br>
input_4.txt : 21 22 23 24 25 <br>


#### Output:
![QnJMUW](https://user-images.githubusercontent.com/60577496/156435134-eefead36-cf17-4bbb-9a8a-9e96791c43ca.png)

