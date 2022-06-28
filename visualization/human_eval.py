import json
import os
from analyze_helper import *

BENCH_BASE_DIR = os.environ["MPI_CORRECTNESS_BM_DIR"]

xTools = ['CIVL-1.21_5476', 'PARCOACH', 'MPI-Checker']

for tool in xTools:
    Tools = [tool]
    raw_data = load_data(Tools,BENCH_BASE_DIR)
    data = reduce_data(load_data(Tools,BENCH_BASE_DIR),Tools)
    by_tools=score_by_tool(Tools,data)
    by_category=score_by_category(Tools,data)

    fail = [by_tools[tool][ERR] for tool in Tools]
    true_negative = [by_tools[tool][TN] for tool in Tools]
    true_positive = [by_tools[tool][TP] for tool in Tools]
    false_negative = [by_tools[tool][FN] for tool in Tools]
    false_positive = [by_tools[tool][FP] for tool in Tools]
    good_warn = [by_tools[tool][TW] for tool in Tools]
    bad_warn = [by_tools[tool][FW] for tool in Tools]

    true_negative = [x + y for x, y in zip(bad_warn, true_negative)]

    print("TP " + Tools[0] + ": " + str(true_positive[0]))
    print("TN " + Tools[0] + ": " + str(true_negative[0]))
    print("FP " + Tools[0] + ": " + str(false_positive[0]))
    print("FN " + Tools[0] + ": " + str(false_negative[0]))