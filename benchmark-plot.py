import os
import json
from itertools import groupby

import pandas as pd
import matplotlib.pyplot as plt


def load_json(filepath):
    with open(filepath) as json_file:
        return json.load(json_file)


def filter_dict(dictionary, wanted_keys):
    return {key: dictionary[key] for key in wanted_keys}


def rename_dict_key(dictionary, old_key, new_key):
    dictionary[new_key] = dictionary.pop(old_key)


def get_benchmarks(filepath):
    wanted_keys = [
        "name",
        "family_index",
        "per_family_instance_index",
        "iterations",
        "real_time",
        "cpu_time",
        "time_unit",
    ]
    keyfunc = lambda bm: bm["family_index"]

    benchmarks = load_json(filepath)
    benchmarks = benchmarks["benchmarks"]

    for i in range(len(benchmarks)):
        bm = benchmarks[i]
        # filter
        benchmarks[i] = filter_dict(bm, wanted_keys)
        # map
        benchmarks[i]["name"], _, _ = bm["name"].partition("/")
        rename_dict_key(benchmarks[i], "per_family_instance_index", "index")

    # group
    benchmarks = [pd.DataFrame(g) for k, g in groupby(benchmarks, keyfunc)]

    return benchmarks


def main():
    # TODO(cahian): Use some CLI Python Library for filepath and y-axis.
    benchmarks = get_benchmarks("1536a-benchmark.json")
    for i in range(len(benchmarks)):
        bm = benchmarks[i]
        plt.plot(bm["index"], bm["real_time"], label=bm["name"][0])

    plt.xlabel("index")
    plt.ylabel("real time")
    plt.title("Benchmark")
    plt.legend()
    plt.show()


if __name__ == "__main__":
    main()
