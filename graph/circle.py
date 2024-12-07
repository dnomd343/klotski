import marimo

__generated_with = "0.9.27"
app = marimo.App(width="medium")


@app.cell
def __():
    import klotski

    code = klotski.CommonCode(0x1A9BF0C00)

    code.to_string(shorten=True)  # just demo
    return code, klotski


@app.cell(hide_code=True)
def __():
    import igraph as ig

    g = ig.Graph.Read_Pickle('main_combined.pkl')

    for i in range(g.vcount()):
        g.vs[i]['codes'] = ['|'.join(g.vs[i]['codes'])]

    g.summary()
    return g, i, ig


@app.cell
def __():
    # loop = g.girth(return_shortest_circle=True)

    # [(g.vs[x].degree(), g.vs[x]) for x in loop]  # 8852(78) / 8923(79) / 8848(78) / 8922(79)
    return


@app.cell
def __():
    # print(g.vs[8852].degree() + g.vs[8848].degree())  # endpoint pattern 1 -> 10
    # print(g.vs[8923].degree() + g.vs[8922].degree())  # endpoint pattern 2 -> 8
    return


@app.cell(hide_code=True)
def __():
    import marimo as mo
    return (mo,)


@app.cell(hide_code=True)
def __(mo):
    mo.md(
        r"""
        `endpoints: 8852 <---> 8848`

        So, we should try to combine `8923` and `8922`, the step are both `79`.
        """
    )
    return


@app.cell
def __():
    # neigh_a = g.vs[8923].neighbors()
    # neigh_b = g.vs[8922].neighbors()
    # neigh = sorted(set(neigh_a) | set(neigh_b))

    # print([x.index for x in neigh_a])
    # print([x.index for x in neigh_b])
    # print([x.index for x in neigh])

    # new_point = g.add_vertex()

    # assert g.vs[8923]['step'] == g.vs[8922]['step']

    # new_point['step'] = g.vs[8923]['step']
    # new_point['codes'] = g.vs[8923]['codes'] + g.vs[8922]['codes']

    # new_point
    return


@app.cell
def __(g):
    # edges = [(x.index, new_point.index) for x in neigh]

    # print(edges)

    # g.add_edges(edges)

    # g.delete_vertices([8923, 8922])

    g.summary()
    return


@app.cell
def __(g):
    def find_combine_points(circle_points: list[int]) -> list[tuple[int, int]]:
        step_list = [g.vs[x]['step'] for x in circle_points]

        # step_list = [53, 52, 51, 52, 53, 54, 55, 56, 57, 56, 55, 54]
        size = len(step_list)
        assert size % 2 == 0
        half_size = int(size / 2)

        target = -1
        search_list = step_list + step_list + step_list
        for index in range(size, size * 2):
            right = search_list[index+1:index+half_size]
            left = list(reversed(search_list[index+1-half_size:index]))
            # print(f'{search_list[index]}: {left} vs {right} ({left == right})')
            if left == right:
                target = index - size
                break

        # TODO: check degree() value
        assert target != -1
        # print(target, step_list[target])

        def fix_index(val: int):
            if val < 0:
                return val + size
            if val >= size:
                return val - size
            return val

        combine_points = []
        for index in range(target + 1, target + half_size):
            mirror = target * 2 - index
            combine_points.append((fix_index(index), fix_index(mirror)))

        return [(circle_points[x], circle_points[y]) for x, y in combine_points]

    # find_combine_points([8852, 8923, 8848, 8922])
    return (find_combine_points,)


@app.cell
def __(find_combine_points, g):
    # NOTE: only for mirror circle pattern
    def combine_points(point_pairs: list[tuple[int, int]]):
        g.vs['id'] = range(g.vcount())

        for id_a, id_b in point_pairs:
            point_a = [x for x in g.vs if x['id'] == id_a][0]
            point_b = [x for x in g.vs if x['id'] == id_b][0]
            assert point_a['step'] == point_b['step']
            print('combine:', point_a, point_b)

            neigh_a = point_a.neighbors()
            neigh_b = point_b.neighbors()
            neigh = sorted(set(neigh_a) | set(neigh_b))

            new_point = g.add_vertex()
            new_point['step'] = point_a['step']
            new_point['codes'] = point_a['codes'] + point_b['codes']
            # print(new_point)
            g.add_edges([(x.index, new_point.index) for x in neigh])
            g.delete_vertices([point_a.index, point_b.index])

    for i_ in range(146):
        print(g.summary())
        loop = g.girth(return_shortest_circle=True)
        print('circle:', loop)
        combine_points(find_combine_points(loop))

    print(g.summary())
    g.write_pickle('main_circle.pkl')
    return combine_points, i_, loop


if __name__ == "__main__":
    app.run()
