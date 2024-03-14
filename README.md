# P2P-Cryptocurrency-Network

## Necessary installation
The simulator generates blockchain tree for each node. To represent this it uses visualization tool graphviz. This script requires the few visualization libraries. To set up this visualization script please run the following commands:
- `sudo apt-get install graphviz libgraphviz-dev`
- `pip install pygraphviz`
- `pip install matplotlib`

Run the `init.sh` script

## Make the project
- To change configuration edit the constants/SimulationParameters.h
- for the configurations to take effect run make 
`make`

## Start the simulator
The simulation requires two runtime parameters,
1] % of slow nodes
2] % of low cpu nodes

`./simulation.cpp slow_node_percent low_cpu_percent`

## Result and visualization
After running the simulator the output csv files will be stored in the output folder. To generate blockchain tree pdfs run,
`bash results.sh`
The tree pdfs will be stored in the outputs folder itself


Useful links:
 https://www.slideshare.net/VasishtaBhargava/discrete-event-system-simulation-control-flow-chart
 https://people.orie.cornell.edu/mru8/orie3120/lec/lec10.pdf
 http://cs.baylor.edu/~maurer/aida/desauto/chapter3.pdf
 https://www.cs.cmu.edu/~music/cmsip/readings/intro-discrete-event-sim.html