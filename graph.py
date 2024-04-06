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
        edge_type = edge[4]
        attacker_type = edge[5]

        edge_color = 'green'
        if edge_type == 's':
            if attacker_type == '1':
                edge_color = 'red'
            elif attacker_type == '2':
                edge_color = 'blue'

        G.add_edge(edge[0], edge[1], color=edge_color)


# Create a layout using graphviz_layout
pos = graphviz_layout(G, prog='dot')

root_node='-1'

# Find the position of the root node
root_pos = pos[root_node]

pos = {k: (pos[k][1], -pos[k][0]) for k in pos}

edges_list = list(G.edges())


for edge in edges_list:
    G.edges[edge[0], edge[1]]
    edge_colors.append(G.edges[edge[0], edge[1]]["color"])


# Visualize the blockchain tree
# nx.draw(G, pos, with_labels=True, node_size=250, node_color='black', edge_color=edge_colors, font_size=6, font_color='white', arrowsize=3)
nx.draw(G, pos, with_labels=False, node_size=30, node_color='black', edge_color=edge_colors, font_size=6, arrowsize=3)

# Get the base name of the input CSV file (excluding extension)
file_name = os.path.splitext(os.path.basename(csv_file))[0]

# Save the plot as a PDF with the same name as the input CSV file
pdf_file_name = f"./outputs/{file_name}.pdf"
plt.savefig(pdf_file_name)
# plt.show()