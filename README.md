# P2P-Crypto-Network
This is the repository for the Simulation of P2P Cyptocurrency Network, Course Assignment for CS765: Blockchain and Smart Contracts.

# Team Members:
1. [Aneesh Garg](https://github.com/aneeshgarg18) - 180050007
2. [Raaghav Raaj](https://github.com/raaghavraaj) - 180050082
3. [Shivam Goel](https://github.com/shivam-18) - 180050098

# Running the simulator
1. Head into the directory ```P2P-Crypto-Network``` and open file ```main.cpp```.
2. In lines 19-30 set the parameters for the simulation. The variables have their standard meanings. The boolean variable ```invalid``` is supposed to be set true only when you want a peer to generate invalid blocks
3. Open terminal and run the ```make``` command as shown:
```
P2P-Crypto-Network $ make
```
4. This generates a file named ```output```. Run the output script as shown:
```
P2P-Crypto-Network $ ./output
```
5. This generates ```peer_info.txt``` files for every peer which contains the time of arrival of every block in the tree.
6. 4th step would also generate a file ```tree.txt``` which contains the description of the edges of the global blockchain tree.
7. If the ```invalid``` is set to ```True```, the program also prints the block IDs of all the invalid blocks.
8. To visualise a tree, we have also provided a python script ```showTree.py``` which opens ```tree.txt``` and prints a tree on the terminal itself. This can be done in the following manner:
```
P2P-Crypto-Network $ pip3 install treelib
P2P-Crypto-Network $ python3 showTree.py
```
