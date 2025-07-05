#!/usr/bin/env python3

import io
import numpy as np
from lxml import etree
from dataclasses import dataclass
from abc import ABC, abstractmethod
import matplotlib.colors as mcolors


@dataclass
class Config:
    is_yed: bool
    colors: list[str]
    pretty_xml: bool = True

    yed_xmlns: str = 'http://www.yworks.com/xml/graphml'
    yed_node_type: str = 'ellipse'
    yed_node_width: int = 50
    yed_node_height: int = 50
    yed_node_font_size: int = 10


class INode(ABC):
    @staticmethod
    def _add_yed_key(graphml: etree.Element) -> None:
        etree.SubElement(graphml, 'key', attrib={
            'id': 'info',
            'for': 'node',
            'yfiles.type': 'nodegraphics'
        })

    @staticmethod
    def _yed_render(cfg: Config, color: str, text: str) -> etree.Element:
        yed_ns = f'{{{cfg.yed_xmlns}}}'
        info = etree.Element('data', attrib={'key': 'info'})
        shape = etree.SubElement(info, f'{yed_ns}ShapeNode')
        etree.SubElement(shape, f'{yed_ns}Fill', attrib={
            'color': color
        })
        etree.SubElement(shape, f'{yed_ns}Shape', attrib={
            'type': cfg.yed_node_type
        })
        etree.SubElement(shape, f'{yed_ns}Geometry', attrib={
            'height': str(cfg.yed_node_width),
            'width': str(cfg.yed_node_height),
        })
        label = etree.SubElement(shape, f'{yed_ns}NodeLabel', attrib={
            'fontSize': str(cfg.yed_node_font_size),
            'modelName': 'internal'
        })
        label.text = text
        return info

    @staticmethod
    @abstractmethod
    def add_keys(graphml: etree.Element, cfg: Config) -> None:
        pass

    @abstractmethod
    def render(self, cfg: Config) -> etree.Element:
        pass


class IEdge(ABC):
    @staticmethod
    @abstractmethod
    def add_keys(graphml: etree.Element, cfg: Config) -> None:
        pass

    @abstractmethod
    def render(self, cfg: Config) -> etree.Element:
        pass


class GraphML:
    def __init__(self, tag: str, nodes: list[INode], edges: list[IEdge]):
        self.__tag = tag
        self.__nodes = nodes
        self.__edges = edges
        assert len(nodes) > 0 and len(edges) > 0

    @staticmethod
    def __nsmap(cfg: Config) -> dict[str | None, str]:
        return {
            None: 'http://graphml.graphdrawing.org/xmlns',
            'xsi': 'http://www.w3.org/2001/XMLSchema-instance',
            **({'y': cfg.yed_xmlns} if cfg.is_yed else {})
        }

    @staticmethod
    def build_colors(num: int, bwr: list[str]) -> list[str]:
        cmap = mcolors.LinearSegmentedColormap.from_list('custom_bwr', bwr)
        return [mcolors.to_hex(cmap(x)) for x in np.linspace(0, 1, num)]

    def __build_graphml(self, cfg: Config) -> etree.Element:
        graphml = etree.Element('graphml', nsmap=self.__nsmap(cfg))
        graphml.set(
            '{http://www.w3.org/2001/XMLSchema-instance}schemaLocation',
            'http://graphml.graphdrawing.org/xmlns http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd'
        )
        self.__nodes[0].add_keys(graphml, cfg)
        self.__edges[0].add_keys(graphml, cfg)
        graph = etree.SubElement(graphml, 'graph', id=self.__tag, edgedefault='undirected')
        [graph.append(x.render(cfg)) for x in self.__nodes]
        [graph.append(x.render(cfg)) for x in self.__edges]
        return graphml

    def save_graphml(self, file: str, cfg: Config) -> None:
        xml_tree = etree.ElementTree(self.__build_graphml(cfg))

        fake_output = io.BytesIO()
        xml_tree.write(fake_output, pretty_print=cfg.pretty_xml, xml_declaration=True, encoding='utf-8')
        content = fake_output.getvalue().decode('utf-8')
        with open(file, 'w') as fp:
            fp.write(content.replace('&amp;#10;', '&#10;'))
