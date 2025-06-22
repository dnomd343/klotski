#!/usr/bin/env python3

from __future__ import annotations

import os
import re
import igraph as ig
from lxml import etree
from dataclasses import dataclass


class GraphML:
    @dataclass(frozen=True)
    class Node:
        id: str
        code: str
        step: int

    @dataclass(frozen=True)
    class Edge:
        src: str  # node id
        dst: str  # node id

    @dataclass(frozen=True)
    class Graph:
        name: str
        nodes: list[GraphML.Node]
        edges: list[GraphML.Edge]

    @staticmethod
    def __load(name: str, graph: ig.Graph) -> GraphML.Graph:
        nodes = []
        id_len = len(str(graph.vcount() - 1))
        for index in range(graph.vcount()):
            info = graph.vs[index]
            nodes.append(GraphML.Node(f'n{index:0{id_len}d}', info['code'], info['step']))

        edges = []
        for n1, n2 in graph.get_edgelist():
            node_1 = nodes[n1]
            node_2 = nodes[n2]
            if node_1.step < node_2.step:
                node_1, node_2 = node_2, node_1
            edges.append(GraphML.Edge(node_1.id, node_2.id))

        return GraphML.Graph(name, nodes, edges)

    def __init__(self, *graphs: tuple[str, ig.Graph]):
        self.__graphs = [self.__load(*x) for x in graphs]

    def __dump_node(self, node: Node) -> etree.Element:
        node_xml = etree.Element('node', id=node.id)
        etree.SubElement(node_xml, 'data', key='v_code').text = node.code
        etree.SubElement(node_xml, 'data', key='v_step').text = str(node.step)
        return node_xml

    def __dump_edge(self, edge: Edge) -> etree.Element:
        return etree.Element('edge', source=edge.src, target=edge.dst)

    def __dump_graph(self, graph: Graph) -> etree.Element:
        graph_xml = etree.Element('graph', id=graph.name, edgedefault='undirected')
        for node in graph.nodes:
            graph_xml.append(self.__dump_node(node))
        for edge in graph.edges:
            graph_xml.append(self.__dump_edge(edge))
        return graph_xml

    def save_graph(self, output: str) -> None:
        graphml = etree.Element('graphml', nsmap={
            None: 'http://graphml.graphdrawing.org/xmlns',
            'xsi': 'http://www.w3.org/2001/XMLSchema-instance'
        })
        graphml.set(
            '{http://www.w3.org/2001/XMLSchema-instance}schemaLocation',
            'http://graphml.graphdrawing.org/xmlns http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd'
        )
        etree.SubElement(graphml, 'key', attrib={
            'id': 'v_code',
            'for': 'node',
            'attr.name': 'code',
            'attr.type': 'string'
        })
        etree.SubElement(graphml, 'key', attrib={
            'id': 'v_step',
            'for': 'node',
            'attr.name': 'step',
            'attr.type': 'int'
        })

        for graph in self.__graphs:
            graphml.append(self.__dump_graph(graph))
        xml_tree = etree.ElementTree(graphml)
        xml_tree.write(output, pretty_print=True, xml_declaration=True, encoding='utf-8')


def to_graphml(inputs: list[tuple[str, str]], output: str) -> None:
    print(f'Convert into {output}')
    gml = GraphML(*((x, ig.Graph.Read_Pickle(y)) for x, y in inputs))
    gml.save_graph(output)


def convert_graphs(input_dir: str, output_dir: str) -> None:
    files = [x.removesuffix('.pkl') for x in os.listdir(input_dir) if x.endswith('.pkl')]
    tags = sorted(x for x in files if re.match(r'^\d\-\d\d(L|R|M)$', x))

    for tag in tags:
        output_file = os.path.join(output_dir, f'{tag}.graphml')
        pkls = sorted(x for x in files if x.startswith(tag))
        to_graphml([(x, os.path.join(input_dir, f'{x}.pkl')) for x in pkls], output_file)


if __name__ == "__main__":
    os.makedirs('output-gml', exist_ok=True)
    convert_graphs('output-ig', 'output-gml')
