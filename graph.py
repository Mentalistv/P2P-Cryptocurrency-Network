import networkx as nx
import matplotlib.pyplot as plt
from networkx.drawing.nx_agraph import graphviz_layout
import csv
import sys
import os

# Check if the correct number of command-line arguments is provided
if len(sys.argv) != 2:
    print("Usage: python script.py <csv_file>")
    sys.exit(1)

# Extract the CSV file from the command-line arguments
csv_file = sys.argv[1]

# Create a directed graph
G = nx.DiGraph()

edge_colors = []
# Read edges from CSV file
with open(csv_file, 'r') as csvfile:
    edges = csv.reader(csvfile)
  
    for edge in edges:
        G.add_edge(edge[0], edge[1])
        edge_type = edge[4]
        if edge_type == 's':
            edge_colors.append('red')
        else:
            edge_colors.append('blue')

# Create a layout using graphviz_layout
pos = graphviz_layout(G, prog='dot')

root_node='-1'

# Find the position of the root node
root_pos = pos[root_node]

pos = {k: (pos[k][1], -pos[k][0]) for k in pos}

#print(G.nodes().size())

# Create a list of colors, assigning a different color to the root node
node_colors = ['red' if node == root_node else 'blue' for node in G.nodes()]

# Visualize the blockchain tree
nx.draw(G, pos, with_labels=False, node_size=20, node_color=node_colors, edge_color=edge_colors, font_size=4, arrowsize=3)

# Get the base name of the input CSV file (excluding extension)
file_name = os.path.splitext(os.path.basename(csv_file))[0]

# Save the plot as a PDF with the same name as the input CSV file
pdf_file_name = f"./outputs/{file_name}.pdf"
plt.savefig(pdf_file_name)
# plt.show()