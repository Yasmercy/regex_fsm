import networkx as nx
import matplotlib.pyplot as plt
import functools as ft
import itertools as it
import json


def read_json(filename):
    """ Reads the json file and returns the deserialized object """
    return json.load(open(filename))


def parse_json(j):
    """ Reads the json data and returns the nodelist and edge_list """
    start = j["start"]["id"]
    terminals = [term["id"] for term in j["terminals"]]
    transitions = {
        (transition[0][0]["id"], transition[0][1]) : tuple((endpoint["id"] for endpoint in transition[1]))
        for transition in j["transition"]
    }
    return transitions


def draw_graph(edges):
    """
    Returns a nx.DiGraph() object with the nodes and edges
    (as well as some labels)
    """
    # key=start, value=end
    # add the proper labels for the edges
    E_labels = [{(k[0], vi): k[1] for vi in v} for k, v in edges.items()]
    E_labels = ft.reduce(lambda a, b: {**a, **b}, E_labels)
    nodes = set(it.chain.from_iterable(E_labels.keys()))

    # create the graph
    G = nx.DiGraph()
    G.add_nodes_from(nodes)
    G.add_edges_from(E_labels.keys())
    # draw the graph
    # pos = nx.spectral_layout(G)
    pos = nx.kamada_kawai_layout(G)
    # remove overlapping pos
    nx.draw_networkx(G, pos=pos, connectionstyle='arc3, rad = 0.1', arrows=True, with_labels=True)
    nx.draw_networkx_edge_labels(G, pos=pos, edge_labels=E_labels, label_pos=0.8, font_size=8) 
    return G


def show_graph():
    # show the graph
    plt.title("Graph")
    plt.show()


def main():
    data = read_json("dump.json")
    parsed = parse_json(data)
    draw_graph(parsed)
    show_graph()


if __name__ == "__main__":
    main()
