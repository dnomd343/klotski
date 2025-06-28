#!/usr/bin/env python3

from __future__ import annotations

import io
import os
from typing import override
from abc import ABC, abstractmethod

import numpy as np
import igraph as ig
from lxml import etree
import matplotlib.pyplot as plt
from dataclasses import dataclass
import matplotlib.colors as mcolors


@dataclass(frozen=True)
class Node:
    id: str
    code: str
    step: int


@dataclass(frozen=True)
class Edge:
    src: str  # node id
    dst: str  # node id


class Builder(ABC):
    def __init__(self, _: list[Node]):
        pass

    @abstractmethod
    def nsmap(self) -> dict[str | None, str]:
        pass

    @abstractmethod
    def build_keys(self) -> list[etree.Element]:
        pass

    @abstractmethod
    def build_node(self, node: Node) -> etree.Element:
        pass

    @abstractmethod
    def build_edge(self, edge: Edge) -> etree.Element:
        pass

    @abstractmethod
    def post_modify(self, content: str) -> str:
        pass


class KlskBuilder(Builder):
    def __init__(self, _: list[Node]):
        pass

    @override
    def nsmap(self) -> dict[str | None, str]:
        return {
            None: 'http://graphml.graphdrawing.org/xmlns',
            'xsi': 'http://www.w3.org/2001/XMLSchema-instance',
        }

    @override
    def build_keys(self) -> list[etree.Element]:
        k_node = etree.Element('key', attrib={
            'id': 'code',
            'for': 'node',
            'attr.name': 'code',
            'attr.type': 'string'
        })
        k_step = etree.Element('key', attrib={
            'id': 'step',
            'for': 'node',
            'attr.name': 'step',
            'attr.type': 'int'
        })
        return [k_node, k_step]

    @override
    def build_node(self, node: Node) -> etree.Element:
        node_xml = etree.Element('node', id=node.id)
        etree.SubElement(node_xml, 'data', key='code').text = node.code
        etree.SubElement(node_xml, 'data', key='step').text = str(node.step)
        return node_xml

    @override
    def build_edge(self, edge: Edge) -> etree.Element:
        return etree.Element('edge', source=edge.src, target=edge.dst)

    @override
    def post_modify(self, content: str) -> str:
        return content


class yEdBuilder(KlskBuilder):
    def __build_colors(self, num: int) -> list[str]:
        cmap = mcolors.LinearSegmentedColormap.from_list('custom_bwr', ['#0000ff', '#e8daef', '#ff0000'])
        return [mcolors.to_hex(cmap(x)) for x in np.linspace(0, 1, num)]

    def __init__(self, nodes: list[Node]):
        max_step = max(x.step for x in nodes)
        self.__colors = self.__build_colors(max_step + 1)
        self.__yed_ns = 'http://www.yworks.com/xml/graphml'

    @override
    def nsmap(self) -> dict[str | None, str]:
        return {
            **super().nsmap(),
            'y': self.__yed_ns
        }

    @override
    def build_keys(self) -> list[etree.Element]:
        k_info = etree.Element('key', attrib={
            'id': 'info',
            'for': 'node',
            'yfiles.type': 'nodegraphics'
        })
        return super().build_keys() + [k_info]

    @override
    def build_node(self, node: Node) -> etree.Element:
        shape = etree.Element(f'{{{self.__yed_ns}}}ShapeNode')
        etree.SubElement(shape, f'{{{self.__yed_ns}}}Fill', attrib={'color': self.__colors[node.step]})
        etree.SubElement(shape, f'{{{self.__yed_ns}}}Shape', attrib={'type': 'ellipse'})
        etree.SubElement(shape, f'{{{self.__yed_ns}}}Geometry', attrib={'height': '50', 'width': '50'})
        label = etree.SubElement(shape, f'{{{self.__yed_ns}}}NodeLabel', attrib={'fontSize': '10', 'modelName': 'internal'})
        label.text = f'{node.code}&#10;({node.step})'

        node_xml = super().build_node(node)
        node_info = etree.SubElement(node_xml, 'data', attrib={'key': 'info'})
        node_info.append(shape)
        return node_xml

    @override
    def post_modify(self, content: str) -> str:
        return content.replace('&amp;#10;', '&#10;')


class GraphML:
    @dataclass(frozen=True)
    class Graph:
        name: str
        nodes: list[Node]
        edges: list[Edge]

    @staticmethod
    def __load_graph(name: str, graph: ig.Graph) -> GraphML.Graph:
        nodes = []
        id_len = len(str(graph.vcount() - 1))
        for index in range(graph.vcount()):
            info = graph.vs[index]
            nodes.append(Node(f'n{index:0{id_len}d}', info['code'], info['step']))

        edges = []
        for n1, n2 in graph.get_edgelist():
            node_1 = nodes[n1]
            node_2 = nodes[n2]
            if node_1.step < node_2.step:
                node_1, node_2 = node_2, node_1
            edges.append(Edge(node_1.id, node_2.id))

        return GraphML.Graph(name, nodes, edges)

    def __init__(self, tag: str, graph: ig.Graph):
        self.__graph = self.__load_graph(tag, graph)

    def __dump_graph(self, graph: Graph, builder: Builder) -> etree.Element:
        graph_xml = etree.Element('graph', id=graph.name, edgedefault='undirected')
        for node in graph.nodes:
            graph_xml.append(builder.build_node(node))
        for edge in graph.edges:
            graph_xml.append(builder.build_edge(edge))
        return graph_xml

    def save_graph(self, output: str, builder_t: type[Builder]) -> None:
        builder = builder_t(self.__graph.nodes)

        graphml = etree.Element('graphml', nsmap=builder.nsmap())
        graphml.set(
            '{http://www.w3.org/2001/XMLSchema-instance}schemaLocation',
            'http://graphml.graphdrawing.org/xmlns http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd'
        )
        [graphml.append(x) for x in builder.build_keys()]
        graphml.append(self.__dump_graph(self.__graph, builder))

        fake_output = io.BytesIO()
        xml_tree = etree.ElementTree(graphml)
        xml_tree.write(fake_output, pretty_print=True, xml_declaration=True, encoding='utf-8')
        content = fake_output.getvalue().decode('utf-8')
        with open(output, 'w') as fp:
            fp.write(builder.post_modify(content))


def to_graphml(tag: str, input: str, output: str, is_yed: bool) -> None:
    print(f'Convert graph {input} into {output}')
    gml = GraphML(tag, ig.Graph.Read_Pickle(input))
    gml.save_graph(output, yEdBuilder if is_yed else KlskBuilder)


if __name__ == '__main__':
    os.makedirs('output-gml', exist_ok=True)
    os.makedirs('output-yed', exist_ok=True)
    for name in sorted(os.listdir('output-ig')):
        name = name.removesuffix('.pkl')
        tag = name.split('_')[0] if '_' in name else name
        to_graphml(tag, f'output-ig/{name}.pkl', f'output-gml/{name}.graphml', False)
        to_graphml(tag, f'output-ig/{name}.pkl', f'output-yed/{name}.graphml', True)
